#!/bin/bash -ev
build_program()
{
	bin/scpp $1 tmp/cc.i
	bin/scc tmp/cc.i tmp/cc.asm
	bin/asm tmp/cc.asm $2
}
if [ $# != 1 ]
then
	echo "HELP: ./build.sh <build-type>"
	echo "Available Build Types:"
	echo "arm64 x86_64"
	exit 1
fi
_LIB_=$1
mkdir -p tmp release_$_LIB_ include
cp include_$_LIB_/* include
cp include_common/* include
build_program src/mkfs_ext2.c release_$_LIB_/mkfs.ext2
build_program src/mkfs_ext3.c release_$_LIB_/mkfs.ext3
build_program src/mkfs_ext4.c release_$_LIB_/mkfs.ext4
build_program src/mkfs_fat.c release_$_LIB_/mkfs.fat
build_program src/mkfs_exfat.c release_$_LIB_/mkfs.exfat
