cmd_libbb/makedev.o := powerpc-timesys-linux-gnuspe-gcc -Wp,-MD,libbb/.makedev.o.d   -std=gnu99 -Iinclude -Ilibbb  -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D"BB_VER=KBUILD_STR(1.19.4)" -DBB_BT=AUTOCONF_TIMESTAMP  -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wunused-function -Wunused-value -Wmissing-prototypes -Wmissing-declarations -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -Os     -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(makedev)"  -D"KBUILD_MODNAME=KBUILD_STR(makedev)" -c -o libbb/makedev.o libbb/makedev.c

deps_libbb/makedev.o := \
  libbb/makedev.c \
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
  /home/dmcdonald/lantisdev/lantis-top/lantis/toolchain/bin/../../toolchain/usr/include/sys/sysmacros.h \

libbb/makedev.o: $(deps_libbb/makedev.o)

$(deps_libbb/makedev.o):
