#!/bin/bash

echo -e "BUILDING \e[1;32mNETZHAUT-INSTALLER\e[0m"
echo 

DIR="$(dirname $(dirname $( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )))"
gcc -std=gnu99 -no-pie -o$DIR/bin/NhInstaller -lpthread -ldl -lX11 -lX11-xcb -lXcursor -lxkbcommon -lxkbcommon-x11 \
$DIR/src/bin/NhInstaller/Binaries.c \
$DIR/src/bin/NhInstaller/Documents.c \
$DIR/src/bin/NhInstaller/Main.c \
$DIR/src/bin/NhInstaller/Download.c \
$DIR/src/bin/NhInstaller/Util.c \
$DIR/src/bin/NhInstaller/X11.c \
$DIR/src/bin/NhInstaller/GUI.c \
$DIR/src/bin/NhInstaller/Libraries.c \
$DIR/src/bin/NhInstaller/Objects.c \
$DIR/src/bin/NhInstaller/Message.c \
$DIR/src/bin/NhInstaller/Unicode.c \
$DIR/src/bin/NhInstaller/WebIDL.c \
$DIR/src/bin/NhInstaller/Common/Result.c \

if [ $? = 0 ]; then
    echo
    echo -e "BUILD $DIR/bin/\e[1;32mNhInstaller\e[0m"
else
    echo
    echo -e "BUILD \e[1;31mFAILURE\e[0m"
fi
