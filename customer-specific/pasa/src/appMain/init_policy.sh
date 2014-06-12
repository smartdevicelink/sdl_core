# This script prepare policy database for QDB on QNX
# !!! DON'T INSERT EMPTY LINES TO THIS FILE !!!
#
#mkdir -p /fs/rwdata/storage/policy/
#
#
# Uncomment line below if you don't have policy.* at /fs/mp/sql/. Copy policy.cfg and policy.sql to /fs/mp/etc/AppLink
# for correct policy configuration.
#
#mount -uw /fs/mp
#cp -f /fs/mp/etc/AppLink/policy.cfg /fs/mp/sql/policy.cfg
#cp -f /fs/mp/etc/AppLink/policy.sql /fs/mp/sql/policy.sql
#
#
cat /fs/mp/sql/policy.cfg > /pps/qnx/qdb/config/policy.db
