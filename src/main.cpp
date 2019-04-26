#include "headers/board.h"

using namespace std;

int main(){
	// THIS IS JUST FOR TESTING FOR NOW 

	string position;
   	string colour;	
	string direction;
	string piecetype;
	cout << "ENTER PIECETYPE/COLOUR, POS, DIR\n";
	cin >> piecetype >> colour >> position >> direction;
	
	Direction dir = Board::strToDirection(direction);
	PieceType pt = Board::strToPieceType(piecetype);
	PieceType ct = Board::strToPieceType(colour);
	SquarePos pos = Board::strToPosition(position);

	cout << Board::pieceTypeToStr(pt) << "\n";
	cout << Board::pieceTypeToStr(ct) << "\n";
	cout << Board::positionToStr(pos) << "\n";
	cout << Board::directionToStr(dir) << "\n";

	Board *board = new Board();
	// THE FOLLOWING GETS THE TARGET ATTACK SQUARES 
	// NOT INCLUDING THE PIECES OF THE PLAYER
	board->setBoard(pt,~(~board->getQueenAttacks(pos) |
					board->getBitBoard(ct)));
	board->printBitBoard(pt);
		
	board->setBoard(pt,~(~board->getRookAttacks(pos) |
					board->getBitBoard(ct)));
	board->printBitBoard(pt);
	board->deleteStructures();
	
	board->setBoard(pt,~(~board->getBishopAttacks(pos) | 
					board->getBitBoard(ct)));

	board->printBitBoard(pt);
	delete(board);
	
	return 0;
}
