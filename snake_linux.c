#include <stdio.h>
#include <stdlib.h> //system() func
#include <unistd.h> //sleep func
#include <time.h> //for Render no delay
#include <termios.h> //for use getch
#include <pthread.h> //for use phtread
#include <string.h>
#define SEED 300

//array - 9*12 0~8, 0~11
//snake 1~70, wall 82, food 77

int x=5;int y=4; //start point;
char keyState;
int fx;
int fy;

char getch;
int UpdatePause = 0;

pthread_mutex_t mutex_lock;
int stopHere = 0;
int endThisGame = 0;


int snake[12][9] =
{
	 82,82,82,82,82,82,82,82,82
	,82,0 ,0 ,0 ,0 ,0 ,0 ,0 ,82
	,82,0 ,0 ,0 ,0 ,0 ,0 ,0 ,82
	,82,0 ,0 ,0 ,0 ,0 ,0 ,0 ,82
	,82,0 ,0 ,0 ,0 ,0 ,0 ,0 ,82
	,82,0, 0, 0, 0, 0, 0, 0, 82
	,82,0 ,0 ,0 ,0 ,0 ,0 ,0 ,82
	,82,0 ,0 ,0 ,0 ,0 ,0 ,0, 82
	,82,0, 0, 0, 0, 0, 0, 0, 82
	,82,0 ,0 ,0 ,0 ,0 ,0 ,0 ,82
	,82,0, 0, 0, 0, 0, 0, 0, 82
	,82,82,82,82,82,82,82,82,82

}; //12 row , 9 col

void init(){
	x=5; y = 4;
    for(int i=1; i<11; i++){
        for(int j=1; j<8; j++){
            snake[i][j]=0;
        }
    }
} //make snake's way 0

void makeFood(int seed){
	// in 11,8 -> make food random
	srand(seed);
	fx = (rand()%10)+1;
	fy = (rand()%7)+1;
	if(snake[fx][fy]==0){
		snake[fx][fy] = 77;
	}else{ makeFood(seed+123); }

	//recursion -> make stack overflow, (gcc's stack mem = 2mb, if srand makes same result, then overflow)
}

void startPoint(){
	snake[5][4] =3;snake[6][4] =2;snake[7][4] =1;
	//head start point 5,4
	makeFood(SEED);
}

int linux_getch(void) 

{
	int ch;
	struct termios buf, save;
	tcgetattr(0, &save);
	buf = save;
	buf.c_lflag &= ~(ICANON | ECHO);
	buf.c_cc[VMIN] = 1;
	buf.c_cc[VTIME] = 0;
	tcsetattr(0, TCSAFLUSH, &buf);
	ch = getchar();
	tcsetattr(0, TCSAFLUSH, &save);
	return ch;
	//left D , Right C, UP A, DOWN B 
}

void goToUp(){
	 
	 int noMinus = 1; //when snake eat food then arrary's -1 shouldn't happen

	//crash trigger
	if(snake[x-1][y] > 77 )
	{
		printf("    Game over :: Wall Crash   \n");
		pthread_mutex_lock(&mutex_lock);
				stopHere = 1;
		pthread_mutex_unlock(&mutex_lock);


	}
		else if( 0 < snake[x-1][y] && snake[x-1][y] <71 )
		{  
			printf("  Game over :: Body Crash \n");
				pthread_mutex_lock(&mutex_lock);
				stopHere = 1;
				pthread_mutex_unlock(&mutex_lock);
		}
		else if (snake[x-1][y]==77)
		{	//printf("find food\n");
			snake[x-1][y]=snake[x][y]+1;
			x=x-1;
			noMinus = 0;
			//makeLCD();
			makeFood(SEED);
		}
		else{
			//does not happen crash
			snake[x-1][y] = snake[x][y]+1;
			x=x-1;
			}


	 if(noMinus==1){
	 	for(int i=1; i<11; i++){
        	for(int j=1; j<8; j++){
			if(snake[i][j]>0) {
				if(snake[i][j]!=77) 
				snake[i][j]= snake[i][j] -1;
			}
			
       	 	}
		}
    }

	noMinus = 1;
	//crash trigger. 
	
}

void goToDown(){
	  int noMinus = 1; //when snake eat food then arrary's -1 shouldn't happen
	
	//crash trigger
	if(snake[x+1][y] > 77)
	{
		printf("    Game over :: Wall Crash   \n");   
		pthread_mutex_lock(&mutex_lock);
				stopHere = 1;
		pthread_mutex_unlock(&mutex_lock);
	}
	else if( 0 < snake[x+1][y] && snake[x+1][y] <70 )
		{  
			printf("  Game over :: Body Crash \n");
			pthread_mutex_lock(&mutex_lock);
				stopHere = 1;
			pthread_mutex_unlock(&mutex_lock);
		}
	else if (snake[x+1][y]==77)
		{	//printf("find food");
			snake[x+1][y]=snake[x][y]+1;
			x=x+1;
			noMinus = 0;
			makeFood(SEED);
		}
		else{
			snake[x+1][y] = snake[x][y]+1;
			x=x+1;}
	 
	 if(noMinus==1){
	 	for(int i=1; i<11; i++){
        	for(int j=1; j<8; j++){
			if(snake[i][j]>0) {
				if(snake[i][j]!=77) 
				snake[i][j]= snake[i][j] -1;
			}
			
       	 	}
		}
    }

	noMinus = 1;
	//crash trigger. 
	
}

void goToLeft(){
	  int noMinus = 1; //when snake eat food then arrary's -1 shouldn't happen

	//crash trigger
	if(snake[x][y-1] > 77)
	{
		printf("    Game over :: Wall Crash   \n");   
		pthread_mutex_lock(&mutex_lock);
				stopHere = 1;
			pthread_mutex_unlock(&mutex_lock);
	}
	else if( 0 < snake[x][y-1] && snake[x][y-1] <70 )
		{  
			printf("  Game over :: Body Crash \n");
			pthread_mutex_lock(&mutex_lock);
				stopHere = 1;
			pthread_mutex_unlock(&mutex_lock);
		}
	else if (snake[x][y-1]==77)
		{	//printf("find food");
			snake[x][y-1]=snake[x][y]+1;
			y=y-1;
			noMinus = 0;
			makeFood(SEED);
		}
		else{
			snake[x][y-1] = snake[x][y]+1;
			y=y-1;}
	 
	 if(noMinus==1){
	 	for(int i=1; i<11; i++){
        	for(int j=1; j<8; j++){
			if(snake[i][j]>0) {
				if(snake[i][j]!=77) 
				snake[i][j]= snake[i][j] -1;
			}
			
       	 	}
		}
    }

	noMinus = 1;
	//crash trigger. 
	
}

void goToRight(){
	int noMinus = 1; //when snake eat food then arrary's -1 shouldn't happen
	
	//crash trigger
	if(snake[x][y+1] > 77 )
	{
		printf("    Game over :: Wall Crash   \n");  
		pthread_mutex_lock(&mutex_lock);
				stopHere = 1;
			pthread_mutex_unlock(&mutex_lock);
		//gameoverTrigger();
	}
	else if( 0 < snake[x][y+1] && snake[x][y+1] <70 )
		{  
			printf("  Game over :: Body Crash \n");
			pthread_mutex_lock(&mutex_lock);
				stopHere = 1;
			pthread_mutex_unlock(&mutex_lock);
		}
	else if (snake[x][y+1]==77)
		{	//printf("find food");
			snake[x][y+1]=snake[x][y]+1;
			y=y+1;
			noMinus = 0;
			makeFood(SEED);
		}
		else{
			snake[x][y+1] = snake[x][y]+1;
			y=y+1;}
	 
	 if(noMinus==1){
	 	for(int i=1; i<11; i++){
        	for(int j=1; j<8; j++){
			if(snake[i][j]>0) {
				if(snake[i][j]!=77) 
				snake[i][j]= snake[i][j] -1;
			}
			
       	 	}
		}
		noMinus = 0;
    	}

	noMinus = 1;
}

void Render(){

	 for (int i = 0; i < 12; i++) {
			for (int j = 0; j < 9; j++) {
				printf("[%2d]", snake[i][j]);
			}

			printf("\n");	
		}
}//print game window

void threadEnd(pthread_t t){
	pthread_detach(t); 
}

void* thread_inputUserKey(){
	while(1) {

		switch (getch = linux_getch())
		{
		case 'w':
		 //Update('w'); break;//printf(" UP\n");
		 //Prevents the occurrence of a self-eating situation when going in the opposite direction
		 //진행방향의 정반대키는 입력안되게함 ->진행 방향의 반대키를 누를경우 바로 자기 자신에 충돌하기 떄문에 이를 방지함
		 if(keyState=='s'){
			keyState='s';
		 }
		 else{keyState='w';}break;

		case 'a':
		 //Update('a'); break;//printf(" LEFT\n");
		 if(keyState=='d'){
			keyState='d';
		 }
		 else{keyState='a';} break;
			
		case 's':
		// Update('s'); break;//printf(" DOWN\n");
		if(keyState=='w'){
			keyState='w';
		 }
		else{keyState='s';} break;
			
		case 'd':
		// Update('d'); break;//printf(" RIGHT\n");
		if(keyState=='a'){
			keyState='a';
		 }else{keyState='d';} break;
		
		case 'f':
		makeFood(SEED); break;

		case 'r':
		system("clear");
		Render(); 
		printf("rendering complete") ; break;
		
		case 'p':
			keyState = 'p';
			pthread_exit((void *)2);
			break;
		
		case 'q':
		pthread_mutex_lock(&mutex_lock);
				endThisGame = 1;
		pthread_exit(NULL); //exit  thread_inputUserKey()
		pthread_mutex_unlock(&mutex_lock);
		break;

		default :
		break;
		
		}

		
	}
}

void* thread_whichWay(){
	
	pthread_mutex_lock(&mutex_lock);
	keyState = 'n';
	pthread_mutex_unlock(&mutex_lock);

	while(1){

		if(stopHere == 1){
			break;
		}

		switch (keyState)
		{
		case 'w':
			do{
				goToUp();
				system("clear");
				Render();
				usleep(400000);
			} while (keyState=='w' && stopHere ==0);
			break;
		
		case 's':
			do{
				goToDown();
				system("clear");
				Render();
				usleep(400000);
			} while (keyState=='s' && stopHere ==0);
			break;

		case 'a':
			do{
				goToLeft();
				system("clear");
				Render();
				usleep(400000);
			} while (keyState=='a' && stopHere ==0);
			break;
		case 'd':
			do{
				goToRight();
				system("clear");
				Render();
				usleep(400000);
			} while (keyState=='d' && stopHere ==0);
			break;		
			
		default:
			break;
		}

	
	}
	printf("\n Game over Restart : P, Quit : Q  \n");
	

	pthread_mutex_lock(&mutex_lock);
	stopHere = 0;
	pthread_mutex_unlock(&mutex_lock);
	pthread_exit((void *)3);
}


void StartGame(){

	void *tret = NULL;

	init();
	startPoint();
	Render();

	pthread_t thread, thread2;
	pthread_create(&thread, NULL, thread_inputUserKey ,NULL);
	pthread_create(&thread2, NULL, thread_whichWay ,NULL);


	printf("Snake Game\n");

	pthread_join(thread2,&tret);
	pthread_join(thread,&tret);


	if(endThisGame != 1){
			StartGame();
	}

	printf("all thread Stopped, game end c'u next time!");

}

int main() {
	StartGame();
	return 0; 
}
