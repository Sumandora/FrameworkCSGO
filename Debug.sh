#!/bin/sh

echo "Building Debug-Binary"
mkdir -p Build-Debug

cmake -B Build-Debug -D CMAKE_BUILD_TYPE=Debug -D CMAKE_CXX_FLAGS_RELEASE=$CXXFLAGS || exit 1
make $MAKEOPTS -C Build-Debug || exit 1

# Set the DEBUGGER variable on the cmdline to use lldb or any debugger
# Make sure it supports the following commands
DEBUGGER="${DEBUGGER:=gdb}"
echo "Using '$DEBUGGER' to inject"
if ! [ -x "$(command -v $DEBUGGER)" ]; then
	echo "$DEBUGGER does not exist"
	echo "Install it or set the DEBUGGER variable to a replacement"
	exit 1
fi

# Set the SU variable on the cmdline to use e.g. doas
SU="${SU:=sudo}"
if ! [ $(id -u) = 0 ]; then
	echo "Using '$SU' for upgrading privileges"
	if ! [ -x "$(command -v $SU)" ]; then
		echo "$SU does not exist"
		echo "Install it or set the SU variable to a replacement"
		exit 1
	fi
else
	SU=""
fi

csgo_pid=$(pidof csgo_linux64)
if [ -z "$csgo_pid" ]; then
	echo "CS:GO can't be found, is the game running?"
	exit 1
fi

lib_name="lib$(cat ProjectName).so"
$SU cp Build-Debug/$lib_name /usr/lib64/

# https://www.kernel.org/doc/Documentation/security/Yama.txt
$SU sysctl -w kernel.yama.ptrace_scope=2 # Only allows root to inject code. This is temporary until reboot.

$SU $DEBUGGER -p $csgo_pid \
	-ex "call ((void*(*)(char*, int)) dlopen)(\"/usr/lib64/$lib_name\", 1)" \
	-ex "continue"
