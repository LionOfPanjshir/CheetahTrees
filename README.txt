CHEETAH TREES

This is a console application that demonstrates a data structure consisting of a hash table that uses AVL trees for collisions.

ANALYSIS

Total Words: 1,741
Max Bucket Depth: 5

-Insert: O(1) for most words. O(log B) in the worst case, where B is the max bucket depth.

-Retrieval: O(log B)

INSTRUCTIONS FOR COMPILATION:

1) Ensure the documents folder is in the same directory as the .cpp files.  Otherwise, you will have edit the directory path within the initTable function in the implemenation.cpp file.

2) The project consists of 3 files: Header.h, Main.cpp, and Implementation.cpp

3) Compile and run. Enter a word to search or 0 to exit.

4) Search for a partial term using the asterisk (*). For example, "p*" will search for all words starting with p.

5) Enjoy!