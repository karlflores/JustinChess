#include "headers/board.h"

using namespace std;

//
//NOTE WHEN SHIFTING COLUMNS -> SHIFT 1 COL to the RIGHT << 1
//1 ROW UP << 8
/*
 * METHODS ON THE BOARD 
 */

// Constructor 
Board::Board(){
	// set the initial positions of each piece 
	pieceBB[P_WHITE] = RANK1 | (RANK1 << 8) ;
	pieceBB[P_BLACK] = (RANK8 >> 8) | RANK8;	

	// now for each of the pieces -- here we are bitshifting by the 
	// actual square position on the board 
	
	//P_PAWN
	pieceBB[P_PAWN] = (RANK1 << 8) | (RANK8 >> 8);	
	//P_KNIGHT
	pieceBB[P_KNIGHT] = 0L | 1L << (B1) | 1L << (G1) | 1L << (G8) | 1L << (B8);
	//P_BISHOP
	pieceBB[P_BISHOP] = 0L | 1L << (C1) | 1L << (F1) | 1L << (F8) | 1L << (C8);
	//P_ROOK
	pieceBB[P_ROOK] = 0L | 1L << (A1) | 1L << (H8) | 1L << (H1) | 1L << (A8);
	//P_QUEEN
	pieceBB[P_QUEEN] = 0L | 1L << (D1) | 1L << (D8); 
	//P_KING	
	pieceBB[P_KING] = 0L | 1L << (E1) | 1L << (E8); 

	// populate the pieceDict	
	knightAttackSetGeneration();
	kingAttackSetGeneration();
	lineAttackGeneration();
}

// test functions on the board

// Get the pieces of a specific colour 
u_int64 Board::getPieceSet(PieceType pt, PieceType ct){ 
	return pieceBB[pt] & pieceBB[ct];
}

// get the bitboard of a specific piece set 	
u_int64 Board::getBitBoard(PieceType pt){
	return pieceBB[pt];
}
// test function to get all the pawns on the board 
u_int64 Board::getPawns(PieceType ct){
	return pieceBB[P_PAWN] & pieceBB[ct];
}

// test functions for setting, getting pieces on the board
void Board::setBoard(PieceType pt, u_int64 board){
	pieceBB[pt] = board;	
}

// print the board in a logical manner 
void Board::printBitBoard(PieceType pt){
	int boardWidth = 8;
	cout << "BOARD: [" << ptString(pt) << "]\n";

	// print the bit board in an 8 x 8 grid that mirrors the file rank system 
	// of chess
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
bool Board::containsPiece(PieceType pt, SquarePos pos){
	// bitshift the mask the the right location on the board 
	// if the result is greater than zero we know a piece resides on 
	// that location
	
	return (pieceBB[pt] & (getMovingMask(pos))) > 0; 	
}

// helper function for the movePiece method -> moving a 
// piece from one location to another 
u_int64 Board::getMovingMask(SquarePos pos){

	return 1L << pos;
}

// moves a piece from its old position to a new position 
// NOTE: this is not a safe function, we need to wrap it in a new function
// maybe we handle the moving of the piece in the Chess Class 
void Board::movePiece(PieceType pt, SquarePos oldPos, SquarePos newPos){
	
	// set the old location to a zero 
	
	pieceBB[pt] = pieceBB[pt] & ~(getMovingMask(oldPos));
	// set the new location to a 1 
	pieceBB[pt] = pieceBB[pt] | getMovingMask(newPos);

	// todo -- make sure that the P_WHITE and P_BLACK bitboards are updated 
}

void Board::movePiece(PieceType pt, PieceType ct, SquarePos oldPos, SquarePos newPos){
	movePiece(pt, oldPos, newPos);
	movePiece(ct, oldPos, newPos);	
}
Board Board::copy(){
	// create a new copy of the board, then copy everything inside the array  
	Board newBoard = Board();
	
	for(int i = 0 ; i <=7; i++){
		newBoard.pieceBB[i] = pieceBB[i];
	}

	return newBoard;
}

// get the name of the piece type enum for printing
string Board::ptString(PieceType pt){
	return PT_STRINGS[pt];
}

u_int64 Board::getEmptySquares(){
	return ~(pieceBB[P_WHITE] | pieceBB[P_BLACK]);
}

// Clean up

void Board::deleteStructures(){
	delete(kingAttackSet);
	delete(knightAttackSet);
	delete(fileAttackSet);
	delete(rankAttackSet);
	delete(diagonalAttackSet);
	delete(antiDiagonalAttackSet);
}

// Helper methods
/**
 * bitScanForward
 * @author Kim Walisch (2012)
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of least significant one bit
 */ 
int Board::bitScanForward(u_int64 bb){
	assert(bb != 0);

	return bitscanIndex64[((bb ^ (bb-1)) * DEBRUIJN64) >> 58]; 
}

/**
 * bitScanReverse
 * @authors Kim Walisch, Mark Dickinson
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of most significant one bit
 */
int Board::bitScanReverse(u_int64 bb) {
   assert(bb != 0);

   bb |= bb >> 1;
   bb |= bb >> 2;
   bb |= bb >> 4;
   bb |= bb >> 8;
   bb |= bb >> 16;
   bb |= bb >> 32;

   return bitscanIndex64[(bb * DEBRUIJN64) >> 58];
}

//Board Transformations 

// All the following are one step transformations 
u_int64 Board::tNorth(u_int64 bb){
	return bb << N;
}

u_int64 Board::tSouth(u_int64 bb){
	return bb >> abs(abs(S));
}

u_int64 Board::tEast(u_int64 bb){
	return (bb << E) & NOT_A_FILE; 
}

u_int64 Board::tWest(u_int64 bb){
	return (bb >> abs(W)) & NOT_H_FILE;
}

u_int64 Board::tNorthEast(u_int64 bb){
	return (bb << NE) & NOT_A_FILE;
}

u_int64 Board::tNorthWest(u_int64 bb){
	return (bb << NW) & NOT_H_FILE;
}

u_int64 Board::tSouthEast(u_int64 bb){
	return (bb >> abs(SE)) & NOT_A_FILE;
}

u_int64 Board::tSouthWest(u_int64 bb){
	return (bb >> abs(SW)) & NOT_H_FILE;
}

// For knight movement generation
u_int64 Board::tNNE(u_int64 bb){
	return (bb << NNE) & NOT_A_FILE;
}
u_int64 Board::tNEE(u_int64 bb){
	return (bb << NEE) & NOT_AB_FILE;
}
u_int64 Board::tSEE(u_int64 bb){
	return (bb >> abs(SEE)) & NOT_AB_FILE;
}
u_int64 Board::tSSE(u_int64 bb){
	return (bb >> abs(SSE)) & NOT_A_FILE;
}
u_int64 Board::tNNW(u_int64 bb){
	return (bb << NNW) & NOT_H_FILE;
}
u_int64 Board::tNWW(u_int64 bb){
	return (bb << NWW) & NOT_GH_FILE;
}
u_int64 Board::tSWW(u_int64 bb){
	return (bb >> abs(SWW)) & NOT_GH_FILE;
}
u_int64 Board::tSSW(u_int64 bb){
	return (bb >> abs(SSW)) & NOT_H_FILE;
}

// Pawn movements 

u_int64 Board::wSinglePawnPushDest(){
	// shift the board up by one rank, and with the empty squares 
	// to get the destinations of pawns if they move one square 
	return tNorth(getPieceSet(P_PAWN,P_WHITE)) & getEmptySquares();
}

u_int64 Board::bSinglePawnPushDest(){
	// shift the board down by one rank
	return tSouth(getPieceSet(P_PAWN,P_BLACK)) & getEmptySquares();
}

u_int64 Board::wDoublePawnPushDest(){
	u_int64 RANK4 = RANK1 << 8*3;
	u_int64 singlePushes = wSinglePawnPushDest();

	// find the single push of that colour 
	// shift it up one rank, and it with rank 4 to get 
	// the possible positions in that rank 
	return tNorth(singlePushes) & RANK4 & getEmptySquares();
}

u_int64 Board::bDoublePawnPushDest(){
	u_int64 RANK5 = RANK1 << 8*4;
	u_int64 singlePushes = bSinglePawnPushDest();
	return tSouth(singlePushes) & RANK5 & getEmptySquares();
}

u_int64 Board::wSinglePawnPushSrc(){
	return tSouth(getEmptySquares()) & getPieceSet(P_PAWN,P_WHITE) & pieceBB[P_PAWN];
}

u_int64 Board::bSinglePawnPushSrc(){
	return tNorth(getEmptySquares()) & getPieceSet(P_PAWN,P_BLACK);
}

u_int64 Board::wDoublePawnPushSrc(){
	u_int64 RANK4 = RANK1 << 8*3;
	u_int64 emptyRANK3 =  tSouth(getEmptySquares() & RANK4) & getEmptySquares();
	// find the single push destinations for the white pawns and and it with 
	return getPieceSet(P_PAWN,P_WHITE) & tSouth(emptyRANK3);
}

u_int64 Board::bDoublePawnPushSrc(){
	u_int64 RANK5 = RANK1 << 8*4;
	u_int64 emptyRANK6 = tNorth(getEmptySquares() & RANK5) & getEmptySquares();
	// find the single push destinations for the white pawns and and it with 
	return getPieceSet(P_PAWN,P_BLACK) & tNorth(emptyRANK6);

}

// Pawn Attacks 

u_int64 Board::wPawnEastAttacks(){
	return tNorthEast(getPieceSet(P_PAWN,P_WHITE));
}

u_int64 Board::wPawnWestAttacks(){
	return tNorthWest(getPieceSet(P_PAWN,P_WHITE));
}

u_int64 Board::bPawnEastAttacks(){
	return tSouthEast(getPieceSet(P_PAWN,P_BLACK));
}

u_int64 Board::bPawnWestAttacks(){
	return tSouthWest(getPieceSet(P_PAWN,P_BLACK));
}

u_int64 Board::wPawnAllAttacks(){
	return wPawnEastAttacks() | wPawnWestAttacks();
}

u_int64 Board::wPawnDoubleAttacks(){
	return wPawnEastAttacks() & wPawnWestAttacks();
}

u_int64 Board::wPawnSingleAttacks(){
	return wPawnEastAttacks() ^ wPawnWestAttacks();
}

u_int64 Board::bPawnAllAttacks(){
	return bPawnEastAttacks() | bPawnWestAttacks();
}

u_int64 Board::bPawnDoubleAttacks(){
	return bPawnEastAttacks() & bPawnWestAttacks();
}

u_int64 Board::bPawnSingleAttacks(){
	return bPawnEastAttacks() ^ bPawnWestAttacks();
}

// Pawn Captures 
u_int64 Board::wPawnCaptureEast(){
	return getPieceSet(P_PAWN,P_WHITE) & bPawnWestAttacks();
}

u_int64 Board::wPawnCaptureWest(){
	return getPieceSet(P_PAWN,P_WHITE) & bPawnEastAttacks();
}

u_int64 Board::wPawnCaptureAll(){
	return getPieceSet(P_PAWN,P_WHITE) & bPawnAllAttacks();
}

u_int64 Board::bPawnCaptureEast(){
	return getPieceSet(P_PAWN,P_BLACK) & wPawnWestAttacks();
}

u_int64 Board::bPawnCaptureWest(){
	return getPieceSet(P_PAWN,P_BLACK) & wPawnEastAttacks();
}

u_int64 Board::bPawnCaptureAll(){
	return getPieceSet(P_PAWN,P_BLACK) & wPawnAllAttacks();
}

/*
 * KNIGHT AND KING MOVE GENERATION
 */

// Knight Dict Generation
void Board::knightAttackSetGeneration(){

	knightAttackSet = new u_int64[64];

	u_int64 start = 1L;
	u_int64 currKnight;
	for(int i = A1 ; i <= H8 ; i++){
		currKnight = start << i;

		knightAttackSet[i] = tNNW(currKnight) |	
							 tNNE(currKnight) | 
							 tNEE(currKnight) |	
							 tSEE(currKnight) | 
							 tSSE(currKnight) |
							 tSSW(currKnight) |
							 tSWW(currKnight) |
							 tNWW(currKnight); 
	}
}

// King Attack Set Generation
void Board::kingAttackSetGeneration(){
	kingAttackSet = new u_int64[64];
	
	u_int64 start = 1L;
	u_int64 currKing;

	for(int i = A1 ; i <= H8 ; i++){
		currKing = start << i;
		kingAttackSet[i] = 	tNorth(currKing)|
							tSouth(currKing)|
							tWest(currKing) |
							tEast(currKing) |
						tNorthWest(currKing)|
						tNorthEast(currKing)|
						tSouthWest(currKing)|
						tSouthEast(currKing);	

	}		
}
// populate the line attacks 
void Board::lineAttackGeneration(){
	fileAttackSet = new u_int64[64];
	rankAttackSet = new u_int64[64];
	diagonalAttackSet = new u_int64[64];
	antiDiagonalAttackSet = new u_int64[64];

	for(int pos = A1; pos <= H8 ; pos++){
		fileAttackSet[pos] = fileAttackMask(pos);
		rankAttackSet[pos] = rankAttackMask(pos);
		diagonalAttackSet[pos] = diagonalAttackMask(pos);
		antiDiagonalAttackSet[pos] = antiDiagonalAttackMask(pos);

	}
}
// King and Knight Captures
u_int64 Board::wKingCaptureMap(){
	u_int64 king = pieceBB[P_KING] & pieceBB[P_WHITE];
	if(king == 0L)
			return king;
	return kingAttackSet[bitScanForward(king)] & pieceBB[P_BLACK];
}
// King and Knight Captures
u_int64 Board::bKingCaptureMap(){
	u_int64 king = pieceBB[P_KING] & pieceBB[P_BLACK];
	if(king == 0L)
			return king;
	return kingAttackSet[bitScanForward(king)] & pieceBB[P_WHITE];
}

u_int64 Board::wKnightCaptureMap(){
	u_int64 king = pieceBB[P_KNIGHT] & pieceBB[P_WHITE];
	if(king == 0L)
			return king;
	return kingAttackSet[bitScanForward(king)] & pieceBB[P_BLACK];
}
// King and Knight Captures
u_int64 Board::bKnightCaptureMap(){
	u_int64 king = pieceBB[P_KNIGHT] & pieceBB[P_BLACK];
	if(king == 0L)
			return king;
	return kingAttackSet[bitScanForward(king)] & pieceBB[P_WHITE];
}

// Sliding piece generation 

// Code adapted from Chess Programming.org
u_int64 Board::rankAttackMask(int pos){
	return RANK1 << (pos & 56);
}

u_int64 Board::fileAttackMask(int pos){
	return AFILE << (pos & 7);
}

u_int64 Board::diagonalAttackMask(int pos){
	// code from chessprogramming.net 
	
	int diag = 8*(pos & 7) - (pos & 56);
	int north = -diag & (diag >> 31);
	int south = diag & (-diag >> 31);

	return (DIAGONAL >> south) << north;
}

u_int64 Board::antiDiagonalAttackMask(int pos){ 
		
	// code from chessprogramming.net 
	
	int diag = 56 - 8*(pos & 7) - (pos & 56);
	int north = -diag & (diag >> 31);
	int south = diag & (-diag >> 31);

	return (ANTIDIAGONAL >> south) << north;
}

u_int64 Board::getLineAttack(Direction dir, SquarePos pos){
	u_int64 lineAttack;
	switch(dir){
		case N:
		case S:
			// this also counts as S 
			lineAttack = fileAttackSet[pos];			
			break;
		case E:
		case W:
			// also W
			lineAttack = rankAttackSet[pos];
			break;
		case NE:
		case SW:
			// also SW
			lineAttack = diagonalAttackSet[pos];
			break;
		case SE:
		case NW:
			// also NW
			lineAttack = antiDiagonalAttackSet[pos];
			break;
		default:
			return -1;
	}
	return lineAttack;
}

u_int64 Board::getPositiveRay(u_int64 lineAttack, SquarePos pos){
	return lineAttack & ((u_int64)-2 << pos);	
}

u_int64 Board::getNegativeRay(u_int64 lineAttack, SquarePos pos){
	return lineAttack & (((u_int64)1 << pos) - 1);	
}

// Get a particular ray give a position and a direction -- this is 
// used to find the sliding attacks of the queen, bishop and rook
u_int64 Board::getRay(Direction dir, SquarePos pos){
	// switch on direction 
	u_int64 lineAttack = getLineAttack(dir, pos);
	// switch on the direction --> return positive ray or negative 
	// ray depending on the direction --> this is indicated by the 
	// Direction ENUM
	switch(dir){
		case N:
		case E:
		case NW:
		case NE:
			return getPositiveRay(lineAttack, pos);
		case S:
		case W:
		case SE:
		case SW:
			return getNegativeRay(lineAttack, pos);
		default:
			return -1;
	}
}

// RAY ATTACK GENERATION 
u_int64 Board::getPositiveRayAttacks(Direction dir, SquarePos pos){
	u_int64 ray;
	u_int64 occupied = pieceBB[P_WHITE] | pieceBB[P_BLACK];

	switch(dir){
		case N:
		case E:
		case NE:
		case NW:
			ray = getRay(dir,pos);
			break;
			// TODO - ERROR HANDLING HERE 
		default:
			return -1;
	}
	int blockerPos;	

	u_int64 attacks = ray;
	u_int64 blocker = ray & occupied; 

	if(blocker){
		blockerPos = bitScanForward(blocker);
		attacks = attacks ^ getRay(dir,static_cast<SquarePos>(blockerPos));	
	}
	return attacks;
}

u_int64 Board::getNegativeRayAttacks(Direction dir, SquarePos pos){
	u_int64 ray;
	u_int64 occupied = pieceBB[P_WHITE] | pieceBB[P_BLACK];
	// this only will work with the following directions
	// I need a way to handle errors --> need to work this out in c++
	switch(dir){
		case S:
		case SE:
		case SW:
		case W:
			ray = getRay(dir,pos);
			break;
		default:
			// TODO - ERROR HANDLING HERE 
			return -1;
	}
	int blockerPos;	

	u_int64 attacks = ray;
	u_int64 blocker = ray & occupied; 

	if(blocker){
		blockerPos = bitScanReverse(blocker);
		attacks = attacks ^ getRay(dir,static_cast<SquarePos>(blockerPos));	
	}
	return attacks;
}

// wrapper function for getPos and getNeg rau attacks -- this allows us to just 
// get the ray attacks without having to deal with the positive and negative
u_int64 Board::getRayAttacks(Direction dir, SquarePos pos){
	switch(dir){
		case N:
		case E:
		case NW:
		case NE:
			return getPositiveRayAttacks(dir, pos);
		case S:
		case W:
		case SE:
		case SW:
			return getNegativeRayAttacks(dir, pos);
		default:
			cout << "WHYYYYYYYY\n";
			return -1;
	}
	
}

// FINALLY the AttackSets of the Rook, Bishop and the Queen 
// Unlike the other pieces, this requires the specific position
// of the pawn that we are wanting to evaluate / move 
// HERE we just concatenate the specific ray direction attacks 
u_int64 Board::getRookAttacks(SquarePos pos){
	return getRayAttacks(E,pos) | 
		   getRayAttacks(W,pos) | 
		   getRayAttacks(N,pos) | 
		   getRayAttacks(S,pos);
}

u_int64 Board::getBishopAttacks(SquarePos pos){
	return getRayAttacks(NE,pos) | 
		   getRayAttacks(SE,pos) | 
		   getRayAttacks(SW,pos) | 
		   getRayAttacks(NW,pos);
}

u_int64 Board::getQueenAttacks(SquarePos pos){
	return getRayAttacks(E,pos)  | 
		   getRayAttacks(W,pos)  | 
		   getRayAttacks(N,pos)  | 
		   getRayAttacks(S,pos)  |
		   getRayAttacks(NE,pos) | 
		   getRayAttacks(SE,pos) | 
		   getRayAttacks(SW,pos) | 
		   getRayAttacks(NW,pos);
}

// OTHER HELPTER METHODS 

