// ICMPTool.h
// ICMPTool definition - base class for ICMP-based tests
// Copyright (c) 2011 Fluke Corporation. All rights reserved.
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

#ifndef ICMP_TOOL_H
#define ICMP_TOOL_H

#include <FnetTimer.h>
#include <string>
#include <list>


#include <errno.h>
#include <sys/time.h>

#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/file.h>

#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>

const int MAXPACKET = 9600;    /// max packet size
const uint PACKETDELAY_MS = 1000;    /// max packet size

/**
 * \brief ICMPTool sets up ICMP communication and currently only supports ICMPEcho/Ping
 * 
 * \author Jason Liptak 
 *  
 * This was taken from the pinger test from a c file and moved into a class. 
 * This will set up a socket and then send a ping message to the given server. 
 * It will then listen for the response. 
 */
class ICMPTool
{
public:

    struct PingResultsStruct
    {
        int            dnsLookup;
        int            pingAvg;
        std::string    macAddr;
        int            status;

        uint     pingMin;
        uint     pingMax;
        uint     sent;
        uint     received;
        uint     lost;
        uint     code;
        uint     subCode;
        uint     current;
    };

    struct PingPacket
    {
        uint num;
        uint sentTime;
    };

    enum ReturnStatus {
        STATUS_INCOMPLETE,
        STATUS_BIG_PACKET,
        STATUS_UNKNOWN_PROTOCOL,
        STATUS_SOCKET_BIND_FAILURE,
        STATUS_ICMP_TIMEOUT,
        STATUS_DNS_TIMEOUT,
        STATUS_DNS_LOOKUP_ERROR,
        STATUS_PACKET_LOST,
        STATUS_OKAY,
    };

    /// <summary>
    /// Given a file name this will run the numbers converting them into the correct address.
    /// It will then run those address accesses into the given cache
    /// </summary>
    /// <param name="fName">File Name you wish to access</param>
    /// <param name="theCache">A pointer to the cache you will run the given file on</param>
    ICMPTool();
    virtual ~ICMPTool();
    void initializeICMP();
    int runPingTest(std::string& url, int protocol, std::string interface);
    void stopPingTest ();
    void logPingResults();
    void getPingResults(PingResultsStruct&);

    int getStatus() { return _status; };
    void setCount(uint newCount) { _count = newCount; }
    void setFrameSize(int frame) { _frameSize = frame; }
    void setDontFragment(bool frag){ _dontFragment = frag; }
    void setTimeLimit(int timeLimit){ _timeLimit = timeLimit; }



private:
    int pinger(int socket);
    int pinger6(int socket);
    std::string prType(int t);
    int prPack( u_char * buf, int cc, struct sockaddr_in * from );
    int prPack6( u_char * buf, int cc, struct sockaddr_in6 * from );
    void tvSub( timeval* out, timeval* in );
    unsigned int tvToMs(struct timeval &tv);
    int runPing(std::string ipaddr);
    u_short inCksum(u_short * addr, int len);

    int resolveDNS(std::string ipaddr);
    std::string effAddr();

    int _status; ///Current status of the running test, holds error or ok status on return
    int _dnslookup; ///Once done running holds how much time the dns look up took
    int _protocol; ///Holds ipv4 or ipv6 lookup flag
    std::string _interface; ///**TODO** get rid of this, interface is handled else where

    u_char	_packet[MAXPACKET]; ///Packet we will get back

    sockaddr* _whereto; ///Address that the ICMP Echo will be sent too
    int _datalen; ///How much data will be sent

    int _ntransmitted; ///Number of packets transmitted
    int _ident; ///Unique id of the ICMP message
    
    addrinfo* _dnsResult; ///Information from the getaddrinfo call, holds resolved ip addresses
    std::string _macAddr;
    bool _cancelFlag; ///if True this class will exit
    std::list<PingPacket> _outstanding;

    int _frameSize;
    uint _count;
    bool _dontFragment;
    int  _timeLimit; //ms

    uint _pingTotal;
    uint _pingCurr;
    uint _pingMin;
    uint _pingMax;
    uint _received;
    uint _sent;
    uint _lost;
    uint _code;
    uint _subCode;
};

#endif //ICMP_TOOL_H
