# This script prepare policy database on QNX for PASA

cat /fs/mp/sql/policy.cfg > /pps/qnx/qdb/config/policy.db
mkdir -p /fs/rwdata/storage/policy
