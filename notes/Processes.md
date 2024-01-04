# Processes

#### Process Parents

- initially it is the process that created it
- if a process’ parent terminates, its parent becomes process 1

##### commands

- **sh** ... for creating processes via object-file name
-  **ps** ... show process information 
- **w** ... show per-user process information 
- **top** ... show high-cpu-usage process information
- **kill** ... send a signal to a process

#### Zombie processs

- a process can’t terminate until its parent is notified

#### posix_spawn()

- run a new process

```c
#include <spawn.h>
int posix_spawn(pid_t *pid, const char *path,
								const posix_spawn_file_actions_t *file_actions,
                const posix_spawnattr_t *attrp,
                char *const argv[], char *const envp[]);
```

- ***pid** set to process id of new program 
- creates new process, running program at **path** 
- **file_actions** specifies file actions to be performed before running program 
  - can be used to re-direct stdin or stdout to file or pipe 
  - advanced topic 
- **attrp** specifies attributes for new process
  - not covered in COMP1521
- **argv** specifies argv of new program 
- **envp** specifies environment of new program 

###### example

```c 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <spawn.h>

pid_t pid;
extern char **environ;
char *date_argv[] = {"/bin/date", "--utc", NULL};
// spawn "/bin/date" as a separate process
if (posix_spawn(&pid, "/bin/date", NULL, NULL, date_argv, environ) != 0) {
  perror("spawn");
  exit(1);
}
// wait for spawned processes to finish
int exit_status;
if (waitpid(pid, &exit_status, 0) == -1) {
  perror("waitpid");
  exit(1);
}
printf("/bin/date exit status was %d\n", exit_status);
```



#### fork

```c
#include <sys/types.h>
#include <unistd.h>
pid_t fork(void);
```

###### example

```c
// fork creates 2 identical copies of program
// only return value is different
pid_t pid = fork();
if (pid == -1) {//fail
	perror("fork"); // print why the fork failed
} else if (pid == 0) {//child
	printf("I am the child because fork() returned %d.\n", pid);
} else {//parent
  int exit_status;
  if (waitpid(pid, &exit_status, 0) == -1) {
    perror("waitpid");
    exit(1);
  }
	printf("I am the parent because fork() returned %d.\n", pid);
}
```



#### execvp

- relaces current process by executing file

```c
#include <unistd.h>
int execvp(const char *file, char *const argv[]);
```

- **argv** specifies argv of new program

###### example

```c
char *echo_argv[] = {"/bin/echo","good-bye","cruel","world",NULL};
execv("/bin/echo", echo_argv);
// if we get here there has been an error
perror("execv");
```



#### system

```c
#include <stdlib.h>
int system(const char *command);
```

###### example

```c
int exit_status = system("/bin/date --utc");
printf("/bin/date exit status was %d\n", exit_status);
return 0;
```



#### getpid & getppid

```c
#include <sys/types.h>
#include <unistd.h>
pid_t getpid(void);
pid_t getppid(void);
```

-  **getpid** returns the process ID of the current process

- **getppid** returns process ID of the the parent of current process



#### waitpid

- **waitpid** pauses current process until process pid changes state

```c
#include <sys/types.h>
#include <sys/wait.h>
pid_t waitpid(pid_t pid, int *wstatus, int options);
pid_t wait(int *wstatus);
```

- if **pid = -1**, wait on any child process 
- if **pid = 0**, wait on any child in process group 
- if **pid > 0**, wait on the specified process



### Enviroment variables

```c
// print all environment variables
extern char **environ;
for (int i = 0; environ[i] != NULL; i++) {
	printf("%s\n", environ[i]);
}
```



#### getenv

- search environment variable array for **name=value**

```c
#include <stdlib.h>
char *getenv(const char *name);
```

###### example

```c
char *value = getenv("STATUS");
printf("Environment variable 'STATUS' has value '%s'\n", value);
```



#### setenv

- adds **name=value** to environment variable array

```c
#include <stdlib.h>
int setenv(const char *name, const char *value, int overwrite);
```

###### example

```c
// set environment variable STATUS
setenv("STATUS", "great", 1);
char *getenv_argv[] = {"./get_status", NULL};
pid_t pid;
extern char **environ;
if (posix_spawn(&pid, "./get_status", NULL, NULL,
  getenv_argv, environ) != 0) {
  perror("spawn");
  exit(1);
}
```



#### exit

```c
#include <stdlib.h>
void exit(int status);
void _exit(int status);
```



#### pipe

- a pipe is a unidirectional byte stream provided by operating system 
- **pipefd[0]** - set to file descriptor of **read** end of pipe 
- **pipefd[1]** - set to file descriptor of **write** end of pipe

```c
#include <unistd.h>
int pipe(int pipefd[2]);
```



#### popen & pclose

- runs command via /bin/sh

```c
#include <stdio.h>
FILE *popen(const char *command, const char *type);
int pclose(FILE *stream);
```

###### example 1

- capturing output from a process

```c
// popen passes string to a shell for evaluation
// brittle and highly-vulnerable to security exploits
// popen is suitable for quick debugging and throw-away programs only
FILE *p = popen("/bin/date --utc", "r");
if (p == NULL) {
  perror("");
  return 1;
}
char line[256];
if (fgets(line, sizeof line, p) == NULL) {
  fprintf(stderr, "no output from date\n");
  return 1;
}
printf("output captured from /bin/date was: '%s'\n", line);
pclose(p); // returns command exit status
```

###### example 2

- sending input to a process

```c
int main(void) {
  // popen passes command to a shell for evaluation
  // brittle and highly-vulnerable to security exploits
  // popen is suitable for quick debugging and throw-away programs only
  //
  // tr a-z A-Z - passes stdin to stdout converting lower case to upper case
  FILE *p = popen("tr a-z A-Z", "w");
  if (p == NULL) {
    perror("");
    return 1;
  }
  fprintf(p, "plz date me\n");
  pclose(p); // returns command exit status
  return 0;
}
```

