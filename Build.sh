#!/bin/bash

error () {
	echo "A error appeared"
	echo "A build.log file has been generated, read it for more details"
	echo "If you are unable to figure out the problem and solve it yourself,"
	echo "you can contact a support instance."
	echo "Supply the build.log file to speed up the support process"
	exit 1
}

echo "Building..."
echo "This can take up to a couple minutes on slow hardware"

rm -f build.log
rm -rf Build
mkdir Build

if [ -z "$MAKEOPTS" ]; then
	# Love for Gentoo ^^
	# Other-Distro users can simply supply MAKEOPTS themselves

	# I would love to also add the CXXFLAGS, but I expect many users to have hardening flags set,
	# which would lead to many users ending up always suppling other CXXFLAGS anyway
	# The correct way of implementing this, is probably using an EBuild, so everybody can set individual flags for this package
	# But that is overkill for now
	
	MAKE_CONF=/etc/portage/make.conf
	if test -f "$MAKE_CONF"; then
		source $MAKE_CONF
	fi
fi

# Change the language for the log
# I don't wanna sit there with a russian to english translation,
# trying to solve some compilation bug >:(
# Also allow other compilers (e.g. clang) to be used
LANG=en cmake -B Build -D CMAKE_BUILD_TYPE=Release . >> build.log 2>&1 || error
LANG=en make $MAKEOPTS -C Build >> build.log 2>&1 || error

strip -x -s Build/libFrameworkCSGO.so >> build.log 2>&1 || error
patchelf --set-soname "libMangoHud.so" Build/libFrameworkCSGO.so >> build.log 2>&1 || error # May be unneeded because of the following line?
sed -i 's/libFrameworkCSGO.so/                   /' Build/libFrameworkCSGO.so >> build.log 2>&1 || error # This should never happen; We are not allowed to change the file size

echo "The ELF-Binary has been built in the \"Build\"-directory"
