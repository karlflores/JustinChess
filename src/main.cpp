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
	board->setBoard(pt,~(~board->getRookAttacks(pos) |
					board->getBitBoard(ct)));
	board->printBitBoard(pt);
	
	board->setBoard(pt,~(~board->getBishopAttacks(pos) | 
					board->getBitBoard(ct)));

	board->printBitBoard(pt);

	board->setBoard(pt,~(~board->getQueenAttacks(pos) |
					board->getBitBoard(ct)));
	board->printBitBoard(pt);
		
		
	// get the piecePosList 
	SquarePos *posList = Board::bbToPosList(board->getBitBoard(pt));

	// Iterate through the position list 
	for(SquarePos *pos = posList ; *pos!=EMPTY ; pos++){
		cout << Board::positionToStr(*pos) << " ";
	}
	cout << "\n";
	u_int64 *pieces = board->copy();
	for(int i = 0 ; i < 8 ; i++){
		cout << pieces[i] << "\n";
	}

	delete(posList);
	
	delete(pieces);
	
	board->deleteStructures();

	delete(board);
	
	return 0;
}
