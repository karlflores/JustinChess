#include <iostream> 
#include <string> 
#include <vector> 

using namespace std;

#define NUM_PIECES 8
typedef unsigned long long u_int64;

class Board{ 
	// Definitions 
	private:
		u_int64 pieceBB[NUM_PIECES];

	public:
		
		Board(void);
		
		// PieceType enum -- for indexing on the pieceBB
		enum PieceType{
			pWhite,
			pBlack,
			pPawn,
			pKnight,
			pBishop,
			pRook,
			pQueen,
			pKing	
		};
		
		enum ColourType{
			white,
			black, 	
		};

		// NOTE these are Little Endian Rank File Mapping Magic Numbers 
		static const u_int64 AFILE =            	0x0101010101010101;
		static const u_int64 HFILE =           		0x8080808080808080;
		static const u_int64 RANK1 =         		0x00000000000000FF;
		static const u_int64 RANK8 =         		0xFF00000000000000;
		static const u_int64 DIAGONAL =   			0x8040201008040201;
		static const u_int64 ANTIDIAGONAL = 	 	0x0102040810204080;
		static const u_int64 LIGHT_SQUARES =    	0x55AA55AA55AA55AA;
		static const u_int64 DARK_SQUARES =     	0xAA55AA55AA55AA55;
		
		// NOTE: we are using Little Endian Rank-File Mappings 	
		enum SquarePos{
			A1, B1, C1, D1, E1, F1, G1, H1,
		  	A2, B2, C2, D2, E2, F2, G2, H2,
		 	A3, B3, C3, D3, E3, F3, G3, H3,
		  	A4, B4, C4, D4, E4, F4, G4, H4,
		  	A5, B5, C5, D5, E5, F5, G5, H5,
		  	A6, B6, C6, D6, E6, F6, G6, H6,
		  	A7, B7, C7, D7, E7, F7, G7, H7,
		  	A8, B8, C8, D8, E8, F8, G8, H8
		};		
		
		/*
		 * METHODS ON THE BOARD 
		 */

		// test functions on the board
		
		// Get the pieces of a specific colour 
		u_int64 getPieceSet(PieceType pt, PieceType ct){ 
			return pieceBB[pt] & pieceBB[ct];
		}
		
		// get the bitboard of a specific piece set 	
		u_int64 getBitBoard(PieceType pt){
			return pieceBB[pt];
		}
		// test function to get all the pawns on the board 
		u_int64 getPawns(PieceType ct){
			return pieceBB[pPawn] & pieceBB[ct];
		}
		
		// test functions for setting, getting pieces on the board
		void setBoard(PieceType pt, u_int64 board){
			pieceBB[pt] = board;	
		}
		
		// print the board in a logical manner 
		void printBitBoard(PieceType pt){
			int boardWidth = 8;
			cout << "BOARD: [" << pt << "]\n";
			for(int rank = boardWidth-1; rank >= 0 ; rank--){
				for(int file = 0; file < 8 ; file ++){
					cout << containsPiece(pt, static_cast<SquarePos>(rank*boardWidth + file));
					cout << " ";
				}
				cout << "\n";

			}	
		}
		
		// returns true if a square at a given location is occupied 
		// Need to provide it with the position of that piece as well as the piece type  
		bool containsPiece(PieceType pt, SquarePos pos){
			// bitshift the mask the the right location on the board 
			// if the result is greater than zero we know a piece resides on 
			// that location
			
			return (pieceBB[pt] & (getMovingMask(pos))) > 0; 	
		}
		
		// helper function for the movePiece method -> moving a piece from one location to another 
		u_int64 getMovingMask(SquarePos pos){

			return 1L << pos;
		}

		// moves a piece from its old position to a new position 
		// NOTE: this is not a safe function, we need to wrap it in a new function
		// maybe we handle the moving of the piece in the Chess Class 

		void movePiece(PieceType pt, SquarePos oldPos, SquarePos newPos){
			
			// set the old location to a zero 
			pieceBB[pt] = pieceBB[pt] & !(getMovingMask(oldPos));

			// set the new location to a 1 
			pieceBB[pt] = pieceBB[pt] | getMovingMask(newPos);

			// todo -- make sure that the pWhite and pBlack bitboards are updated 
		}
};

// Constructor 
Board::Board(){
	// set the initial positions of each piece 
	pieceBB[pWhite] = Board::RANK1 | (Board::RANK1 << 8) ;
	pieceBB[pBlack] = (Board::RANK8 >> 8) | Board::RANK8;	

	// now for each of the pieces -- here we are bitshifting by the actual square position 
	// on the board 
	
	//pPawn
	pieceBB[pPawn] = (Board::RANK1 << 8) | (Board::RANK8 >> 8);	
	//pKnight
	pieceBB[pKnight] = 0L | 1L << (B1) | 1L << (G1) | 1L << (G8) | 1L << (B8);
	//pBishop
	pieceBB[pBishop] = 0L | 1L << (C1) | 1L << (F1) | 1L << (F8) | 1L << (C8);
	//pRook
	pieceBB[pRook] = 0L | 1L << (A1) | 1L << (H8) | 1L << (H1) | 1L << (A8);
	//pQueen
	pieceBB[pQueen] = 0L | 1L << (D1) | 1L << (D8); 
	//pKing	
	pieceBB[pKing] = 0L | 1L << (E1) | 1L << (E8); 
	
}

int main(){
	// THIS IS JUST FOR TESTING FOR NOW 
	Board board = Board();

	// test ranks 
	for(int r = 0; r < 8 ; r++){	
			cout << r <<  "\n\n";
			board.setBoard(Board::pWhite,board.RANK1 << 8*r);
			board.printBitBoard(Board::pWhite);
	}

	// test file 	
	for(int h = 0; h < 8 ; h++){	
			cout << h <<  "\n\n";
			board.setBoard(Board::pWhite,board.AFILE << h);
			board.printBitBoard(Board::pWhite);
	}	

	cout << "\n MOVE PIECE from A1 to A4\n\n";
	// move piece from 0,0 to 4,4 
	board.movePiece(Board::pWhite,Board::A1,Board::A4);

	board.printBitBoard(Board::pWhite);

	cout << "\n MOVE PIECE from A4 to D4\n\n";
	// move piece from 0,0 to 4,4 
	board.movePiece(Board::pWhite,Board::A4,Board::D4);

	board.printBitBoard(Board::pWhite);
	
	// Bit board testing 
	board = Board();

	cout << "\n\n\n\n";
	cout << "testing all bit boards\n";
	for(int b = board.pWhite ; b <= board.pKing ; b++){
		board.printBitBoard(static_cast<Board::PieceType>(b));
	}
	
	// testing each of the magic numbers to see if they are correct	
	board.setBoard(board.pWhite,board.getBitBoard(board.pWhite) & 
								board.getBitBoard(board.pPawn));	
	board.printBitBoard(board.pWhite);

	return 0;
}
