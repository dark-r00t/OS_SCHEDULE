# Programming Assignment #1 - Schedulers 

#### PLACE "processes.in" INTO THE ROOT DIRECTORY

#### RUN MAKE ($ make) or ($ make rebuild)

#### TYPE: ($ ./bin/run) TO EXECUTE THE NOW COMPILED SOURCE CODE

#### A FILE IN THE ROOT DIRECTORY WILL BE CREATED AS "processes.out"

```
> make
> ./bin/run
> cat processes.out
```

### ADDITIONAL OPTION #1
#### YOU MAY SPECIFY A FILE IN ANY DIRECTORY TO RUN BY ADDING AN ARGUMENT
#### PLEASE NOTE, IF IT DOES NOT WORK YOU MAY HAVE SPELT THE FILE WRONG
```
> make
> ./bin/run cases/some_test_case.in
> cat processes.out
```

### ADDITIONAL OPTION #2
#### Building off of OPTIONAL #1, you may use the provided shell script: "run.sh"
```
> chmod +x run.sh
> ./run.sh
```

## Instructions

### Assignment: Implement the First-Come First-Served, preemptive Shortest Job First, and Round-Robin algorithms as for single processors.
### Input: Your program will read a file from the current directory called `processes.in`, which will be formatted as follows.  Your program should ignore everything on a line after a `#` mark and ignore additional spaces in input.  The parameters will be presented in this order, with the obvious exception of the quantum parameter, which will either not be present if the scheduler isn't Round Robin or will be commented out.  In other words, you should only be checking that if the scheduler is "rr" then you quantum must be provided.
#### Example: `processes.in`
```
processcount 2        # Read 5 processes
runfor 15             # Run for 15 time units
use rr                # Can be fcfs, sjf, or rr
quantum 2             # Time quantum – only if using rr
process name P1 arrival 3 burst 5
process name P2 arrival 0 burst 9
end
```
#### Example: `processes.out`
```
2 processes
Using Round-Robin
Quantum 2
Time 0: P2 arrived
Time 0: P2 selected (burst 9)
Time 2: P2 selected (burst 7)
Time 3: P1 arrived
Time 4: P1 selected (burst 5)
Time 6: P2 selected (burst 5)
Time 8: P1 selected (burst 3)
Time 10: P2 selected (burst 3)
Time 12: P1 selected (burst 1)
Time 13: P1 finished
Time 13: P2 selected (burst 1)
Time 14: P2 finished
Time 14: idle
Finished at time 15

P1 wait 5 turnaround 10
P2 wait 5 turnaround 14
```

### This version of Round-Robin should not run the scheduler immediately upon the arrival of a new process, unless the CPU is currently idle.
### Your program will not be given an input that results in an ambiguous decision, such as identical arrival times for Round-Robin or identical burst lengths for SJF; you should avoid generating an error in that case on general principles but it will not appear in either the example inputs or the grading inputs.

### In the event of exceptions like a program not completing before the given run time, print:
```
<process name> wait <wait time> did not complete
```
### If a process could not be scheduled within the run time:
```
<process name> could not be scheduled
```
