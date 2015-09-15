

include_directories (
  ./
  ./include
)


set (APR_SOURCES
   atomic/win32/apr_atomic.c
   dso/win32/dso.c
   file_io/win32/dir.c
   file_io/win32/filedup.c
   file_io/win32/filepath.c
   file_io/win32/filesys.c
   file_io/win32/flock.c
   file_io/win32/open.c
   file_io/win32/pipe.c
   file_io/win32/readwrite.c
   file_io/win32/seek.c
   locks/win32/proc_mutex.c
   locks/win32/thread_cond.c
   locks/win32/thread_mutex.c
   locks/win32/thread_rwlock.c
   memory/win32/apr_tools.c
   misc/win32/apr_app.c
   misc/win32/charset.c
   misc/win32/errorcodes.c
   misc/win32/getopt.c
   misc/win32/internal.c
   misc/win32/misc.c
   misc/win32/otherchild.c
   misc/win32/rand.c
   misc/win32/start.c
   misc/win32/utf8.c
   misc/win32/version.c
   mmap/win32/common.c
   mmap/win32/mmap.c
   network_io/win32/inet_ntop.c
   network_io/win32/inet_pton.c
   network_io/win32/multicast.c
   network_io/win32/select.c
   network_io/win32/sendrecv.c
   network_io/win32/sockaddr.c
   network_io/win32/sockets.c
   network_io/win32/sockopt.c
   passwd/win32/apr_getpass.c
   random/win32/apr_random.c
   random/win32/sha2.c
   random/win32/sha2_glue.c
   shmem/win32/shm.c
   strings/win32/apr_cpystrn.c
   strings/win32/apr_fnmatch.c
   strings/win32/apr_snprintf.c
   strings/win32/apr_strings.c
   strings/win32/apr_strnatcmp.c
   strings/win32/apr_shrtok.c
   strings/win32/apr_hash.c
   strings/win32/apr_tables.c
   strings/win32/proc.c
   strings/win32/signals.c
   strings/win32/thread.c
   strings/win32/threadpriv.c
   time/win32/access.c
   time/win32/time.c
   time/win32/timestr.c
   user/win32/groupinfo.c
   user/win32/userinfo.c
)



add_library("apr-1" ${APR_SOURCES})

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
#target_link_libraries(${PROJECT} ${LIBRARIES})
#link_library
#target_link_libraries("lib_msp_vr"  ${soxr-lsr})
endif()
