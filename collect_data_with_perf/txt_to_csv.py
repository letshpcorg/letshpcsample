"""
Author : Omkar Damle, Rajdeep Pinge
Date : September, 2017

This script converts the text data file generated to a csv file of appropriate format 
which can to used to generate graphs by uploading on the letshpc graph generator
"""

import csv
import maps

with open("codes_run_file") as f:
	prob = f.read().split("-")

#numProblemSizes = maps.problem_size["divide_and_conquer"]
numProblemSizes = maps.problem_size[prob[2]]

lineCount = 0
run_id = 0

txt_file_name = "combined_logs.txt"
csv_file_basic_metrics_name = "combined_logs_basic_metrics.csv"
csv_file_basic_perf_name = "combined_logs_perf_metrics.csv"

field_names_basic_metrics = ["n", "p", "run_id", "algS", "e2eS"]
# field_names_perf_metrics = ["n", "p", "run_id", "cycles,instructions", "cache-references", "cache-misses", "bus-cycles", "L1-dcache-loads", "L1-dcache-load-misses", "L1-dcache-stores", "dTLB-loads", "dTLB-load-misses", "LLC-loads", "LLC-load-misses", "LLC-stores", "branches", "branch-misses", "context-switches", "cpu-migrations", "page-faults"]
field_names_perf_metrics = []

csv_wrtiter_basic_metrics = csv.DictWriter(open(csv_file_basic_metrics_name, 'wb'), field_names_basic_metrics)
csv_wrtiter_perf_metrics = csv.DictWriter(open(csv_file_perf_metrics_name, 'wb'), field_names_perf_metrics)

		
with open(txt_file_name, 'r') as txt_file:
	for line in txt_file:
		lineCount += 1

		seq = line.split(',')
		dictionary = {}

		algS = float(seq[6]) + float(seq[7])/1e9
		e2eS = float(seq[4]) + float(seq[5])/1e9

		seq[5] = str(algS)
		seq[6] = str(e2eS)

		for i in range(len(field_names_basic_metrics)):
			dictionary[field_names_basic_metrics[i]] = seq[2+i] #remove the first two columns
		
		dictionary["run_id"] = run_id

		csv_wrtiter_basic_metrics.writerow(dictionary)	
		
		if lineCount == len(numProblemSizes) * (max(maps.processor_range)+1):
			lineCount = 0
			run_id += 1

with open(txt_file_name, 'r') as txt_file:
	for line in txt_file:
		lineCount += 1
		if lineCount == 0:
			field_names_perf_metrics = line.split(",")[2:5] + line.split(",")[9:]

		seq = line.split(',')
		dictionary_perf = {}
		dictionary_perf["n"] = seq[2]
		dictionary_perf["p"] = seq[3]
		for i in range(3, len(field_names_perf_metrics)):
			dictionary_perf[field_names_perf_metrics[i]] = seq[6+i] #remove the first two columns
		
		dictionary_perf["run_id"] = run_id

		csv_wrtiter_perf_metrics.writerow(dictionary_perf)	
		
		if lineCount == len(numProblemSizes) * (max(maps.processor_range)+1):
			lineCount = 0
			run_id += 1
