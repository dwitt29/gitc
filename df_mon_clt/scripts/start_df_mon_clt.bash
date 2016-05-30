#!/bin/bash

export MALLOC_TRACE=/tmp/dave.mtrace
##rm /tmp/dave.mtrace

ulimit -c unlimited
test -e core && rm core

export LOGFILE=/tmp/df_mon_clt.log
export listener_hostname="d8100.witt.net"
export listener_portname="test"
export listener_protocol="tcp"
export fstab_path="/etc/fstab"
export df_cmd="df -k -P"
export df_cmd_sleep_time="5"
export warn_limit="10"
export severe_limit="85"
export danger_limit="90"
export fatal_limit="95"
export df_format="%*s%*s%*s%*s%s%s"
export db_hostname="d8100.witt.net"
export db_login="wittda"
export db_login_pw="test123"
export db_database="test"

../exe/df_mon_clt 
#gdb ../exe/df_mon_clt -directory ../src


#mtrace ../exe/df_mon_clt /tmp/dave.mtrace
