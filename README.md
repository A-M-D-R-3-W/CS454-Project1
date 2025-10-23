# CS-454

Compilation:

This program relies on the Boost C++ library for it's multipricision.
Boost must be installed (in our case using MSYS2), and the program can be compiled as follows:
g++ -I"C:\msys64\mingw64\include" -std=c++17 -O2 main.cpp -o main

Contribution:

Both I (Andrew) and John have contributed in a way that was, I believe, evenly distributed.
John created the project with the initial driving program and helper functions. He took the
reigns on Problem 1 with my support, while I took the reigns on Problem 2 with his support.
Together we addressed bugs and deviations in the output from what was expected.

AI Use:

I (Andrew) initially had difficulty understanding and converting the formal definition of
Problem 2 to code. I used AI to help setup the structure of the solution to Problem 2. 
It was a huge help in describing this problem in a way I had an easier time understanding. 
It has deviated significantly from what was provided, but in transparency was still used.

Output:

1. Problem 1
2. Problem 2
4. TERMINATE
1
Enter string length (n): 62
Valid strings of length 62: 359900538890035317508583976
1. Problem 1
2. Problem 2
4. TERMINATE
1
Enter string length (n): 100
Valid strings of length 100: 2366245453462025611146486720383147159575632
1. Problem 1
2. Problem 2
4. TERMINATE
2
Enter string length (n, must be even): 62
Valid strings of length 62 with middle 'aa': 8167397856378064530466812
1. Problem 1
2. Problem 2
4. TERMINATE
2
Enter string length (n, must be even): 100
Valid strings of length 100 with middle 'aa': 53698358586035946091697705580011523688344
1. Problem 1
2. Problem 2
4. TERMINATE
4
Terminating...