# letshpcsample
This repository has the scripts and a sample code to show the ease of use for the task of benchmarking a code for the platform.

## Directory Structure of this repo.
* _collect\_data\_without\_perf_ : Use the collect_data_without_perf to gather data without using the perf utility. Extensive instructions have been provided in the README file in the directory.

## Instructions to Run

* Step 1 : Installing perf tool : Install the perf tool using the following command 
```bash
./setup.sh
```

* Step 2 : Run the script : 
```bash
./run_script.sh
```

* After the script has run sucessfully, the _combined\_logs.txt_ file and a file in the _perf\_log_ directory would be created which contain the data regarding the execution times and perf-tool data respectively.