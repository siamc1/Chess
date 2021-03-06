// Chess.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;
class Game;
class Piece;
class King;
class Board {
public:
	Piece * board[8][8];
	int BKingX;
	int BKingY;
	int WKingX;
	int WKingY;
	string toString();
	Board();
	Board(const Board& b2);
	Board& operator = (const Board& b2);
	~Board();
};


class Piece {
public:
	int x;
	int y;
	int pdir;
	bool isMoved;
	bool isEnpessant;
	Piece(int X, int Y, int Dir) {
		x = X;
		y = Y;
		pdir = Dir;
		isMoved = false;
	}
	Piece() {
		x = 0;
		y = 0;
		pdir = 0;
		isMoved = false;
	}
	virtual bool isMoveLegal(Board& board, int X, int Y) = 0;
	virtual bool move(Board& b, int X, int Y) = 0;
	virtual string getPiece() = 0;

};




class Empty : public Piece {
public:
	Empty(int X, int Y) : Piece(X, Y, 0) {}
	bool isMoveLegal(Board& b, int X, int Y) {
		return false;
	}
	bool move(Board& b, int X, int Y) {
		return false;
	}
	string getPiece() {
		return "   ";
	}
};
class Pawn: public Piece {
public:

	Pawn(int X, int Y, int Dir) : Piece(X, Y, Dir) {
		isEnpessant = false;
		isMoved = false;
	}

	bool isMoveLegal(Board& b, int X, int Y) {
		if (X < 9 && X > 0 && Y < 9 && Y > 0) {
			switch (isMoved) {
			case false:
				if (Y == y + pdir * 2 && X == x) {
					if (b.board[X - 1][Y - 1]->pdir == 0) {
						isEnpessant = true;
						return true;
					}

				}
			case true:
				if (isEnpessant && Y == y + pdir * 1 && (X == x + 1 || X == x - 1)) {
					if (b.board[X - 1][y - 1]->pdir != pdir && b.board[X - 1][y - 1]->pdir != 0) {
						isEnpessant = false;
						delete b.board[X - 1][y - 1];
						b.board[X - 1][y - 1] = new Empty(X, y);
						return true;
					}
				}
				if (Y == y + pdir * 1) {
					if (X == x) {
						if (b.board[X - 1][Y - 1]->pdir != pdir) {
							isEnpessant = false;
							return true;
						}
					}
					else if (X == x + 1 || X == x - 1) {
						if (b.board[X - 1][Y - 1]->pdir != pdir && b.board[X - 1][Y - 1]->pdir != 0) {
							isEnpessant = false;
							return true;
						}
					}
				}
			}
			return false;
		}
		return false;
	}

	bool move(Board& b, int X, int Y) {
		if (isMoveLegal(b, X, Y)) {
			isMoved = true;
			delete b.board[X - 1][Y - 1];
			b.board[X - 1][Y - 1] = b.board[x - 1][y - 1];
			b.board[x - 1][y - 1] = new Empty(x, y);
			x = X;
			y = Y;
			return true;
		}
		else {
			return false;
		}
	}

	string getPiece() {
		if (pdir == -1) {
			return "WPN";
		}
		else {
			return "BPN";
		}
	}
};

class Bishop : public Piece {
public:
	Bishop(int X, int Y, int Dir) : Piece(X, Y, Dir){}

	bool isMoveLegal(Board& b, int X, int Y) {
		if (X < 9 && X > 0 && Y < 9 && Y > 0) {
			if (abs(X - x) == abs(Y - y) && b.board[X - 1][Y - 1]->pdir != pdir) {
				int run = abs(X - x);
				int runDirX = (X - x) / abs(X - x);
				int runDirY = (Y - y) / abs(Y - y);
				for (int i = 1; i < run; i++) {
					if (b.board[x - 1 + i * runDirX][y - 1 + i * runDirY]->getPiece().compare("   ") != 0) {
						return false;
					}
				}
				return true;
			}
		}
		return false;
	}
	bool move(Board& b, int X, int Y) {
		if (isMoveLegal(b, X, Y)) {
			isMoved = true;
			delete b.board[X - 1][Y - 1];
			b.board[X - 1][Y - 1] = b.board[x - 1][y - 1];
			b.board[x - 1][y - 1] = new Empty(x, y);
			x = X;
			y = Y;
			return true;
		}
		else {
			return false;
		}
	}
	string getPiece() {
		if (pdir == -1) {
			return "WBP";
		}
		else {
			return "BBP";
		}
	}
};
class Knight : public Piece {
public:
	Knight(int X, int Y, int Dir) : Piece(X, Y, Dir){}
	bool isMoveLegal(Board& b, int X, int Y) {
		if (X < 9 && X > 0 && Y < 9 && Y > 0) {
			if (b.board[X - 1][Y - 1]->pdir != pdir) {
				if ((abs(X - x) == 2 && abs(Y - y) == 1) || (abs(X - x) == 1 && abs(Y - y) == 2)) {
					return true;
				}
			}
		}
		return false;
	}
	bool move(Board& b, int X, int Y) {
		if (isMoveLegal(b, X, Y)) {
			isMoved = true;
			delete b.board[X - 1][Y - 1];
			b.board[X - 1][Y - 1] = b.board[x - 1][y - 1];
			b.board[x - 1][y - 1] = new Empty(x, y);
			x = X;
			y = Y;
			return true;
		}
		else {
			return false;
		}
	}
	string getPiece() {
		if (pdir == -1) {
			return "WKN";
		}
		else {
			return "BKN";
		}
	}
};
class Rook : public Piece {
public:
	Rook(int X, int Y, int Dir) : Piece(X, Y, Dir) {}

	bool isMoveLegal(Board& b, int X, int Y) {
		if (X < 9 && X > 0 && Y < 9 && Y > 0) {
			if (b.board[X - 1][Y - 1]->pdir != pdir) {
				if ((abs(X - x) > 0 && Y - y == 0) || (abs(Y - y) > 0 && X - x == 0)) {
					int runDirX = abs(X - x);
					int runDirY = abs(Y - y);
					int run;
					if (runDirX == 0) {
						run = abs(Y - y);
						runDirY = (Y - y) / runDirY;
					}
					else {
						run = abs(X - x);
						runDirX = (X - x) / runDirX;
					}
					for (int i = 1; i < run; i++) {
						if (b.board[x + i * runDirX - 1][y + i * runDirY - 1]->pdir != 0) {
							return false;
						}
					}
					return true;
				}
			}
		}
		return false;
	}
	bool move(Board& b, int X, int Y) {
		if (isMoveLegal(b, X, Y)) {
			isMoved = true;
			delete b.board[X - 1][Y - 1];
			b.board[X - 1][Y - 1] = b.board[x - 1][y - 1];
			b.board[x - 1][y - 1] = new Empty(x, y);
			x = X;
			y = Y;
			return true;
		}
		else {
			return false;
		}
	}
	string getPiece() {
		if (pdir == -1) {
			return "WRK";
		}
		else {
			return "BRK";
		}
	}
};

class Queen : public Piece {
public:
	Queen(int X, int Y, int Dir) : Piece(X, Y, Dir) {}
	bool isMoveLegal(Board& b, int X, int Y) {
		if (X < 9 && X > 0 && Y < 9 && Y > 0) {
			if (b.board[X - 1][Y - 1]->pdir != pdir) {
				if (abs(X - x) == abs(Y - y)) {;
					int run = abs(X - x);
					int runDirX = (X - x) / abs(X - x);
					int runDirY = (Y - y) / abs(Y - y);
					for (int i = 1; i < run; i++) {
						if (b.board[x - 1 + i * runDirX][y - 1 + i * runDirY]->getPiece().compare("   ") != 0) {
							return false;
						}
					}
					return true;
				}
				if ((abs(X - x) > 0 && Y - y == 0) || (abs(Y - y) > 0 && X - x == 0)) {
					int runDirX = abs(X - x);
					int runDirY = abs(Y - y);
					int run;
					if (runDirX == 0) {
						run = abs(Y - y);
					}
					else {
						run = abs(X - x);
					}
					for (int i = 1; i < run; i++) {
						if (b.board[x - 1 + i * runDirX][y - 1 + i * runDirY]->getPiece().compare("   ") != 0) {
							return false;
						}
					}
					return true;
				}
			}
		}
		return false;
	}
	bool move(Board& b, int X, int Y) {
		if (isMoveLegal(b, X, Y)) {
			isMoved = true;
			delete b.board[X - 1][Y - 1];
			b.board[X - 1][Y - 1] = b.board[x - 1][y - 1];
			b.board[x - 1][y - 1] = new Empty(x, y);
			x = X;
			y = Y;
			return true;
		}
		else {
			return false;
		}
	}
	string getPiece() {
		if (pdir == -1) {
			return "WQN";
		}
		else {
			return "BQN";
		}
	}
};





class Game {
public:
	Board b;
	bool isGameOn;
	Game() : isGameOn(true) {
		
	}
	bool isCheck(int dir, int X, int Y) {
		if (dir == 1) {
			if (X < 1 || X > 8 || Y < 1 || Y > 8 || (b.board[X - 1][Y - 1]->pdir == 1 && b.board[X - 1][Y - 1]->getPiece().compare("BKG") != 0)) {
				return true;
			}
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					if (b.board[i][j]->pdir == -1) {
						if (b.board[i][j]->isMoveLegal(b, X, Y)) {
							return true;
						}
					}
				}
			}
		}
		else {
			if (X < 1 || X > 8 || Y < 1 || Y > 8 || (b.board[X - 1][Y - 1]->pdir == -1 && b.board[X - 1][Y - 1] ->getPiece().compare("WKG") != 0)) {
				return true;
			}
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					if (b.board[i][j]->pdir == 1) {
						if (b.board[i][j]->isMoveLegal(b, X, Y)) {
							return true;
						}
					}
				}
			}
		}
		return false;
	}
	bool isCheckMate(int dir) {
		int X;
		int Y;
		
		if (dir == 1) {
			if (isCheck(dir, b.BKingX, b.BKingY)) {
				for (int i = -1; i < 2; i++) {
					for (int j = -1; j < 2; j++) {
						X = b.BKingX + i;
						Y = b.BKingY + j;
						if (!isCheck(dir, X, Y)) {
							return false;
						}
					}
				}
			}
			else {
				return false;
			}
		}
		else {
			if (isCheck(dir, b.WKingX, b.WKingY)) {
				for (int i = -1; i < 2; i++) {
					for (int j = -1; j < 2; j++) {
						X = b.WKingX + i;
						Y = b.WKingY + j;
						if (!isCheck(dir, X, Y)) {
							return false;
						}
					}
				}
			}
			else {
				return false;
			}
		}
		isGameOn = false;
		return true;
	}
};
class King : public Piece {
public:
	King(int X, int Y, int Dir) : Piece(X, Y, Dir) {
	}
	bool isMoveLegal(Board& b, int X, int Y) {
		if (!isMoved) {
			if (pdir == 1) {
				if (X == 7 && Y == 8 && b.board[7][7]->isMoveLegal(b, 6, 8) && !b.board[7][7]->isMoved) {
					b.board[7][7]->move(b, 6, 8);
					return true;
				}
				else if (X == 3 && Y == 8 && b.board[0][7]->isMoveLegal(b, 4, 8) && !b.board[0][7]->isMoved) {
					b.board[0][7]->move(b, 4, 8);
					return true;
				}
			}
			else {
				if (X == 7 && Y == 1 && b.board[7][0]->isMoveLegal(b, 6, 8) && !b.board[7][0]->isMoved) {
					b.board[7][0]->move(b, 6, 1);
					return true;
				}
				else if (X == 3 && Y == 1 && b.board[0][0]->isMoveLegal(b, 4, 1) && !b.board[0][0]->isMoved) {
					b.board[0][0]->move(b, 4, 1);
					return true;
				}
			}
		}
		if (b.board[X - 1][Y - 1]->pdir != pdir && X < 9 && X > 0 && Y < 9 && Y > 0 && abs(X - x) < 2 && abs(X - x) >= 0 && abs(Y - y) < 2 && abs(Y - y) >= 0 && sqrt((Y - y)^2 + (X - x)^2) > 0) {
			return true;
		}
		return false;
	}
	bool move(Board& b, int X, int Y) {
		if (isMoveLegal(b, X, Y)) {
			isMoved = true;
			delete b.board[X - 1][Y - 1];
			b.board[X - 1][Y - 1] = b.board[x - 1][y - 1];
			b.board[x - 1][y - 1] = new Empty(x, y);
			x = X;
			y = Y;
			if (pdir == 1) {
				b.BKingX = x;
				b.BKingY = y;
			}
			else {
				b.WKingX = x;
				b.WKingY = y;
			}
			return true;
		}
		else {
			return false;
		}
	}
	string getPiece() {
		if (pdir == -1) {
			return "WKG";
		}
		else {
			return "BKG";
		}
	}
};
Board::Board(const Board& b2) {
	int dir = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			string name = b2.board[j][i]->getPiece();
			if (name.substr(0, 1).compare("W") == 0) {
				dir = -1;
			}
			else if (name.substr(0, 1).compare("B") == 0) {
				dir = 1;
			}
			if (name.substr(1, 2).compare("PN") == 0) {
				board[j][i] = new Pawn(j + 1, i + 1, dir);
			}
			else if (name.substr(1, 2).compare("BP") == 0) {
				board[j][i] = new Bishop(j + 1, i + 1, dir);
			}
			else if (name.substr(1, 2).compare("KN") == 0) {
				board[j][i] = new Knight(j + 1, i + 1, dir);
			}
			else if (name.substr(1, 2).compare("RK") == 0) {
				board[j][i] = new Rook(j + 1, i + 1, dir);
			}
			else if (name.substr(1, 2).compare("QN") == 0) {
				board[j][i] = new Queen(j + 1, i + 1, dir);
			}
			else if (name.substr(1, 2).compare("  ") == 0) {
				board[j][i] = new Empty(j + 1, i + 1);
			}
			else if (name.substr(1, 2).compare("KG") == 0) {
				board[j][i] = new King(j + 1, i + 1, dir);
			}
			else {
				cout << "\n\nUNDEFINED STUFF AT " << j + 1 << ", " << i + 1;
			}
		}
	}
	BKingX = b2.BKingX;
	BKingY = b2.BKingY;
	WKingX = b2.WKingX;
	WKingY = b2.WKingY;
}
Board & Board::operator=(const Board & b2){
	int dir = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			string name = b2.board[j][i]->getPiece();
			if (name.substr(0, 1).compare("W") == 0) {
				dir = -1;
			}
			else if (name.substr(0, 1).compare("B") == 0) {
				dir = 1;
			}
			if (name.substr(1, 2).compare("PN") == 0) {
				board[j][i] = new Pawn(j + 1, i + 1, dir);
			}
			else if (name.substr(1, 2).compare("BP") == 0) {
				board[j][i] = new Bishop(j + 1, i + 1, dir);
			}
			else if (name.substr(1, 2).compare("KN") == 0) {
				board[j][i] = new Knight(j + 1, i + 1, dir);
			}
			else if (name.substr(1, 2).compare("RK") == 0) {
				board[j][i] = new Rook(j + 1, i + 1, dir);
			}
			else if (name.substr(1, 2).compare("QN") == 0) {
				board[j][i] = new Queen(j + 1, i + 1, dir);
			}
			else if (name.substr(1, 2).compare("  ") == 0) {
				board[j][i] = new Empty(j + 1, i + 1);
			}
			else if (name.substr(1, 2).compare("KG") == 0) {
				board[j][i] = new King(j + 1, i + 1, dir);
			}
			else {
				cout << "\n\nUNDEFINED STUFF AT " << j + 1 << ", " << i + 1;
			}
		}
	}
	BKingX = b2.BKingX;
	BKingY = b2.BKingY;
	WKingX = b2.WKingX;
	WKingY = b2.WKingY;
	return *this;
}

Board::Board() {
	board[0][0] = new Rook(1, 1, 1);
	board[7][0] = new Rook(8, 1, 1);
	board[1][0] = new Knight(2, 1, 1);
	board[6][0] = new Knight(7, 1, 1);
	board[2][0] = new Bishop(3, 1, 1);
	board[5][0] = new Bishop(6, 1, 1);
	board[3][0] = new Queen(4, 1, 1);
	board[4][0] = new King(5, 1, 1);
	this->BKingX = 5;
	this->BKingY = 1;

	board[0][7] = new Rook(1, 8, -1);
	board[7][7] = new Rook(8, 8, -1);
	board[1][7] = new Knight(2, 8, -1);
	board[6][7] = new Knight(7, 8, -1);
	board[2][7] = new Bishop(3, 8, -1);
	board[5][7] = new Bishop(6, 8, -1);
	board[3][7] = new Queen(4, 8, -1);
	board[4][7] = new King(5, 8, -1);
	this->WKingX = 5;
	this->WKingY = 8;
	for (int i = 0; i < 8; i++) {
		board[i][1] = new Pawn(i + 1, 2, 1);
		board[i][2] = new Empty(i + 1, 3);
		board[i][3] = new Empty(i + 1, 4);
		board[i][4] = new Empty(i + 1, 5);
		board[i][5] = new Empty(i + 1, 6);
		board[i][6] = new Pawn(i + 1, 7, -1);
	}
}
string Board::toString() {
	stringstream output;
	output.clear();
	output << "\n";
	for (int i = 0; i < 8; i++) {
		if (i % 2 == 0) {
			output << "     " << "     " << "/////" << "     " << "/////" << "     " << "/////" << "     " << "/////" << "\n";
		}
		else {
			output << "     " << "/////" << "     " << "/////" << "     " << "/////" << "     " << "/////" << "     " << "\n";
		}
		output << "  " << i + 1 << "  ";
		for (int j = 0; j < 8; j++) {
			if (i % 2 != j % 2) {
				if (board[j][i]->getPiece().compare("   ") == 0) {
					output << "/////";
				}
				else {
					output << "/" << board[j][i]->getPiece() << "/";
				}
			}
			else {
				if (board[j][i]->getPiece().compare("   ") == 0) {
					output << "     ";
				}
				else {
					output << " " << board[j][i]->getPiece() << " ";
				}
			}
		}
		output << "\n";
		if (i % 2 == 0) {
			output << "     " << "     " << "/////" << "     " << "/////" << "     " << "/////" << "     " << "/////" << "\n";
		}
		else {
			output << "     " << "/////" << "     " << "/////" << "     " << "/////" << "     " << "/////" << "     " << "\n";
		}
	}
	output << "\n       1    2    3    4    5    6    7    8\n\n";
	return output.str();
}
Board::~Board() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			delete board[j][i];
		}
	}
}
void promotionCheck(Game& g, int dir) {
	if (dir == 1) {
		for (int i = 0; i < 8; i++) {
			if (g.b.board[i][0]->getPiece().substr(1, 2).compare("PN") == 0) {
				bool isInputGood = false;
				while (!isInputGood) {
					cout << "\nWhat class would you like the pawn to be promoted to? (for example QN for Queen) ";
					string input;
					cin >> input;
					if (input.compare("BP") == 0) {
						delete g.b.board[i][0];
						g.b.board[i][0] = new Bishop(i + 1, 1, dir);
						isInputGood = true;
					}
					else if (input.compare("KN") == 0) {
						delete g.b.board[i][0];
						g.b.board[i][0] = new Knight(i + 1, 1, dir);
						isInputGood = true;
					}
					else if (input.compare("RK") == 0) {
						delete g.b.board[i][0];
						g.b.board[i][0] = new Rook(i + 1, 1, dir);
						isInputGood = true;
					}
					else if (input.compare("QN") == 0) {
						delete g.b.board[i][0];
						g.b.board[i][0] = new Queen(i + 1, 1, dir);
						isInputGood = true;
					}
					else {
						cout << "Invalid Input Try Again\n";
					}
				}

			}
		}
	}
	else {
		for (int i = 0; i < 8; i++) {
			if (g.b.board[i][7]->getPiece().substr(1, 2).compare("PN") == 0) {
				bool isInputGood = false;
				while (!isInputGood) {
					cout << "\nWhat class would you like the pawn to be promoted to? (for example QN for Queen) ";
					string input;
					cin >> input;
					if (input.compare("BP") == 0) {
						delete g.b.board[i][7];
						g.b.board[i][7] = new Bishop(i + 1, 8, dir);
						isInputGood = true;
					}
					else if (input.compare("KN") == 0) {
						delete g.b.board[i][0];
						g.b.board[i][7] = new Knight(i + 1, 8, dir);
						isInputGood = true;
					}
					else if (input.compare("RK") == 0) {
						delete g.b.board[i][0];
						g.b.board[i][7] = new Rook(i + 1, 8, dir);
						isInputGood = true;
					}
					else if (input.compare("QN") == 0) {
						delete g.b.board[i][0];
						g.b.board[i][7] = new Queen(i + 1, 8, dir);
						isInputGood = true;
					}
					else {
						cout << "Invalid Input Try Again\n";
					}
				}

			}
		}
	}
}
void turn(Game& g, int dir) {
	int move, x, y, X, Y;
	bool isMoveAcceptable = false;
	bool isCheckPassed = false;
	Game GTemp = g;


	if (dir == -1) {
		cout << "Whites turn. please enter the move using 2 coordinates in the format XiYiXfYf: ";
	}
	else {
		cout << "Blacks turn. please enter the move using 2 coordinates in the format XiYiXfYf: ";
	}
	cin >> move;
	x = move / 1000;
	move = move % 1000;
	y = move / 100;
	move = move % 100;
	X = move / 10;
	move = move % 10;
	Y = move;
	if (dir == -1) {
		GTemp.b.board[x - 1][y - 1]->move(GTemp.b, X, Y);
		isCheckPassed = !GTemp.isCheck(dir, GTemp.b.WKingX, GTemp.b.WKingY);
	}
	else if (dir == 1) {
		GTemp.b.board[x - 1][y - 1]->move(GTemp.b, X, Y);
		isCheckPassed = !GTemp.isCheck(dir, GTemp.b.BKingX, GTemp.b.BKingY);
	}
	if (move < 10 && isCheckPassed) {
		if (g.b.board[x - 1][y - 1]->pdir == dir) {
			isMoveAcceptable = g.b.board[x - 1][y - 1]->move(g.b, X, Y);
		}
	}
	GTemp = g;
	while (!isMoveAcceptable) {
		move = 0;
		cout << "Illegal move, try again: ";
		cin >> move;
		x = move / 1000;
		move = move % 1000;
		y = move / 100;
		move = move % 100;
		X = move / 10;
		move = move % 10;
		Y = move;
		if (dir == -1) {
			GTemp.b.board[x - 1][y - 1]->move(GTemp.b, X, Y);
			isCheckPassed = !GTemp.isCheck(dir, GTemp.b.WKingX, GTemp.b.WKingY);
		}
		else if (dir == 1) {
			GTemp.b.board[x - 1][y - 1]->move(GTemp.b, X, Y);
			isCheckPassed = !GTemp.isCheck(dir, GTemp.b.BKingX, GTemp.b.BKingY);
		}

		if (move < 10 && isCheckPassed) {
			if (g.b.board[x - 1][y - 1]->pdir == dir) {
				isMoveAcceptable = g.b.board[x - 1][y - 1]->move(g.b, X, Y);
			}
		}
		GTemp = g;
	}
	promotionCheck(g, dir);
	cout << g.b.toString();
}


int main()
{
	char c;
	Game game;
	cout << game.b.toString();
	while (game.isGameOn) {
		turn(game, -1);
		game.isCheckMate(1);
		if (game.isCheck(1, game.b.BKingX, game.b.BKingY)) {
			cout << "\nBlack in Check!\n";
		}
		if (game.isGameOn) {
			turn(game, 1);
			game.isCheckMate(-1);
			if (!game.isGameOn) {
				cout << "Checkmate! Black Wins!\n";
			}
			else if (game.isCheck(-1, game.b.WKingX, game.b.WKingY)) {
				cout << "\nWhite in Check!\n";
			}
		}
		else {
			cout << "Checkmate! White Wins!\n";
		}
		
	}
	cin;
    return 0;
}

