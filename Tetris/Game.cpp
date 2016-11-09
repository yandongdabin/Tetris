#include "StdAfx.h"
#include "Game.h"
#include <ctime>
#include "conio.h"



const int Game::chess[Game::chessNumber][4] = {
	0x0660,0x0660,0x0660,0x0660, //[]
	0x0f00,0x2222,0x00f0,0x4444, // ----
	0x0c44,0x2E00,0x2230,0x0074, // q
	0x0c60,0x2640,0x0630,0x0264, // z
	0x0644,0x0E20,0x2260,0x0470  // p
};
const COLORREF Game::colors[4] ={
	RGB(204,51,51),
	RGB(255,131,250),
	RGB(153,255,102),
	RGB(0,51,153)
};
Game::Game(void){
	state = OVER;
	srand((unsigned int)time(NULL));
	resetGame();
	//startGame();
}
void Game::startGame(){
	if(state == OVER){
		state = START;
		resetGame();
	}else if(state == STOP){
		state = START;
	}
}
void Game::stopGame(){
	state = STOP;
}
void Game::resetGame(){
	//clear the board
	for(int i = 0;i < ROW; ++i){
		for(int j = 0;j<COL;++j){
			board[i][j] = false;
			boardcolor[i][j] = 0;
		}
	}
	currentChess = nextChess = -1;
	score = 0;
	chessReady = false;
	getNextChess();
	getNextChess();
}
void Game::rotate(){
	int next = chess[currentChess][(curpos.shape+1)%4];
	int x = curpos.x;
	int y = curpos.y;
	bool can = true;
	for(int i = 0;i<4;++i){
		for(int j = 0;j<4;++j){
			if(getRealShape(i,j,next)){
				if(x + i >= ROW || y + j >= COL || x + i < 0 || y + j < 0){can = false;break;}
				if(board[x + i][y + j]){can = false;break;} //already has chess
			}
		}
		if(!can) break;
	}
	//AllocConsole();
	//cprintf("%d %d\n",currentChess,curpos.shape);
	//FreeConsole();
	if(can){curpos.shape = (curpos.shape+1)%4;}
}
bool Game::moveDown(){
	int x = curpos.x;
	int y = curpos.y;
	

	
	int shape = chess[currentChess][curpos.shape];

	bool can = true;
	if(x + 1 >= ROW) can = false;
	x += 1;
	for(int i = 0;i<4;++i){
		for(int j = 0;j<4;++j){
			if(!can) break;
			if(getRealShape(i,j,shape)){
				if(x + i < 0 || y + j < 0) {continue;}
				if(x + i >= ROW || y + j >= COL || board[x + i][y + j]){can = false;break;} //already has chess
			}
		}
		if(!can) break;
	}
	if(can){
		curpos.x += 1;
	}else{
		chessReady = false;
		for(int i = 0;i<4;++i){
			for(int j = 0;j<4;++j){
				if(getRealShape(i,j,shape)){
					//if(x + i < 0 || y + j < 0 || x + i >= ROW || y + j >= COL) continue;
					board[curpos.x + i][curpos.y + j] = true;
					boardcolor[curpos.x + i][curpos.y + j] = curpos.color;
				}
			}
		}
		clearLines();
	}
	return can;
}
void Game::moveLeft(){
	int x = curpos.x;
	int y = curpos.y;
	int shape = chess[currentChess][curpos.shape];
	
	y -= 1;
	for(int i = 0;i<4;++i){
		for(int j = 0;j<4;++j){
			if(getRealShape(i,j,shape)){
				if(x + i < 0 || y + j < 0) return;
				if(x + i >= ROW || y + j >= COL) return;
				if(board[x + i][y + j]){return;} //already has chess
			}
		}
	}
	curpos.y -= 1;
}
void Game::moveRight(){
	int x = curpos.x;
	int y = curpos.y;
	int shape = chess[currentChess][curpos.shape];
	y += 1;
	for(int i = 0;i<4;++i){
		for(int j = 0;j<4;++j){
			if(getRealShape(i,j,shape)){
				if(x + i < 0 || y + j < 0) return;
				if(x + i >= ROW || y + j >= COL) return;
				if(board[x + i][y + j]){return;} //already has chess
			}
		}
	}
	curpos.y += 1;
}
void Game::getNextChess(){
	currentChess = nextChess;
	curpos.shape = nextChessShape;
	curpos.color = rand() % 4;
	curpos.x = -3;
	curpos.y = 5;
	nextChess = rand() % chessNumber;
	nextChessShape = rand() % 4;
	chessReady = true;
}
bool Game::getRealShape(int i,int j,int shape){
	return shape & (1<<((3-i) * 4 + (3-j)));
}
int Game::hasChess(int i,int j){
	if(!board[i][j]) return -1;
	return boardcolor[i][j];
}
int Game::getChessPos(int i,int j){
	int shape = chess[currentChess][curpos.shape];
	if(getRealShape(i,j,shape)){
		int x = curpos.x + i;
		int y = curpos.y + j;
		return (x << 4) | y;
	}else{
		return -1;
	}
}
void Game::move(UINT nChar){
	if(state != START || !chessReady) return;
	switch(nChar)
	{
		case VK_UP   : rotate();break;	
		case VK_DOWN : moveDown(); break;
		case VK_LEFT : moveLeft();break;
		case VK_RIGHT: moveRight();break;
	}
}
void Game::clearLines(){
	for(int i = 0;i<ROW;++i){
		bool flag = true;
		for(int j = 0;j<COL;++j){
			if(board[i][j] == false){
				flag = false;
				break;
			}
		}
		if(flag){
			score += 100;
			for(int k = i - 1;k>=0;--k){
				for(int p = 0;p<COL;++p){
					board[k+1][p] = board[k][p];
				}
			}
			for(int k = i - 1;k>=0;--k){
				for(int p = 0;p<COL;++p){
					boardcolor[k+1][p] = boardcolor[k][p];
				}
			}
		}
	}
}
bool Game::lose(){
	bool ans = curpos.x < 0;
	if(ans){
		state = OVER;
	}
	return ans;
}
bool Game::hasChessInHere(int i,int j){
	int shape = chess[nextChess][0];
	return getRealShape(i,j,shape);
}
Game::~Game(void){
}
