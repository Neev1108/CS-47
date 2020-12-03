Neeval Kumar 011877086 Mac OS 64 bit machine. I used terminal to compile.

To compile gcc -o p3 p3.cpp

The program works fine. I tested all 3 categories, checking for partial matches, case insensitivity etc.

Here are some tests I ran that worked:
./p3 p3.data1 Arturo 1001 (check a match)

./p3 p3.data1 David 1001 (check a match with different id)

./p3 p3.data1 Sam 1001 (not in the list)

./p3 p3.data1 DAVID 1010 (case insensitivity check)

./p3 p3.data1 Chr 1010 
(partial match check, if one word is a substring of other, it still wont work. Here Chr is a substring of Chris so it still will not be found) 