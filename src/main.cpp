#include "headers/board.h"

using namespace std;

int main(){
	// THIS IS JUST FOR TESTING FOR NOW 

	Board *board = new Board();
	board->setBoard(P_KNIGHT,board->getQueenAttacks(E3));
	board->printBitBoard(P_KNIGHT);
		
	board->deleteStructures();
	delete(board);
	return 0;
}
