# make rebuild && ./bin/run && bat leak_info.txt && diff processes.out processes.out.tmp # && bat processes.out
make rebuild

echo '' && echo '' && echo 'TEST ONE (1)'
./bin/run cases/set1_process.in && diff processes.out cases/set1_processes.out
bat processes.out

echo '' && echo '' && echo 'TEST TWO (2)'
./bin/run cases/set2_process.in && diff processes.out cases/set2_processes.out
bat processes.out

echo '' && echo '' && echo 'TEST THREE (3)'
./bin/run cases/set3_process.in && diff processes.out cases/set3_processes.out
bat processes.out

echo '' && echo '' && echo 'TEST FOUR (4)'
./bin/run cases/set4_process.in && diff processes.out cases/set4_processes.out
bat processes.out
