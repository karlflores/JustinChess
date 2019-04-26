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
	// move pawn from A2 to A3
	board = Board();
	board.printBitBoard(board.P_WHITE);
	board.movePiece(board.P_BLACK,board.P_PAWN,Board::D7,Board::D5);
	board.movePiece(board.P_WHITE,board.P_PAWN,Board::C2,Board::C4);
	board.printBitBoard(board.P_WHITE);	
	board.printBitBoard(board.P_PAWN);	
	board.setBoard(board.P_PAWN,board.bPawnCaptureAll() | board.wPawnCaptureEast());
	board.printBitBoard(board.P_PAWN);

	for(int i = 0 ; i < 64 ; i++){
		cout << "KNIGHT AT " << i << "\n";
		board.setBoard(board.P_KNIGHT,board.kingAttackSet[i]);
		board.printBitBoard(board.P_KNIGHT);
	}
	board.setBoard(board.P_KNIGHT,Board::NOT_A_FILE);
	board.printBitBoard(board.P_KNIGHT);

	board = Board();
	board.setBoard(board.P_KNIGHT,board.getQueenAttacks(Board::E3));
	board.printBitBoard(board.P_KNIGHT);
		
	board.deleteStructures();
	return 0;
}
