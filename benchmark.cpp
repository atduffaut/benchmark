#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

int main() {
    //creating two pipes
    int p1[2]; // parent -> child
    int p2[2]; // child -> parent
    if (pipe(p1) == -1) exit(1);
    if (pipe(p2) == -1) exit(2);

    int rc = fork(); //create child process

    if (rc < 0) exit(3);
    else if (rc == 0) {
        //in child process
        close(p1[1]);
        close(p2[0]);
        while(true) {
            char buf[0];
            while (read(p1[0], &buf, sizeof(buf)) == -1) {
                //wait...
            }
            write(p2[1], &buf, sizeof(buf));
        }
    }

    else {
        //in parent, this executes first
        close(p1[0]);
        close(p2[1]);
        struct timeval current_time;
        while(true) {
            sleep(1);
            char buf[0];
            write(p1[1], &buf, sizeof(buf));
            gettimeofday(&current_time, NULL);
            int start_usec = current_time.tv_usec;
            while (read(p2[0], &buf, sizeof(buf)) == -1) { //waiting for child to send back
                //wait...
            }
            gettimeofday(&current_time, NULL);
            int end_usec = current_time.tv_usec;
            std::cout << "miliseconds elapsed to perform context switch: " << end_usec - start_usec << std::endl;
        }
    }
    return 0;
}
