PLACE "processes.in" INTO THE ROOT DIRECTORY

RUN MAKE ($ make) or ($ make rebuild)

TYPE: ($ ./bin/run) TO EXECUTE THE NOW COMPILED SOURCE CODE

A FILE IN THE ROOT DIRECTORY WILL BE CREATED AS "processes.out"

```
> make
> ./bin/run
> cat processes.out
```

OPTIONAL #1
YOU MAY SPECIFY A FILE IN ANY DIRECTORY TO RUN BY ADDING AN ARGUMENT
PLEASE NOTE, IF IT DOES NOT WORK YOU MAY HAVE SPELT THE FILE WRONG
```
> make
> ./bin/run cases/some_test_case.in
> cat processes.out
```

OPTIONAL #2
Building off of OPTIONAL #1, you may use the provided shell script: "run.sh"
```
> chmod +x run.sh
> ./run.sh
```