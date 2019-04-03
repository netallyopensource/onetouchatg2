#!/bin/sh
#
# mdev mount.sh
#
# Called from mdev
#
# Attempt to mount any added block devices 
# and unmount any removed devices, as well
# as clean up the mount points.
#
# 2010-03-24 (JJW):
#    Modified script to autodetect whether or not add or remove
#    should occur

MOUNT="/bin/mount"
UMOUNT="/bin/umount"
name=$1
DEVNAME="/dev/$name"

# Cleanup should be atomic - we only want to delete dirs that were
# auto-created.
cleanup() {
	test -e "/tmp/.automount-$name" && rm_dir "/media/$name"
	rm /tmp/.automount-$name
}

automount() {	
	# We check to see if the directory already exists.  If it
	# does, we don't want to delete it when we cleanup.
	! test -d "/media/$name" && \
	  mkdir -p "/media/$name" && \
	  touch "/tmp/.automount-$name"

	if ! $MOUNT -t auto -o sync $DEVNAME "/media/$name"
	then
		# This occurs if a mount was manually removed.
		# We should clean up after ourselves.
		cleanup
	else
		echo "Auto-mount of [/media/$name] successful" > /dev/console
	fi
}
	
rm_dir() {
	# We do not want to rm -r populated directories
	if test "`find "$1" | wc -l | tr -d " "`" -lt 2 -a -d "$1"
	then
		! test -z "$1" && rm -r "$1"
	else
		logger "mount.sh/automount" "Not removing non-empty directory [$1]"
	fi
}

# If device is already mounted, remove it.  Otherwise, add it.
# This is naive- mounts could have been manually removed.  In that case,
# we still "add", but clean up when the mount fails.
if cat /proc/mounts | awk '{print $1}' | grep -q "^$DEVNAME$"; then
	ACTION="remove"
else
	ACTION="add"
fi

if [ $ACTION == "add" ] && [ -n "$DEVNAME" ]; then
	if [ -x $MOUNT ]; then
    		$MOUNT $DEVNAME 2> /dev/null
	fi

	# If the device isn't mounted at this point, it isn't configured in fstab
	cat /proc/mounts | awk '{print $1}' | grep -q "^$DEVNAME$" || automount 
	
fi

if [ $ACTION == "remove" ] && [ -x "$UMOUNT" ] && [ -n "$DEVNAME" ]; then
	for mnt in `cat /proc/mounts | grep "$DEVNAME" | cut -f 2 -d " " `
	do
		$UMOUNT $mnt
	done
	
	# Clean up after ourselves
	cleanup
fi
