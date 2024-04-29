#!/bin/bash -ev
if [ ! $CC ]
then
	CC=gcc
fi
if [ ! $LD ]
then
	LD=ld
fi
if [ ! $OBJCOPY ]
then
	OBJCOPY=objcopy
fi
if [ ! $CFLAGS ]
then
	CFLAGS=-Os\ -nostdlib\ -fno-stack-protector\ -fcf-protection=none\ -fno-builtin
fi
CC=$CROSS_COMPILE$CC
LD=$CROSS_COMPILE$LD
OBJCOPY=$CROSS_COMPILE$OBJCOPY
mkdir -p tmp release
for fs in exfat ext2 ext4 fat
do
	$CC -c src/mkfs_$fs.c -o tmp/mkfs_$fs.o $CFLAGS
	$LD tmp/mkfs_$fs.o -o tmp/mkfs.$fs -Ttext 0x100b0 -z max-page-size=4096
	$OBJCOPY -R .comment -R .note.gnu.property -R .eh_frame -S tmp/mkfs.$fs release/mkfs.$fs
done
