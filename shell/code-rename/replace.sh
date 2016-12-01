set -e

DIR=$1

sed 's/IMPALA/DSQL/g' `grep IMPALA -rl ${DIR} --exclude-dir=thirdparty`
sed 's/Impala/Dsql/g' `grep Impala -rl ${DIR} --exclude-dir=thirdparty`
sed 's/impala/dsql/g' `grep impala -rl ${DIR} --exclude-dir=thirdparty`
sed 's/CLOUDERA/ICT/g' `grep CLOUDERA -rl ${DIR} --exclude-dir=thirdparty`
sed 's/Cloudera/Ict/g' `grep Cloudera -rl ${DIR} --exclude-dir=thirdparty`
sed 's/cloudera/ict/g' `grep cloudera -rl ${DIR} --exclude-dir=thirdparty`
