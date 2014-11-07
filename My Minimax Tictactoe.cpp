// My Minimax Tictactoe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>  

//************* Denbo
const char empty = ' ';
const char human = 'X';
const char comp = 'O';
const int NumberofTiles = 9;
#define togglePlayer(p) ((p == human)?comp:human)

char theBoard[NumberofTiles] = {empty,empty,empty,empty,empty,empty,empty,empty,empty};

/*char theBoard[NumberofTiles] = {
			human,empty,empty,
			empty,human,empty,
			comp,empty,comp};

*/

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

void drawboard(char *b){
	for(int i = 0;i<3;i++){
		for(int j = 0;j< 3;j++)
		{
			printf(" %c %c",b[(i*3)+j], ((j<2)? '|':empty) );
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
			return (theBoard[winningcombo[i][0]] == comp ? 1 : -1);
		}
	}
	return 0;
}

void humanMove(){
	int move=0;
	do{
		printf("Enter Move: ");
		scanf_s("%d",&move);
	}while(!movetile(move,human));

}

int minimax(char player){
	int myscores[NumberofTiles];
	int tempscore = (player == comp) ? -1000 : 1000;
	int bestmove  =-1; 

	if (player == human) tempscore = 1000;
	
	int score = win();
	if (score !=0) return score;
	char otherplayer = togglePlayer(player);
	for (int i=0; i < NumberofTiles;i++){
		myscores[i] = -42;
		if (movetile(i,player)){
			score = minimax(otherplayer);
//			drawboard(theBoard);
//			printf("%d\n",score);
			myscores[i]=score;
			if (player == human){  // we want minimum score
				if (score < tempscore){
					tempscore = score;
					bestmove = i;
				}
			}
			else{  // we want max score
				if (score > tempscore){
					tempscore = score;
					bestmove = i;
				}
			}
			unmovetile(i);
		}
	}
	return score;
}
void computerMove(){
	int myscores[NumberofTiles];
	int maxscore = -1000;
	int bestmove=-1000;
	for (int i = 0;i< NumberofTiles;i++){
		myscores[i] = -42;
		if (movetile(i,comp)){
			//drawboard(theBoard);
			int score = minimax(human);
			//printf("%d\n",score);
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

int _tmain(int argc, _TCHAR* argv[])
{
	while (!win()){
		drawboard(theBoard);
		humanMove();
		drawboard(theBoard);
		computerMove();
	}

	drawboard(theBoard);
	printf ("Win %d\n",win());
	char s[20];
	
	scanf_s("%s",&s);
	return 0;
}

