#include <iostream> 
#include <string> 
#include <vector> 

#define NUM_PIECES 8
#define MAX_STR_LEN 20
#define MAX_PT_NUM 8
typedef unsigned long long u_int64;
using namespace std;

class Board{ 
	// Definitions 
	private:
		u_int64 pieceBB[NUM_PIECES];
		string pt_names[MAX_PT_NUM] = 
		{
			"WHITE",
			"BLACK",
			"PPAWN",
			"PKNIGHT",
			"PBISHOP",
			"PROOK",
			"PQUEEN",
			"PKING"
		};
	public:
		
		Board(void);
		
		// PieceType enum -- for indexing on the pieceBB
		enum PieceType{
			P_WHITE,
			P_BLACK,
			P_PAWN,
			P_KNIGHT,
			P_BISHOP,
			P_ROOK,
			P_QUEEN,
			P_KING,	
		};
		
		enum ColourType{
			white,
			black, 	
		};

		// location offsets  
		enum RayDirections{
			N = 8,
			NE = 1,
			SE = -7,
			S = -8,
			SW = -9,
			W = -1,
			NW =7,
		};

		// knight directions 

		enum KnightDirections{
			NNW = 15,
			NNE = 17,
			NEE = 10,
			SEE = -6,
			SSE = -15,
			SSW = -17,
			SWW = -10,
		   	NWW = 6,	
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
		u_int64 getPieceSet(PieceType pt, PieceType ct);
		
		// get the bitboard of a specific piece set 	
		u_int64 getBitBoard(PieceType pt);
		// test function to get all the pawns on the board 
		u_int64 getPawns(PieceType ct);
		
		// test functions for setting, getting pieces on the board
		void setBoard(PieceType pt, u_int64 board);
		
		// print the board in a logical manner 
		void printBitBoard(PieceType pt);
		
		// returns true if a square at a given location is occupied 
		// Need to provide it with the position of that piece as well as the piece type  
		bool containsPiece(PieceType pt, SquarePos pos);
		
		// helper function for the movePiece method -> moving a piece from one location to another 
		u_int64 getMovingMask(SquarePos pos);
		// moves a piece from its old position to a new position 
		// NOTE: this is not a safe function, we need to wrap it in a new function
		// maybe we handle the moving of the piece in the Chess Class 

		void movePiece(PieceType pt, SquarePos oldPos, SquarePos newPos);

		// copy board
		Board copy();

		string pt_name(PieceType pt);
};
