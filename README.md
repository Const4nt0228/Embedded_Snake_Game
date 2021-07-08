# Embedded_Snake_Game
Achro-EM 보드의 FPGA 디바이스를 이용한 뱀 게임 구현
Push Switch입력으로 방향 제어, 게임 종료
Dot Matrix를 통해 뱀의 동작 구현
Text LCD를 통해 현재 점수 출력

![image](https://user-images.githubusercontent.com/66546156/124921770-b637db80-e033-11eb-8f21-54d75bbf9f73.png)


 # 뱀 게임 알고리즘
 벽, 먹이, 뱀의 움직임을 구현하기위해 12x9 배열을 선언
 
 ![image](https://user-images.githubusercontent.com/66546156/124921803-c059da00-e033-11eb-981a-b9c6c5cf835c.png)

Dot Matrix가 배열 구조로 이루어져 있기 때문에,   Dot Matrix에 LED 점등 매핑을 원활 하게 하기 위해서 뱀 게임의 모든 객체를 12*9 배열의 요소 값으로 표현.

뱀의 몸체는 머리와 꼬리 부분으로 구성, 머리 부분은 현재 뱀의 전체 길이를 나타냄.

 벽 객체는 82, 먹이는 77, 뱀의 신체는 최대 10*7 의 길이를 가질 수 있음. 

Dot Matrix에 LED 점등 매핑 시 전체 뱀 배열의 1~10행 * 1~8열만 불러와서 매핑 수행하며 0보다 큰 값만 불을 켜 줌.


# 뱀 게임 알고리즘 - 이동

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
![image](https://user-images.githubusercontent.com/66546156/124922269-4b3ad480-e034-11eb-8426-044e8b8788f9.png)

