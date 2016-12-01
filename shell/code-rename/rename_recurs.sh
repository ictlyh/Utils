#大小写转换用 str=`echo $file|tr 'A-Z' 'a-z'`
#http://blog.chinaunix.net/uid-14735472-id-2983637.html
#可用正则表达式

if [ $# -ne 3 ]; then

	echo 'example: ./rn_sub_recurs.sh RocketMQ3.7 rocketmq dtube include="subdir1|subdir2" exclude="*.h|subdir3"'
	echo "Usage: ./rename_recurs.sh dir oldstr newstr"
	echo "Usage: ./rename_recurs.sh dir [1-9]bc newstr"
	
	exit
fi

function scandir() 
{
		local entry #for recursive
	  	
 		cd $1

		#echo -n into: && pwd
		 
    for entry in $(ls)
    do    
        if test -d ${entry};then
            scandir ${entry}
        fi
        if echo ${entry} |grep ${oldstr} >/dev/null ; then
          mv $entry `echo $entry|awk '{gsub(oldstr,newstr,$1);print $1;}' oldstr=$oldstr newstr=$newstr`          
        fi
      
    done
		cd ..
		#echo -n exit_to.. && pwd

}

oldstr=$2
newstr=$3

if test -d $1
then		
    scandir $1
elif test -f $1
then
    echo "pls check it's a directory."
    exit 1
else
    echo "the Directory U put doesn't exist"
    exit 1
fi




