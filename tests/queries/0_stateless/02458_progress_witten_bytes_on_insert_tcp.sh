#!/usr/bin/env bash

CURDIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=../shell_config.sh
. "$CURDIR"/../shell_config.sh

$CLICKHOUSE_CLIENT -q "drop table if exists my_table;"
$CLICKHOUSE_CLIENT -q "create table my_table(s UInt16) engine = MergeTree order by s;"

# We should have correct env vars from shell_config.sh to run this test
python3 "$CURDIR"/02458_progress_witten_bytes_on_insert_tcp.python

$CLICKHOUSE_CLIENT -q "drop table if exists my_table;"
