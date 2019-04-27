# JustinChess 

A basic chess engine written in C++

## Completed:
* Bitboard representation of the chess board, including representations for each pieces 
* Added methods to move pieces, set the board and print the board 
* Pawn Attack sets
* Knight attack sets 
* King attack sets 
## TODO: 
* move Generation
* Chess rule implementation 
* One the chess rules have been implemented correctly, we can then work on the AI component 
* Knight and King captures 
* Also knight and king attack sets based on moves --> we need to and it with the opponent squares to find pieces that we can capture 
* NEED to implement capturing pieces 
* Need to implement enpassant and castling, fifty rule 
* Need to figure out how to properly copy the board --> maybe refactor the code such that the class only takes care of the essential things --> could move constants and attacksets into another class specifically designed to just hold that information so that the board is only holding minimal information. Need to ensure that the copy of the board is able to easily and quickly reconstruct piece lists and game states.
