#include <iostream> 
#include <string> 
#include <vector> 

using namespace std;

#define NUM_PIECES 8
typedef unsigned long long u_int64;

enum PieceType{
	pt_pempty = 0,
	pt_wpawn = 1,
	pt_bpawn = 2,
	pt_knight = 3,
	pt_rook = 5,
	pt_queen = 6,
	pt_king = 7,
};

class Board{ 
	
	// Definitions 
	private:
		u_int64 pieceBB[NUM_PIECES];

	public:
		
		Board(void);

		enum PieceType{
			nWhite,
			nBlack,
			nPawn,
			nKnight,
			nBishop,
			nRook,
			nQueen,
			nKing	
		};
		
		enum ColourType{
			white,
			black, 	
		};
		
		// test functions on the board
		u_int64 getPieceSet(PieceType pt, PieceType ct){ 
			return pieceBB[pt] & pieceBB[ct];
		}
		
		// test function to get all the pawns on the board 
		u_int64 getPawns(ColourType ct){
			return pieceBB[nPawn] & pieceBB[ct];
		}
		
		// test functions for setting, getting pieces on the board
		void setBoard(PieceType pt, u_int64 board){
			pieceBB[pt] = board;	
		}

		void printBoard(PieceType pt){
			int boardWidth = 8;
			
			for(int rank = 0; rank < boardWidth ; rank++){
				for(int file = 0; file <boardWidth ; file++){
					cout << getPiece(pt, rank, file);		
					cout << " ";
				}
				cout << "\n";
			}	
		}

		bool getPiece(PieceType pt, int rank, int file){
			// bitshift the mask the the right location on the board 
			// if the result is greater than zero we know a piece resides on 
			// that location
			
			return (pieceBB[pt] & (getMovingMask(rank,file))) > 0; 	
		}
		

		u_int64 getMovingMask(int rank, int file){

			return 1L << (rank * 8 + file);
		}
		void movePiece(PieceType pt, int old_r, int old_f, int new_r, int new_f){
			
			// set the old location to a zero 
			pieceBB[pt] = pieceBB[pt] & !(getMovingMask(old_r,old_f));

			// set the new location to a 1 
			pieceBB[pt] = pieceBB[pt] | getMovingMask(new_r,new_f);
		}
};

Board::Board(){
	for(int i = 0 ; i < 8 ; i++){
		pieceBB[i] = 0L;	
	}
}

int main(){

	Board board = Board();
	board.setBoard(Board::nWhite,1L);
	board.printBoard(Board::nWhite);

	cout << "\n MOVE PIECE from 0,0 to 4,4\n\n";
	// move piece from 0,0 to 4,4 
	board.movePiece(Board::nWhite,0,0,4,4);

	board.printBoard(Board::nWhite);

	cout << "\n MOVE PIECE from 4,4 to 1,1\n\n";
	// move piece from 0,0 to 4,4 
	board.movePiece(Board::nWhite,4,4,6,1);

	board.printBoard(Board::nWhite);
	return 0;
}
