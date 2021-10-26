#!/bin/bash

process_name="word_finder"														# processes' name
log_file_path="/home/word-finder/logs/monitor-$(date +%Y-%m-%d).log"			# where to record monitoring info
run_interval=60																	# run this script every 60 seconds
cpu_usage_threshold="95.00"														# upper limit for cpu usage per-process
mem_usage_threshold="95.00"														# upper limit for mem usage per-process

# $1: pid
# $2: cpu usage
# $3: memory usage
# $4: process state
# $5: Error subject
report_error() {
	subject=$5
	content="PID is: $1 | CPU usage is: $2 | Memory usage is: $3 | Process state is: $4"

	echo "$subject" >> $log_file_path
	echo "$content" >> $log_file_path

	# kill all processes
	python3 $PWD/kill.py

	# TODO(bitate): restart master process
}

while true;
do
	for pid in $(pgrep "$process_name")
	do 
		# monitor memory usage, cpu usage, process state
		process_infos=$(ps -p $pid -o %cpu=,%mem=,state=)
		cpu=$(echo "$process_infos" | awk '{print $1}')
		mem=$(echo "$process_infos" | awk '{print $2}')
		state=$(echo "$process_infos" | awk '{print $3}')

		# if process consumes too many system resources
		is_cpu_overloaded=$(echo "$cpu > $cpu_usage_threshold" | bc)
		is_mem_overloaded=$(echo "$mem > $mem_usage_threshold" | bc)
		if [ $is_cpu_overloaded -eq 1 ] || [ $is_mem_overloaded -eq 1 ]
		then
			report_error $pid, $cpu, $mem, $state, "Attention: process($pid) is consuming too many system resources on $(hostname) at $(date)"
		fi

		# if process state is abnormal
		if [ $state != "S" ] && [ $state != "R" ]
		then
			report_error $pid, $cpu, $mem, $state, "Attention: process($pid) is in abnormal state on $(hostname) at $(date)"
		fi
	done
sleep $run_interval
done