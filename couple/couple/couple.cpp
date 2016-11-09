#include <Turboc.h>//stdio, stdlib, conio, time, windows 과같은 여러가지 헤더파일과 기능들을 정의해논 헤더파일이다.



#define LEFT 75 
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ESC 27

int difficulty = 4; // 게임 난이도 조절(이종훈)
enum Status { HIDDEN, FLIP, TEMPFLIP };//카드 숨겨진상태, 뒤집어진상태, 일시적으로 뒤집어진상태를 정의한다.
struct tag_Cell
{
	int Num;
	Status St;
};
struct userdb
{
	char username[20];
	int userscore;
};
tag_Cell arCell[6][6];//함수 최대배열값을 정의한다(이종훈)
int nx, ny;
int count;
int score;//점수 기록용 변수(이종훈)
int highscore;//최고점 기록을위한변수(이종훈)

int searchuser(userdb userdb);//db데이터에서 중복되는 닉네임이있는지 찾기(이종훈)
void insertuser(userdb userdb);//db데이터에 유저정보 입력(이종훈)
void InitGame(userdb userdb);//게임시작
void DrawScreen(BOOL bHint,userdb userdb);//화면그리기
void GetTempFlip(int *tx, int *ty);
int GetRemain();//남은 카드수 구하기

void main()
{
	
	
	userdb userdb;
	int ch;//게임시작시 입력받는 변수
	int tx, ty;//
	
	int whileflag=1; // 전체적인 while문을 위한 플래그값(이종훈)
	int gameflag = 1;//게임실행 반복while문을 위한 플래그값(이종훈)
	int setflag; // 메인 메뉴 case문을 위한 플래그값(이종훈)
	int forwhileflag = 1; //세팅 case문을 위한 플래그값(이종훈)
	int userwhileflag = 1;//user데이터 생성시 while문을 위한 플래그값(이종훈)
	//------------------------------------ 파일입출력
	//while (!feof(fpin)) { //파일을 다 읽어들일때까지 while문이 돈다

		//fscanf(fpin, "%s %d", temp->first, temp->second, temp->third); // 파일로부터 snum,name,정답을 읽어 각변수에 저장한다.


	//-------------------------------------


	while (whileflag) {   //1번 while문(이종훈)
		printf("☆짝 맞추기 게임☆\n");//표시용(이종훈)
		printf("1. 게임실행\n");//게임실행while문으로진입을 표기해줌(이종훈)
		printf("2. 게임소개\n");//(이종훈)
		printf("3. 랭킹\n");//(이종훈)
		printf("4. 난이도설정\n");
		printf("5. 종료\n");
		printf("번호를 입력해주세용 : ");
		scanf("%d", &setflag);
		switch (setflag) {
		case 1:       //게임 실행
			clrscr(); //기존의 메인메뉴화면 지우고 게임진입
			userwhileflag = 1;
			while (userwhileflag) {//닉네임 중복확인 및 게임진입을 위한 반복문
				printf("닉네임을 입력하세요");
				printf("당신의 닉네임은 : ");
				char yorn;
				scanf("%s", userdb.username);
				if (searchuser(userdb) == 1) {
					clrscr();
					printf("%s 는 랭킹데이터에 이미 존재하는 사용자입니다. 계속하시려면 Y를 입력해주세요 : ", userdb.username);
					
					scanf(" %c", &yorn);
					
					if ((89 == yorn) || (121 == yorn)) {
						clrscr();
						userwhileflag = 0;

					}
					else
						clrscr();
				}
				else {
					userwhileflag = 0;
					clrscr;
				}
			}
			randomize();
			InitGame(userdb);
			gameflag = 1;

			while (gameflag) {//2번 while문
				gotoxy(nx * 5 + 2, ny * 3 + 2);
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
						gameflag = 0;
						clrscr();
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
										DrawScreen(FALSE,userdb);//drawscreen 함수호출(인자로 불리언값과 유저구조체 넘겨준다)(이종훈)
										gotoxy(26, 26); puts("축하합니다. 다시 시작합니다.");
										score = 100 - (count - ((difficulty*difficulty / 2) - (GetRemain() / 2))) * 2;//점수(이종훈)
										userdb.userscore = score;//구조체에 점수저장(이종훈)
										insertuser(userdb);
										if (score < 0) {
											gotoxy(26, 24);	printf("당신의 점수는 -> 0점<- 입니다~~!");//스코어가 0밑으로 내려가지않도록 조건문출력(이종훈)
										}
										else{ 
											gotoxy(26, 24); printf("당신의 점수는 ->%d점<- 입니다~~!", score);//스코어출력(이종훈)
										}delay(2000);
										InitGame(userdb);//initgame함수안에서 drawscreen함수를 호출하므로 userdb 넘겨준다(이종훈)
									}
								}
								else {
									arCell[nx][ny].St = TEMPFLIP;
									DrawScreen(FALSE,userdb);
									delay(1000);
									arCell[tx][ty].St = HIDDEN;
									arCell[nx][ny].St = HIDDEN;
								}
							}
							DrawScreen(FALSE,userdb);
						}
						break;
					}
				}
			}
			break;
		case 2:
			clrscr();
			printf("짝 찾기 게임이란 2x2, 4x4, 6x6 과 같은 칸에 들어있는 섞여있는 카드중 숫자가 같은 짝을 찾는 게임이다. 일종의 기억력 테스트 게임이라고 할 수 있다.\n게임을 시작하면 2초간 숫자들이 어디 숨어 있는지를 보여주는데 배치 상황을 재빨리 기억해 두어야 한다.잠시 후 모든 숫자는 ? 로 바뀌는데 숨어 있는 숫자의 짝을 최소한의 시도로 찾는 것이 게임의 목표이다.커서 이동키와 공백키로 게임을 진행하며 Esc를 누르면 게임이 종료된다.커서키로 숫자들 사이를 이동하며 공백키로 숫자를 열어 보고 짝이 맞는 두 숫자를 연속으로 선택하여 숨겨진 숫자쌍을 찾는다.틀릴 경우는 왜 틀렸는지를 1초간 보여주는데 이때 숨겨진 숫자의 위치를 잘 기억해 두는 것이 이 게임의 요령이다.\n ");
			printf("\n\n아무키나 누르면 이전화면으로 돌아갑니다");
			if (_getch() != NULL) {
				clrscr();
				break;
			}
		case 3:
			printf("1. 랭킹순위\n");//(이종훈)
			printf("2. 랭킹초기화\n");//(이종훈)
			break;
		case 4:
			int temp;
			clrscr();
			printf("난이도를 조절합니다. (기본 난이도는 2=4x4)\n"); //난이도조절(이종훈)
			printf("원하는 난이도의 숫자를 입력해주세요.(1=2x2, 2=4x4, 3=6x6)\n");//난이도 조절(이종훈)
			printf("----> ");
			
			while (forwhileflag) {//난이도조절시 예외처리(이종훈)
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
			break;
		case 5:
			whileflag = 0;//반복문 종료
			break;
			}
			
	}
	
}


void InitGame(userdb userdb)
{
	int i, j;
	int x, y;
	nx = ny = 0;
	count = 0;

	memset(arCell, 0, sizeof(arCell));
	for (i = 1; i <= difficulty*difficulty /2; i++) {
		for (j = 0; j<2; j++) {
			do {
				x = random(difficulty);//
				y = random(difficulty);//
			} while (arCell[x][y].Num != 0);
			arCell[x][y].Num = i;
		}
	}

	DrawScreen(TRUE,userdb);
	delay(2000);
	clrscr();
	DrawScreen(FALSE,userdb);
}

void DrawScreen(BOOL bHint,userdb userdb)
{
	
	int x, y;

	for (y = 0; y<difficulty; y++) {//
		for (x = 0; x<difficulty; x++) {//
			gotoxy(x * 5 + 2, y * 3 + 2);
			if (bHint == TRUE || arCell[x][y].St == FLIP) {
				gotoxy(wherex() - 1, wherey());
				printf("[%d]", arCell[x][y].Num);
			}
			else if (arCell[x][y].St == TEMPFLIP) {
				printf("%d", arCell[x][y].Num);
			}
			else {
				printf("?");
			}
		}
	}

	gotoxy(40, 4); printf("사용자 이름 : %s",userdb.username);
	gotoxy(40, 6); puts("커서키:이동. 공백:뒤집기. Esc:종료");
	gotoxy(40, 8); printf("총 시도 회수 : %d", count);
	gotoxy(40, 10); printf("아직 못 찾은 것 : %d ", GetRemain());
	gotoxy(40, 12); printf("틀린 횟수 : %d ", count-((difficulty*difficulty/2)-(GetRemain()/2)));
}

void GetTempFlip(int *tx, int *ty)
{
	int i, j;
	for (i = 0; i<difficulty; i++) {//
		for (j = 0; j<difficulty; j++) {//
			if (arCell[i][j].St == TEMPFLIP) {
				*tx = i;
				*ty = j;
				return;
			}
		}
	}
	*tx = -1;
}

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
int searchuser(userdb userdb)//유저닉네임 입력시 비교해서 중복여부를 사용자에게 확인해주고 기존의 기록에 오버라이트한다.(이종훈)
{
	char tempuser[20];
	int detect = 0;
	FILE *fpin;//파일포인터변수의선언(종훈)
	
	fpin = fopen("userdb.txt", "r");//userdb.txt 파일을 읽어들인다(이종훈)
	while (!feof(fpin)) { //파일을 다 읽어들일때까지 while문이 돈다
		fscanf(fpin,"%s %d",tempuser); // 파일로부터 데이터를 읽어 각변수에 저장한다
		if (!strcmp(userdb.username,tempuser)) {
			detect = 1;
		}
	}
	fclose(fpin);//연결중인 file 스트림을 종료함(이종훈)
	if (detect == 1)return 1;
	else return 0;
	
}
void insertuser(userdb userdb)//유저db에 데이터를 순차적으로 삽입한다.(이종훈) 
{
	FILE *fpout;//파일포인터변수의선언(종훈)

	fpout = fopen("userdb.txt", "w");//userdb.txt파일을 추출한다.(이종훈)
	fprintf(fpout, "%s %d", userdb.username, userdb.userscore);//파일db에 기록한다(이종훈)

	fclose(fpout);//연결중인 file스트림을 종료함(이종훈)
}
