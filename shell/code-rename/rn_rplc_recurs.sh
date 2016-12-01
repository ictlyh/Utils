if [ $# -lt 3 ]; then

	echo "Usage: ./rn_sub_recurs.sh dir oldstr newstr [include=xxx] [exclude=xxx]"
	echo "Example: ./rn_sub_recurs.sh RocketMQ3.7 rocketmq dtube"
	echo 'or: ./rn_sub_recurs.sh RocketMQ3.7 rocketmq dtube exclude="^pom.xml$"'
	echo 'or: ./rn_sub_recurs.sh RocketMQ3.7 rocketmq dtube include="*.java"'	
	echo "To replace recursively all subdir and files under subdir1 and subdir2 except .h files:"
	echo './rn_sub_recurs.sh RocketMQ3.7 rocketmq dtube include="subdir1|subdir2" exclude="*.h"'
	exit
fi

function scandir() 
{
		local entry #for recursive
	  
	  scanflag=$2
 		cd $1		
		
		#echo -n into: && pwd
		 
    for entry in $(ls)
    do
        if [ -n "${exclude}" ] && echo ${entry} |egrep ${exclude} >/dev/null ; then
          continue;
        fi
        
        if [ -n "${include}" -a -z "${scanflag}" ]  ; then
        	if echo ${entry} |egrep ${include} >/dev/null; then
        		scanflag="subinc"
        	else
        		scanflag=""
        		continue;
        	fi
        fi
        
        if test -d ${entry};then
            scandir ${entry} ${scanflag}
        else
        	sed --follow-symlinks -i "s#${oldstr}#${newstr}#g" ${entry}
        fi
        if echo ${entry} |egrep ${oldstr} >/dev/null ; then
          mv $entry `echo $entry|awk '{gsub(oldstr,newstr,$1);print $1;}' oldstr=$oldstr newstr=$newstr`          
        fi
      
    done
		cd ..
		#echo -n exit_to.. && pwd

}

oldstr=$2
newstr=$3

cnt=0;
for i in $@ ;do
				if ((++cnt <= 3));then
					continue
				fi
        if echo "$i" | grep "^include=" >/dev/null ;then        	
                include=`echo "$i" | awk '{match($1,"include=(.*)",arry);print arry[1]}'`
        fi
        if echo "$i" | grep "^exclude=" >/dev/null ;then
                exclude=`echo "$i" | awk '{match($1,"exclude=(.*)",arry);print arry[1]}'`
        fi        
done

#echo "|"$include"|" "|"$exclude"|"


if test -d $1
then		
    scandir $1
elif test -f $1
then
    echo "you input a file but not a directory,pls reinput and try again"
    exit 1
else
    echo "the Directory isn't exist which you input,pls input a new one!!"
    exit 1
fi
