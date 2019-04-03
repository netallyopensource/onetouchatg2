TOPDIR=$(shell pwd)
PATH_TO_BUILD_ROOT := ../../..

include $(PATH_TO_BUILD_ROOT)/source/include/Make_environment.inc

all: _busybox _busybox_static

install: all
	@cp busybox $(DESTDIR)/bin/busybox

busybox-1.19.4/.config: busybox_config
	@cp busybox_config busybox-1.19.4/.config
	@cd busybox-1.19.4; make oldconfig

menuconfig: busybox-1.19.4/.config
	@cd busybox-1.19.4; make menuconfig
	@cp busybox-1.19.4/.config busybox_config

_busybox: busybox-1.19.4/.config
	@cd busybox-1.19.4; rm -f busybox*; make CROSS_COMPILE=${TOOLPREFIX}; cp busybox ../busybox

_busybox_static: busybox-1.19.4/.config
	@cd busybox-1.19.4; rm -f busybox*; LDFLAGS="--static" make CROSS_COMPILE=${TOOLPREFIX}; cp busybox ../busybox_static

clean:
	@cd busybox-1.19.4; make distclean
	@rm -f busybox busybox_static

