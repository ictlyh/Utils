#!/bin/bash

ROOT=$(cd $(dirname $0); pwd)
pushd ${ROOT}

master_host=127.0.0.1
master_port=5432
master_pghome=/home/luoyuanhao/Softwares/pgsql
master_pgdata=/home/luoyuanhao/Softwares/pgdata

slave_host=127.0.0.1
slave_port=5433
slave_pghome=/home/luoyuanhao/Softwares/pgsql
slave_pgdata=/home/luoyuanhao/Softwares/pgstandby

for ARG in $*
do
	case "$ARG" in
		-role=master)
			ROLE=master
			;;
		-role=slave)
			ROLE=slave
			;;
		-role=*)
			echo "Invalid role type. Valid values are: master, slave"
			exit 1
			;;
	esac
done

: "${ROLE? missing -role=\{master | slave\}}"

if [ $ROLE = 'master' ]; then
	${master_pghome}/bin/psql -p ${master_port} -c "CREATE USER replicator REPLICATION LOGIN ENCRYPTED PASSWORD 'thepassword'"
	cat >>${master_pgdata}/postgresql.conf << EOF
	wal_level = hot_standby
	max_wal_senders = 3
	wal_keep_segments = 8
EOF
	cat >>${master_pgdata}/pg_hba.conf << EOF
	host replication replicator ${slave_host}/32 trust
EOF
	${master_pghome}/bin/pg_ctl -D ${master_pgdata} -l ${master_pgdata}/logfile restart

else
	rm -rf ${slave_pgdata}
	${slave_pghome}/bin/pg_basebackup -h ${master_host} -p ${master_port} -D ${slave_pgdata} -U replicator -x -v -P
	sed -i "s/.*port = .*/port=${slave_port}/g" ${slave_pgdata}/postgresql.conf
	echo "hot_standby = on" >> ${slave_pgdata}/postgresql.conf
	cat >>${slave_pgdata}/recovery.conf << EOF
	standby_mode = 'on'
	primary_conninfo = 'host=${master_host} port=${master_port} user=replicator password=thepassword'
	trigger_file = '/tmp/postgresql.trigger'
EOF
	rm -rf ${slave_pgdata}/postmaster.*
	${slave_pghome}/bin/pg_ctl -D ${slave_pgdata} start
fi
