#include <Turboc.h>//stdio, stdlib, conio, time, windows ������ �������� ������ϰ� ��ɵ��� �����س� ��������̴�.



#define LEFT 75 
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ESC 27

int difficulty = 4; // ���� ���̵� ��������
enum Status { HIDDEN, FLIP, TEMPFLIP };//ī�� ����������, ������������, �Ͻ������� �����������¸� �����Ѵ�.
struct tag_Cell
{
	int Num;
	Status St;
};
struct userdb
{
	char dbusername[20];
	int dbscore;
	struct userdb* next;
	struct userdb* prev;
	
};
userdb *head;
userdb *tail;
tag_Cell arCell[6][6];//�Լ� �ִ�迭���� �����Ѵ�
int nx, ny;
int count;
int score;//���� ��Ͽ� ����
int highscore;//�ְ��� ���������Q����
char username[20];
int userscore;
int overlap; //db�� ����ִ� ���������ƴ���
void printrank();
void init();
int searchdb(char *username);//db�����Ϳ��� �ߺ��Ǵ� �г������ִ��� ã��
int inserttodb(char *username, int userscore);//db�����Ϳ� �������� �Է�
void InitGame();//���ӽ���
void dbsave();
void dbload();
void menu();
void DrawScreen(BOOL bHint);//ȭ��׸���
void GetTempFlip(int *tx, int *ty);
int GetRemain();//���� ī��� ���ϱ�
void introducinggame();//���ӼҰ�

void main()
{
	overlap = 0;//�������� �ߺ����� ���� 0���� �ʱ�ȭ
	init(); //�����Ҵ��ʱ�ȭ
	int ch;//���ӽ��۽� �Է¹޴� ����	
	int tx, ty;//
	int whileflag=1; // ��ü���� while���� ���� �÷��װ�
	int gameflag = 1;//���ӽ��� �ݺ�while���� ���� �÷��װ�
	int setflag; // ���� �޴� case���� ���� �÷��װ�
	int forwhileflag = 1; //���� case���� ���� �÷��װ�
	int userwhileflag = 1;//user������ ������ while���� ���� �÷��װ�
	dbload();//db�ε�

	while (whileflag) {   //1�� while��
		menu();//�ʱ� �޴� ���
		scanf("%d", &setflag);
		switch (setflag) {//�޴� switch ��
		case 1:       //���� ����
			clrscr(); //������ ���θ޴�ȭ�� ����� ��������
			userwhileflag = 1;
			while (userwhileflag) {//�г��� �ߺ�Ȯ�� �� ���������� ���� �ݺ���
				printf("�г����� �Է��ϼ���");
				printf("����� �г����� : ");
				char yorn;//y�Է¹ޱ� ������ ���� ����
				scanf("%s", username);//�����г��� �Է¹޴´�.
				if (searchdb(username) == 1) {
					clrscr();
					printf("%s �� ��ŷ�����Ϳ� �̹� �����ϴ� ������Դϴ�. ����Ͻ÷��� Y�� �Է����ּ��� : ", username);
					
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
					clrscr();
				}
			}
			randomize();
			InitGame();
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
										DrawScreen(FALSE);//drawscreen �Լ�ȣ��
										gotoxy(26, 26); puts("�����մϴ�. �ٽ� �����մϴ�.");
										score = 100 - (count - ((difficulty*difficulty / 2) - (GetRemain() / 2))) * 2;//����
										
										highscore=	inserttodb(username,score);//2�߿��Ḯ��Ʈ�� �����ͻ���
					
										if (score < 0) {//���ھ 0������ ���������ʵ��� ���ǹ����
											gotoxy(26, 24);	printf("����� ������ -> 0��<- �Դϴ�~~!");
										}
										else{ 
											gotoxy(26, 24); printf("����� ������      ->%d��<- �Դϴ�~~!", score);//���ھ����
											gotoxy(26, 28); printf("����� �ְ� ������ ->%d��<- �Դϴ�~~!", highscore);//�ְ����� ���
										}delay(2000);
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
			introducinggame();
			if (_getch() != NULL) {
				clrscr();
				break;
			}
		case 3:
			printrank();
			printf("1. ��ŷ����\n");//
			printf("2. ��ŷ�ʱ�ȭ\n");//
			break;
		case 4:
			int temp;
			clrscr();
			printf("���̵��� �����մϴ�. (�⺻ ���̵��� 2=4x4)\n"); //���̵�����
			printf("���ϴ� ���̵��� ���ڸ� �Է����ּ���.(1=2x2, 2=4x4, 3=6x6)\n");//���̵� ����
			printf("----> ");
			
			while (forwhileflag) {//���̵������� ����ó��
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


void InitGame()
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

	DrawScreen(TRUE);
	delay(2000);
	clrscr();
	DrawScreen(FALSE);
}

void DrawScreen(BOOL bHint)
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
	//���ӿ� ������ ����
	gotoxy(40, 4); printf("����� �̸� : %s",username);
	gotoxy(40, 6); puts("Ŀ��Ű:�̵�. ����:������. Esc:����");
	gotoxy(40, 8); printf("�� �õ� ȸ�� : %d", count);
	gotoxy(40, 10); printf("���� �� ã�� �� : %d ", GetRemain());
	gotoxy(40, 12); printf("Ʋ�� Ƚ�� : %d ", count-((difficulty*difficulty/2)-(GetRemain()/2)));

}

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
int searchdb(char *username)//�����г��� �Է½� ���ؼ� �ߺ����θ� ����ڿ��� Ȯ�����ְ� ������ ��Ͽ� ��������Ʈ�Ѵ�.
{
	userdb *tmp;
	tmp = head->next;
	while (strcmp(tmp->dbusername, username) != 0 && tmp != tail)
	{
		tmp = tmp->next;
	}
	if (tmp == tail) {
		return 0;
	}
	else {
		overlap = 1;//������ �ߺ����·� ����
		return 1;
	}
	
}
void printrank()
{
	userdb *tmp = head->next;    // tmp�� �Ӹ��� ���� ��� �ּҰ��� ����
	int ranknum = 1;
	while (tmp != tail)     // ����� �� ( �������� )
	{
		printf("���� : %d	�г��� : %10s	���� : %10d\n",ranknum++,tmp->dbusername,tmp->dbscore); // tmp�� ����Ű�� ��� �����͸� ���
		tmp = tmp->next;    // tmp �������� �̵�
	}
	puts("");


}
int inserttodb(char *username,int userscore) {
	userdb *tmp = head->next;//�������� �������� �Ӹ�����
	userdb *node = (userdb*)malloc(sizeof(userdb));//�� ��� �����Ҵ�
		strcpy(node->dbusername, username);//�� ��忡 �̸�����
		node->dbscore = userscore;//�� ��忡 ���� ����
	if (head->next == tail)//��尡 ������(�Ӹ� ������ ������)
	{
		//�Ӹ��� ���� ���̿� �� ��� ����
		head->next = node;
		node->prev = head;
		node->next = tail;
		tail->prev = node;
		return 0;
	}
	else
	{
		while (1)//���ѷ���
		{
			if (overlap == 0) {
				if (tmp->dbscore > node->dbscore)//������ ����� ������ �� �������� ũ��
				{
					node->next = tmp;       // �����ǳ�� �տ� ����� ����
					node->prev = tmp->prev;
					tmp->prev->next = node;
					tmp->prev = node;
					return 85;
				}
			}
		
			else {
				if (!strcmp(tmp->dbusername, node->dbusername)) { //���������� ������
					if (tmp->dbscore < node->dbscore) {//�������������� ���������� ��ũ��
						tmp->dbscore = node->dbscore;//������
						return tmp->dbscore;//�����Ѵ�.
					}
					else {
						
						return tmp->dbscore;//������ ��������ũ�� ���������� ����
					}
				}

			}
			

			if (tmp == tail)        // �� ����� ������ ���� ũ��
			{
				node->next = tail;      // ���� �տ� �� ��� ����
				node->prev = tail->prev;
				tail->prev->next = node;
				tail->prev = node;
				return 89;
			}
			tmp = tmp->next;        // ������ ��� ������ �̵�


		}
	}
}

void dbload()
{
	char tempname[20];
	int tempscore = 0;

	
	FILE *fpin;//���������ͺ����Ǽ���
	
	fpin = fopen("userdb.txt", "r");//userdb.txt ������ �о���δ�
	
	while (!feof(fpin)) //������ �� �о���϶����� while���� ����
	{
		userdb *node = (userdb*)malloc(sizeof(userdb)); // �� ��� �Ҵ�
		printf("�������");
		fscanf(fpin,"%s %d",tempname,&tempscore);
		printf("�������2");
		strcpy(node->dbusername, tempname);
		node->dbscore = tempscore;
			if (head->next == tail)     // ��尡 ������ ( �Ӹ������� ������ )
			{
				// �Ӹ��� ���� ���̿� �� ������
				head->next = node;
				node->prev = head;
				node->next = tail;
				tail->prev = node;
				
			}
			else         // �ٸ���尡 ������
			{
				tail->prev->next = node; // ���� �ٷ� ���� �� ��� �߰�
				node->prev = tail->prev;    // �� ��� ������ ������ �������� ( �������� ���� ���̿� �� ��� �� )
				node->next = tail;   // �� ��� ������ ����
				tail->prev = node;   // ���� ������ �� ���
				
			}
			//node = node->next;
	}

	fclose(fpin);//�������� file ��Ʈ���� ������
	printf("db�ε� ����\n");
}
void dbsave()//����db�� �����͸� ���������� �����Ѵ�
{
	userdb *tmp = head->next;//tmp�� �Ӹ��� ���� ��� �ּҰ��� ����
	
	FILE *fpout;//���������ͺ����Ǽ���
    fpout = fopen("userdb.txt", "w");//fpout ���Ϻ��� �ʱ�ȭ
	while (tmp != tail) {//����� ��(��������)
		fprintf(fpout, "%s %d", tmp->dbusername,tmp->dbscore);//����db�� ����Ѵ�
		tmp = tmp->next; //tmp �������� �̵�
	}
	fclose(fpout);//�������� file��Ʈ���� ������
	puts("db���� �Ϸ�\n");
}
void init() {
	head = (userdb*)malloc(sizeof(userdb)); // �Ӹ������Ҵ�
	tail = (userdb*)malloc(sizeof(userdb)); // ���������Ҵ�
	head->next = tail;      // �Ӹ������� ���� ����
	head->prev = head;      // �Ӹ������� �ڱ��ڽ� ����Ŵ
	tail->prev = head;      // ���������� �Ӹ�
	tail->next = tail;      // ���������� �ڱ��ڽ�

	

}
void menu() {
	printf("��¦ ���߱� ����1��\n");//ǥ�ÿ�
	printf("1. ���ӽ���\n");//���ӽ���while������������ ǥ������
	printf("2. ���ӼҰ�\n");//
	printf("3. ��ŷ\n");//
	printf("4. ���̵�����\n");
	printf("5. ����\n");
	printf("��ȣ�� �Է����ּ��� : ");
}
void introducinggame() {
	printf("¦ ã�� �����̶� 2x2, 4x4, 6x6 �� ���� ĭ�� ����ִ� �����ִ� ī���� ���ڰ� ���� ¦�� ã�� �����̴�. ������ ���� �׽�Ʈ �����̶�� �� �� �ִ�.\n������ �����ϸ� 2�ʰ� ���ڵ��� ��� ���� �ִ����� �����ִµ� ��ġ ��Ȳ�� �绡�� ����� �ξ�� �Ѵ�.��� �� ��� ���ڴ� ? �� �ٲ�µ� ���� �ִ� ������ ¦�� �ּ����� �õ��� ã�� ���� ������ ��ǥ�̴�.Ŀ�� �̵�Ű�� ����Ű�� ������ �����ϸ� Esc�� ������ ������ ����ȴ�.Ŀ��Ű�� ���ڵ� ���̸� �̵��ϸ� ����Ű�� ���ڸ� ���� ���� ¦�� �´� �� ���ڸ� �������� �����Ͽ� ������ ���ڽ��� ã�´�.Ʋ�� ���� �� Ʋ�ȴ����� 1�ʰ� �����ִµ� �̶� ������ ������ ��ġ�� �� ����� �δ� ���� �� ������ ����̴�.\n ");
	printf("\n\n�ƹ�Ű�� ������ ����ȭ������ ���ư��ϴ�");
}