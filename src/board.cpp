#include "headers/board.h"

using namespace std;

		// PieceType enum -- for indexing on the pieceBB

/*
 * METHODS ON THE BOARD 
 */

// Constructor 
Board::Board(){
	// set the initial positions of each piece 
	pieceBB[P_WHITE] = Board::RANK1 | (Board::RANK1 << 8) ;
	pieceBB[P_BLACK] = (Board::RANK8 >> 8) | Board::RANK8;	

	// now for each of the pieces -- here we are bitshifting by the actual square position 
	// on the board 
	
	//P_PAWN
	pieceBB[P_PAWN] = (Board::RANK1 << 8) | (Board::RANK8 >> 8);	
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
	cout << "BOARD: [" << pt_name(pt) << "]\n";

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

// helper function for the movePiece method -> moving a piece from one location to another 
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
string Board::pt_name(PieceType pt){
	return pt_names[pt];
}

u_int64 Board::getEmptySquares(){
	return ~(pieceBB[P_WHITE] | pieceBB[P_BLACK]);
}

// Clean up

void Board::deleteStructures(){
	delete(kingAttackSet);
	delete(knightAttackSet);
}
//Board Transformations 

u_int64 Board::tNorth(u_int64 bb){
	return bb << Board::N;
}

u_int64 Board::tSouth(u_int64 bb){
	return bb >> Board::S;
}

u_int64 Board::tEast(u_int64 bb){
	return (bb << Board::E) & Board::NOT_A_FILE; 
}

u_int64 Board::tWest(u_int64 bb){
	return (bb >> Board::W) & Board::NOT_H_FILE;
}

u_int64 Board::tNorthEast(u_int64 bb){
	return (bb << Board::NE) & Board::NOT_A_FILE;
}

u_int64 Board::tNorthWest(u_int64 bb){
	return (bb << Board::NW) & Board::NOT_H_FILE;
}

u_int64 Board::tSouthEast(u_int64 bb){
	return (bb >> Board::SE) & Board::NOT_A_FILE;
}

u_int64 Board::tSouthWest(u_int64 bb){
	return (bb >> Board::SW) & Board::NOT_H_FILE;
}

u_int64 Board::tNNE(u_int64 bb){
	return (bb << Board::NNE) & Board::NOT_A_FILE;
}
u_int64 Board::tNEE(u_int64 bb){
	return (bb << Board::NEE) & Board::NOT_AB_FILE;
}
u_int64 Board::tSEE(u_int64 bb){
	return (bb >> Board::SEE) & Board::NOT_AB_FILE;
}
u_int64 Board::tSSE(u_int64 bb){
	return (bb >> Board::SSE) & Board::NOT_A_FILE;
}
u_int64 Board::tNNW(u_int64 bb){
	return (bb << Board::NNW) & Board::NOT_H_FILE;
}
u_int64 Board::tNWW(u_int64 bb){
	return (bb << Board::NWW) & Board::NOT_GH_FILE;
}
u_int64 Board::tSWW(u_int64 bb){
	return (bb >> Board::SWW) & Board::NOT_GH_FILE;
}
u_int64 Board::tSSW(u_int64 bb){
	return (bb >> Board::SSW) & Board::NOT_H_FILE;
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

// Knight Dict Generation
void Board::knightAttackSetGeneration(){

	knightAttackSet = new u_int64[64];

	u_int64 start = 1L;
	u_int64 currKnight;
	for(int i = Board::A1 ; i <= Board::H8 ; i++){
		currKnight = start << i;

		Board::knightAttackSet[i] = tNNW(currKnight) | 
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

	for(int i = Board::A1 ; i <= Board::H8 ; i++){
		currKing = start << i;
		kingAttackSet[i] = 	tNorth(currKing) |
							tSouth(currKing) |
							tWest(currKing)  |
							tEast(currKing);

	}		
}

// King and Knight Captures
