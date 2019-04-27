#include <iostream> 
#include <cstdlib>
#include <string> 
#include <vector> 
#include <map>
#include <algorithm>
#include <stack>

#define NUM_PIECES 8
#define NUM_SQUARES 64
#define MAX_STR_LEN 20
#define MAX_PT_NUM 8

typedef unsigned long long u_int64;
using namespace std;

// NOTE these are Little Endian Rank File Mapping Magic Numbers 
static const u_int64 AFILE =            	0x0101010101010101;
static const u_int64 HFILE =           		0x8080808080808080;
static const u_int64 RANK1 =         		0x00000000000000FF;
static const u_int64 RANK8 =         		0xFF00000000000000;
static const u_int64 DIAGONAL =   			0x8040201008040201;
static const u_int64 ANTIDIAGONAL = 	 	0x0102040810204080;
static const u_int64 LIGHT_SQUARES =    	0x55AA55AA55AA55AA;
static const u_int64 DARK_SQUARES =     	0xAA55AA55AA55AA55;
static const u_int64 NOT_A_FILE = 			0xfefefefefefefefe;
static const u_int64 NOT_H_FILE = 			0x7f7f7f7f7f7f7f7f;
static const u_int64 NOT_AB_FILE = 	   ~(AFILE | (AFILE << 1));
static const u_int64 NOT_GH_FILE = 		(HFILE | (HFILE >> 1));
static const u_int64 DEBRUIJN64 = 			0x03f79d71b4cb0a89;


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

const string PT_STRINGS[] = 
{
	"WHITE",
	"BLACK",
	"PAWN",
	"KNIGHT",
	"BISHOP",
	"ROOK",
	"QUEEN",
	"KING"
};

enum ColourType{
	WHITE,
	BLACK, 	
};

const string CT_STRINGS[] = 
{
	"WHITE",
	"BLACK"
};

// location offsets  
enum Direction{
	N =   8,
	NE =  9,
	E =   1,
	SE = -7,
	S =  -8,
	SW = -9,
	W =  -1,
	NW =  7,
	NNW = 15,
	NNE = 17,
	NEE = 10,
	SEE = -6,
	SSE = -15,
	SSW = -17,
	SWW = -10,
	NWW = 6,	
};

const map<Direction,string> DIR_STR_DICT{
	{N, "N"},
	{S, "S"},
	{W, "W"},
	{E, "E"},
	{NW, "NW"},
	{NE, "NE"},
	{SE, "SE"},
	{SW, "SW"},	
	{NNW, "NNW"},
	{NNE, "NNE"},
	{NWW, "NWW"},
	{NEE, "NEE"},
	{SSW, "SSW"},
	{SWW, "SWW"},
	{SEE, "SEE"},
	{SSE, "SSE"},	
};

// NOTE: we are using Little Endian Rank-File Mappings 	
enum SquarePos{
	A1, B1, C1, D1, E1, F1, G1, H1,
	A2, B2, C2, D2, E2, F2, G2, H2,
	A3, B3, C3, D3, E3, F3, G3, H3,
	A4, B4, C4, D4, E4, F4, G4, H4,
	A5, B5, C5, D5, E5, F5, G5, H5,
	A6, B6, C6, D6, E6, F6, G6, H6,
	A7, B7, C7, D7, E7, F7, G7, H7,
	A8, B8, C8, D8, E8, F8, G8, H8,
	EMPTY
};		

const string POS_STRING[]{
	"A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1",
	"A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2",
	"A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3",
	"A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4",
	"A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5",
	"A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6",
	"A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7",
	"A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8"
};		

// FOR DEBRUJIN BITSCAN ALGORITHM 
const int BITSCAN_INDEX64[] = {
	0, 47,  1, 56, 48, 27,  2, 60,
	57, 49, 41, 37, 28, 16,  3, 61,
	54, 58, 35, 52, 50, 42, 21, 44,
	38, 32, 29, 23, 17, 11,  4, 62,
	46, 55, 26, 59, 40, 36, 15, 53,
	34, 51, 20, 43, 31, 22, 10, 45,
	25, 39, 14, 33, 19, 30,  9, 24,
	13, 18,  8, 12,  7,  6,  5, 63
};

class Board{ 
	// Definitions 
	private:
		u_int64 pieceBB[NUM_PIECES];
		
		stack<u_int64*> history;
				
		// Attack set generation	
		void knightAttackSetGeneration();
		void kingAttackSetGeneration();
		
		void lineAttackGeneration();
		u_int64 rankAttackMask(int pos);
		u_int64 fileAttackMask(int pos);
		u_int64 diagonalAttackMask(int pos);
		u_int64 antiDiagonalAttackMask(int pos);

		
		u_int64 getPositiveRay(u_int64 lineAttacks, SquarePos pos);
		u_int64 getNegativeRay(u_int64 lineAttacks, SquarePos pos);	
		u_int64 getLineAttack(Direction dir, SquarePos pos);

		u_int64 getNegativeRayAttacks(Direction dir, SquarePos pos);
		u_int64 getPositiveRayAttacks(Direction dir, SquarePos pos);

	public:
		
		Board(void);
			
		// ATTACK SETS -- FOR KNIGHT AND KING, THESEARE INDEXED BY POSITION OF THE KING i.e 0..63 -> A1..H8.	
		// e.g. knightAttackSet[Board::A1]
		// KNIGHT ATTACK DICTIONARY 
		u_int64 *knightAttackSet;	

		// KING ATTACK DICTIONARY 
		u_int64 *kingAttackSet;
		u_int64 *fileAttackSet;
		u_int64 *rankAttackSet;
		u_int64 *diagonalAttackSet;
		u_int64 *antiDiagonalAttackSet;
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
		
		static SquarePos* bbToPosList(u_int64 bb);		
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

		// Move a piece on a specific bitboard 
		void movePiece(PieceType pt, SquarePos oldPos, SquarePos newPos);

		// Move a coloured piece on the board, as well as moving it on the 
		// specific bitboard 
		void movePiece(PieceType pt, PieceType ct, SquarePos oldPos, SquarePos newPos);
		
		u_int64 getEmptySquares();

		// copy board
		u_int64 *copy();

		
		// Board transformations 

		u_int64 tNorth(u_int64 bb);
		u_int64 tSouth(u_int64 bb);
		u_int64 tEast(u_int64 bb);
		u_int64 tWest(u_int64 bb);
		
		u_int64 tNorthEast(u_int64 bb);
		u_int64 tNorthWest(u_int64 bb);
		u_int64 tSouthEast(u_int64 bb);
		u_int64 tSouthWest(u_int64 bb);

		u_int64 tNNE(u_int64 bb);
		u_int64 tNEE(u_int64 bb);
		u_int64 tSEE(u_int64 bb);
		u_int64 tSSE(u_int64 bb);
		u_int64 tNNW(u_int64 bb);
		u_int64 tNWW(u_int64 bb);
		u_int64 tSWW(u_int64 bb);
		u_int64 tSSW(u_int64 bb);

		// PAWN MOVES
		u_int64 wSinglePawnPushDest();			
		u_int64 bSinglePawnPushDest();			
		u_int64 wDoublePawnPushDest();
		u_int64 bDoublePawnPushDest();

		u_int64 wSinglePawnPushSrc();
		u_int64 wDoublePawnPushSrc();
		u_int64 bSinglePawnPushSrc();
		u_int64 bDoublePawnPushSrc();
		
		// PAWN ATTACKS 
		u_int64 wPawnEastAttacks();
		u_int64 wPawnWestAttacks();
		u_int64 bPawnEastAttacks();
		u_int64 bPawnWestAttacks();

		u_int64 wPawnAllAttacks();
		u_int64 wPawnDoubleAttacks();
		u_int64 wPawnSingleAttacks();
		
		u_int64 bPawnAllAttacks();
		u_int64 bPawnDoubleAttacks();
		u_int64 bPawnSingleAttacks();
		// PAWN CAPTURES 

		u_int64 wPawnCaptureEast();
		u_int64 wPawnCaptureWest();
		u_int64 wPawnCaptureAll();

		u_int64 bPawnCaptureEast();
		u_int64 bPawnCaptureWest();
		u_int64 bPawnCaptureAll();
		

		// King and Knight Captures 
		u_int64 wKingCaptureMap();
		u_int64	bKingCaptureMap(); 
		u_int64 wKnightCaptureMap();
		u_int64	bKnightCaptureMap(); 

		// Sliding Piece Generation 
		u_int64 getRookAttacks(SquarePos pos);
		u_int64 getBishopAttacks(SquarePos pos);
		u_int64 getQueenAttacks(SquarePos pos);
		// clean up methods 
		void deleteStructures();
		
		// helper methods 
		static int bitScanForward(u_int64 bb);	
		static int bitScanReverse(u_int64 bb);	
		
		// ray attack helper methods 
		u_int64 getRay(Direction dir, SquarePos pos);
		u_int64 getRayAttacks(Direction dir, SquarePos pos);	

		// other helper methods 
		static Direction strToDirection(string dir);
		static SquarePos strToPosition(string pos);
		static PieceType strToPieceType(string pos);
		static string directionToStr(Direction dir);
		static string positionToStr(SquarePos pos);	
		static string pieceTypeToStr(PieceType pt);
};
