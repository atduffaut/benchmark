# Context-Switching Benchmark
Simple program made to emulate a context-switch between a parent and child process, blocking and switching while they share information between two pipes.
This emulation shows the time it takes an operating system to save the registers for one process, and restore the registers for another.
This is shown by running two processes, parent and child, on the same CPU core, and sending a sort of "go" signal between the two, and timing the context switch that occurs for the core to switch between the two processes.
The program displays time elapsed in miliseconds, and runs until it is killed (with Ctrl+C).

Note: MacOS does not allow setting processes to specific cores, so the Linux syscalls `sched_getaffinity()` and `sched_setaffinity()` are abstracted in the top of the program. If running on MacOS, leave those functions in the code. If using Linux, comment them out so the true syscalls can be made, and the processes will be set to the same core.

To use, simply run:
```
make
```
To quit at anytime, kill the program with ^C

To clean up the directory and get rid of the executable created, simply run: 
```
make clean
```
