#!/bin/bash

total_connect_count=0

total_request_abnormal_count=0
total_handle_success_count=0
total_handle_failure_count=0

request_abnormal_num=0
handle_success_num=0
handle_failure_num=0

request_abnormal_num=22
array=($(seq 1 $request_abnormal_num))
for i in ${array[*]}
do
    ./restful_client 10.0.80.121 8090 0 >/dev/null
done
let total_request_abnormal_count+=$request_abnormal_num

handle_success_num=15
array=($(seq 1 $handle_success_num))
for i in ${array[*]}
do
    ./restful_client 10.0.80.121 8090 1 >/dev/null
done
let total_handle_success_count+=$handle_success_num

handle_success_num=25
array=($(seq 1 $handle_success_num))
for i in ${array[*]}
do
    ./restful_client 10.0.80.121 8090 2 >/dev/null
done
let total_handle_success_count+=$handle_success_num

handle_failure_num=12
array=($(seq 1 $handle_failure_num))
for i in ${array[*]}
do
    ./restful_client 10.0.80.121 8090 3 >/dev/null
done
let total_handle_failure_count+=$handle_failure_num

handle_success_num=86
array=($(seq 1 $handle_success_num))
for i in ${array[*]}
do
    ./restful_client 10.0.80.121 8090 2 >/dev/null
done
let total_handle_success_count+=$handle_success_num

handle_success_num=72
array=($(seq 1 $handle_success_num))
for i in ${array[*]}
do
    ./restful_client 10.0.80.121 8090 1 >/dev/null
done
let total_handle_success_count+=$handle_success_num

handle_failure_num=34
array=($(seq 1 $handle_failure_num))
for i in ${array[*]}
do
    ./restful_client 10.0.80.121 8090 3 >/dev/null
done
let total_handle_failure_count+=$handle_failure_num

let total_connect_count=$total_handle_success_count+$total_handle_failure_count+$total_request_abnormal_count

echo "
==================== statics ====================
总的成功连接数: $total_connect_count
异常请求连接数: $total_request_abnormal_count
请求处理成功数: $total_handle_success_count
请求处理失败数: $total_handle_failure_count
==================== statics ===================="
