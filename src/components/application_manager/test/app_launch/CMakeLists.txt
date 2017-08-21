# Copyright (c) 2016, Ford Motor Company
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

include_directories(
    ${GMOCK_INCLUDE_DIRECTORY}
    ${COMPONENTS_DIR}/application_manager/include/
    ${COMPONENTS_DIR}/application_manager/test/include/
    ${COMPONENTS_DIR}/connection_handler/include/
    ${COMPONENTS_DIR}/utils/include/
    ${COMPONENTS_DIR}/include/test
    ${COMPONENTS_DIR}/include/
)

set(LIBRARIES
    gmock
    Resumption
    jsoncpp
    SmartObjects
    ApplicationManager
)

  if (ENABLE_LOG)
    list(APPEND LIBRARIES log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
    list(APPEND LIBRARIES apr-1 -L${APR_LIBS_DIRECTORY})
    list(APPEND LIBRARIES aprutil-1 -L${APR_UTIL_LIBS_DIRECTORY})
    list(APPEND LIBRARIES expat -L${EXPAT_LIBS_DIRECTORY})
  endif()

set(APP_LAUNCH_DATA_TEST_SOURCES
    app_launch_data_db_test.cc
    app_launch_data_json_test.cc
)

create_test("app_launch_ctrl_test" app_launch_ctrl_test.cc "${LIBRARIES}")
create_test("app_launch_data_test" "${APP_LAUNCH_DATA_TEST_SOURCES}" "${LIBRARIES}")
