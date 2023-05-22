#!/bin/sh

error () {
	echo "A error appeared"
	echo "A build.log file has been generated, read it for more details"
	echo "If you are unable to figure out the problem and solve it yourself,"
	echo "you can contact a support instance."
	echo "Supply the build.log file to speed up the support process"
	echo "It is stored in your /tmp/ folder"
	exit 1
}

log_and_check() {
    tmpfile=$(mktemp)
    if "$@" >"$tmpfile" 2>&1; then
        cat "$tmpfile" >> /tmp/build.log
    else
        cat "$tmpfile" >> /tmp/build.log
        rm "$tmpfile"
        error
    fi
    rm "$tmpfile"
}

echo "Building..."
echo "This can take up to a couple minutes on slow hardware"

rm -f build.log
rm -rf Build
mkdir Build

# Change the language for the log
# I don't wanna sit there with a russian to english translation,
# trying to solve some compilation bug >:(
# Also allow other compilers (e.g. clang) to be used
LANG=en log_and_check cmake -B Build -D CMAKE_BUILD_TYPE=Release -D CMAKE_CXX_FLAGS_RELEASE="$CXXFLAGS"
LANG=en log_and_check make $MAKEOPTS -C Build

log_and_check strip -x -s Build/lib$(cat ProjectName).so

echo "The ELF-Binary has been built in the \"Build\"-directory"
