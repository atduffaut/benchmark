#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <iostream>


// COMMENT OUT THESE FUNCTIONS IMPLEMENTATIONS TO EXECUTE ON LINUX

// Unix implementation of the Linux syscall "sched_getaffinity"
// and "sched_setaffinity"

#define SYSCTL_CORE_COUNT   "machdep.cpu.core_count"

typedef struct cpu_set {
  uint32_t    count;
} cpu_set_t;

static inline void
CPU_ZERO(cpu_set_t *cs) { cs->count = 0; }

static inline void
CPU_SET(int num, cpu_set_t *cs) { cs->count |= (1 << num); }

static inline int
CPU_ISSET(int num, cpu_set_t *cs) { return (cs->count & (1 << num)); }

int sched_getaffinity(pid_t pid, size_t cpu_size, cpu_set_t *cpu_set)
{
  int32_t core_count = 0;
  size_t  len = sizeof(core_count);
  int ret = sysctlbyname(SYSCTL_CORE_COUNT, &core_count, &len, 0, 0);
  if (ret) {
    printf("error while get core count %d\n", ret);
    return -1;
  }
  cpu_set->count = 0;
  for (int i = 0; i < core_count; i++) {
    cpu_set->count |= (1 << i);
  }

  return 0;
} // Simplified version of Linux syscall to retrieve number of cores

int sched_setaffinity(pid_t pid, size_t cpu_size, cpu_set_t *cpu_set) {
    return 0;
}; // No MacOS APIs exist to implement this Linux syscall

int main() {

    // Set CPU 0
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);

    //creating two pipes
    int p1[2]; // parent -> child
    int p2[2]; // child -> parent
    if (pipe(p1) == -1) exit(1);
    if (pipe(p2) == -1) exit(2);

    int rc = fork(); //create child process

    if (rc < 0) exit(3);
    else if (rc == 0) {
        //in child process
        if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &set) == -1) {
            exit(3);
        }

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
        if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &set) == -1) {
            exit(4);
        }
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