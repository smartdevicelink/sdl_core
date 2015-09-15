

include_directories (
  ./
  ./include
  ../apr/include
  ./include/private
  xml/expat/lib
)


set (APR_UTIL_SOURCES
   buckets/apr_brigade.c
xml/apr_xml.c
xlate/xlate.c
uri/apr_uri.c
strmatch/apr_strmatch.c
dbm/sdbm/sdbm_pair.c
dbm/sdbm/sdbm_lock.c
dbm/sdbm/sdbm_hash.c
dbm/sdbm/sdbm.c
misc/apu_version.c
misc/apr_rmm.c
misc/apr_reslist.c
misc/apr_queue.c
misc/apr_date.c
ldap/apr_ldap_option.c
ldap/apr_ldap_url.c
ldap/apr_ldap_init.c
hooks/apr_hooks.c
encoding/apr_base64.c
dbm/apr_dbm_sdbm.c
dbm/apr_dbm_gdbm.c
dbm/apr_dbm_berkeleydb.c
dbm/apr_dbm.c
dbd/apr_dbd_sqlite3.c
dbd/apr_dbd_sqlite2.c
dbd/apr_dbd_pgsql.c
dbd/apr_dbd.c
crypto/uuid.c
crypto/getuuid.c
crypto/apr_sha1.c
crypto/apr_md5.c
crypto/apr_md4.c
buckets/apr_buckets_socket.c
buckets/apr_buckets_simple.c
buckets/apr_buckets_refcount.c
buckets/apr_buckets_pool.c
buckets/apr_buckets_pipe.c
buckets/apr_buckets_mmap.c
buckets/apr_buckets_heap.c
buckets/apr_buckets_flush.c
buckets/apr_buckets_file.c
buckets/apr_buckets_eos.c
buckets/apr_buckets_alloc.c
buckets/apr_buckets.c

)



add_library("aprutil-1" ${APR_UTIL_SOURCES})

add_subdirectory(./xml/expat/)
target_link_libraries("aprutil-1" expat)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
#target_link_libraries(${PROJECT} ${LIBRARIES})
#link_library
#target_link_libraries("lib_msp_vr"  ${soxr-lsr})
endif()
