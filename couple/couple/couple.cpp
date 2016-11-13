#include <Turboc.h>//stdio, stdlib, conio, time, windows 과같은 여러가지 헤더파일과 기능들을 정의해논 헤더파일이다.

#pragma comment(lib, "winmm.lib")         //BGM을 위한 전처리기

#define LEFT 75 
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ESC 27

int difficulty = 4; // 게임 난이도 조절변수
enum Status { HIDDEN, FLIP, TEMPFLIP };//카드 숨겨진상태, 뒤집어진상태, 일시적으로 뒤집어진상태를 정의한다.
struct tag_Cell //각 칸에대한 데이터를 저장하는 구조체
{
	int Num;
	Status St;
};
struct userdb //유저정보를 저장하는 구조체(이중연결리스트)
{
	char dbusername[20];
	int dbscore;
	struct userdb* next;
	struct userdb* prev;

};
userdb *head; //전역변수 head(처음)
userdb *tail; //전역변수 tail노드(마지막)
tag_Cell arCell[6][6];//함수 최대배열값을 정의한다
					  //--------------변수선언
int sound = 1;               //프로그램 시작할 때만 BGM틀기위해
int startflag = 1;            //gameStart함수를 한번만 실행하기 위해
int nx, ny;//xy축
int count;//카운팅용
int score;//점수 기록용 변수
int highscore;//최고점 기록을위한Q변수
char username[20];//유저이름 일시저장
int gamecount = 0;//유저 스코어 일시저장
int firstdatainsert = 0;//데이터 가 기존에있는지 아니면 없이 처음 삽입하는지.
int userscore;
int overlap; //db에 들어있는 유저인지아닌지
			 //--------------함수선언
void printrank();//랭킹리스트 출력(이종훈)
void init();//이중연결리스트 노드 동적할당 및 초기화(이종훈)
int searchdb(char *username);//db데이터에서 중복되는 닉네임이있는지 찾기(이종훈)
void InitGame();//게임시작(같이)
void dbsave();//파일에 데이터 기록(이종훈);
void dbload();//파일으로부터 데이터 로드(이종훈);
int inserttodb(char *username, int userscore);//이중연결리스트에 데이터삽입함수(이종훈)
void menu();//메뉴 출력함수(이종훈)
void DrawScreen(BOOL bHint);//화면그리기함수(같이)
void GetTempFlip(int *tx, int *ty);//뒤집어진카드의 좌표를 반납(같이)
int GetRemain();//남은 카드수 구하기(같이)
void introducinggame();//게임소개(이종훈);
void gameStart(); //게임시작시 디스플레이 부분(허건)

void main()
{

	init(); //동적할당초기화
	int ch;//게임시작시 입력받는 변수   
	int tx, ty;//
	int SwitchForRank;//랭킹 스위치문을 위한 변수
	int whileflag = 1; // 전체적인 while문을 위한 플래그값
	char y_1;//y를 입력받기 위한 변수
	char y_2;//y를 입력받기 위한 변수2
	int gameflag = 1;//게임실행 반복while문을 위한 플래그값
	int setflag; // 메인 메뉴 case문을 위한 플래그값
	int forwhileflag = 1; //세팅 case문을 위한 플래그값
	int userwhileflag = 1;//user데이터 생성시 while문을 위한 플래그값

	dbload();//db로드

	gameStart(); //BGM실행과 처음화면을 꾸민 함수

	while (whileflag) {   //1번 while문
		menu();//초기 메뉴 출력
		scanf("%d", &setflag);
		switch (setflag) {//메뉴 switch 문
		case 1:       //게임 실행
			clrscr(); //기존의 메인메뉴화면 지우고 게임진입
			userwhileflag = 1;
			gamecount = 0;
			overlap = 0;//유저정보 중복여부 변수 0으로 초기화
			while (userwhileflag) {//닉네임 중복확인 및 게임진입을 위한 반복문
				printf("닉네임을 입력하세요\n");
				printf("당신의 닉네임은 : ");

				scanf("%s", username);//유저닉네임 입력받는다.
				if (searchdb(username) == 1) {
					clrscr();
					printf("%s 는 랭킹데이터에 이미 존재하는 사용자입니다. 계속하시려면 Y를 입력해주세요 : ", username);

					scanf(" %c", &y_1);

					if ((89 == y_1) || (121 == y_1)) {
						clrscr();
						userwhileflag = 0;
						overlap = 1;
					}
					else
						clrscr();
				}
				else {
					userwhileflag = 0;
					clrscr();
				}
			}
			randomize();
			InitGame();
			gameflag = 1;//게임을 한기록 남긴다


			while (gameflag) {//2번 while문
				if (gamecount != 0)
					overlap = 1;
				gotoxy(nx * 8 + 3, ny * 3 + 2);
				ch = _getch();
				if (ch == 0xE0) {
					ch = _getch();
					switch (ch) {
					case LEFT:
						if (nx > 0) nx--;
						break;
					case RIGHT:
						if (nx < difficulty - 1) nx++;//
						break;
					case UP:
						if (ny > 0) ny--;
						break;
					case DOWN:
						if (ny < difficulty - 1) ny++;//
						break;
					}
				}
				else {
					switch (ch) {

					case ESC: // esc누를시 현 반복문 종료와함께 1번while문으로 돌아가며 게임화면 지우기
						clrscr();
						printf("게임이 종료됩니다. 계속하시려면 (Y/N) 눌러주세요 : ");
						scanf(" %c", &y_2);

						if ((89 == y_2) || (121 == y_2)) {
							clrscr();
							gameflag = 0;

							break;
						}
						else
						{
							clrscr();
							DrawScreen(TRUE);

						}

						//exit(0);
						break;
					case ' ':
						if (arCell[nx][ny].St == HIDDEN) {
							GetTempFlip(&tx, &ty);
							if (tx == -1) {
								arCell[nx][ny].St = TEMPFLIP;
							}
							else {
								count++;
								if (arCell[tx][ty].Num == arCell[nx][ny].Num) {
									_putch('\a');
									arCell[tx][ty].St = FLIP;
									arCell[nx][ny].St = FLIP;
									if (GetRemain() == 0) {
										DrawScreen(FALSE);//drawscreen 함수호출

										score = 100 - (count - ((difficulty*difficulty / 2) - (GetRemain() / 2))) * 2;//점수

										highscore = inserttodb(username, score);//2중연결리스트에 데이터삽입

										if (score < 0) {//스코어가 0밑으로 내려가지않도록 조건문출력
											gotoxy(50, 12);   printf("당신의 점수는 -> 0점<- 입니다~~!");
										}
										else {
											gotoxy(50, 12); printf("당신의 점수는      ->%d점<- 입니다~~!", score);//스코어출력
											gotoxy(50, 14); printf("당신의 최고 점수는 ->%d점<- 입니다~~!", highscore);//최고점수 출력
											delay(1000);
											gotoxy(50, 16); puts("축하합니다. 다시 시작합니다.");
										}delay(1000);
										dbsave();//게임저장
										gamecount++;


										InitGame();
									}
								}
								else {
									arCell[nx][ny].St = TEMPFLIP;
									DrawScreen(FALSE);
									delay(1000);
									arCell[tx][ty].St = HIDDEN;
									arCell[nx][ny].St = HIDDEN;
								}
							}
							DrawScreen(FALSE);
						}
						break;
					}
				}
			}
			break;
		case 2:
			clrscr();
			introducinggame();//게임 소개
			if (_getch() != NULL) {
				clrscr();
				break;
			}
		case 3:
			clrscr();
			printf("1. 랭킹순위\n");//
			printf("2. 랭킹초기화\n");//
			scanf("%d", &SwitchForRank);
			switch (SwitchForRank) {
			case 1:
				printrank();
				break;
			case 2:
				head->next = tail; //헤드노드를 마지막노드로 이어줌으로써 초기화
				dbsave();
				printf("초기화 완료");
				delay(500);
				clrscr();
				break;
			}
			break;
		case 4:
			int temp;
			clrscr();
			printf("난이도를 조절합니다. (기본 난이도는 2=4x4)\n"); //난이도조절
			printf("원하는 난이도의 숫자를 입력해주세요.(1=2x2, 2=4x4, 3=6x6)\n");//난이도 조절
			printf("----> ");

			while (forwhileflag) {//난이도조절시 예외처리
				scanf("%d", &temp);
				if (temp >= 1 && temp <= 3) {
					difficulty = temp * 2;
					clrscr();
					break;
				}
				else {
					printf("잘못입력하셨습니다. 1~3사이의 숫자를 입력해주세요\n");
					printf("----> ");
				}

			}
			//난이도에 따른 노래재생
			if (temp == 1)
				PlaySound(TEXT("Sin.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			else if (temp == 2)
				PlaySound(TEXT("RockinNightStyle.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			else
				PlaySound(TEXT("Nightmare.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			break;
		case 5:
			whileflag = 0;//반복문 종료
			break;
		}

	}

}

//게임판을 초기화 하는 함수
void InitGame()
{
	int i, j;
	int x, y;
	nx = ny = 0;
	count = 0;

	memset(arCell, 0, sizeof(arCell));
	for (i = 1; i <= difficulty*difficulty / 2; i++) {
		for (j = 0; j<2; j++) {
			do {
				x = random(difficulty);//
				y = random(difficulty);//
			} while (arCell[x][y].Num != 0);
			arCell[x][y].Num = i;
		}
	}

	DrawScreen(TRUE);
	delay(2000);
	clrscr();
	DrawScreen(FALSE);
}

//화면을 그리는 함수
void DrawScreen(BOOL bHint)
{

	int x, y;
	int s, g;

	for (y = 0; y<difficulty; y++) {//
		for (x = 0; x<difficulty; x++) {//
			gotoxy(x * 8 + 2, y * 3 + 2);
			if (bHint == TRUE || arCell[x][y].St == FLIP) {
				gotoxy(wherex() - 1, wherey());
				s = x * 8 + 2;
				g = y * 3 + 2;
				gotoxy(s - 2, g - 1);
				printf("┏━━┓");
				gotoxy(s - 2, g);
				printf("┃%2d  ┃", arCell[x][y].Num);
				gotoxy(s - 2, g + 1);
				printf("┗━━┛");
			}
			else if (arCell[x][y].St == TEMPFLIP) {
				s = x * 8 + 2;
				g = y * 3 + 2;
				gotoxy(s - 2, g - 1);
				printf("┏    ┓");
				gotoxy(s - 2, g);
				printf("  %2d   ", arCell[x][y].Num);
				gotoxy(s - 2, g + 1);
				printf("┗    ┛");
			}
			else {
				s = x * 8 + 2;
				g = y * 3 + 2;
				gotoxy(s - 2, g - 1);
				printf("┏━━┓");
				gotoxy(s - 2, g);
				printf("┃ ♥ ┃");
				gotoxy(s - 2, g + 1);
				printf("┗━━┛");
			}
		}
	}

	gotoxy(50, 2); printf("사용자 이름 : %s", username);
	gotoxy(50, 4); puts("커서키:이동. 공백:뒤집기. Esc:종료");
	gotoxy(50, 6); printf("총 시도 회수 : %d", count);
	gotoxy(50, 8); printf("아직 못 찾은 것 : %d ", GetRemain());
	gotoxy(50, 10); printf("틀린 횟수 : %d ", count - ((difficulty*difficulty / 2) - (GetRemain() / 2)));

}

//임시로 뒤집혀진 칸의 번호를 조사
void GetTempFlip(int *tx, int *ty)
{
	int i, j;
	for (i = 0; i<difficulty; i++) {
		for (j = 0; j<difficulty; j++) {
			if (arCell[i][j].St == TEMPFLIP) {
				*tx = i;
				*ty = j;
				return;
			}
		}
	}
	*tx = -1;
}

//아직 찾지못한 칸의 갯수를 조사
int GetRemain()
{
	int i, j;
	int remain = difficulty*difficulty;//


	for (i = 0; i<difficulty; i++) {//
		for (j = 0; j<difficulty; j++) {//
			if (arCell[i][j].St == FLIP) {
				remain--;
			}

		}
	}
	return remain;
}
int searchdb(char *username)//유저닉네임 입력시 비교해서 중복여부를 사용자에게 확인해주고 기존의 기록에 오버라이트한다.
{
	userdb *tmp;
	tmp = head->next;
	while (strcmp(tmp->dbusername, username) != 0 && tmp != tail) //
	{
		tmp = tmp->next;
	}
	if (tmp == tail) {
		return 0;
	}
	else {
		overlap = 1;//데이터 중복상태로 변경
		return 1;
	}

}
void printrank()
{
	clrscr();
	userdb *tmp = head->next;    // tmp는 머리의 다음 노드 주소값을 가짐
	int ranknum = 1;
	char error = '儆';

	if (tmp->dbusername[0] == error&&gamecount == 0) {
		printf("정보가 존재하지 않습니다.");
		if (_getch() != NULL) {
			clrscr();
		}
	}
	else
	{
		if (firstdatainsert == 1) {
			tmp = tmp->next;

		}
		while (tmp != tail)     // 노드의 끝 ( 꼬리까지 )
		{

			printf("순위 : %d   닉네임 : %10s   점수 : %10d\n", ranknum++, tmp->dbusername, tmp->dbscore); // tmp가 가리키는 노드 데이터를 출력
			tmp = tmp->next;    // tmp 다음으로 이동
		}
	}

	if (_getch() != NULL) {
		clrscr();

	}



}
int inserttodb(char *username, int userscore) {
	userdb *tmp = head->next;//포인터의 시작점은 머리다음
	userdb *node = (userdb*)malloc(sizeof(userdb));//새 노드 동적할당
	strcpy(node->dbusername, username);//새 노드에 이름복사
	node->dbscore = userscore;//새 노드에 점수 복사
	if (head->next == tail)//노드가 없으면(머리 다음이 꼬리면)
	{

		//머리와 꼬리 사이에 새 노드 넣음
		head->next = node;
		node->prev = head;
		node->next = tail;
		tail->prev = node;
		return 0;
	}
	else
	{
		if (overlap == 0)
		{
			while (1)//무한루프
			{

				if (tmp->dbscore < node->dbscore)//기존의 노드의 점수가 새 점수보다 크면
				{

					node->next = tmp;       // 기존의노드 앞에 새노드 삽입
					node->prev = tmp->prev;
					tmp->prev->next = node;
					tmp->prev = node;
					return userscore;
				}
				if (tmp == tail)        // 새 노드의 점수가 제일 크면
				{

					node->next = tail;      // 꼬리 앞에 새 노드 삽입
					node->prev = tail->prev;
					tail->prev->next = node;
					tail->prev = node;
					return userscore;
				}
				tmp = tmp->next;        // 기존의 노드 포인터 이동
			}
		}
		else
		{
			while (1)
			{

				if (!strcmp(tmp->dbusername, node->dbusername))  //같은유저가 있으면
				{
					if (tmp->dbscore < node->dbscore) //기존의점수보다 현재점수가 더크면
					{
						tmp->dbscore = node->dbscore;//저장후
						return tmp->dbscore;//리턴한다.
					}
					else
					{
						return tmp->dbscore;//기존의 점수가더크면 기존의점수 리턴
					}

				}
				tmp = tmp->next;
			}
		}
	}
}

void dbload()
{
	char tempname[20];
	int tempscore = 0;
	char error_1 = '儆';

	FILE *fpin;//파일포인터변수의선언

	fpin = fopen("userdb.txt", "r");//userdb.txt 파일을 읽어들인다

	while (!feof(fpin)) //파일을 다 읽어들일때까지 while문이 돈다
	{
		userdb *node = (userdb*)malloc(sizeof(userdb)); // 새 노드 할당

		fscanf(fpin, "%s %d", tempname, &tempscore);
		if (tempname[0] == error_1)
		{

			firstdatainsert = 1;
		}
		strcpy(node->dbusername, tempname);
		node->dbscore = tempscore;
		if (head->next == tail)     // 노드가 없으면 ( 머리다음이 꼬리면 )
		{


			// 머리와 꼬리 사이에 새 노드넣음
			head->next = node;
			node->prev = head;
			node->next = tail;
			tail->prev = node;

		}
		else         // 다른노드가 있으면
		{

			tail->prev->next = node; // 꼬리 바로 전에 새 노드 추가
			node->prev = tail->prev;    // 새 노드 이전을 꼬리의 이전으로 ( 기존노드와 꼬리 사이에 새 노드 들어감 )
			node->next = tail;   // 새 노드 다음은 꼬리
			tail->prev = node;   // 꼬리 이전은 새 노드

		}
		//node = node->next;
	}

	fclose(fpin);//연결중인 file 스트림을 종료함
}
void dbsave()//유저db에 데이터를 순차적으로 삽입한다
{
	char error_1 = '儆';

	userdb *tmp = head->next;//tmp는 머리의 다음 노드 주소값을 가짐

	FILE *fpout;//파일포인터변수의선언
	fpout = fopen("userdb.txt", "w");//fpout 파일변수 초기화
	while (tmp != tail) {//노드의 끝(꼬리까지)

		if (tmp->dbusername[0] != error_1) {

			if (tmp->next != tail) {

				fprintf(fpout, "%s %d\n", tmp->dbusername, tmp->dbscore);//파일db에 기록한다

			}
			else {

				fprintf(fpout, "%s %d", tmp->dbusername, tmp->dbscore);
			}
		}

		tmp = tmp->next; //tmp 다음으로 이동
	}
	fclose(fpout);//연결중인 file스트림을 종료함

}
void init() {
	head = (userdb*)malloc(sizeof(userdb)); // 머리동적할당
	tail = (userdb*)malloc(sizeof(userdb)); // 꼬리동적할당
	head->next = tail;      // 머리다음에 꼬리 연결
	head->prev = head;      // 머리이전은 자기자신 가리킴
	tail->prev = head;      // 꼬리이전은 머리
	tail->next = tail;      // 꼬리다음은 자기자신



}
void menu() {
	printf("☆짝 맞추기 게임☆\n");//표시용
	printf("1. 게임실행\n");//게임실행while문으로진입을 표기해줌
	printf("2. 게임소개\n");//
	printf("3. 랭킹\n");//
	printf("4. 난이도설정\n");
	printf("5. 종료\n");
	printf("번호를 입력해주세용 : ");
}
void introducinggame() {
	printf("짝 찾기 게임이란 2x2, 4x4, 6x6 과 같은 칸에 들어있는 섞여있는 카드중 숫자가 같은 짝을 찾는 게임이다. 일종의 기억력 테스트 게임이라고 할 수 있다.\n게임을 시작하면 2초간 숫자들이 어디 숨어 있는지를 보여주는데 배치 상황을 재빨리 기억해 두어야 한다.잠시 후 모든 숫자는 ? 로 바뀌는데 숨어 있는 숫자의 짝을 최소한의 시도로 찾는 것이 게임의 목표이다.커서 이동키와 공백키로 게임을 진행하며 Esc를 누르면 게임이 종료된다.커서키로 숫자들 사이를 이동하며 공백키로 숫자를 열어 보고 짝이 맞는 두 숫자를 연속으로 선택하여 숨겨진 숫자쌍을 찾는다.틀릴 경우는 왜 틀렸는지를 1초간 보여주는데 이때 숨겨진 숫자의 위치를 잘 기억해 두는 것이 이 게임의 요령이다.\n ");
	printf("\n\n아무키나 누르면 이전화면으로 돌아갑니다");
}

void gameStart()
{
	system("title 카드 짝 맞추기");            //프로그램 제목
	system("mode con:cols=90 lines=20");      //콘솔 창 크기변경

											  //처음 프로그램 실행 시 BGM재생
	if (sound == 1)
	{
		PlaySound(TEXT("OBLIVION.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		sound = 0;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);      //폰트 색상변경
	int delayNum = 1;      //딜레이 조절변수
	if (startflag == 1)
	{
		//1,2번째 줄
		for (int j = 0; j < 2; j++)
		{
			for (int i = 0; i < 44; i++)
			{
				printf("☆");
				delay(delayNum);
			}
			printf("\n");
		}

		//3번째 줄
		for (int i = 0; i < 22; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 21; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("\n");

		//4번째 줄
		for (int i = 0; i < 22; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 4; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		for (int i = 0; i < 12; i++)
		{
			printf("★");
			delay(delayNum);
		}
		for (int i = 0; i < 5; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("\n");

		//5번째 줄
		for (int j = 0; j < 2; j++)
		{
			for (int i = 0; i < 2; i++)
			{
				printf("☆");
				delay(delayNum);
			}
			for (int i = 0; i < 8; i++)
			{
				printf("★");
				delay(delayNum);
			}
		}
		for (int i = 0; i < 2; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 9; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 5; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 5; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("\n");

		//6번째 줄
		for (int i = 0; i < 5; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		for (int i = 0; i < 2; i++)
		{
			printf("★");
			delay(delayNum);
		}
		for (int i = 0; i < 8; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		for (int i = 0; i < 2; i++)
		{
			printf("★");
			delay(delayNum);
		}
		for (int i = 0; i < 5; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 9; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 5; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 5; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("\n");

		//7번째 줄
		for (int i = 0; i < 4; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 2; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 6; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 2; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 4; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		for (int i = 0; i < 3; i++)
		{
			printf("★");
			delay(delayNum);
		}
		for (int i = 0; i < 7; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 5; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 5; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("\n");

		//8번째 줄
		for (int i = 0; i < 3; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 4; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 4; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 4; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 3; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 9; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 5; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 5; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("\n");

		//9번째줄
		for (int i = 0; i < 2; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 6; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 2; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 6; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 2; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 9; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 5; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 5; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("\n");



		//10번째줄
		printf("☆");
		delay(delayNum);
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 8; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		for (int i = 0; i < 2; i++)
		{
			printf("★");
			delay(delayNum);
		}
		for (int i = 0; i < 8; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		printf("☆");
		delay(delayNum);
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 3; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		for (int i = 0; i < 16; i++)
		{
			printf("★");
			delay(delayNum);
		}
		for (int i = 0; i < 2; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("\n");

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		//11번째 줄
		for (int i = 0; i < 44; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("\n");

		//12번째 줄
		for (int i = 0; i < 8; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		for (int i = 0; i < 15; i++)
		{
			printf("★");
			delay(delayNum);
		}
		for (int i = 0; i < 11; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 9; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("\n");

		//13번째 줄
		for (int i = 0; i < 22; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 11; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 9; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("\n");

		//14번째 줄
		for (int i = 0; i < 22; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 8; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		for (int i = 0; i < 7; i++)
		{
			printf("★");
			delay(delayNum);
		}
		for (int i = 0; i < 6; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("\n");

		//15번째 줄
		for (int i = 0; i < 22; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 7; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 7; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 5; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("\n");

		//16번째 줄
		for (int i = 0; i < 22; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 6; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 9; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 4; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("\n");

		//17번째 줄 = 15번째 줄
		for (int i = 0; i < 22; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 7; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 7; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 5; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("\n");

		//18번째 줄 = 14번째 줄
		for (int i = 0; i < 22; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("★");
		delay(delayNum);
		for (int i = 0; i < 8; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		for (int i = 0; i < 7; i++)
		{
			printf("★");
			delay(delayNum);
		}
		for (int i = 0; i < 6; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("\n");

		//19, 20 번째 줄
		for (int i = 0; i < 44; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		printf("\n");
		for (int i = 0; i < 44; i++)
		{
			printf("☆");
			delay(delayNum);
		}
		delay(1000);


		//색상바꿈효과주기
		for (int i = 15; i >= 1; i--)
		{
			gotoxy(0, 0);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i);
			//printf("★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n★★★★★★★★★★★★★★★★★★★★★★☆★★★★★★★★★★★★★★★★★★★★★\n★★★★★★★★★★★★★★★★★★★★★★☆★★★★☆☆☆☆☆☆☆☆☆☆☆☆★★★★★\n★★☆☆☆☆☆☆☆☆★★☆☆☆☆☆☆☆☆★★☆★★★★★★★★★☆★★★★★☆★★★★★\n★★★★★☆☆★★★★★★★★☆☆★★★★★☆★★★★★★★★★☆★★★★★☆★★★★★\n★★★★☆★★☆★★★★★★☆★★☆★★★★☆☆☆★★★★★★★☆★★★★★☆★★★★★\n★★★☆★★★★☆★★★★☆★★★★☆★★★☆★★★★★★★★★☆★★★★★☆★★★★★\n★★☆★★★★★★☆★★☆★★★★★★☆★★☆★★★★★★★★★☆★★★★★☆★★★★★\n★☆★★★★★★★★☆☆★★★★★★★★☆★☆★★★☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆★★\n★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★☆★★★★★★★★★\n★★★★★★★★☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆★★★★★★★★★★★☆★★★★★★★★★\n★★★★★★★★★★★★★★★★★★★★★★☆★★★★★★★★★★★☆★★★★★★★★★\n★★★★★★★★★★★★★★★★★★★★★★☆★★★★★★★★☆☆☆☆☆☆☆★★★★★★\n★★★★★★★★★★★★★★★★★★★★★★☆★★★★★★★☆★★★★★★★☆★★★★★\n★★★★★★★★★★★★★★★★★★★★★★☆★★★★★★☆★★★★★★★★★☆★★★★\n★★★★★★★★★★★★★★★★★★★★★★☆★★★★★★★☆★★★★★★★☆★★★★★\n★★★★★★★★★★★★★★★★★★★★★★☆★★★★★★★★☆☆☆☆☆☆☆★★★★★★\n★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★");
			printf("☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆\n☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆\n☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆★☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆\n☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆★☆☆☆☆★★★★★★★★★★★★☆☆☆☆☆\n☆☆★★★★★★★★☆☆★★★★★★★★☆☆★☆☆☆☆☆☆☆☆☆★☆☆☆☆☆★☆☆☆☆☆\n☆☆☆☆☆★★☆☆☆☆☆☆☆☆★★☆☆☆☆☆★☆☆☆☆☆☆☆☆☆★☆☆☆☆☆★☆☆☆☆☆\n☆☆☆☆★☆☆★☆☆☆☆☆☆★☆☆★☆☆☆☆★★★☆☆☆☆☆☆☆★☆☆☆☆☆★☆☆☆☆☆\n☆☆☆★☆☆☆☆★☆☆☆☆★☆☆☆☆★☆☆☆★☆☆☆☆☆☆☆☆☆★☆☆☆☆☆★☆☆☆☆☆\n☆☆★☆☆☆☆☆☆★☆☆★☆☆☆☆☆☆★☆☆★☆☆☆☆☆☆☆☆☆★☆☆☆☆☆★☆☆☆☆☆\n☆★☆☆☆☆☆☆☆☆★★☆☆☆☆☆☆☆☆★☆★☆☆☆★★★★★★★★★★★★★★★★☆☆\n☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆★☆☆☆☆☆☆☆☆☆\n☆☆☆☆☆☆☆☆★★★★★★★★★★★★★★★☆☆☆☆☆☆☆☆☆☆☆★☆☆☆☆☆☆☆☆☆\n☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆★☆☆☆☆☆☆☆☆☆☆☆★☆☆☆☆☆☆☆☆☆\n☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆★☆☆☆☆☆☆☆☆★★★★★★★☆☆☆☆☆☆\n☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆★☆☆☆☆☆☆☆★☆☆☆☆☆☆☆★☆☆☆☆☆\n☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆★☆☆☆☆☆☆★☆☆☆☆☆☆☆☆☆★☆☆☆☆\n☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆★☆☆☆☆☆☆☆★☆☆☆☆☆☆☆★☆☆☆☆☆\n☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆★☆☆☆☆☆☆☆☆★★★★★★★☆☆☆☆☆☆\n☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆\n☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆");
			delay(150);
		}

		//게임실행중 폰트색상설정
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);


		startflag = 0; //다시돌지 않게 0으로 바꿔줌
		delay(1000);
		clrscr();
	}
}