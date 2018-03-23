# round-robin-scheduler

An implementation of scheduling round-robin-algorithm with a changing time quantum for each job made for educational purposes.

The implementation involves creating a shared library *libsched.so* (in Linux environment), linking it with a driver-program *test_lib.exec* and automation of the entire process above by writing a *Makefile*.

You can vary the total number of jobs put to the scheduling by changing the `PROCESS_NUMBER` constant in *sched.h*.

## How-to-build:

For the purposes of this example, everything will happen in /home/username/foo.

### Manually:
  
  1. Compiling *sched.c*
  ```
  $ gcc -c -Wall -Werror -fPIC sched.c
  ```
  
  2. Creating a shared library from an object file
  ```
  $ gcc -shared -o libsched.so sched.o
  ```
  
  3. Linking *test_sched.c* with a shared library to build a driver-program
  ```
  $ gcc -L/home/username/foo -Wl,-rpath=/home/username/foo -Wall -o test_lib.exec test_sched.c -ldl -lsched
  ```
  
  4. Running the test
  ```
  $ ./test_lib.exec libsched.so
  ```
  
### Makefile:
  
  1. Build and run the test
  ```
  $ make
  $ ./test_lib.exec libsched.so
  ```
  
  2. Clean project
  ```
  $ make clean
  ```
