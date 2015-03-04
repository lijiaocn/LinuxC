#!/bin/bash

if [[ ! $# -eq 1 ]]
then
    echo "usage: $0 directory"
    exit
fi

LOCPATH=`pwd`

DstDir="${LOCPATH}/$1"

echo "The Dst Path is: $DstDir [any key continue]"
read

yum install --installroot=$DstDir -y rootfiles
yum install --installroot=$DstDir -y bash
yum install --installroot=$DstDir -y coreutils
yum install --installroot=$DstDir -y hostname
echo "Container" >"$DstDir/etc/hostname"
echo 'PS1="[\u@Container \W]\\$ "' >>"$DstDir/root/.bashrc"
