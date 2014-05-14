# This script prepare policy database for QDB on QNX

cat /fs/mp/sql/policy.cfg > /pps/qnx/qdb/config/policy.db
mkdir -p /fs/rwdata/storage/policy
