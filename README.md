# Context-Switching Benchmark
Simple program made to emulate a context-switch between a parent and child process, blocking and switching while they share information between two pipes.
This emulation shows the time it takes an operating system to save the registers for one process, and restore the registers for another. 
The program displays time elapsed in miliseconds, and runs until it is killed.
To use, simply run:
```
make
```
To quit at anytime, kill the program with ^C
