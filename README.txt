TO COMPILE AND RUN CODE:
Navigate to directory where source code is located
Type in "make" to compile all code using make file
To run enter: ./BestFirstSearch <inputfile.txt> <outputfile.txt>

TO CLEAN DIRECTORY OF OBJECT FILES AND EXECUTABLE FILES
Enter "make clean" in command line prompt

DATA STRUCTURES:
Node Class -used as a container to hold the weight, height, bound, and profit of each node in the tree.
Item Struct -object used as a container for holding each individual items weight and profit. Allows for simplicity when sorting in profit/weight ratio as opposed to keeping track of two separate arrays, one with weight and one with profit.

COMPUTATION TIME AND ANALYSIS:
This algorithm runs in worst case O(2^n) however most of the time it does better than that. This is because the algorithm prunes the tree as it computes the bounds and it prioritizes higher bounds over lower bounds.  


CLASSES USED:
String -Used for string to int conversion
Algorithm -Used for sorting 
Vector -Used for parsing and tracking optimal knapsack
Queue -Used for implementing priority queue
fstream -used for reading and writing to file
iostream -used for writing out statements during debugging
sstream -used for parsing CSV input file
