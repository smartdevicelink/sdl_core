#!/usr/bin/env bash

# Copyright (c) 2013, Ford Motor Company
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following
# disclaimer in the documentation and/or other materials provided with the
# distribution.
#
# Neither the name of the Ford Motor Company nor the names of its contributors
# may be used to endorse or promote products derived from this software
# without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

declare -i TEST_PASSED=0
declare -i TEST_FAILED=0
FAILED_TEST_NUMBERS=""
RES_CODE=0
KEY_VALUE_LOCAL="any"
if [ "$KEY_VALUE" ]; then
  KEY_VALUE_LOCAL=$KEY_VALUE
fi

function run_test() {
  echo "Run test number #"$(($TEST_PASSED+$TEST_FAILED+1)) $1
  sh -c "$2" 2>/dev/null 1>/dev/null
  if [ $? == 0 ]; then
    echo "Test PASSED"
    TEST_PASSED=$TEST_PASSED+1
  else
    echo "Test FAILED"
    RES_CODE=1
    TEST_FAILED=$TEST_FAILED+1
    FAILED_TEST_NUMBERS=$FAILED_TEST_NUMBERS$(($TEST_PASSED+$TEST_FAILED))";"
  fi
  echo "==========================="
}

echo "==========================="
run_test "GNU C++ compiler" 				"dpkg-query -W -f='${Status}\n' g++"
run_test "BlueZ Bluetooth protocol stack implementation" 	"dpkg-query -W -f='${Status}\n' libbluetooth3 libbluetooth-dev"
run_test "log4cxx library" 				"dpkg-query -W -f='${Status}\n' liblog4cxx10 liblog4cxx10-dev"
run_test "Chromium browser" 				"dpkg-query -W -f='${Status}\n' chromium-browser"
run_test "Chromium ffmpeg codec" 				"dpkg-query -W -f='${Status}\n' chromium-codecs-ffmpeg-extra"
run_test "Pulseaudio development files" 			"dpkg-query -W -f='${Status}\n' libpulse-dev"
run_test "Avahi client development files" 			"dpkg-query -W -f='${Status}\n' libavahi-client-dev"
run_test "libudev development files" 			"dpkg-query -W -f='${Status}\n' libudev-dev"
run_test "BlueZ tools" 					"dpkg-query -W -f='${Status}\n' bluez-tools"
run_test "Gstreamer development files" 			"dpkg-query -W -f='${Status}\n'  gstreamer1.0-alsa gstreamer1.0-libav gstreamer1.0-libav-dbg gstreamer1.0-pulseaudio gstreamer1.0-tools gstreamer1.0-x libgstreamer1.0-0 libgstreamer1.0-0-dbg libgstreamer1.0-dev libglib2.0-0 libglib2.0-bin libglib2.0-data libglib2.0-dev"
run_test "Cmake build system" 				"dpkg-query -W -f='${Status}\n' cmake"
run_test "USB permissions" 				"grep \"SUBSYSTEM==usb, GROUP=users, MODE=0666\" /etc/udev/rules.d/90-usbpermission.rules 2>/dev/null | wc -l"

if [[ $KEY_VALUE_LOCAL =~ "-a" ]]; then
  run_test "Qt 5.1.0 (additional for -a || -qt)" 					"./FindQt.sh -v 5.1.0"
  run_test "OpenGL development files (additional for -a || -qnx)" 			"dpkg-query -W -f='${Status}\n' libgl1-mesa-dev"
  run_test "Qt 4.8.5 (additional for (-a && -qt) || (-qnx && -qt))" 			"./FindQt.sh -v 4.8.5"
  run_test "dbus-1.7.8" 								"test -e '${QNX_TARGET}/usr/bin/dbus-daemon'"
  run_test "expat-2.1.0" 								"test -e '${QNX_TARGET}/usr/lib/libexpat.a'"
  run_test "QNX SDP 6.5.0 SP1 cross platform tools (additional for -a || -qnx)" 	"test -d '${QNX_TARGET}/'"
  run_test "SSH server (additional for -a || -qnx)"					"dpkg-query -W -f='${Status}\n' openssh-server ssh"
  run_test "log4xx for QNX" 							"test -d '${QNX_TARGET}/usr/include/log4cxx'"
  run_test "Subversion (additional for -a)" 					"dpkg-query -W -f='${Status}\n' subversion"
  run_test "Git (additional for -a)" 						"dpkg-query -W -f='${Status}\n' git"
  run_test "Doxygen (additional for -a)" 						"dpkg-query -W -f='${Status}\n' doxygen"
  run_test "Graphviz (additional for -a)" 						"dpkg-query -W -f='${Status}\n' graphviz"
  run_test "MSC generator (additional for -a)" 					"dpkg-query -W -f='${Status}\n' mscgen"
else
  if [[ $KEY_VALUE_LOCAL =~  "-qt" ]]; then
    if [[ $KEY_VALUE_LOCAL =~  "-qnx" ]] ; then
      run_test "Qt 4.8.5 (additional for (-a && -qt) || (-qnx && -qt))" 		"./FindQt.sh -v 4.8.5"
      run_test "dbus-1.7.8" 							"test -e '${QNX_TARGET}/usr/bin/dbus-daemon'"
      run_test "expat-2.1.0" 							"test -e '${QNX_TARGET}/usr/lib/libexpat.a'"
    else
      run_test "Qt 5.1.0 (additional for -a || -qt)" 				"./FindQt.sh -v 5.1.0"
    fi
  fi
  if [[ $KEY_VALUE_LOCAL =~ "-qnx" ]]; then
    run_test "QNX SDP 6.5.0 SP1 cross platform tools (additional for -a || -qnx)" 	"test -d '${QNX_TARGET}/'"
    run_test "SSH server (additional for -a || -qnx)" 				"dpkg-query -W -f='${Status}\n' openssh-server ssh"
    run_test "log4xx for QNX"							"test -d '${QNX_TARGET}/usr/include/log4cxx/'"
  fi
fi


if [ $FAILED_TEST_NUMBERS ]; then
    FAILED_TEST_NUMBERS="[ "$FAILED_TEST_NUMBERS" ]"
fi

echo "Total test run :" $(($TEST_PASSED + $TEST_FAILED))
echo "Passed         :" $TEST_PASSED
echo "FAILED         :" $TEST_FAILED $FAILED_TEST_NUMBERS
exit $RES_CODE	

