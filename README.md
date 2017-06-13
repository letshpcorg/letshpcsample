# letshpcsample
This repository has the scripts and a sample code to show the ease of use for the task of benchmarking a code for the platform.

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