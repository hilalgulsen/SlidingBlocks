#include<iostream>
#include<fstream>
#include<vector>
#include<queue>
#include<stack>
#include<map>
#include<list>
#define SIZE 6
using namespace std;
//To keep every Block data on the Board
struct Block{
	int blockX;
	int blockY;
	int blockLen;
	char blockLie;
	int blockId;
};

class Board{
public:
	int order;
	Board();
	int** twoDimBlock;		//2D matrix which holds block states on board
	int numBlocks;			//Number of blocks
	vector<Block>blocks;	//Represent every block at this board
	bool addBlock(int x, int y, int len, char lie);	//To add block to board
	void print();
	void BFS(Board root,char*arg);	//Breadth First Search
	void DFS(Board root,char*arg);
	bool isSolution();		//Is there any path to block out of the environment
	Board & operator=(const Board & node);	//To copy one Board to another
	friend bool operator==(Board & first, Board & second);	//To check two boards are same
	bool moveObstacle(int newplace, int id);	//To change block's location to newplace
	bool giveDirection(int id, bool upright);	//Rotate to direction(up,down,left,right) according to being horizontal or vertical
	vector<Board> calculateStates(Board root);	//Calculate states from given node and send these board states as vector
	void writeToFile(vector<Board> board,char*arg);
};

int main(int argc, char * argv[]){
	int x, y, len;
	char lie;
	Board board;
	string algorithm;
	algorithm = argv[1];
	ifstream inputFile;
	inputFile.open(argv[2]);
	int counter = 0;
	while (!inputFile.eof()){	//Take x,y,length,direction
		inputFile >> x >> y >> len >> lie;
		counter++;
		if (inputFile.eof()) break;
		board.addBlock(x, y, len, lie);
	}
	board.numBlocks = counter - 1;
	if (algorithm == "bfs"){
		board.BFS(board,argv[3]);
	}
	else if (algorithm == "dfs"){
		board.DFS(board,argv[3]);
	}

	return 0;
	
}

Board::Board(){	//Constructor for Board
	twoDimBlock = new int*[SIZE];	//Create 2D matrix and set them 0
	for (int xi = 0; xi < SIZE; xi++){
		twoDimBlock[xi] = new int[SIZE];
	}
	for (int xi = 0; xi < SIZE; xi++){
		for (int yi = 0; yi < SIZE; yi++){
			twoDimBlock[xi][yi] = 0;
		}
	}
}
bool Board::addBlock(int x, int y, int len, char lie){
	if (len <= 0 || len > SIZE)	//If length is negative, zero or bigger than the board size
		return false;
	if (lie == 'h'){	//If block is horizontal
		if (x< 1 || x> SIZE || y < 0 || y > SIZE - len)	// X coordinate is between 1 and SIZE. Y coordinate is between 0 and SIZE-length
			return false;
	}
	else{	//If block is vertical
		if (x < len || x> SIZE || y < 0 || y > SIZE - 1)		//X coordinate is between length and SIZE. Y coordinate is between 0 and SIZE-1
			return false;
	}

	if (lie == 'h'){	//If block is horizontal
		for (int i = 0; i < len; i++){
			if (twoDimBlock[x - 1][y + i] != 0)	//If there is a block which is added before
				return false;
		}
	}
	else{	//If block is vertical
		for (int i = 0; i < len; i++){
			if (twoDimBlock[x - 1 - i][y] != 0)	//If there is a block which is added before
				return false;
		}
	}
	Block newBlock;	//Create a new block and set properties
	newBlock.blockX = x;
	newBlock.blockY = y;
	newBlock.blockLen = len;
	newBlock.blockLie = lie;
	newBlock.blockId = blocks.size() + 1;
	blocks.push_back(newBlock);	//Add this block to blocks vector
	if (lie == 'h'){	//If block is horizontal
		for (int i = 0; i < len; i++)
		{
			twoDimBlock[x - 1][y + i] = newBlock.blockId;	//Set appropriate cells to block's id
		}
	}
	else{	//If block is vertical
		for (int i = 0; i < len; i++)
		{
			twoDimBlock[x - 1 - i][y] = newBlock.blockId;	//Set appropriate cells to block's id
		}
	}
	return true;
}
void Board::print(){	//Print board's 2D matrix
	for (int row = 0; row < SIZE; row++)
	{
		for (int col = 0; col < SIZE; col++)
		{
			cout << twoDimBlock[row][col] << "  ";
			if (col% SIZE == SIZE - 1)
				cout << endl;
		}
	}
	cout << endl;
}


Board & Board::operator=(const Board & node){	//Copy one board to another
	for (int xi = 0; xi < SIZE; xi++)
	{
		for (int yi = 0; yi < SIZE; yi++)
		{
			twoDimBlock[xi][yi] = node.twoDimBlock[xi][yi];	//Set 2D matrix

		}
	}
	blocks.clear();	//Remove blocks which are added before
	for (vector<Block>::size_type i = 0; i < node.blocks.size(); i++)	//Copy other node's blocks
	{
		blocks.push_back(node.blocks[i]);
	}
	numBlocks = node.numBlocks;
	order = node.order;
	return *this;
}

bool Board::moveObstacle(int newplace, int id){
	Block obstacle = blocks[id - 1];	//Take block according to id
	if (obstacle.blockLen == 'h'){	//If it is horizontal
		if (newplace < 0 || newplace > SIZE - obstacle.blockLen)	//new Y position must be between 0 and SIZE - length
			return false;
	}
	else{	//If it is vertical
		if (newplace < obstacle.blockLen || newplace > SIZE)	//new X position must be between length and SIZE
			return false;
	}

	if (obstacle.blockLie == 'h'){	//if it is horizontal
		for (int i = 0; i<obstacle.blockLen; i++){
			if (twoDimBlock[obstacle.blockX - 1][newplace + i] != 0 && twoDimBlock[obstacle.blockX - 1][newplace + i] != id)	//if new block cells are not zero or id,this means it is full
				return false;
		}
	}
	else{	//If it is horizontal
		for (int i = 0; i<obstacle.blockLen; i++){
			if (twoDimBlock[newplace - i - 1][obstacle.blockY] != 0 && twoDimBlock[newplace - i - 1][obstacle.blockY] != id)	//if new block cells are not zero or id,this means it is full
				return false;
		}
	}

	if (obstacle.blockLie == 'h'){	//If block is horizontal
		for (int i = 0; i < obstacle.blockLen; i++){
			twoDimBlock[obstacle.blockX - 1][obstacle.blockY + i] = 0;	//Set block cells to 0 
		}
	}
	else{	//If block is vertical
		for (int i = 0; i < obstacle.blockLen; i++){
			twoDimBlock[obstacle.blockX - i - 1][obstacle.blockY] = 0;	//Set block cells to 0 
		}
	}

	if (blocks[id - 1].blockLie == 'h'){	//If block is horizontal
		blocks[id - 1].blockY = newplace;	//Set its Y position to newplace
	}
	else{	//If block is vertical
		blocks[id - 1].blockX = newplace;	//Set its X position to newplace
	}

	if (blocks[id - 1].blockLie == 'h'){	//If block is horizontal
		for (int i = 0; i < obstacle.blockLen; i++){
			twoDimBlock[blocks[id - 1].blockX - 1][blocks[id - 1].blockY + i] = id;	//Set appropriate cells to its id
		}
	}
	else{	//If block is vertical
		for (int i = 0; i < obstacle.blockLen; i++){
			twoDimBlock[blocks[id - 1].blockX - i - 1][blocks[id - 1].blockY] = id;	//Set appropriate cells to its id
		}
	}
	return true;
}

bool Board::giveDirection(int id, bool upright){	//Send block to left,right,up,down
	if (id < 1 || id > blocks.size())	//Check id must be between 1 and number of blocks
		return 0;

	Block block = blocks[id - 1];	//Take appropriate block according to id

	if (block.blockLie == 'h'){	//If it is horizontal
		if (upright == 0){
			return this->moveObstacle(block.blockY - 1, id);	//Send it to left
		}
		else{
			return this->moveObstacle(block.blockY + 1, id);	//Send it to right
		}
	}
	else{	//If it is vertical
		if (upright == 0){
			return this->moveObstacle(block.blockX + 1, id);	//Send it to down
		}
		else{
			return this->moveObstacle(block.blockX - 1, id);	//Send it to up
		}
	}

	return 0;
}
bool Board::isSolution(){	//Check target block is at exit gate
	bool isSolved = true;
	for (int i = blocks[0].blockY + blocks[0].blockLen; i < SIZE; i++){
		if (twoDimBlock[blocks[0].blockX - 1][i] != 1 )
			isSolved = false;
	}
	return isSolved;
}

void Board::BFS(Board root,char* arg){	//Breadth First Search
	queue<Board>queue;	//Create queue which holds Board states(node)
	vector<Board>visited;	//Create a visited vector to prevent recheck same node multiple times
	map<int,int>parents;
	int counter = 1;
	root.order = counter;	//To find the previous nodes add boards to order number
	queue.push(root);	//Enqueue root node
	visited.push_back(root);	//Push root to visited
	parents[root.order]=root.order;
	Board board;
	while (!queue.empty()){	//While queue is not empty
		board = queue.front();	//Take board from queue
		if (board.isSolution()){	//If it is solution break the while loop
			break;
		}
		bool isVisited = 0;
		vector<Board>candidates = calculateStates(board);	//If it is not solution take other states
		for (vector<Board>::iterator it = candidates.begin(); it != candidates.end(); it++){	//If any state is visited before
			for (vector<Board>::iterator itt = visited.begin(); itt != visited.end(); itt++){
				if (*it == *itt){
					isVisited = 1;
				}
			}
			if (!isVisited){	//If node is not visited
				counter++;
				Board temp;
				temp = *it;
				temp.order = counter;
				queue.push(temp);	//Enqueue this node
				visited.push_back(temp);	//Set it as visited
				parents[counter]=board.order;
			}
			isVisited = 0;
		}
		queue.pop();	//We checked this node so pop it
	}
	deque<int> solution;
	if (board.isSolution()){	//If board is solution
		ofstream outputFile;
		outputFile.open(arg);	//Open output file
		while (board.order != 1){
			int temp = parents[board.order];	//Take parent nodes of solution and push them solution deque
			solution.push_back(temp);
			board.order = temp;
		}
		vector<Board>toFile;	//To write previous states to file keeps nodes
		for (deque<int>::size_type i = 0; i < solution.size(); i++){
			for (vector<Board>::iterator itt = visited.begin(); itt != visited.end(); itt++){
				int ord = solution[i];
				Board b = *itt;
				if (b.order == ord){
					toFile.push_back(b);	//Find board from board order and push it toFile
				}
			}
		}
		writeToFile(toFile,arg);
	}
}


void Board::DFS(Board root,char* arg){	//Depth First Search
	stack<Board>stack;	//Create stack which holds Board states(node)
	vector<Board>visited;	//Create a visited vector to prevent recheck same node multiple times
	map<int,int>parents;
	int counter = 1;
	root.order = counter;	//To find the previous nodes add boards to order number
	stack.push(root);	//Push root node
	visited.push_back(root);	//Push root to visited
	parents[root.order]=root.order;
	Board board;
	while (!stack.empty()){	//While stack is not empty
		board = stack.top();	//Take board from stack
		if (board.isSolution()){	//If it is solution break the while loop
			break;
		}
		bool isVisited = 0;
		vector<Board>candidates = calculateStates(board);	//If it is not solution take other states
		for (vector<Board>::iterator it = candidates.begin(); it != candidates.end(); it++){	//If any state is visited before
			for (vector<Board>::iterator itt = visited.begin(); itt != visited.end(); itt++){
				if (*it == *itt){
					isVisited = 1;
				}
			}
			if (!isVisited){	//If node is not visited
				counter++;
				Board temp;
				temp = *it;
				temp.order = counter;
				stack.push(temp);	//Push this node
				visited.push_back(temp);	//Set it as visited
				parents[counter]=board.order;
			}
			isVisited = 0;
		}
		stack.pop();	//We checked this node so pop it
	}
	deque<int> solution;
	if (board.isSolution()){	//If board is solution
		ofstream outputFile;	
		outputFile.open(arg);	//Open output file
		while (board.order != 1){	//Take parent nodes of solution and push them solution deque
			int temp = parents[board.order];
			solution.push_back(temp);
			board.order = temp;
		}
		vector<Board>toFile;	//To write previous states to file keeps nodes
		for (deque<int>::size_type i = 0; i < solution.size(); i++){		//Find board from board order and push it toFile
			for (vector<Board>::iterator itt = visited.begin(); itt != visited.end(); itt++){
				int ord = solution[i];
				Board b = *itt;
				if (b.order == ord){
					toFile.push_back(b);
				}
			}
		}
		writeToFile(toFile,arg);	//Write Solutions to file
	}
}



vector<Board> Board::calculateStates(Board root){
	vector<Board>states;	//Vector which holds Boards
	int upright = 1;
	Board newBoard;		//Create new board
	newBoard = root;	//Copy root to it
	for (int i = 0; i <= newBoard.numBlocks; i++){	//For every block at this board
		if (newBoard.giveDirection(i, !upright)){	//If it goes down or left
			Board firstchild;	//Create new Board
			firstchild = newBoard;	//Copy newly created board to it
			states.push_back(firstchild);	//Push it to states vector
		}
		newBoard = root;
		if (newBoard.giveDirection(i, upright)){	//If it goes up or right
			Board otherchild;	//Create new Board
			otherchild = newBoard;	//Copy newly created board to it
			states.push_back(otherchild);	//Push it to states vector
		}
		newBoard = root;
	}
	return states;	//When all possibilities are done return states
	
}
bool operator==(Board & first, Board & second){
	if (&first == &second)
		return true;

	if (first.blocks.size() != second.blocks.size())	//If block size is not same return false
		return false;

	for (vector<Block>::size_type i = 0; i < first.blocks.size(); i++){	//If blocks on these board are not same, they are not same,too
		if (first.blocks[i].blockX != second.blocks[i].blockX ||
			first.blocks[i].blockY != second.blocks[i].blockY ||
			first.blocks[i].blockLen != second.blocks[i].blockLen ||
			first.blocks[i].blockLie != second.blocks[i].blockLie)
			return false;
	}

	return true;
}

void Board::writeToFile(vector<Board> board,char*arg){
	ofstream outputFile;
	outputFile.open(arg);
	for (vector<Board>::size_type ii = 0; ii < board.size(); ii++){
		for (vector<Block>::size_type i = 0; i < board[ii].blocks.size(); i++)
		{
			outputFile<<board[ii].blocks[i].blockX<<" "<<board[ii].blocks[i].blockY<<" "<<board[ii].blocks[i].blockLen<<" "<<board[ii].blocks[i].blockLie<<"\n";
		}
		outputFile<<"\n";
	}
	
}
