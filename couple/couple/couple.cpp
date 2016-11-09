#include <Turboc.h>//stdio, stdlib, conio, time, windows ������ �������� ������ϰ� ��ɵ��� �����س� ��������̴�.



#define LEFT 75 
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ESC 27

int difficulty = 4; // ���� ���̵� ����(������)
enum Status { HIDDEN, FLIP, TEMPFLIP };//ī�� ����������, ������������, �Ͻ������� �����������¸� �����Ѵ�.
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
tag_Cell arCell[6][6];//�Լ� �ִ�迭���� �����Ѵ�(������)
int nx, ny;
int count;
int score;//���� ��Ͽ� ����(������)
int highscore;//�ְ��� ��������Ѻ���(������)

int searchuser(userdb userdb);//db�����Ϳ��� �ߺ��Ǵ� �г������ִ��� ã��(������)
void insertuser(userdb userdb);//db�����Ϳ� �������� �Է�(������)
void InitGame(userdb userdb);//���ӽ���
void DrawScreen(BOOL bHint,userdb userdb);//ȭ��׸���
void GetTempFlip(int *tx, int *ty);
int GetRemain();//���� ī��� ���ϱ�

void main()
{
	
	
	userdb userdb;
	int ch;//���ӽ��۽� �Է¹޴� ����
	int tx, ty;//
	
	int whileflag=1; // ��ü���� while���� ���� �÷��װ�(������)
	int gameflag = 1;//���ӽ��� �ݺ�while���� ���� �÷��װ�(������)
	int setflag; // ���� �޴� case���� ���� �÷��װ�(������)
	int forwhileflag = 1; //���� case���� ���� �÷��װ�(������)
	int userwhileflag = 1;//user������ ������ while���� ���� �÷��װ�(������)
	//------------------------------------ ���������
	//while (!feof(fpin)) { //������ �� �о���϶����� while���� ����

		//fscanf(fpin, "%s %d", temp->first, temp->second, temp->third); // ���Ϸκ��� snum,name,������ �о� �������� �����Ѵ�.


	//-------------------------------------


	while (whileflag) {   //1�� while��(������)
		printf("��¦ ���߱� ���ӡ�\n");//ǥ�ÿ�(������)
		printf("1. ���ӽ���\n");//���ӽ���while������������ ǥ������(������)
		printf("2. ���ӼҰ�\n");//(������)
		printf("3. ��ŷ\n");//(������)
		printf("4. ���̵�����\n");
		printf("5. ����\n");
		printf("��ȣ�� �Է����ּ��� : ");
		scanf("%d", &setflag);
		switch (setflag) {
		case 1:       //���� ����
			clrscr(); //������ ���θ޴�ȭ�� ����� ��������
			userwhileflag = 1;
			while (userwhileflag) {//�г��� �ߺ�Ȯ�� �� ���������� ���� �ݺ���
				printf("�г����� �Է��ϼ���");
				printf("����� �г����� : ");
				char yorn;
				scanf("%s", userdb.username);
				if (searchuser(userdb) == 1) {
					clrscr();
					printf("%s �� ��ŷ�����Ϳ� �̹� �����ϴ� ������Դϴ�. ����Ͻ÷��� Y�� �Է����ּ��� : ", userdb.username);
					
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

			while (gameflag) {//2�� while��
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
					case ESC: // esc������ �� �ݺ��� ������Բ� 1��while������ ���ư��� ����ȭ�� �����
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
										DrawScreen(FALSE,userdb);//drawscreen �Լ�ȣ��(���ڷ� �Ҹ��𰪰� ��������ü �Ѱ��ش�)(������)
										gotoxy(26, 26); puts("�����մϴ�. �ٽ� �����մϴ�.");
										score = 100 - (count - ((difficulty*difficulty / 2) - (GetRemain() / 2))) * 2;//����(������)
										userdb.userscore = score;//����ü�� ��������(������)
										insertuser(userdb);
										if (score < 0) {
											gotoxy(26, 24);	printf("����� ������ -> 0��<- �Դϴ�~~!");//���ھ 0������ ���������ʵ��� ���ǹ����(������)
										}
										else{ 
											gotoxy(26, 24); printf("����� ������ ->%d��<- �Դϴ�~~!", score);//���ھ����(������)
										}delay(2000);
										InitGame(userdb);//initgame�Լ��ȿ��� drawscreen�Լ��� ȣ���ϹǷ� userdb �Ѱ��ش�(������)
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
			printf("¦ ã�� �����̶� 2x2, 4x4, 6x6 �� ���� ĭ�� ����ִ� �����ִ� ī���� ���ڰ� ���� ¦�� ã�� �����̴�. ������ ���� �׽�Ʈ �����̶�� �� �� �ִ�.\n������ �����ϸ� 2�ʰ� ���ڵ��� ��� ���� �ִ����� �����ִµ� ��ġ ��Ȳ�� �绡�� ����� �ξ�� �Ѵ�.��� �� ��� ���ڴ� ? �� �ٲ�µ� ���� �ִ� ������ ¦�� �ּ����� �õ��� ã�� ���� ������ ��ǥ�̴�.Ŀ�� �̵�Ű�� ����Ű�� ������ �����ϸ� Esc�� ������ ������ ����ȴ�.Ŀ��Ű�� ���ڵ� ���̸� �̵��ϸ� ����Ű�� ���ڸ� ���� ���� ¦�� �´� �� ���ڸ� �������� �����Ͽ� ������ ���ڽ��� ã�´�.Ʋ�� ���� �� Ʋ�ȴ����� 1�ʰ� �����ִµ� �̶� ������ ������ ��ġ�� �� ����� �δ� ���� �� ������ ����̴�.\n ");
			printf("\n\n�ƹ�Ű�� ������ ����ȭ������ ���ư��ϴ�");
			if (_getch() != NULL) {
				clrscr();
				break;
			}
		case 3:
			printf("1. ��ŷ����\n");//(������)
			printf("2. ��ŷ�ʱ�ȭ\n");//(������)
			break;
		case 4:
			int temp;
			clrscr();
			printf("���̵��� �����մϴ�. (�⺻ ���̵��� 2=4x4)\n"); //���̵�����(������)
			printf("���ϴ� ���̵��� ���ڸ� �Է����ּ���.(1=2x2, 2=4x4, 3=6x6)\n");//���̵� ����(������)
			printf("----> ");
			
			while (forwhileflag) {//���̵������� ����ó��(������)
				scanf("%d", &temp);
				if (temp >= 1 && temp <= 3) {
					difficulty = temp * 2;
					clrscr();
					break;
				}
				else {
				printf("�߸��Է��ϼ̽��ϴ�. 1~3������ ���ڸ� �Է����ּ���\n");
				printf("----> ");
				}
			
			}
			break;
		case 5:
			whileflag = 0;//�ݺ��� ����
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

	gotoxy(40, 4); printf("����� �̸� : %s",userdb.username);
	gotoxy(40, 6); puts("Ŀ��Ű:�̵�. ����:������. Esc:����");
	gotoxy(40, 8); printf("�� �õ� ȸ�� : %d", count);
	gotoxy(40, 10); printf("���� �� ã�� �� : %d ", GetRemain());
	gotoxy(40, 12); printf("Ʋ�� Ƚ�� : %d ", count-((difficulty*difficulty/2)-(GetRemain()/2)));
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
int searchuser(userdb userdb)//�����г��� �Է½� ���ؼ� �ߺ����θ� ����ڿ��� Ȯ�����ְ� ������ ��Ͽ� ��������Ʈ�Ѵ�.(������)
{
	char tempuser[20];
	int detect = 0;
	FILE *fpin;//���������ͺ����Ǽ���(����)
	
	fpin = fopen("userdb.txt", "r");//userdb.txt ������ �о���δ�(������)
	while (!feof(fpin)) { //������ �� �о���϶����� while���� ����
		fscanf(fpin,"%s %d",tempuser); // ���Ϸκ��� �����͸� �о� �������� �����Ѵ�
		if (!strcmp(userdb.username,tempuser)) {
			detect = 1;
		}
	}
	fclose(fpin);//�������� file ��Ʈ���� ������(������)
	if (detect == 1)return 1;
	else return 0;
	
}
void insertuser(userdb userdb)//����db�� �����͸� ���������� �����Ѵ�.(������) 
{
	FILE *fpout;//���������ͺ����Ǽ���(����)

	fpout = fopen("userdb.txt", "w");//userdb.txt������ �����Ѵ�.(������)
	fprintf(fpout, "%s %d", userdb.username, userdb.userscore);//����db�� ����Ѵ�(������)

	fclose(fpout);//�������� file��Ʈ���� ������(������)
}
