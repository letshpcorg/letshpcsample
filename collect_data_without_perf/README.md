# Collect data without perf data

## Regarding code modification and running using script

* See codes on github repo "https://github.com/letshpcorg/letshpcsample" and modify your codes accordingly so that they can be run automatically using script.
* E2E is the total time for which the code runs while ALG is the time of the parallel part in the code. Put these two in your code properly before running the script.
* Put the codes in a folder named 'all_codes'. Keep the name format as id1-id2-problem_name-approach_name (Take problem_name and approach_name from maps.py file)
* For files inside, please keep the label 'serial' or 'parallel' in the name of each file.
* Download the scripts on repo. (OR use the scripts sent to you)
* Adjust number of processors and problem size in maps.py for the given problem. Make sure that it does not take too much time.
* Do ./run_script_without_perf.sh <number of runs> to start data collection and generate csv file. For example, "./run_script_without_perf.sh 10" if you want to run 10 iterations
* Make sure you run the code for at least 10 iterations. 
* Run the script on lab machines or cluster. Modify the number of processors accordingly in maps.py file



## Generating graphs

* Use Google Chrome for this part.
* Go to letshpc.herokuapp.com
* In Analysis Tools, select "Analyse Custom Data"
* Upload CSV file generated after running script using 'Normal' option.
* Ignore the error in red. Select all the cores and 'Update'
* Adjust scale and metric. Choose 'Mean' as statistic. Make sure the scale of x and y-axis is proper and both the axes are labelled.
* Export all the necessary graphs.


## Writing report

* Use chrome for this part.
* Select Report Generator Tool in letshpc.herokuapp.com
* Fill the relevant fields
* Download the latex report
* Download llncs.cls for latex report. (OR use the file that is sent to you)
* Open the file in latex editor. Add necessary details. Add generated graphs at proper places.
* Add any extra sections if necessary.
