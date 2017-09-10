# SlidingBlocks
In this project, you will need to implement the following search algorithms to solve the sliding blocks problem:

    1. Breadth-First Search (BFS)

    2. Depth-First Search (DFS)

In this problem, the objective is moving a target block out of the environment from an exit gate. The environment also contains different sized blocks as obstacles. Environment can be
represented by a 6 x 6 matrix and the exit gate is on location [3,6].To clarify the problem, an example illustration scenario of the game is given in the following figure.

![alt text](https://i.imgur.com/jriYaD6.jpg)

There are some constraints on movement of blocks;

    • Blocks are either 1 x n or n x 1 sized rectangles. Here, n defines the length of the block and it should be greater than 1 (cannot be 1).

    • 1 x n sized blocks can move along right/left directions.

    • n x 1 sized rectangles can move along up/down directions.

The problem description will be given in a text file (blocks.txt) including the properties of the
blocks. In this file, each row represents a block. In each row, the coordinates of the bottom
left corner of the block, the length of the block and its direction is given (h for horizontal, v
for vertical), respectively. The first row always presents the properties of the target block
which is needed to be moved out of the environment.

You have an input file (blocks.txt) contains the properties of the blocks.

Your program should compile and run using the following commands:

    g++ slidingblocks.cpp –o project1
    
    ./project1 bfs input.txt output.txt
    
    ./project1 dfs input.txt output.txt


