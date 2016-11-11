# round-robin-scheduler
## An implementation of scheduling round-robin-algorithm with a changing time quantum for each job made for educational purposes.

The implementation involves creating a shared library *libsched.so* (in Linux environment), linking it with a driver-program *test_lib.exec* and automation of the entire process above by writing a *Makefile*.

## How-to-build:

  * Manually:
  
    * Compiling *sched.c*
    
      ```
      $ gcc -c -Wall -Werror -fPIC sched.c
      ```
      
    * Creating a shared library from an object file
    
      ```
      $ gcc -shared -o libsched.so sched.o
      ```
      
    * Linking *test_sched.c* with a shared library to build a driver-program
    
      ```
      $ gcc -L /home/username/foo -Wl,-rpath=/home/username/foo -Wall -o test_lib.exec test_sched.c -ldl -lsched
      ```
      
    * Running the test
    
      ```
      $ ./test_lib.exec libsched.so
      ```
  * Via the *Makefile*:
  
    * Build and run the test
    
      ```
      $ make
      $ ./test_lib.exec libsched.so
      ```
      
    * Clean project
    
      ```
      $ make clean
      ```
