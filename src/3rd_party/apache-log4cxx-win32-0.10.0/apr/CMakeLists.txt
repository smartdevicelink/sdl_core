

include_directories (
  ./
  ./include
  ./include/arch
  ./include/arch/win32
  ./include/arch/unix
)


set (APR_SOURCES
   user/win32/userinfo.c
user/win32/groupinfo.c
time/win32/timestr.c
time/win32/time.c
time/win32/access.c
threadproc/win32/threadpriv.c
threadproc/win32/thread.c
threadproc/win32/signals.c
threadproc/win32/proc.c
tables/apr_tables.c
tables/apr_hash.c
strings/apr_strtok.c
strings/apr_strnatcmp.c
strings/apr_strings.c
strings/apr_snprintf.c
strings/apr_fnmatch.c
strings/apr_cpystrn.c
shmem/win32/shm.c
random/unix/sha2_glue.c
random/unix/sha2.c
random/unix/apr_random.c
passwd/apr_getpass.c
network_io/win32/sockopt.c
network_io/win32/sockets.c
network_io/unix/sockaddr.c
network_io/win32/sendrecv.c
network_io/unix/multicast.c
poll/unix/select.c
network_io/unix/inet_pton.c
network_io/unix/inet_ntop.c
mmap/win32/mmap.c
mmap/unix/common.c
misc/unix/version.c
misc/win32/utf8.c
misc/win32/start.c
misc/win32/rand.c
misc/unix/otherchild.c
misc/win32/misc.c
misc/win32/internal.c
misc/unix/getopt.c
misc/unix/errorcodes.c
misc/win32/env.c
misc/win32/charset.c
memory/unix/apr_pools.c
locks/win32/thread_rwlock.c
locks/win32/thread_mutex.c
locks/win32/thread_cond.c
locks/win32/proc_mutex.c
file_io/unix/tempdir.c
file_io/win32/seek.c
file_io/win32/readwrite.c
file_io/win32/pipe.c
file_io/win32/open.c
file_io/unix/mktemp.c
file_io/unix/fullrw.c
file_io/win32/flock.c
file_io/win32/filesys.c
file_io/win32/filestat.c
file_io/unix/filepath_util.c
file_io/win32/filepath.c
file_io/win32/filedup.c
file_io/unix/fileacc.c
file_io/win32/dir.c
file_io/unix/copy.c
dso/win32/dso.c
atomic/win32/apr_atomic.c

)



add_library("apr-1" ${APR_SOURCES})

target_link_libraries("apr-1"  winsock coredll)
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
#target_link_libraries(${PROJECT} ${LIBRARIES})
#link_library
#target_link_libraries("lib_msp_vr"  ${soxr-lsr})
endif()
