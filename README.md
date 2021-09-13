# Embedded_Snake_Game : 리눅스 상에서 구현한 뱀 게임
Achro-EM 보드의 FPGA 디바이스를 이용한 뱀 게임 구현

1. Push Switch입력으로 방향 제어, 게임 종료

2. Dot Matrix를 통해 뱀의 동작 구현

3. Text LCD를 통해 현재 점수 출력

gcc -o snake snake.c -lpthread 로 컴파일 

.c 파일은 보드 없이 리눅스 커널에서 동작되는 코드로 이거 베이스로 다른거 만들면됨

![image](https://user-images.githubusercontent.com/66546156/124921770-b637db80-e033-11eb-8f21-54d75bbf9f73.png)


 # 뱀 게임 알고리즘
 뱀 자체의 고유한 객체의 이동과 변화를 구현한 것이 아닌 배열의 성질을 이용하여 움직이는 것처럼, 성장하는 것처럼 만든 코드임. 
 
 벽, 먹이, 뱀의 움직임을 구현하기위해 12x9 배열을 선언
 
 ![image](https://user-images.githubusercontent.com/66546156/124921803-c059da00-e033-11eb-981a-b9c6c5cf835c.png)

Dot Matrix가 배열 구조로 이루어져 있기 때문에 (10행 x 7열 크기),  

Dot Matrix에 LED 점등 매핑을 원활 하게 하기 위해서 뱀 게임의 모든 객체를 12행x9열 배열의 요소 값으로 표현. -> 벽 값까지 생각해야되기 때문

뱀의 몸체는 머리와 꼬리 부분으로 구성, 머리 부분은 현재 뱀의 전체 길이를 나타냄.

 벽 객체는 82, 먹이는 77, 뱀의 신체는 최대 10x7 의 길이를 가질 수 있음. 

Dot Matrix에 LED 점등 매핑 시 전체 뱀 배열의 1~10행 * 1~8열만 불러와서 매핑 수행하며 0보다 큰 값만 불을 켜 줌.


# 뱀 게임 알고리즘 - 이동

뱀 객체가 실제로 이동하는 것이 아닌 배열의 값을 프레임마다 변화시켜주면서 뱀이 움직이는것처럼 보이게 하는것임

![image](https://user-images.githubusercontent.com/66546156/124921962-f5662c80-e033-11eb-9f0e-3177cab5ea22.png)
![image](https://user-images.githubusercontent.com/66546156/124921975-fa2ae080-e033-11eb-9d8f-7f1bcde95099.png)
![image](https://user-images.githubusercontent.com/66546156/124921986-fbf4a400-e033-11eb-975e-4fba48f084ce.png)

snake[x][y] 은 현재 머리의 위치를 나타냄
초기 값으로 5행 4열에 지정되어 있음

Ex) goToUp() 함수 호출 시
진행 방향에서 다음 위치의 값에 머리 위치 값 +1 을 대입 후 머리 위치를 변경.

	snake[x-1][y]=snake[x][y]+1;		
	x=x-1;

2.   전체 배열 값에서 -1 수행.
  		 
	snake[i][j]= snake[i][j] -1
  
Dot Matrix 는 0 이상인 값만 불이 켜지게 설정되어서 위로 이동한 것 처럼 보이게 됨

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

# 뱀 게임 알고리즘 - 충돌 _ 먹이 
![image](https://user-images.githubusercontent.com/66546156/124922076-1a5a9f80-e034-11eb-91fd-85b37f361a1d.png)
![image](https://user-images.githubusercontent.com/66546156/124922081-1c246300-e034-11eb-9795-967d517bc680.png)
![image](https://user-images.githubusercontent.com/66546156/124922093-1e86bd00-e034-11eb-8644-8de7d7779986.png)

진행 방향에서 다음 위치의 값이 먹이 (77) 인 경우는 ‘이동’과 동일하나 전체 배열에서 -1을 해주지 않음     그 결과 전체 길이가 늘어나게 됨

# 뱀 게임 알고리즘 - 충돌 _ 몸체, 벽
![image](https://user-images.githubusercontent.com/66546156/124922240-44ac5d00-e034-11eb-921a-768fca59167d.png)
![image](https://user-images.githubusercontent.com/66546156/124922259-47a74d80-e034-11eb-9474-ec1d024ee5a7.png)

진행 방향에서 다음 위치의 값이 벽 (82) 이거나,      몸체 (1이상 70이하)인 경우를 조건문을 통해 게임  종료에 사용되는 flag 변수 StopHere의 값을 변경시킴

	pthread_mutex_lock(&mutex_lock);
		stopHere = 1;
	pthread_mutex_unlock(&mutex_lock);

이후 다른 함수에서 flag 검사 후 게임을 중단시킴


# Achro-EM Board에서의 표현
conver_dot 함수를 이용하여 achro-em kit 의 dot matrix에 매핑시켜줌 

achro-EM 의 dot matrix의 각각 LED는 16진수로 주소가 지정되어있기 때문에 매핑 필요

snake_linux.c 파일의 경우 키보드로 컨트롤이 가능하며

보드와 합쳐진 snake.c 는 키보드 컨트롤러 함수에 achro-em의 push switch를 이용해서 컨트롤되게끔 함

![image](https://user-images.githubusercontent.com/66546156/124937226-277e8b00-e042-11eb-9118-837c33def2ac.png)
![image](https://user-images.githubusercontent.com/66546156/124937161-19306f00-e042-11eb-83b4-234da4f78c01.png)
