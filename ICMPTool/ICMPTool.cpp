// ICMPTool implementation
// copyright Fluke Networks, 2011
// Author: Jason Liptak

/*
 *          P I N G . C
 *
 * Using the InterNet Control Message Protocol (ICMP) "ECHO" facility,
 * measure round-trip-delays and packet loss across network paths.
 *
 * Author -
 *  Mike Muuss
 *  U. S. Army Ballistic Research Laboratory
 *  December, 1983
 * Modified at Uc Berkeley
 *
 * Changed argument to inet_ntoa() to be struct in_addr instead of u_long
 * DFM BRL 1992
 *
 * Status -
 *  Public Domain.  Distribution Unlimited.
 *
 * Bugs -
 *  More statistics could always be gathered.
 *  This program has to run SUID to ROOT to access the ICMP socket.
 */

#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ICMPTool.h"
#include "AtomicTestConfig.h"
#include "logger.h"
#include "UriParser.h"

//Included needed by ping
#include <string.h>
#include <sys/time.h>

#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/file.h>

#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/ip_icmp.h>
#include <netinet/icmp6.h>
#include <netinet/if_ether.h>
#include <netdb.h>

#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>

using namespace std;

ICMPTool::ICMPTool() 
{
}

ICMPTool::~ICMPTool()
{
    //curl_easy_cleanup(curlHandle);
    if (_dnsResult != NULL) freeaddrinfo(_dnsResult);
}

void ICMPTool::initializeICMP()
{
    WO_TRACE()


    _ntransmitted = 0;       /* sequence # for outbound packets = #sent */
    _received = 0;      /* # of packets we got back */
    _lost = 0;
    _pingTotal = 0;           /* sum of all times, for doing average */
    _pingCurr = 0;
    _ident = 0;
    _cancelFlag = false;
    _dnsResult = NULL;

    //struct timezone _tz;	/* leftover */



    _status = STATUS_INCOMPLETE;
    _dnslookup = 0;
    _protocol = AF_INET;
    _interface = "eth0";

    _macAddr = "";
    _pingMin = -1;
    _pingMax = 0;
    _sent = 0;
    _code = 0;
    _subCode = 0;
    _frameSize = 0;
    _count = 1;
    _dontFragment = 0;
    _timeLimit = 0;

	bzero((char *)&_whereto, sizeof(struct sockaddr) );
	bzero((char *)&_packet, MAXPACKET );

}

int ICMPTool::runPingTest(string& url, int protocol, string interface)
{
    int returnVal(1); // good return
    _protocol = protocol;
    _interface = interface;
    _status = runPing(url);

    return returnVal;
}

string ICMPTool::effAddr() {
    if (_status != STATUS_OKAY) {
        return "";
    }
    char str[INET6_ADDRSTRLEN];
    if(_protocol == AF_INET6) {
        inet_ntop(AF_INET6, &((sockaddr_in6*)_whereto)->sin6_addr, str, INET6_ADDRSTRLEN);
    } else {
        inet_ntop(AF_INET, &((sockaddr_in*)_whereto)->sin_addr, str, INET6_ADDRSTRLEN);
    }
    return str;
}

void ICMPTool::logPingResults()
{
    WO_TRACE()

    int dnsLookup = _dnslookup;
    int pingResponse = ((_status == STATUS_OKAY)?_pingTotal / _received:0);
    string effUrl = effAddr();
    int status = _status;

    printf("DNS look up time    - %d ms\n", dnsLookup);
    printf("Ping Response Time  - %d ms\n", pingResponse);
    printf("Effective URL       - %s\n", effUrl.c_str());
    printf("Status Code         - %d\n", status);

}

void ICMPTool::getPingResults(PingResultsStruct& results)
{
    // create a new message

    int dnsLookup = _dnslookup;
    int pingAvg = ((_status == STATUS_OKAY || _status == STATUS_INCOMPLETE)?
                   _pingTotal / _received:0);
    string effUrl = effAddr();
    int status = _status;

    uint pingCurr = _pingCurr;
    uint pingMin = ((_pingMin == (uint)-1)?0:_pingMin);
    uint pingMax = _pingMax;
    uint sent = _sent;
    uint received = _received;
    uint lost = _lost;
    uint code = ((_status == STATUS_BIG_PACKET)?108:_code);
    uint subCode = _subCode;
    
    results.dnsLookup = dnsLookup;
    results.macAddr = _macAddr;
    results.pingAvg = ((pingAvg == -1)?0:pingAvg);
    results.status = status;
    results.pingMin = pingMin;
    results.pingMax = pingMax;
    results.sent = sent;
    results.received = received;
    results.lost = lost;
    results.code = code;
    results.subCode = subCode;
    results.current = pingCurr;
}

void ICMPTool::stopPingTest () {
    _cancelFlag = true;
}

int ICMPTool::resolveDNS(string ipaddr)
{
    struct addrinfo hints;
    int dnsError;
    memset(&hints, 0, sizeof(addrinfo));

    hints.ai_family = _protocol;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    timeval tv;
    timeval tp;

    gettimeofday( &tp, NULL );
    _status = STATUS_DNS_TIMEOUT;
    if(_cancelFlag) return 0;
    dnsError = getaddrinfo(ipaddr.c_str(), NULL/*port*/, &hints, &_dnsResult);
    _status = STATUS_INCOMPLETE;
    gettimeofday( &tv, NULL );

    if (dnsError != 0) {
        WO_WARN() << "DNS Error: " << gai_strerror(dnsError);
    }

    tvSub( &tv, &tp );
    _dnslookup = tv.tv_sec*1000+(tv.tv_usec/1000);

    _whereto = _dnsResult[0].ai_addr;
    if (_dnsResult[0].ai_family == AF_INET6) {
        ((sockaddr_in6*)_whereto)->sin6_port = 0;
    }
    return dnsError;
}


/*
 *          M A I N
 */
//options |= SO_DEBUG | SO_DONTROUTE
int ICMPTool::runPing(string ipaddr)
{
    WO_TRACE()
    struct protoent* proto;
    int _socket;

    if (resolveDNS(ipaddr) != 0) {
        _status = STATUS_DNS_LOOKUP_ERROR;
        return _status;
    }
    if(_cancelFlag) return 0;

    int headerLen;
    if (_protocol == AF_INET) {
        headerLen = 14 + 20 + 8; //Mac + Ipv4 + Icmp headers
    } else {
        headerLen = 14 + 40 + 8; //Mac + Ipv6 + Icmpv6 headers
    }
    _frameSize -= headerLen;
    if (_frameSize > MAXPACKET) { //9600 max on wired, 2290 max on wireless
        WO_ERROR() << "ping: packet size too largen";
        return STATUS_BIG_PACKET;
    }
    //if (_datalen >= (int)sizeof(timeval))    /* can we time 'em? */
        //printf("timing = 1\n");


    _ident = getpid() & 0xFFFF;

    string icmpType = ((_protocol == AF_INET6)?"ipv6-icmp":"icmp");
    if ((proto = getprotobyname(icmpType.c_str())) == NULL) {
        WO_ERROR() << "icmp: unknown protocol";
        return STATUS_UNKNOWN_PROTOCOL;
    }

    if ((_socket = socket(_protocol, SOCK_RAW, proto->p_proto)) < 0) {
        return STATUS_SOCKET_BIND_FAILURE;
    }

    if(_cancelFlag) return 0;
    ifreq ifr;
    ifr.ifr_addr.sa_family = (sa_family_t)_protocol;
    strcpy(ifr.ifr_name, _interface.c_str());
    if (ioctl(_socket, SIOCGIFINDEX, &ifr) < 0) {
        WO_ERROR() << "problem getting ioctl information";
        close(_socket);
        return STATUS_SOCKET_BIND_FAILURE;
    }
    if (ioctl(_socket, SIOCGIFHWADDR, &ifr) < 0) {
        WO_ERROR() << "problem getting ioctl information";
        close(_socket);
        return STATUS_SOCKET_BIND_FAILURE;
    }
    if (setsockopt(_socket, SOL_SOCKET, SO_BINDTODEVICE, &ifr, sizeof(ifr)) < 0) {
        WO_ERROR() << "problem binding to a device";
        close(_socket);
        return STATUS_SOCKET_BIND_FAILURE;
    }
    int dontFrag = ((_dontFragment)?IP_PMTUDISC_DO:IP_PMTUDISC_DONT);
    if (_protocol == AF_INET) {
        if (setsockopt(_socket, IPPROTO_IP, IP_MTU_DISCOVER, &dontFrag, sizeof(dontFrag)) < 0) {
            WO_ERROR() << "problem setting up dont fragment";
            close(_socket);
            return STATUS_SOCKET_BIND_FAILURE;
        }
    }

    PingPacket packet;
    struct timeval tv, tp;
    int packetId = 0;
    for (uint i = 0; i < _count || _count == (uint)-1; i++) {
        gettimeofday( &tp, NULL );
        packetId = ((_protocol == AF_INET6)?pinger6(_socket):pinger(_socket));
        WO_INFO() << "Packet Sent";
        if (_status == STATUS_BIG_PACKET) {
            close(_socket);
            return STATUS_BIG_PACKET;
        }
        packet.num = packetId;
        packet.sentTime = tvToMs(tp);
        _outstanding.push_back(packet);
        _sent++;
        fd_set sockfd;
        FD_ZERO(&sockfd);
        FD_SET(_socket,&sockfd);
    
        sockaddr_in from;
        sockaddr_in6 from6;
        socklen_t fromlen = ((_protocol == AF_INET6)?sizeof(from6):sizeof(from));
        int cc;

        sockaddr* fromSock = ((_protocol == AF_INET6)?(sockaddr *)&from6:(sockaddr *)&from);

        //A list of outstanding packet is used here
        //A list is used since the most common cases are the first outstanding packet comes back
        //Or the packet we just sent comes back
        for (;;) {
            usleep(1000);
            if(_cancelFlag) {
                close(_socket);
                if (_lost > 0) {
                    return STATUS_PACKET_LOST;
                }
                return 0;
            }
            gettimeofday( &tv, NULL );
            uint sleepTime = tvToMs(tv);
            tvSub( &tv, &tp );
            uint lastSent = tvToMs(tv);
            if (lastSent >= PACKETDELAY_MS && _sent < _count) {
                break;
            }
            if (_sent >= _count && _outstanding.empty()) {
                break;
            }

            if (!_outstanding.empty() && sleepTime - _outstanding.front().sentTime >= (uint)_timeLimit) {
                _outstanding.pop_front();
                _lost++;
                continue;
            }
            cc=recvfrom(_socket, _packet, sizeof(_packet), MSG_DONTWAIT, fromSock, &fromlen);
            if (cc == -1) {
                continue;
            }
            packetId = ((_protocol == AF_INET6)?prPack6( _packet, cc, &from6):prPack( _packet, cc, &from));
            if (packetId != -1 && !_outstanding.empty()) {
                for (std::list<PingPacket>::iterator it = _outstanding.begin() ; it != _outstanding.end(); it++) {
                    if ((*it).num == (uint)packetId) {
                        WO_INFO() << "Found Match";
                        if (_pingCurr != 0) {
                            _received++;
                            _pingTotal += _pingCurr;
                            if (_pingCurr < _pingMin) {
                                _pingMin = _pingCurr;
                            }
                            if (_pingCurr > _pingMax) {
                                _pingMax = _pingCurr;
                            }
                        } else {
                            _lost++;
                        }
                        if (_macAddr.empty()) {
                            _macAddr = Routing::instance()->lookupEther(ipaddr, _interface);
                            WO_INFO() << "Looked up: " << _macAddr << 
                                " for " << ipaddr <<
                                " on " << _interface;
                            if (strcmp(_macAddr.c_str(), "") == 0) {
                                _macAddr = Routing::instance()->gatewayEther(_interface);
                                WO_INFO() << "Router lookup: " << _macAddr;
                            }
                        }
                        _outstanding.erase(it);
                        WO_INFO() << "Last Sent: " << lastSent << " Sleeping for: " << ((PACKETDELAY_MS - lastSent) * 1000);
                        if (lastSent < PACKETDELAY_MS) {
                            usleep((PACKETDELAY_MS - lastSent) * 1000);
                        }
                        break;
                    }
                }
            }
            WO_INFO() << "Received: " << _received << " Count: " << _count;
            if (_received >= _count) {
                break;
            }
        }
    }
    close(_socket);
    _outstanding.clear();

    if (_sent == _count && _sent != _received && _status != STATUS_BIG_PACKET) {
        return STATUS_PACKET_LOST;
    }
    return STATUS_OKAY;
}


/*
 *          P I N G E R
 * 
 * Compose and transmit an ICMP ECHO REQUEST packet.  The IP packet
 * will be added on by the kernel.  The ID field is our UNIX process ID,
 * and the sequence number is an ascending integer.  The first 8 bytes
 * of the data portion are used to hold a UNIX "timeval" struct in VAX
 * byte-order, to compute the round-trip time.
 */
int ICMPTool::pinger(int socket)
{
    static u_char outpack[MAXPACKET];
    icmp* icp = (icmp *) outpack;
    int i, cc;
    timeval* tp = (timeval *) &outpack[8];
    u_char* datap = &outpack[8+sizeof(struct timeval)];

    icp->icmp_type = ICMP_ECHO;
    icp->icmp_code = 0;
    icp->icmp_cksum = 0;
    icp->icmp_seq = (u_int16_t)_ntransmitted++;
    icp->icmp_id = (u_int16_t)_ident;        /* ID */

    cc = _frameSize+8;         /* skips ICMP portion */

    gettimeofday( tp, NULL );

    for( i=8; i<_frameSize; i++)   /* skip 8 for time */
        *datap++ = (u_char)i;

    /* Compute ICMP checksum here */
    icp->icmp_cksum = inCksum( (u_short *)icp, cc );

    /* cc = sendto(s, msg, len, flags, to, tolen) */
    i = sendto( socket, outpack, cc, 0, _whereto, sizeof(sockaddr) );

    if( i < 0 || i != cc )  {
        if( i<0 )  perror("sendto");
        WO_DEBUG() << "Packet to big to send";
        _cancelFlag = true;
        _status = STATUS_BIG_PACKET;
    }
    return icp->icmp_seq;
}


/*
 *          P I N G E R
 * 
 * Compose and transmit an ICMP ECHO REQUEST packet.  The IP packet
 * will be added on by the kernel.  The ID field is our UNIX process ID,
 * and the sequence number is an ascending integer.  The first 8 bytes
 * of the data portion are used to hold a UNIX "timeval" struct in VAX
 * byte-order, to compute the round-trip time.
 */
int ICMPTool::pinger6(int socket)
{
    static u_char outpack[MAXPACKET];
    icmp6_hdr* icp6 = (icmp6_hdr *) outpack;
    int i, cc;
    timeval* tp = (timeval *) &outpack[8];
    u_char* datap = &outpack[8+sizeof(struct timeval)];

    icp6->icmp6_type = ICMP6_ECHO_REQUEST;
    icp6->icmp6_code = 0;
    icp6->icmp6_cksum = 0;
    icp6->icmp6_seq = (u_int16_t)_ntransmitted++;
    icp6->icmp6_id = (u_int16_t)_ident;        /* ID */

    cc = _frameSize+8;         /* skips ICMP portion */

    gettimeofday( tp, NULL );

    for( i=8; i<_frameSize; i++)   /* skip 8 for time */
        *datap++ = (u_char)i;

    /* Compute ICMP checksum here */
    icp6->icmp6_cksum = inCksum( (u_short *)icp6, cc );

    /* cc = sendto(s, msg, len, flags, to, tolen) */
    i = sendto( socket, outpack, cc, 0, _whereto, sizeof(sockaddr_in6) );

    if( i < 0 || i != cc )  {
        if( i<0 )  perror("sendto");
        WO_DEBUG() << "Packet to big to send";
        _cancelFlag = true;
        _status = STATUS_BIG_PACKET;
    }
    return icp6->icmp6_seq;
}

/*
 *          P R _ T Y P E
 *
 * Convert an ICMP "type" field to a printable string.
 */
string ICMPTool::prType(int t)
{
    static string ttab[] = {
        "Echo Reply",
        "ICMP 1",
        "ICMP 2",
        "Dest Unreachable",
        "Source Quench",
        "Redirect",
        "ICMP 6",
        "ICMP 7",
        "Echo",
        "ICMP 9",
        "ICMP 10",
        "Time Exceeded",
        "Parameter Problem",
        "Timestamp",
        "Timestamp Reply",
        "Info Request",
        "Info Reply"
    };

    if( t < 0 || t > 16 )
        return("OUT-OF-RANGE");

    return(ttab[t]);
}

/*
 *          P R _ P A C K
 *
 * Print out the packet, if it came from us.  This logic is necessary
 * because ALL readers of the ICMP socket get a copy of ALL ICMP packets
 * which arrive ('tis only fair).  This permits multiple copies of this
 * program to be run without having intermingled output (or statistics!).
 */
int ICMPTool::prPack( u_char * buf, int cc, struct sockaddr_in * from )
{
    WO_TRACE()
    ip* ip;
    icmp* icp;
    timeval tv;
    timeval *tp;
    int hlen;
    uint triptime;

    from->sin_addr.s_addr = ntohl( from->sin_addr.s_addr );
    gettimeofday( &tv, NULL );

    ip = (struct ip *) buf;
    hlen = ip->ip_hl << 2;
    if (cc < hlen + ICMP_MINLEN) {
        char str[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET, &((sockaddr_in*)from)->sin_addr, str, INET6_ADDRSTRLEN);
        WO_INFO() << "packet too short (" << cc << "bytes) from " << str;
        return -1;
    }
    cc -= hlen;
    icp = (icmp *)(buf + hlen);

    if( icp->icmp_type != ICMP_ECHOREPLY)  {
        char str[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET, &((sockaddr_in*)from)->sin_addr, str, INET6_ADDRSTRLEN);
        WO_INFO() << cc << " bytes from " << str << ": icmp_type=" << hex << icp->icmp_type << 
            " icmp_code="<< icp->icmp_code;
        int tmpCode = icp->icmp_type;
        int tmpSubCode = icp->icmp_code;

        //origonal message is always 8 bytes in
        //This message includes the IP header and the ICMP message
        ip = (struct ip *)(buf + hlen + 8);
        int hlenOrig = ip->ip_hl << 2;
        icp = (icmp *)(buf + hlen + hlenOrig + 8);

        if( icp->icmp_id == _ident ){
            _code = tmpCode;
            _subCode = tmpSubCode;
            _pingCurr = 0;
            return icp->icmp_seq;
        } else {
            return -1;
        }
    }
    if( icp->icmp_id != _ident ){
        WO_INFO() << "Not our echo: " << hex << icp->icmp_id;
        return -1;         /* 'Twas not our ECHO */
    }

    tp = (timeval *)&icp->icmp_data[0];
    tvSub( &tv, tp );
    triptime = tv.tv_sec*1000+(tv.tv_usec/1000);
    //in the case that dnslookup is submillisecond -1 will mean <1ms
    triptime = ((triptime == 0)?-1:triptime);

    char str[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET, &((sockaddr_in*)from)->sin_addr, str, INET6_ADDRSTRLEN);
    WO_DEBUG() << cc << " bytes from " << str <<
        ": icmp_seq=" << icp->icmp_seq <<
        " time=" << triptime << " msn";

    _pingCurr = triptime;
    return icp->icmp_seq;
}

int ICMPTool::prPack6( u_char * buf, int cc, struct sockaddr_in6 * from )
{
    ip6_hdr* ip6;
    icmp6_hdr* icp;
    timeval tv;
    timeval *tp;
    int hlen;
    uint triptime;

    //from->sin6_addr.s_addr = ntohl( from->sin6_addr.s_addr );
    gettimeofday( &tv, NULL );

    ip6 = (struct ip6_hdr *) buf;
    hlen = sizeof(ip6_hdr);
    
    cc -= hlen;
    icp = (icmp6_hdr *)(buf);

    if( icp->icmp6_type != ICMP6_ECHO_REPLY)  {
        char str[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &((sockaddr_in6*)from)->sin6_addr, str, INET6_ADDRSTRLEN);
        WO_INFO() << cc << " bytes from " << str << ": icmp_type=" << hex << icp->icmp6_type << 
            " icmp_code="<< icp->icmp6_code;
        int tmpCode = icp->icmp6_type;
        int tmpSubCode = icp->icmp6_code;

        //origonal message is always 8 bytes in
        //This message includes the IP header and the ICMP message
        ip6 = (struct ip6_hdr *)(buf + hlen + 8);
        int hlenOrig = sizeof(ip6_hdr);
        icp = (icmp6_hdr *)(buf + hlen + hlenOrig + 8);

        if( icp->icmp6_id == _ident ){
            _code = tmpCode;
            _subCode = tmpSubCode;
            _pingCurr = 0;
            return icp->icmp6_seq;
        } else {
            return -1;
        }
    }
    if( icp->icmp6_id != _ident )
        return -1;         /* 'Twas not our ECHO */

    tp = (timeval *)&icp->icmp6_data8[4];
    tvSub( &tv, tp );
    triptime = tv.tv_sec*1000+(tv.tv_usec/1000);
    //in the case that dnslookup is submillisecond -1 will mean <1ms
    triptime = ((triptime == 0)?-1:triptime);

    char str[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &((sockaddr_in6*)from)->sin6_addr, str, INET6_ADDRSTRLEN);
    WO_DEBUG() << cc << " bytes from " << str << 
        ": icmp_seq=" << icp->icmp6_seq << 
        " time=" << triptime << " msn";

    _pingCurr = triptime;
    return icp->icmp6_seq;
}


/*
 *          I N _ C K S U M
 *
 * Checksum routine for Internet Protocol family headers (C Version)
 *
 */
u_short ICMPTool::inCksum(u_short * addr, int len)
{
    int nleft = len;
    u_short* w = addr;
    u_short answer;
    int sum = 0;

    /*
     *  Our algorithm is simple, using a 32 bit accumulator (sum),
     *  we add sequential 16 bit words to it, and at the end, fold
     *  back all the carry bits from the top 16 bits into the lower
     *  16 bits.
     */
    while( nleft > 1 )  {
        sum += *w++;
        nleft -= 2;
    }

    /* mop up an odd byte, if necessary */
    if( nleft == 1 ) {
        u_short u = 0;

        *(u_char *)(&u) = *(u_char *)w ;
        sum += u;
    }

    /*
     * add back carry outs from top 16 bits to low 16 bits
     */
    sum = (sum >> 16) + (sum & 0xffff); /* add hi 16 to low 16 */
    sum += (sum >> 16);         /* add carry */
    answer = (u_short)~sum;             /* truncate to 16 bits */
    return (answer);
}

/*
 *          T V S U B
 * 
 * Subtract 2 timeval structs:  out = out - in.
 * 
 * Out is assumed to be >= in.
 */
void ICMPTool::tvSub( timeval* out, timeval* in )
{
    if( (out->tv_usec -= in->tv_usec) < 0 )   {
        out->tv_sec--;
        out->tv_usec += 1000000;
    }
    out->tv_sec -= in->tv_sec;
}

unsigned int ICMPTool::tvToMs(struct timeval &tv)
{
    return (unsigned int)tv.tv_sec * 1000 + ((unsigned int)tv.tv_usec + 500) / 1000;
}

