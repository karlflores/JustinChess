#include "headers/board.h"

using namespace std;

		// PieceType enum -- for indexing on the pieceBB

/*
 * METHODS ON THE BOARD 
 */

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
	return pieceBB[pPawn] & pieceBB[ct];
}

// test functions for setting, getting pieces on the board
void Board::setBoard(PieceType pt, u_int64 board){
	pieceBB[pt] = board;	
}

// print the board in a logical manner 
void Board::printBitBoard(PieceType pt){
	int boardWidth = 8;
	cout << "BOARD: [" << pt << "]\n";

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
	pieceBB[pt] = pieceBB[pt] & !(getMovingMask(oldPos));

	// set the new location to a 1 
	pieceBB[pt] = pieceBB[pt] | getMovingMask(newPos);

	// todo -- make sure that the pWhite and pBlack bitboards are updated 
}

Board Board::copy(){
	// create a new copy of the board, then copy everything inside the array  
	Board newBoard = Board();
	
	for(int i = 0 ; i <=7; i++){
		newBoard.pieceBB[i] = pieceBB[i];
	}

	return newBoard;
}

