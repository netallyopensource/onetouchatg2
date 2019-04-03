cmd_libbb/systemd_support.o := powerpc-timesys-linux-gnuspe-gcc -Wp,-MD,libbb/.systemd_support.o.d   -std=gnu99 -Iinclude -Ilibbb  -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D"BB_VER=KBUILD_STR(1.19.4)" -DBB_BT=AUTOCONF_TIMESTAMP  -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wunused-function -Wunused-value -Wmissing-prototypes -Wmissing-declarations -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -Os     -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(systemd_support)"  -D"KBUILD_MODNAME=KBUILD_STR(systemd_support)" -c -o libbb/systemd_support.o libbb/systemd_support.c

deps_libbb/systemd_support.o := \
  libbb/systemd_support.c \
    $(wildcard include/config/feature/systemd.h) \
  include/libbb.h \
    $(wildcard include/config/selinux.h) \
    $(wildcard include/config/feature/utmp.h) \
    $(wildcard include/config/locale/support.h) \
    $(wildcard include/config/feature/shadowpasswds.h) \
    $(wildcard include/config/use/bb/shadow.h) \
    $(wildcard include/config/use/bb/pwd/grp.h) \
    $(wildcard include/config/lfs.h) \
    $(wildcard include/config/feature/buffers/go/on/stack.h) \
    $(wildcard include/config/feature/buffers/go/in/bss.h) \
    $(wildcard include/config/feature/ipv6.h) \
    $(wildcard include/config/feature/seamless/lzma.h) \
    $(wildcard include/config/feature/seamless/bz2.h) \
    $(wildcard include/config/feature/seamless/gz.h) \
    $(wildcard include/config/feature/seamless/z.h) \
    $(wildcard include/config/feature/check/names.h) \
    $(wildcard include/config/feature/prefer/applets.h) \
    $(wildcard include/config/long/opts.h) \
    $(wildcard include/config/feature/getopt/long.h) \
    $(wildcard include/config/feature/pidfile.h) \
    $(wildcard include/config/feature/syslog.h) \
    $(wildcard include/config/feature/individual.h) \
    $(wildcard include/config/echo.h) \
    $(wildcard include/config/printf.h) \
    $(wildcard include/config/test.h) \
    $(wildcard include/config/kill.h) \
    $(wildcard include/config/chown.h) \
    $(wildcard include/config/ls.h) \
    $(wildcard include/config/xxx.h) \
    $(wildcard include/config/route.h) \
    $(wildcard include/config/feature/hwib.h) \
    $(wildcard include/config/desktop.h) \
    $(wildcard include/config/feature/crond/d.h) \
    $(wildcard include/config/use/bb/crypt.h) \
    $(wildcard include/config/feature/adduser/to/group.h) \
    $(wildcard include/config/feature/del/user/from/group.h) \
    $(wildcard include/config/ioctl/hex2str/error.h) \
    $(wildcard include/config/feature/editing.h) \
    $(wildcard include/config/feature/editing/history.h) \
    $(wildcard include/config/feature/editing/savehistory.h) \
    $(wildcard include/config/feature/tab/completion.h) \
    $(wildcard include/config/feature/username/completion.h) \
    $(wildcard include/config/feature/editing/vi.h) \
    $(wildcard include/config/pmap.h) \
    $(wildcard include/config/feature/show/threads.h) \
    $(wildcard include/config/feature/ps/additional/columns.h) \
    $(wildcard include/config/feature/topmem.h) \
    $(wildcard include/config/feature/top/smp/process.h) \
    $(wildcard include/config/killall.h) \
    $(wildcard include/config/pgrep.h) \
    $(wildcard include/config/pkill.h) \
    $(wildcard include/config/pidof.h) \
    $(wildcard include/config/sestatus.h) \
    $(wildcard include/config/unicode/support.h) \
    $(wildcard include/config/feature/mtab/support.h) \
    $(wildcard include/config/feature/devfs.h) \
  include/platform.h \
    $(wildcard include/config/werror.h) \
    $(wildcard include/config/big/endian.h) \
    $(wildcard include/config/little/endian.h) \
    $(wildcard include/config/nommu.h) \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../lib/gcc/powerpc-timesys-linux-gnuspe/4.4.5/include-fixed/limits.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../lib/gcc/powerpc-timesys-linux-gnuspe/4.4.5/include-fixed/syslimits.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/limits.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/features.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/predefs.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/sys/cdefs.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/wordsize.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/gnu/stubs.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/gnu/stubs-32.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/posix1_lim.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/local_lim.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/linux/limits.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/posix2_lim.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/xopen_lim.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/stdio_lim.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/byteswap.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/byteswap.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/endian.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/endian.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/stdint.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/wchar.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../lib/gcc/powerpc-timesys-linux-gnuspe/4.4.5/include/stdbool.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/unistd.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/posix_opt.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/environments.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/types.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/typesizes.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../lib/gcc/powerpc-timesys-linux-gnuspe/4.4.5/include/stddef.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/confname.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/getopt.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/ctype.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/xlocale.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/dirent.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/dirent.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/errno.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/errno.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/linux/errno.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/asm/errno.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/asm-generic/errno.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/asm-generic/errno-base.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/fcntl.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/fcntl.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/sys/types.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/time.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/sys/select.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/select.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/sigset.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/time.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/sys/sysmacros.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/pthreadtypes.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/uio.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/stat.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/inttypes.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/netdb.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/netinet/in.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/sys/socket.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/sys/uio.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/socket.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/sockaddr.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/asm/socket.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/asm/sockios.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/in.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/rpc/netdb.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/siginfo.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/netdb.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/setjmp.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/setjmp.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/signal.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/signum.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/sigaction.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/sigcontext.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/asm/sigcontext.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/asm/ptrace.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/sigstack.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/sys/ucontext.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/sigthread.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/stdio.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/libio.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/_G_config.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/wchar.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../lib/gcc/powerpc-timesys-linux-gnuspe/4.4.5/include/stdarg.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/sys_errlist.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/stdlib.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/waitflags.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/waitstatus.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/alloca.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/string.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/sys/poll.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/poll.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/sys/ioctl.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/ioctls.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/asm/ioctls.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/asm/ioctl.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/asm-generic/ioctl.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/ioctl-types.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/termios.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/termios.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/sys/ttydefaults.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/sys/mman.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/mman.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/sys/stat.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/sys/time.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/sys/wait.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/sys/resource.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/resource.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/sys/param.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/linux/param.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/asm/param.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/mntent.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/paths.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/sys/statfs.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/statfs.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/utmp.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/bits/utmp.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/pwd.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/grp.h \
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/arpa/inet.h \
  include/xatonum.h \

libbb/systemd_support.o: $(deps_libbb/systemd_support.o)

$(deps_libbb/systemd_support.o):
