

#!/bin/bash

# starttime=`date +'%Y-%m-%d %H:%M:%S'`

clients=5

array=($(seq 1 $clients))
for i in ${array[*]}
do
	./restful_client &
	echo "开启第" $i "个客户端"
done

sleep 15