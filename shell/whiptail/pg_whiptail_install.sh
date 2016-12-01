#!/bin/bash

SCRIPT=$(basename $(readlink -nf $0))
CURRENT_DIR=$(dirname $(readlink -f $0))

function show_license {
  whiptail --title "License" --textbox $CURRENT_DIR/License 12 80
}

# arguments:
# $@ host list
function ping_hosts() {
  for host in "$@"; do
    ping $host -c 2 -W 2 1>/dev/null 2>&1
    if [[ $? -eq 1 ]]; then
      echo "$host is not reachable, please re-check ip lists" >&1
      return 1
    fi
  done
  return 0
}

# arguments:
# $@ host list
function check_passwordless_connection() {
  for host in "$@"; do
    ssh -t -o PasswordAuthentication=no $host exit 1>/dev/null 2>&1
    if [[ $? -eq 255 ]]; then
      echo "Can't connect to $host without password, we require passwordless connetion" >&1
      return 1
    fi
  done
  return 0
}

# arguments:
# $1 directory
# $2 whether to create diretory if not exist(true/false)
# $3: host list
function check_directory_and_create() {
  dir=$1
  create=$2
  hosts=${@:3}
  for host in $hosts; do
    ssh -t $host [ -d $dir ] 1>/dev/null 2>&1
    if [[ $? -eq 0 ]]; then
      echo "$dir already exist in $host, please select another directory" >&1
      return 1
    else
      if [[ $create -eq "true" ]]; then
        ssh -t $host mkdir -p $dir 1>/dev/null 2>&1
        if [[ $? -eq 1 ]]; then
          echo "Can't create direcory $dir, please check the permissions" >&1
          return 1
        fi
      fi
    fi
  done
  return 0
}

# arguments:
# $1 directory
# $2: host list
function remove_directory() {
  dir=$1
  hosts=${@:2}
  for host in $hosts; do
    ssh -t $host rm -rf $dir 1>/dev/null 2>&1
  done
  return 0
}

# arguments:
# $1 source directory
# $2 destination direcotry 
# $3: host list
function rsync_directory() {
  srcdir=$1
  destdir=$2
  hosts=${@:3}
  for host in $hosts; do
    rsync -rq $srcdir $host:$destdir
  done
  return 0
}

# arguments:
# $1 port number 
# $2: host list
function check_port() {
  port=$1
  hosts=${@:2}
  for host in $hosts; do
    nc -z $host $port 1>/dev/null 2>&1
    if [[ $? -eq 0 ]]; then
      echo "Port $port is in-used in host $host, please use another port" >&1
      return 1
    fi
  done
  return 0
}

# arguments:
# $1 pg data direcotry 
# $2: host list
function init_postgres() {
  install_dir=$1
  pg_data=$2
  hosts=${@:3}
  for host in $hosts; do
    ssh -t $host "$install_dir/bin/initdb -D $pg_data" 1>/dev/null 2>&1
    if [[ $? -eq 1 ]]; then
      echo "Init $pg_datapg_datapg_datast $host fail, you can't be run as root" >&1
      return 1
    fi
  done
  return 0
}

# arguments:
# $1 pg port 
# $2: host list
function update_listen_and_port() {
  pg_data=$1
  port=$2
  hosts=${@:3}
  for host in $hosts; do
    ssh -t $host "sed -i \"s/^.*listen_addresses = .*$/listen_addresses = '*'/g\" $pg_data/postgresql.conf" 1>/dev/null 2>&1
    ssh -t $host "sed -i \"s/^.*port = .*$/port = $port/g\" $pg_data/postgresql.conf" 1>/dev/null 2>&1
  done
  return 0
}

# arguments:
# $1 pg data directory 
# $2 host list
# $3 hba list
function update_pghba() {
  pgdata=$1
  hosts=$2
  hbas=$3
  for host in $hosts; do
    for hba in $hbas; do
      sed append $host:$dir/pg_hba.conf $hba
    done
  done
  return 0
}

# arguments:
# $1 pg data directory
# $2 worker list
function update_worker_list() {
  pgdata=$1
  port=$2
  workers=${@:3}
  if [[ ! -f $pgdata/pg_worker_list.conf ]]; then
    touch $pgdata/pg_worker_list.conf
    if [[ $? -eq 1 ]]; then
      echo "Can't touch file $pgdata/pg_worker_list.conf, please check the permissions" >&1
      return 1
    fi
  fi
  for worker in $workers; do
    echo "$worker $port" >> $pgdata/pg_worker_list.conf
  done
  return 0
}

# arguments:
# $1 pg data directory
# $2 host list
function add_preload_library() {
  pgdata=$1
  hosts=${@:2}
  for host in $hosts; do
    ssh -t $host "echo \"shared_preload_libraries = 'citus'\" >> $pgdata/postgresql.conf" 1>/dev/null 2>&1
  done
  return 0
}


function show_message() {
  title=$1
  msg=${@:2}
  whiptail --title "$title" --msgbox "$msg" 8 78
}

function main() {
  # show license and version info
  show_license

  # get ip addresses workers
  ret_code=1
  while [[ $ret_code -eq 1 ]]; do
    workers=$(whiptail --inputbox "Ip addresses of workers(space seperate):" 8 78 localhost --title "Worker List" 3>&1 1>&2 2>&3)
                                                                        # A trick to swap stdout and stderr.
    exitstatus=$?
    if [ $exitstatus -eq 0 ]; then
      show_message "Worker address" $workers
      msg=$(ping_hosts $workers)
      ret_code=$?
      if [[ $ret_code -eq 1 ]]; then
        show_message "Error" $msg
      fi
    else
      show_message "Notice" "Quit installation process"
      exit 0
    fi
  done

  # check passwordless connection
  msg=$(check_passwordless_connection $workers)
  if [[ $? -eq 1 ]]; then
    show_message "Error" $msg
    exit 1
  fi

  # get installation destination
  ret_code=1
  while [[ $ret_code -eq 1 ]]; do
    install_dir=$(whiptail --inputbox "Input installation directory:" 8 78 /home/$USER --title "Installation Directory" 3>&1 1>&2 2>&3)
                                                                        # A trick to swap stdout and stderr.
    exitstatus=$?
    if [[ $exitstatus = 0 ]]; then
      msg=$(check_directory_and_create $install_dir true $workers)
      ret_code=$?
      if [[ $ret_code -eq 1 ]]; then
        show_message "Error" $msg
      fi
    else
      show_message "Notice" "Quit installation process"
      exit 0
    fi
  done

  # get postgres port
  ret_code=1
  while [[ $ret_code -eq 1 ]]; do
    pg_port=$(whiptail --inputbox "Input port of postgres:" 8 78 5432 --title "Postgres Port" 3>&1 1>&2 2>&3)
                                                                        # A trick to swap stdout and stderr.
    exitstatus=$?
    if [[ $exitstatus = 0 ]]; then
      msg=$(check_port $pg_port $workers)
      ret_code=$?
      if [[ $ret_code -eq 1 ]]; then
        show_message "Error" $msg
      fi
    else
      show_message "Notice" "Quit installation process"
      exit 0
    fi
  done

  # copy installation file to remote hosts
  rsync_directory $CURRENT_DIR/ $install_dir $workers

  # input pg data directory
  ret_code=1
  while [[ $ret_code -eq 1 ]]; do
    pg_data=$(whiptail --inputbox "Input directory of pg data:" 8 78 /home/$USER --title "PGDATA" 3>&1 1>&2 2>&3)
                                                                        # A trick to swap stdout and stderr.
    exitstatus=$?
    if [[ $exitstatus = 0 ]]; then
      msg=$(check_directory_and_create $pg_data false $workers)
      ret_code=$?
      if [[ $ret_code -eq 1 ]]; then
        show_message "Error" $msg
      fi
    else
      show_message "Notice" "Quit installation process"
      exit 0
    fi
  done

  # init postgres data directory
  msg=$(init_postgres $install_dir $pg_data $workers)
  if [[ $? -eq 1 ]]; then
    show_message "Error" $msg
    exit 0
  fi

  # update port and listen of postgresql.conf
  update_listen_and_port $pg_data $pg_port $workers

  # update pg_hba.conf

  # add pg_worker_list.conf
  msg=$(update_worker_list $pg_data $pg_port $workers)
  if [[ $? -eq 1 ]]; then
    show_message "Error" $msg
    exit 0
  fi

  # add preload_library
  add_preload_library $pg_data $workers 

  # show success message
  show_message "Success" "You have installed DSQL cluster successfully"
}

main
