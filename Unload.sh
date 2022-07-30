#!/bin/bash

# Set the DEBUGGER variable on the cmdline to use lldb or any debugger
# Make sure it supports the following commands
DEBUGGER="${DEBUGGER:=gdb}"
echo "Using '$DEBUGGER' to unload"
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

$SU killall -19 steam
$SU killall -19 steamwebhelper

rm -f gdb.log

$SU $DEBUGGER -p $csgo_pid -n -q -batch \
  -ex "set \$library = ((void*(*)(char*, int)) dlopen)(\"/usr/lib/libMangoHud.so\", 6)" \
  -ex "set \$dlclose = (int(*)(void*)) dlclose" \
  -ex "call \$dlclose(\$library)" \
  -ex "call \$dlclose(\$library)" >> gdb.log 2>&1 || {
	echo "A error has appeared"
	echo "$DEBUGGER has failed to dlopen/dlclose the library"

	if [ $DEBUGGER != "gdb" ]; then
		echo "- Make sure that $DEBUGGER supports the same usage as GDB"
	fi
	
	echo "- A possible error could be this"
	echo "https://github.com/LWSS/Fuzion/issues/621#issuecomment-777815272"
}

sleep 1
$SU killall -18 steamwebhelper
$SU killall -18 steam

echo "Process complete"
echo "If you face problems related to the unload process"
echo "you should provide the 'gdb.log' file to whoever is helping you."
