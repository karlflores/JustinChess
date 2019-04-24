#include "headers/board.h"
using namespace std;

int main(){
	// THIS IS JUST FOR TESTING FOR NOW 
	Board board = Board();

	// test ranks 
	for(int r = 0; r < 8 ; r++){	
			cout << r <<  "\n\n";
			board.setBoard(Board::P_WHITE,board.RANK1 << 8*r);
			board.printBitBoard(Board::P_WHITE);
	}

	// test file 	
	for(int h = 0; h < 8 ; h++){	
			cout << h <<  "\n\n";
			board.setBoard(Board::P_WHITE,board.AFILE << h);
			board.printBitBoard(Board::P_WHITE);
	}	

	cout << "\n MOVE PIECE from A1 to A4\n\n";
	// move piece from 0,0 to 4,4 
	board.movePiece(Board::P_WHITE,Board::A1,Board::A4);

	board.printBitBoard(Board::P_WHITE);

	cout << "\n MOVE PIECE from A4 to D4\n\n";
	// move piece from 0,0 to 4,4 
	board.movePiece(Board::P_WHITE,Board::A4,Board::D4);

	board.printBitBoard(Board::P_WHITE);
	
	// Bit board testing 
	board = Board();

	cout << "\n\n\n\n";
	cout << "testing all bit boards\n";
	for(int b = board.P_WHITE ; b <= board.P_KING ; b++){
		board.printBitBoard(static_cast<Board::PieceType>(b));
	}
	
	// testing each of the magic numbers to see if they are correct	
	/*
	board.setBoard(board.P_WHITE,board.getBitBoard(board.P_WHITE) & 
								board.getBitBoard(board.P_PAWN));	

	board.printBitBoard(board.P_WHITE);
*/	
	board.setBoard(board.P_WHITE,board.bDoublePawnPushSrc());
	board.printBitBoard(board.P_WHITE);
	return 0;
}
