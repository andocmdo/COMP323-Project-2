# COMP323-Project-2
A project for COMP323 that simulates the producer-consumer problem in C
Andy Sisinger
Project 2 - Producer Consumer
11/10/17
COMP323

My program was compiled with the following command:
"gcc -pthread -lrt -Wall -o proj2.out sisinger_andrew_proj2_COMP323_v2.c"

I have included a makefile to simplify this.
Type "make" in the directory to compile.
Type "make clean" to remove last compiled executable.


Usage: ./proj2 [sleep in sec] [number of producer threads] [number of consumer threads]

Run the program by:
"./proj2 2 3 4"

Where the first argument is how long to let the main process sleep before exiting. The
second is how many producer threads, and the third is how many consumer threads.

The output will print the ring buffer (with -1 showing empty spaces), and the program
will output a log of each insertion and removal.

Funny story: I accidentally ran the program without any random wait in the
producer and consumer threads, so they ran full speed for a couple of seconds.
I couldn't read the output that fast, so I piped the output into a text file.
This created a text file that was 50MB in less than 2 seconds! Amazing how fast
modern computers are (and SSD drives)! Oops! I realized my mistake and added the
random wait times for the producer and consumer threads.

Thanks,
Andy Sisinger
