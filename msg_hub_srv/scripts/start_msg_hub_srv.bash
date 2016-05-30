#!/bin/bash

export MALLOC_TRACE=/tmp/dave.mtrace
rm /tmp/dave.mtrace

ulimit -c unlimited
test -e core && rm core

export db_hostname="d8100.witt.net"
export db_login="wittda"
export db_login_pw="test123"
export db_database="test"

../exe/msg_hub_srv
#gdb ../exe/msg_hub_srv -directory ../src


#mtrace ../exe/msg_hub_srv /tmp/dave.mtrace

