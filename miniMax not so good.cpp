// miniMax.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"


//#define togglePlayer(p) ((p == human)?comp:human)

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
/* ==============  future mod to expand to N dimension


#define togglePlayer(p) (p^1)  // 0 or 1
typedef enum Play  { eHuman, eComp, eEmpty };
*/
const char *Players = "XO "; // human computer empty 
const int Dim = 3; // default : 3x3 
const int nTiles = Dim*Dim;

//typedef enum WinStates  { row1Full, row2Full, row3Full, 
//	               col1Full, col2Full, col3Full,
//				   leftDiag, rightDiag, nil };

const short nWinStates = Dim*2 + 2; 
short Wins[nWinStates];

//====================================================
// show binary placement for a short
void showbin(short n)
{
	short i, max;
	const short siz = sizeof(n)*8+1;
	char arr[siz];
	max =  sizeof(n)*8;
  memset(arr, 0, sizeof(arr));

  for(i=max-1; i>=0; --i)
     	arr[max-1-i] = '0' + ((n >> i) & 1);
	
  i=0;
  char *p = strchr(arr, '1');
  memmove(arr, p, sizeof(arr) - (p-arr));
  printf("%s\n", arr);
}

//====================================================
// calculate the Winning binary placement
void createWins(short dim)
{ // if dim == 3 .. there are 2^3 winning states
	int val=1, ct=0, r=0, c=0, i=0;
	
	/*	
	{0,1,2},  111 000 000
	{3,4,5},  000 111 000
	{6,7,8},  000 000 111
	//all rows
	*/
	for (r=dim-1; r>=0; r--, ++ct)
	{	
		for (val=0,c=0; c<dim; c++)
			val = val<<1 | 1;
	    val = val << r*dim;
		Wins[ct] = val;
	}  
	
	/*	
	{0,3,6},    100 100 100  
	{1,4,7},    010 010 010
	{2,5,8}     001 001 001
	 all columns
	*/
	for (c=dim; c>0; c--, ++ct)
	{	// 2... 0
		for (r=0,val=0; r<dim; r++)
		{	
			i = 1<<(c-1);  
			val = (val<<dim) | i;   
		}
		Wins[ct] = val;
	}
    /*	
	{0,4,8},  // 100 010 001
	{2,4,6    // 001 010 100
	 all diag 
	*/
	// right diagonal 
	for (val=1, c=0;c<dim-1;c++)
	{	  
		val = (val<<(dim+1)) | 1;  
	}
	Wins[ct++] = val;
	
	// left diagonal   001 010 100  0001 0010  0100   1000
	for (val=0,c=0; c<dim; c++)
	{	
		val = (val| 1)<<(dim-1) ;        
	}
	Wins[ct++] = val;
	
	for (i=0; i<ct; i++)
		showbin(Wins[i]);

}

//=====================================================
void displayBrd( char b[Dim][Dim])
{  char i, j;
	printf("________\n");
	for (j=0;j<3;j+=3)
	{	for (i=0;i<3;i+=3)
			printf("|%c|%c|%c|\n", b[j][i], b[j][i+1], b[j][i+2]);
		printf("- - - - -\n");
	}
}

//====================================================
// Display all branches under one node with "level"-depth
// - future implemntation : put this in a text file
void display(char level)
{   char i, j;
    int nodes;
	if (level==-1)
		return;
	nodes = 2<<(level-1);
	printf("\nLevel %d\n", Dim-level);
	for (i=1; i<=nodes;i++, printf("\n"))
		for (j=i+1; j<=9; j++)
			printf("%*c%d",nodes, ' ', j);
	
	display(level-1);
}


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
	int myscores[nTiles];
	int tempscore = (player == comp) ? -1000 : 1000;
	int bestmove  =-1; 

	if (player == human) tempscore = 1000;
	
	int score = win();
	if (score !=0) return score;
	char otherplayer = togglePlayer(player);
	for (int i=0; i < nTiles;i++){
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
	int myscores[nTiles];
	int maxscore = -1000;
	int bestmove=-1000;
	for (int i = 0;i< nTiles;i++){
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


int main(int argc, _TCHAR* argv[])
{
#ifdef _DebugON_
	FILE *fp;   // a text file to store debug data
	errno_t err;
	char *fname = "./board.txt";
	if ((err  = fopen_s( &fp, fname, "w+"))!=0)
	{
		printf("Fail to open %s for Writing!\n", fname);
		getchar();
		return 0;
	}
#endif
	createWins(Dim);

	while (!win())
	{
		drawboard(theBoard);
		humanMove();
		drawboard(theBoard);
		computerMove();
	}

	drawboard(theBoard);
	printf ("Win %d\n",win());
	return 0;
}
