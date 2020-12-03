to compile:
gcc -o calc p1_64.cpp

to run: 
./calc 10 5



I use a 64 bit compiler on my MacOS. Thus it should work on mac. 

For the mod function, when doing any type of negative modular arithmetic, the program may gave a floating point exception. I put a statement to catch that exception just in case, even though it was not part of the assignment. The program will give a warning about my catching of the exception, but it does not matter. 