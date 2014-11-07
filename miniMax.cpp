// My Minimax Tictactoe.cpp : Defines the entry point for the console application.
//  comment 1

#include "stdafx.h"
#include <Windows.h>
#include <stdlib.h>  
#include <time.h>

#define _RandomON_

const char empty = ' ';
const char human = 'X';
const char comp = 'O';
const int NumberofTiles = 9;
#define togglePlayer(p) ((p == human)?comp:human)
int minimax(char player);

char theBoard[NumberofTiles] = {empty,empty,empty,empty,empty,empty,empty,empty,empty};

int winningcombo[8][3] = {	
	{0,1,2},  //across
	{3,4,5},
	{6,7,8},
	{0,3,6},  //down
	{1,4,7},
	{2,5,8},
	{0,4,8},  //diags
	{2,4,6}
		};


void emptyboard(){
	for (int i = 0;i< NumberofTiles;i++){
		theBoard[i] = empty;
	}
}

void drawboard(char *b){
	for(int i = 0;i<3;i++){
		for(int j = 0;j< 3;j++)
		{
			//printf(" %c %c",(b[(i*3)+j] != empty) ? b[(i*3)+j] : (j+(i*3))+'0', ((j<2)? '|':empty) );
			printf(" %c %c", b[(i*3)+j], ((j<2)? '|':empty) );
		}
		((i<2)? printf("\n-----------\n") : printf("\n"));
		
	}
	printf("\n");
}

bool movetile(int x, char player){
	if (theBoard[x]==empty){
		theBoard[x] = player;
		return true;
	}else{
		return false;
	}
}

void unmovetile(int x){	
	theBoard[x] = empty;
}


int win(){
	const int numberofwinningcombos = (sizeof(winningcombo)/sizeof(int))/3;
	for (int i = 0;i< numberofwinningcombos;i++){
		if (theBoard[winningcombo[i][0]] != empty && 
			theBoard[winningcombo[i][0]] == theBoard[winningcombo[i][1]] && 
			theBoard[winningcombo[i][1]] == theBoard[winningcombo[i][2]] ){
			return (theBoard[winningcombo[i][0]] == comp ? 10 : -10);
		}
	}

	bool tie = true;

	for (int i = 0;i< NumberofTiles;i++){
		if (theBoard[i] == empty) tie = false;
	}
	if (tie) return 5;
	return 0;
}

#ifdef _RandomON_
void humanMove()
{
	int move=0;
	printf("Hit Any key to generate random #..."); getchar();
	do{
		while (move<=0)
		{	srand(clock());
			move = rand()%10;
		}
		move -=1;
	}while(!movetile(move,human));
}

#else

void humanMove(){
	int move=0;
	do{
		printf("Enter Move: ");
		scanf_s("%d",&move);
		move -=1;
	}while(!movetile(move,human));

}
#endif

int minimax(char player){
	int maxscore=-1000, maxmove=0, minscore=1000, minmove=0;
	
	int score = win();
	if (score !=0) return score;
	char otherplayer = togglePlayer(player);

	for (int i=0; i<NumberofTiles;i++){
		if (movetile(i,player)){
			score = minimax(otherplayer);
			if (score > maxscore){
				maxscore = score;
				maxmove = i;
			}
			if (score < minscore){
				minscore = score;
				minmove = i;
			}
			unmovetile(i);
		}
	}
	if (player == human) return minscore;
	else return maxscore;
}

void computerMove(){
	int myscores[NumberofTiles];
	int maxscore = -1000;
	int bestmove=-1000;
	for (int i = 0;i< NumberofTiles;i++){
		myscores[i] = -42;
		if (movetile(i,comp)){
			if (win() == 10) break;   // optimized: next move is a sure win. comment this out and move to 1,9,3 and see how
									  // computer doesn't take winning move (though it will still win)
			int score = minimax(human);
			myscores[i] = score;
			if (score > maxscore){
				maxscore = score;
				bestmove = i;
			}
			unmovetile(i);
		}
	}
	movetile(bestmove,comp);
}

int main(int argc, char* argv[]){
	int ties=0, wins=0,loss = 0;
		while(1)
		{
		while (!win()){
			drawboard(theBoard);
			humanMove();
			printf("Human Moved\n");
			drawboard(theBoard);
			computerMove();
			printf("Computer Moved\n");
		}

		drawboard(theBoard);
		int score = win();
		if (score == 5) {
			printf("Tie!!\n");
			ties++;
		}
		else if (score == 10) {
			printf("Comp wins!!\n");
			loss++; //human loses
		}
		else if (score == -10){
			printf("Comp wins!!\n");
			wins++;
		}

	//	printf ("Win %d\n",win());
		printf("      Win Loss Ties\n");
		printf("Comp  %3d %3d %3d\n",loss, wins, ties);
		printf("Human %3d %3d %3d\n",wins, loss, ties);
		printf("_________________\n\n");
		emptyboard();
	}
	return 0;
}

