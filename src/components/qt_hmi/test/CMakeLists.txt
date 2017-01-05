# Copyright (c) 2014, Ford Motor Company
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


if(BUILD_TESTS)

cmake_minimum_required(VERSION 2.8.11)

set(QT_HMI_TESTS_LIST 
    ${COMPONENTS_DIR}/qt_hmi/test/src/tst_AddCommand.qml
    ${COMPONENTS_DIR}/qt_hmi/test/src/tst_AddSubMenu.qml
    ${COMPONENTS_DIR}/qt_hmi/test/src/tst_DeleteCommand.qml
    ${COMPONENTS_DIR}/qt_hmi/test/src/tst_ScrollableMessage.qml
    ${COMPONENTS_DIR}/qt_hmi/test/src/tst_SetMediaClockTimer.qml
    ${COMPONENTS_DIR}/qt_hmi/test/src/tst_Show.qml
    ${COMPONENTS_DIR}/qt_hmi/test/src/tst_Slider.qml
    ${COMPONENTS_DIR}/qt_hmi/test/readme.txt
)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CONFIG
    warn_on 
    qmltestcase
)

set(SOURCES
    qt_hmi_test.cc
)

set(target qt_hmi_test)

create_test("qt_hmi_test" "${SOURCES}" "")
qt5_use_modules(${target} Core Qml Quick QuickTest)

foreach( file_i ${QT_HMI_TESTS_LIST})
    file(COPY ${file_i} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach( file_i )

endif()