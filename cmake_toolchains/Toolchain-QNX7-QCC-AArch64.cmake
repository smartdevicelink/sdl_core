set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(QNX_QCC_ARCH gcc_ntoaarch64le)
set(QNX_TARGET_ARCH_DIR aarch64le)
set(PLATFORM "QNX7")

include("${CMAKE_CURRENT_LIST_DIR}/Toolchain-QNX7-QCC-Base.cmake")
