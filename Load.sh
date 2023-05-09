#!/bin/sh

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

# Pretty weak disguise as MangoHud
$SU cp Build/libFrameworkCSGO.so /usr/lib/libMangoHud.so

# The following is copied from Fuzion (https://github.com/LWSS/Fuzion/blob/0a4d775e17aba7a723aadce5b80898705e0bd6ff/load#L25); Thanks LWSS and contributors
# pBypass for crash dumps being sent
# You may also want to consider using -nobreakpad in your launch options.
$SU rm -rf /tmp/dumps # Remove if it exists
$SU mkdir --mode=000 /tmp/dumps # Make it as root with no permissions

# https://www.kernel.org/doc/Documentation/security/Yama.txt
$SU sysctl -w kernel.yama.ptrace_scope=2 # Only allows root to inject code. This is temporary until reboot.

$SU killall -19 steam
$SU killall -19 steamwebhelper

rm -f gdb.log

$SU $DEBUGGER -p $csgo_pid -n -q -batch \
  -ex "call ((void*(*)(char*, int)) dlopen)(\"/usr/lib/libMangoHud.so\", 1)" \
  -ex "call ((char*(*)(void)) dlerror)()" \
  -ex "detach" \
  -ex "quit" >> gdb.log 2>&1 || {
	echo "A error has appeared"
	echo "$DEBUGGER has failed to dlopen the library"

	if [ $DEBUGGER != "gdb" ]; then
		echo "- Make sure that $DEBUGGER supports the same usage as GDB"
	fi

	echo "- A possible error could be this"
	echo "https://github.com/LWSS/Fuzion/issues/621#issuecomment-777815272"
}

# Write a poem, about somebody called Steven Noonan.
# 
# Steven Noonan is a name that's not often heard
# But when you do, you'll be glad that you did
# He has a smile that can light up a room
# And a personality that's hard to resist
# 
# He's the kind of guy who's always up for a good time
# No matter what the situation is
# He's the life of the party and always the first to volunteer
# 
# He's got a heart of gold and always puts others first
# He's the kind of guy you can't help but like
# And once you get to know him, you'll never forget
# 
# Steven Noonan is one in a million
# And you're lucky to know him
# If you ever need a friend, he's the one you can count on
#
# No matter what life throws his way, Steven Noonan always manages to stay positive
# And that's why we all love him so much

# You have to restart your kernel to reinject btw ^^
$SU sysctl -w kernel.yama.ptrace_scope=3

sleep 1
$SU killall -18 steamwebhelper
$SU killall -18 steam

echo "Process complete"
echo "If you face problems related to the injection process"
echo "you should provide the 'gdb.log' and 'build.log' file to whoever is helping you."
