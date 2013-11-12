

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

const char STONE_EMPTY   = '+';
const char STONE         = 'O';
const char STONE_INPLACE = '0';
int width  = 9;
int height = 9;
char *board;
char stats = 0;

//////////////////////////

// prints an error and stops the program
void error(char* msg, char *fun);

// returns the stone at the position
// if it is directly next to the bottom-left border, returns STONE_EMPTY
// if it is outside the board, exits with an error
char getStone(int x, int y);

// same, but with index instead of coordinates
// used for looping through all cells
char getStoneIndex(int i);

// places a stone without any further actions (see placeStone)
// if it is outside the board, exits with an error
void setStone(int x, int y, char c);

// marks itself and the stone above and right as STONE_INPLACE
// if empty or already marked, returns
void markStoneInPlace(int x, int y);

// places a stone
// if it blocks a STONE_INPLACE, marks itself as STONE_INPLACE
void placeStone(int x, int y, char c);

// tries to move a stone
// if it is blocked, tries to move the stone above and right
// this is where the magic happens!
void moveStone(int x, int y);

// outputs the already pre-formatted board
void printBoard();

//////////////////////////

// prints an error and stops the program
void error(char* msg, char *fun) {
	if (fun[0] > 0)
		printf("\n%s\nTerminating\n", msg, fun);
	else printf("\n%s (%s)\nTerminating\n", msg);
	printBoard();
	exit(0);
}

// returns the stone at the position
// if it is directly next to the bottom-left border, returns STONE_EMPTY
// if it is outside the board, exits with an error
char getStone(int x, int y) {
	if (x < -1 || y < -1 || x >= width || y >= height)
		error("Accessing stone out of bounds", "getStone(x,y)");
	if (x < 0 || y < 0) return STONE_EMPTY;
	return board[ x + (height-y-1)*(width+1) ];
}

// same, but with index instead of coordinates
char getStoneIndex(int i) {
	if (i < 0 || i >= (width+1)*height)
		error("Accessing stone out of bounds", "getStone(i)");
	return board[i];
}

// places a stone without any further actions (see placeStone)
// if it is outside the board, exits with an error
void setStone(int x, int y, char c) {
	if (x < 0 || y < 0 || x >= width || y >= height)
		error("Accessing stone out of bounds", "setStone");
	board[ x + (height-y-1)*(width+1) ] = c;
}

// marks itself and the stone above and right as STONE_INPLACE
// if empty or already marked, returns
void markStoneInPlace(int x, int y) {
	if (getStone(x, y) != STONE) return;
	board[ x + (height-y-1)*(width+1) ] = STONE_INPLACE;
	markStoneInPlace(x, y+1);
	markStoneInPlace(x+1, y);
}

// places a stone
// if it blocks a STONE_INPLACE
//     or is in the bottom-left triangle,
//     marks itself as STONE_INPLACE
void placeStone(int x, int y, char c) {
	setStone(x, y, c);
	if (c == STONE_EMPTY) return;
	if (getStone(x-1, y) == STONE_INPLACE || getStone(x, y-1) == STONE_INPLACE)
		markStoneInPlace(x, y);
	else if (x+y < 3) markStoneInPlace(x, y);
}

// tries to move a stone
// if it is blocked, tries to move the stone above and right
// this is where the magic happens!
void moveStone(int x, int y) {
	if (getStone(x, y) == STONE_EMPTY)
		error("Error moving non-existent stone", "moveStone");
 	else if (getStone(x+1, y) != STONE_EMPTY) {
		moveStone(x+1, y);
		return;
	}
	else if (getStone(x, y+1) != STONE_EMPTY) {
		moveStone(x, y+1);
		return;
	}
	setStone(x, y, STONE_EMPTY);
	placeStone(x+1, y, STONE);
	placeStone(x, y+1, STONE);
}

// outputs the already pre-formatted board
void printBoard() {
	int i, stones = 0, inplaces = 0, boardsize = (width+1)*height;
	printf("\n%s\n", board);
	// statistics
	if (!stats) return;
	for (i = 0; i < boardsize; i++) {
		if (getStoneIndex(i) == STONE) stones++;
		else if (getStoneIndex(i) == STONE_INPLACE) inplaces++;
	}
	printf("\nstones:   %i \t%i%%\nblocking: %i \t%i%%\n", stones, 100*stones/boardsize, inplaces, 100*inplaces/boardsize);
}

// print command usage text and terminate the program
void exitUsage(char *command) {
	printf("Usage: %s [-b <width> <height>] [-s]\nwidth, height > 0, -s = stats on", command);
	exit(0);
}

int main(int argc, char *argv[]) {
	int i = 1;
	// parse arguments
	while (i < argc) {
		if (argv[i][0] == '-') {
			// switch
			if (argv[i][1] == 'h') {
				printf("Writing help\n");
				exitUsage(argv[0]);
			}
			else if (argv[i][1] == 's') {
				printf("Writing stats\n");
				stats = 1;
			}
			// parameter
			else if (argv[i][1] == 'b') {
				printf("Setting board size\n");
				width = atoi(argv[i+1]);
				height = atoi(argv[i+2]);
				i += 2;
			}
		}
		else {
			printf("Parse error: %s\n", argv[i]);
			exitUsage(argv[0]);
		}
		i++;
	}
	printf("Board: %ix%i\n", width, height);
	if (stats) printf("Stats are ON\n");
	printf("\n%c empty\n%c stone\n%c blocking\n", STONE_EMPTY, STONE, STONE_INPLACE);
	board = (char *) calloc( (width+1)*height, sizeof(char) );
	for (i = 0; i < (width+1)*height; i++) {
		if ( i % (width+1) == width ) {
			board[i] = '\n';
		}
		else board[i] = STONE_EMPTY;
	}
	board[(width+1)*height-1] = '\0';
	setStone(0, 0, STONE_INPLACE);
	printBoard();
	sleep(1);
	// loop checks the 3 stones in the
	// bottom left corner that must be removed
	// 0????
	// ?0???
	// ??0??
	while (1) {
		int a, x, y;
		for (a = 2; a >= 0; a--) {
			for (x = 0; x <= a; x++) {
				y = a-x;
				if (getStone(x, y) == STONE_INPLACE) {
					moveStone(x, y);
					break; // for loop
				}
			}
		}
		printBoard();
		usleep(100000);
	}
	return 0;
}

