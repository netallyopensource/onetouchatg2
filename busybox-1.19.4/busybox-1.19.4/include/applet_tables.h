/* This is a generated file, don't edit */

#define NUM_APPLETS 237

const char applet_names[] ALIGN1 = ""
"[" "\0"
"[[" "\0"
"addgroup" "\0"
"adduser" "\0"
"ar" "\0"
"arp" "\0"
"arping" "\0"
"ash" "\0"
"awk" "\0"
"base64" "\0"
"basename" "\0"
"blkid" "\0"
"blockdev" "\0"
"bootchartd" "\0"
"bunzip2" "\0"
"bzcat" "\0"
"cat" "\0"
"chgrp" "\0"
"chmod" "\0"
"chown" "\0"
"chpasswd" "\0"
"chpst" "\0"
"chroot" "\0"
"chrt" "\0"
"chvt" "\0"
"clear" "\0"
"cmp" "\0"
"cp" "\0"
"cpio" "\0"
"crond" "\0"
"crontab" "\0"
"cryptpw" "\0"
"cttyhack" "\0"
"cut" "\0"
"date" "\0"
"dc" "\0"
"dd" "\0"
"deallocvt" "\0"
"delgroup" "\0"
"deluser" "\0"
"depmod" "\0"
"df" "\0"
"dirname" "\0"
"dmesg" "\0"
"dnsdomainname" "\0"
"dos2unix" "\0"
"du" "\0"
"echo" "\0"
"egrep" "\0"
"eject" "\0"
"env" "\0"
"envdir" "\0"
"envuidgid" "\0"
"expand" "\0"
"expr" "\0"
"false" "\0"
"fdflush" "\0"
"fdformat" "\0"
"fdisk" "\0"
"fgconsole" "\0"
"fgrep" "\0"
"find" "\0"
"flock" "\0"
"free" "\0"
"ftpget" "\0"
"ftpput" "\0"
"getopt" "\0"
"getty" "\0"
"grep" "\0"
"groups" "\0"
"gunzip" "\0"
"gzip" "\0"
"halt" "\0"
"hdparm" "\0"
"head" "\0"
"hexdump" "\0"
"hostid" "\0"
"hostname" "\0"
"hwclock" "\0"
"id" "\0"
"ifconfig" "\0"
"ifdown" "\0"
"ifenslave" "\0"
"ifup" "\0"
"inetd" "\0"
"init" "\0"
"insmod" "\0"
"install" "\0"
"iostat" "\0"
"ip" "\0"
"kill" "\0"
"killall" "\0"
"killall5" "\0"
"klogd" "\0"
"less" "\0"
"linuxrc" "\0"
"ln" "\0"
"logger" "\0"
"login" "\0"
"losetup" "\0"
"ls" "\0"
"lsmod" "\0"
"lspci" "\0"
"lsusb" "\0"
"md5sum" "\0"
"mdev" "\0"
"mesg" "\0"
"mkdir" "\0"
"mkdosfs" "\0"
"mke2fs" "\0"
"mkfifo" "\0"
"mkfs.ext2" "\0"
"mkfs.vfat" "\0"
"mknod" "\0"
"mkpasswd" "\0"
"mkswap" "\0"
"mktemp" "\0"
"modinfo" "\0"
"modprobe" "\0"
"more" "\0"
"mount" "\0"
"mpstat" "\0"
"mv" "\0"
"nbd-client" "\0"
"nc" "\0"
"netstat" "\0"
"nice" "\0"
"nmeter" "\0"
"nslookup" "\0"
"od" "\0"
"openvt" "\0"
"passwd" "\0"
"patch" "\0"
"pidof" "\0"
"ping" "\0"
"ping6" "\0"
"pivot_root" "\0"
"pmap" "\0"
"poweroff" "\0"
"powertop" "\0"
"printf" "\0"
"ps" "\0"
"pscan" "\0"
"pstree" "\0"
"pwd" "\0"
"pwdx" "\0"
"raidautorun" "\0"
"rdate" "\0"
"readlink" "\0"
"realpath" "\0"
"reboot" "\0"
"renice" "\0"
"reset" "\0"
"resize" "\0"
"rev" "\0"
"rm" "\0"
"rmdir" "\0"
"rmmod" "\0"
"route" "\0"
"rpm" "\0"
"rpm2cpio" "\0"
"run-parts" "\0"
"runsv" "\0"
"runsvdir" "\0"
"rx" "\0"
"sed" "\0"
"seq" "\0"
"setconsole" "\0"
"setkeycodes" "\0"
"setserial" "\0"
"setsid" "\0"
"setuidgid" "\0"
"sh" "\0"
"sha1sum" "\0"
"showkey" "\0"
"slattach" "\0"
"sleep" "\0"
"smemcap" "\0"
"softlimit" "\0"
"sort" "\0"
"start-stop-daemon" "\0"
"stat" "\0"
"strings" "\0"
"stty" "\0"
"su" "\0"
"sv" "\0"
"svlogd" "\0"
"swapoff" "\0"
"swapon" "\0"
"switch_root" "\0"
"sync" "\0"
"sysctl" "\0"
"syslogd" "\0"
"tail" "\0"
"tar" "\0"
"tee" "\0"
"telnet" "\0"
"test" "\0"
"tftp" "\0"
"time" "\0"
"top" "\0"
"touch" "\0"
"tr" "\0"
"traceroute" "\0"
"traceroute6" "\0"
"true" "\0"
"tty" "\0"
"ttysize" "\0"
"ubimkvol" "\0"
"ubirmvol" "\0"
"ubirsvol" "\0"
"ubiupdatevol" "\0"
"udhcpc" "\0"
"umount" "\0"
"uname" "\0"
"unexpand" "\0"
"uniq" "\0"
"unix2dos" "\0"
"unxz" "\0"
"unzip" "\0"
"uptime" "\0"
"users" "\0"
"usleep" "\0"
"vconfig" "\0"
"vi" "\0"
"watchdog" "\0"
"wc" "\0"
"wget" "\0"
"which" "\0"
"whoami" "\0"
"whois" "\0"
"xargs" "\0"
"xz" "\0"
"xzcat" "\0"
"yes" "\0"
"zcat" "\0"
"zcip" "\0"
;

#ifndef SKIP_applet_main
int (*const applet_main[])(int argc, char **argv) = {
test_main,
test_main,
addgroup_main,
adduser_main,
ar_main,
arp_main,
arping_main,
ash_main,
awk_main,
base64_main,
basename_main,
blkid_main,
blockdev_main,
bootchartd_main,
bunzip2_main,
bunzip2_main,
cat_main,
chgrp_main,
chmod_main,
chown_main,
chpasswd_main,
chpst_main,
chroot_main,
chrt_main,
chvt_main,
clear_main,
cmp_main,
cp_main,
cpio_main,
crond_main,
crontab_main,
cryptpw_main,
cttyhack_main,
cut_main,
date_main,
dc_main,
dd_main,
deallocvt_main,
deluser_main,
deluser_main,
depmod_main,
df_main,
dirname_main,
dmesg_main,
hostname_main,
dos2unix_main,
du_main,
echo_main,
grep_main,
eject_main,
env_main,
chpst_main,
chpst_main,
expand_main,
expr_main,
false_main,
freeramdisk_main,
fdformat_main,
fdisk_main,
fgconsole_main,
grep_main,
find_main,
flock_main,
free_main,
ftpgetput_main,
ftpgetput_main,
getopt_main,
getty_main,
grep_main,
id_main,
gunzip_main,
gzip_main,
halt_main,
hdparm_main,
head_main,
hexdump_main,
hostid_main,
hostname_main,
hwclock_main,
id_main,
ifconfig_main,
ifupdown_main,
ifenslave_main,
ifupdown_main,
inetd_main,
init_main,
insmod_main,
install_main,
iostat_main,
ip_main,
kill_main,
kill_main,
kill_main,
klogd_main,
less_main,
init_main,
ln_main,
logger_main,
login_main,
losetup_main,
ls_main,
lsmod_main,
lspci_main,
lsusb_main,
md5_sha1_sum_main,
mdev_main,
mesg_main,
mkdir_main,
mkfs_vfat_main,
mkfs_ext2_main,
mkfifo_main,
mkfs_ext2_main,
mkfs_vfat_main,
mknod_main,
cryptpw_main,
mkswap_main,
mktemp_main,
modinfo_main,
modprobe_main,
more_main,
mount_main,
mpstat_main,
mv_main,
nbdclient_main,
nc_main,
netstat_main,
nice_main,
nmeter_main,
nslookup_main,
od_main,
openvt_main,
passwd_main,
patch_main,
pidof_main,
ping_main,
ping6_main,
pivot_root_main,
pmap_main,
halt_main,
powertop_main,
printf_main,
ps_main,
pscan_main,
pstree_main,
pwd_main,
pwdx_main,
raidautorun_main,
rdate_main,
readlink_main,
realpath_main,
halt_main,
renice_main,
reset_main,
resize_main,
rev_main,
rm_main,
rmdir_main,
rmmod_main,
route_main,
rpm_main,
rpm2cpio_main,
run_parts_main,
runsv_main,
runsvdir_main,
rx_main,
sed_main,
seq_main,
setconsole_main,
setkeycodes_main,
setserial_main,
setsid_main,
chpst_main,
ash_main,
md5_sha1_sum_main,
showkey_main,
slattach_main,
sleep_main,
smemcap_main,
chpst_main,
sort_main,
start_stop_daemon_main,
stat_main,
strings_main,
stty_main,
su_main,
sv_main,
svlogd_main,
swap_on_off_main,
swap_on_off_main,
switch_root_main,
sync_main,
sysctl_main,
syslogd_main,
tail_main,
tar_main,
tee_main,
telnet_main,
test_main,
tftp_main,
time_main,
top_main,
touch_main,
tr_main,
traceroute_main,
traceroute6_main,
true_main,
tty_main,
ttysize_main,
ubi_tools_main,
ubi_tools_main,
ubi_tools_main,
ubi_tools_main,
udhcpc_main,
umount_main,
uname_main,
expand_main,
uniq_main,
dos2unix_main,
unxz_main,
unzip_main,
uptime_main,
who_main,
usleep_main,
vconfig_main,
vi_main,
watchdog_main,
wc_main,
wget_main,
which_main,
whoami_main,
whois_main,
xargs_main,
unxz_main,
unxz_main,
yes_main,
gunzip_main,
zcip_main,
};
#endif

const uint16_t applet_nameofs[] ALIGN2 = {
0x0000,
0x0002,
0x0005,
0x000e,
0x0016,
0x0019,
0x001d,
0x0024,
0x0028,
0x002c,
0x0033,
0x003c,
0x0042,
0x004b,
0x0056,
0x005e,
0x0064,
0x0068,
0x006e,
0x0074,
0x007a,
0x0083,
0x0089,
0x0090,
0x0095,
0x009a,
0x00a0,
0x00a4,
0x00a7,
0x00ac,
0x80b2,
0x00ba,
0x00c2,
0x00cb,
0x00cf,
0x00d4,
0x00d7,
0x00da,
0x00e4,
0x00ed,
0x00f5,
0x00fc,
0x00ff,
0x0107,
0x010d,
0x011b,
0x0124,
0x0127,
0x012c,
0x0132,
0x0138,
0x013c,
0x0143,
0x014d,
0x0154,
0x0159,
0x015f,
0x0167,
0x0170,
0x0176,
0x0180,
0x0186,
0x018b,
0x0191,
0x0196,
0x019d,
0x01a4,
0x01ab,
0x01b1,
0x01b6,
0x01bd,
0x01c4,
0x01c9,
0x01ce,
0x01d5,
0x01da,
0x01e2,
0x01e9,
0x01f2,
0x01fa,
0x01fd,
0x0206,
0x020d,
0x0217,
0x021c,
0x0222,
0x0227,
0x022e,
0x0236,
0x023d,
0x0240,
0x0245,
0x024d,
0x0256,
0x025c,
0x0261,
0x0269,
0x026c,
0x8273,
0x0279,
0x0281,
0x0284,
0x028a,
0x0290,
0x0296,
0x029d,
0x02a2,
0x02a7,
0x02ad,
0x02b5,
0x02bc,
0x02c3,
0x02cd,
0x02d7,
0x02dd,
0x02e6,
0x02ed,
0x02f4,
0x02fc,
0x0305,
0x030a,
0x0310,
0x0317,
0x031a,
0x0325,
0x0328,
0x0330,
0x0335,
0x033c,
0x0345,
0x0348,
0x834f,
0x0356,
0x035c,
0x4362,
0x4367,
0x036d,
0x0378,
0x037d,
0x0386,
0x038f,
0x0396,
0x0399,
0x039f,
0x03a6,
0x03aa,
0x03af,
0x03bb,
0x03c1,
0x03ca,
0x03d3,
0x03da,
0x03e1,
0x03e7,
0x03ee,
0x03f2,
0x03f5,
0x03fb,
0x0401,
0x0407,
0x040b,
0x0414,
0x041e,
0x0424,
0x042d,
0x0430,
0x0434,
0x0438,
0x0443,
0x044f,
0x0459,
0x0460,
0x046a,
0x046d,
0x0475,
0x047d,
0x0486,
0x048c,
0x0494,
0x049e,
0x04a3,
0x04b5,
0x04ba,
0x04c2,
0x84c7,
0x04ca,
0x04cd,
0x04d4,
0x04dc,
0x04e3,
0x04ef,
0x04f4,
0x04fb,
0x0503,
0x0508,
0x050c,
0x0510,
0x0517,
0x051c,
0x0521,
0x0526,
0x052a,
0x0530,
0x4533,
0x453e,
0x054a,
0x054f,
0x0553,
0x055b,
0x0564,
0x056d,
0x0576,
0x0583,
0x058a,
0x0591,
0x0597,
0x05a0,
0x05a5,
0x05ae,
0x05b3,
0x05b9,
0x05c0,
0x05c6,
0x05cd,
0x05d5,
0x05d8,
0x05e1,
0x05e4,
0x05e9,
0x05ef,
0x05f6,
0x05fc,
0x0602,
0x0605,
0x060b,
0x060f,
0x0614,
};

const uint8_t applet_install_loc[] ALIGN1 = {
0x33,
0x11,
0x23,
0x13,
0x13,
0x23,
0x22,
0x33,
0x11,
0x11,
0x34,
0x34,
0x33,
0x13,
0x41,
0x33,
0x31,
0x31,
0x31,
0x11,
0x12,
0x13,
0x31,
0x13,
0x31,
0x33,
0x33,
0x13,
0x31,
0x32,
0x31,
0x33,
0x33,
0x21,
0x31,
0x11,
0x22,
0x33,
0x13,
0x32,
0x22,
0x22,
0x24,
0x32,
0x11,
0x31,
0x23,
0x03,
0x31,
0x21,
0x21,
0x33,
0x23,
0x13,
0x22,
0x23,
0x12,
0x23,
0x21,
0x12,
0x11,
0x41,
0x13,
0x31,
0x33,
0x33,
0x13,
0x11,
0x32,
0x12,
0x13,
0x33,
0x31,
0x42,
0x33,
0x32,
0x33,
0x11,
0x21,
0x12,
0x13,
0x33,
0x13,
0x23,
0x13,
0x33,
0x31,
0x23,
0x31,
0x33,
0x12,
0x13,
0x31,
0x24,
0x22,
0x21,
0x32,
0x31,
0x33,
0x33,
0x13,
0x33,
0x13,
0x33,
0x44,
0x44,
0x12,
0x31,
0x33,
0x33,
0x33,
0x21,
0x21,
0x33,
0x33,
0x33,
0x33,
0x13,
0x02,
};

#define MAX_APPLET_NAME_LEN 17