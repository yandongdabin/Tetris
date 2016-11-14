#pragma once


enum STATE{
	START, //game is going
	OVER, //game is over
	STOP // game is stopped
};
struct Position{
	int x;
	int y;
	int shape;
	int color;
};
class Game
{
public:
	Game(void);
	~Game(void);
public:
	static const int ROW = 21;
	static const int COL = 15;
	static const int colorNumber = 13;
	static const COLORREF colors[colorNumber];
	static const int chessNumber= 7;
	static const COLORREF backGroundLineColor;
	

private:
	
	bool board[ROW][COL];
	int boardcolor[ROW][COL];
	static const int chess[chessNumber][4];
	
	int currentChess;
	int nextChess;
	int nextChessShape;
	Position curpos;
	long score;

	STATE state;

	bool chessReady;

private:
	void resetGame();
	
	
	void clearLines();
	
	bool getRealShape(int i,int j,int shape);

public:
	void moveLeft();
	void moveRight();
	bool moveDown();
	void rotate(); //rotate the current shape
	void move(UINT nChar);

//interface provided for Dialog
public:
	void startGame();
	void stopGame();
	int hasChess(int i,int j);
	int getChessPos(int i,int j);
	void getNextChess();
	bool lose();
	long getScore(){return score;}
	bool hasChessInHere(int i,int j);
	int getCurrentColor(){return curpos.color;}
	


};
