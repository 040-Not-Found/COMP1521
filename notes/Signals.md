# Signals

- **Term** ... terminate the process 
- **Ign** ... ignored - the signal does nothing 
- **Core** ... terminate the process and dump memory image to file named core 
- **Stop** ... pause the process 
- **Cont** ... continue the process (if paused)

![Screen Shot 2020-11-26 at 19.08.21](/Users/a040/Library/Application Support/typora-user-images/Screen Shot 2020-11-26 at 19.08.21.png)

#### Create signal handler 

##### old way

```c
#include <signal.h>
typedef void (*sighandler_t)(int); 
							sighandler_t signal(int signum, sighandler_t handler);
```

##### New way

```c
#include <signal.h>
int sigaction(int signum, const struct sigaction *act,
							struct sigaction *oldact);
```



#### sleep

- Waiting for an event
- 让这个程序等一段时间

```c
#include <unistd.h>
unsigned int sleep(unsigned int seconds);
```

###### example

```c
#include <signal.h>
void signal_handler(int signum) {
	printf("signal number %d received\n", signum);
}
int main(void) {
  struct sigaction action = {.sa_handler = signal_handler};
  sigaction(SIGUSR1, &action, NULL);
  printf("I am process %d waiting for signal %d\n", getpid(), SIGUSR1);
  // suspend execution for 1 hour
  sleep(3600);
}
```



#### kill

- send signal number **sig** to process number **pid**
- if successful, return 0; on error, return -1 and set errno

```c
#include <sys/types.h>
#include <signal.h>
int kill(pid_t pid, int sig);
```

###### example

```c
int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <signal> <pid>\n", argv[0]);
    return 1;
  }
  int signal = atoi(argv[1]);
  int pid = atoi(argv[2]);
  kill(pid, signal);
}
```



