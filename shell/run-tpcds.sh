HOME=/home/lyh/workspace
for ((i=1;i<100;i++));do
  $HOME/impala/bin/impala-shell.sh -d tpcds1pg1g -f $HOME/backup/tpcds/impala-tpcds-queries/q$i.sql > $HOME/results/$i.txt 2>&1
done
