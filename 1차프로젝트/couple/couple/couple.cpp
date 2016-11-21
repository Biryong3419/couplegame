#include <Turboc.h>//stdio, stdlib, conio, time, windows ������ �������� ������ϰ� ��ɵ��� �����س� ��������̴�.

#pragma comment(lib, "winmm.lib")         //BGM�� ���� ��ó����

#define LEFT 75 
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ESC 27

int difficulty = 4; // ���� ���̵� ��������
enum Status { HIDDEN, FLIP, TEMPFLIP };//ī�� ����������, ������������, �Ͻ������� �����������¸� �����Ѵ�.
struct tag_Cell //�� ĭ������ �����͸� �����ϴ� ����ü
{
	int Num;
	Status St;
};
struct userdb //���������� �����ϴ� ����ü(���߿��Ḯ��Ʈ)
{
	char dbusername[20];
	int dbscore;
	struct userdb* next;
	struct userdb* prev;

};
userdb *head; //�������� head(ó��)
userdb *tail; //�������� tail���(������)
tag_Cell arCell[6][6];//�Լ� �ִ�迭���� �����Ѵ�
					  //--------------��������
int sound = 1;               //���α׷� ������ ���� BGMƲ������
int startflag = 1;            //gameStart�Լ��� �ѹ��� �����ϱ� ����
int nx, ny;//xy��
int count;//ī���ÿ�
int score;//���� ��Ͽ� ����
int highscore;//�ְ��� ���������Q����
char username[20];//�����̸� �Ͻ�����
int gamecount = 0;//���� ���ھ� �Ͻ�����
int firstdatainsert = 0;//������ �� �������ִ��� �ƴϸ� ���� ó�� �����ϴ���.
int userscore;
int overlap; //db�� ����ִ� ���������ƴ���
			 //--------------�Լ�����
void printrank();//��ŷ����Ʈ ���(������)
void init();//���߿��Ḯ��Ʈ ��� �����Ҵ� �� �ʱ�ȭ(������)
int searchdb(char *username);//db�����Ϳ��� �ߺ��Ǵ� �г������ִ��� ã��(������)
void InitGame();//���ӽ���(����)
void dbsave();//���Ͽ� ������ ���(������);
void dbload();//�������κ��� ������ �ε�(������);
int inserttodb(char *username, int userscore);//���߿��Ḯ��Ʈ�� �����ͻ����Լ�(������)
void menu();//�޴� ����Լ�(������)
void DrawScreen(BOOL bHint);//ȭ��׸����Լ�(����)
void GetTempFlip(int *tx, int *ty);//��������ī���� ��ǥ�� �ݳ�(����)
int GetRemain();//���� ī��� ���ϱ�(����)
void introducinggame();//���ӼҰ�(������);
void gameStart(); //���ӽ��۽� ���÷��� �κ�(���)

void main()
{

	init(); //�����Ҵ��ʱ�ȭ
	int ch;//���ӽ��۽� �Է¹޴� ����   
	int tx, ty;//
	int SwitchForRank;//��ŷ ����ġ���� ���� ����
	int whileflag = 1; // ��ü���� while���� ���� �÷��װ�
	char y_1;//y�� �Է¹ޱ� ���� ����
	char y_2;//y�� �Է¹ޱ� ���� ����2
	int gameflag = 1;//���ӽ��� �ݺ�while���� ���� �÷��װ�
	int setflag; // ���� �޴� case���� ���� �÷��װ�
	int forwhileflag = 1; //���� case���� ���� �÷��װ�
	int userwhileflag = 1;//user������ ������ while���� ���� �÷��װ�

	dbload();//db�ε�

	gameStart(); //BGM����� ó��ȭ���� �ٹ� �Լ�

	while (whileflag) {   //1�� while��
		menu();//�ʱ� �޴� ���
		scanf("%d", &setflag);
		switch (setflag) {//�޴� switch ��
		case 1:       //���� ����
			clrscr(); //������ ���θ޴�ȭ�� ����� ��������
			userwhileflag = 1;
			gamecount = 0;
			overlap = 0;//�������� �ߺ����� ���� 0���� �ʱ�ȭ
			while (userwhileflag) {//�г��� �ߺ�Ȯ�� �� ���������� ���� �ݺ���
				printf("�г����� �Է��ϼ���\n");
				printf("����� �г����� : ");

				scanf("%s", username);//�����г��� �Է¹޴´�.
				if (searchdb(username) == 1) {
					clrscr();
					printf("%s �� ��ŷ�����Ϳ� �̹� �����ϴ� ������Դϴ�. ����Ͻ÷��� Y�� �Է����ּ��� : ", username);

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
			gameflag = 1;//������ �ѱ�� �����


			while (gameflag) {//2�� while��
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

					case ESC: // esc������ �� �ݺ��� ������Բ� 1��while������ ���ư��� ����ȭ�� �����
						clrscr();
						printf("������ ����˴ϴ�. ����Ͻ÷��� (Y/N) �����ּ��� : ");
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
										DrawScreen(FALSE);//drawscreen �Լ�ȣ��

										score = 100 - (count - ((difficulty*difficulty / 2) - (GetRemain() / 2))) * 2;//����

										highscore = inserttodb(username, score);//2�߿��Ḯ��Ʈ�� �����ͻ���

										if (score < 0) {//���ھ 0������ ���������ʵ��� ���ǹ����
											gotoxy(50, 12);   printf("����� ������ -> 0��<- �Դϴ�~~!");
										}
										else {
											gotoxy(50, 12); printf("����� ������      ->%d��<- �Դϴ�~~!", score);//���ھ����
											gotoxy(50, 14); printf("����� �ְ� ������ ->%d��<- �Դϴ�~~!", highscore);//�ְ����� ���
											delay(1000);
											gotoxy(50, 16); puts("�����մϴ�. �ٽ� �����մϴ�.");
										}delay(1000);
										dbsave();//��������
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
			introducinggame();//���� �Ұ�
			if (_getch() != NULL) {
				clrscr();
				break;
			}
		case 3:
			clrscr();
			printf("1. ��ŷ����\n");//
			printf("2. ��ŷ�ʱ�ȭ\n");//
			scanf("%d", &SwitchForRank);
			switch (SwitchForRank) {
			case 1:
				printrank();
				break;
			case 2:
				head->next = tail; //����带 ���������� �̾������ν� �ʱ�ȭ
				dbsave();
				printf("�ʱ�ȭ �Ϸ�");
				delay(500);
				clrscr();
				break;
			}
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
			//���̵��� ���� �뷡���
			if (temp == 1)
				PlaySound(TEXT("Sin.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			else if (temp == 2)
				PlaySound(TEXT("RockinNightStyle.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			else
				PlaySound(TEXT("Nightmare.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			break;
		case 5:
			whileflag = 0;//�ݺ��� ����
			break;
		}

	}

}

//�������� �ʱ�ȭ �ϴ� �Լ�
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

//ȭ���� �׸��� �Լ�
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
				printf("��������");
				gotoxy(s - 2, g);
				printf("��%2d  ��", arCell[x][y].Num);
				gotoxy(s - 2, g + 1);
				printf("��������");
			}
			else if (arCell[x][y].St == TEMPFLIP) {
				s = x * 8 + 2;
				g = y * 3 + 2;
				gotoxy(s - 2, g - 1);
				printf("��    ��");
				gotoxy(s - 2, g);
				printf("  %2d   ", arCell[x][y].Num);
				gotoxy(s - 2, g + 1);
				printf("��    ��");
			}
			else {
				s = x * 8 + 2;
				g = y * 3 + 2;
				gotoxy(s - 2, g - 1);
				printf("��������");
				gotoxy(s - 2, g);
				printf("�� �� ��");
				gotoxy(s - 2, g + 1);
				printf("��������");
			}
		}
	}

	gotoxy(50, 2); printf("����� �̸� : %s", username);
	gotoxy(50, 4); puts("Ŀ��Ű:�̵�. ����:������. Esc:����");
	gotoxy(50, 6); printf("�� �õ� ȸ�� : %d", count);
	gotoxy(50, 8); printf("���� �� ã�� �� : %d ", GetRemain());
	gotoxy(50, 10); printf("Ʋ�� Ƚ�� : %d ", count - ((difficulty*difficulty / 2) - (GetRemain() / 2)));

}

//�ӽ÷� �������� ĭ�� ��ȣ�� ����
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

//���� ã������ ĭ�� ������ ����
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
	while (strcmp(tmp->dbusername, username) != 0 && tmp != tail) //
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
	clrscr();
	userdb *tmp = head->next;    // tmp�� �Ӹ��� ���� ��� �ּҰ��� ����
	int ranknum = 1;
	char error = '��';

	if (tmp->dbusername[0] == error&&gamecount == 0) {
		printf("������ �������� �ʽ��ϴ�.");
		if (_getch() != NULL) {
			clrscr();
		}
	}
	else
	{
		if (firstdatainsert == 1) {
			tmp = tmp->next;

		}
		while (tmp != tail)     // ����� �� ( �������� )
		{

			printf("���� : %d   �г��� : %10s   ���� : %10d\n", ranknum++, tmp->dbusername, tmp->dbscore); // tmp�� ����Ű�� ��� �����͸� ���
			tmp = tmp->next;    // tmp �������� �̵�
		}
	}

	if (_getch() != NULL) {
		clrscr();

	}



}
int inserttodb(char *username, int userscore) {
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
		if (overlap == 0)
		{
			while (1)//���ѷ���
			{

				if (tmp->dbscore < node->dbscore)//������ ����� ������ �� �������� ũ��
				{

					node->next = tmp;       // �����ǳ�� �տ� ����� ����
					node->prev = tmp->prev;
					tmp->prev->next = node;
					tmp->prev = node;
					return userscore;
				}
				if (tmp == tail)        // �� ����� ������ ���� ũ��
				{

					node->next = tail;      // ���� �տ� �� ��� ����
					node->prev = tail->prev;
					tail->prev->next = node;
					tail->prev = node;
					return userscore;
				}
				tmp = tmp->next;        // ������ ��� ������ �̵�
			}
		}
		else
		{
			while (1)
			{

				if (!strcmp(tmp->dbusername, node->dbusername))  //���������� ������
				{
					if (tmp->dbscore < node->dbscore) //�������������� ���������� ��ũ��
					{
						tmp->dbscore = node->dbscore;//������
						return tmp->dbscore;//�����Ѵ�.
					}
					else
					{
						return tmp->dbscore;//������ ��������ũ�� ���������� ����
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
	char error_1 = '��';

	FILE *fpin;//���������ͺ����Ǽ���

	fpin = fopen("userdb.txt", "r");//userdb.txt ������ �о���δ�

	while (!feof(fpin)) //������ �� �о���϶����� while���� ����
	{
		userdb *node = (userdb*)malloc(sizeof(userdb)); // �� ��� �Ҵ�

		fscanf(fpin, "%s %d", tempname, &tempscore);
		if (tempname[0] == error_1)
		{

			firstdatainsert = 1;
		}
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
}
void dbsave()//����db�� �����͸� ���������� �����Ѵ�
{
	char error_1 = '��';

	userdb *tmp = head->next;//tmp�� �Ӹ��� ���� ��� �ּҰ��� ����

	FILE *fpout;//���������ͺ����Ǽ���
	fpout = fopen("userdb.txt", "w");//fpout ���Ϻ��� �ʱ�ȭ
	while (tmp != tail) {//����� ��(��������)

		if (tmp->dbusername[0] != error_1) {

			if (tmp->next != tail) {

				fprintf(fpout, "%s %d\n", tmp->dbusername, tmp->dbscore);//����db�� ����Ѵ�

			}
			else {

				fprintf(fpout, "%s %d", tmp->dbusername, tmp->dbscore);
			}
		}

		tmp = tmp->next; //tmp �������� �̵�
	}
	fclose(fpout);//�������� file��Ʈ���� ������

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
	printf("��¦ ���߱� ���ӡ�\n");//ǥ�ÿ�
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

void gameStart()
{
	system("title ī�� ¦ ���߱�");            //���α׷� ����
	system("mode con:cols=90 lines=20");      //�ܼ� â ũ�⺯��

											  //ó�� ���α׷� ���� �� BGM���
	if (sound == 1)
	{
		PlaySound(TEXT("OBLIVION.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		sound = 0;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);      //��Ʈ ���󺯰�
	int delayNum = 1;      //������ ��������
	if (startflag == 1)
	{
		//1,2��° ��
		for (int j = 0; j < 2; j++)
		{
			for (int i = 0; i < 44; i++)
			{
				printf("��");
				delay(delayNum);
			}
			printf("\n");
		}

		//3��° ��
		for (int i = 0; i < 22; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 21; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("\n");

		//4��° ��
		for (int i = 0; i < 22; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 4; i++)
		{
			printf("��");
			delay(delayNum);
		}
		for (int i = 0; i < 12; i++)
		{
			printf("��");
			delay(delayNum);
		}
		for (int i = 0; i < 5; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("\n");

		//5��° ��
		for (int j = 0; j < 2; j++)
		{
			for (int i = 0; i < 2; i++)
			{
				printf("��");
				delay(delayNum);
			}
			for (int i = 0; i < 8; i++)
			{
				printf("��");
				delay(delayNum);
			}
		}
		for (int i = 0; i < 2; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 9; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 5; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 5; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("\n");

		//6��° ��
		for (int i = 0; i < 5; i++)
		{
			printf("��");
			delay(delayNum);
		}
		for (int i = 0; i < 2; i++)
		{
			printf("��");
			delay(delayNum);
		}
		for (int i = 0; i < 8; i++)
		{
			printf("��");
			delay(delayNum);
		}
		for (int i = 0; i < 2; i++)
		{
			printf("��");
			delay(delayNum);
		}
		for (int i = 0; i < 5; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 9; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 5; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 5; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("\n");

		//7��° ��
		for (int i = 0; i < 4; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 2; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 6; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 2; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 4; i++)
		{
			printf("��");
			delay(delayNum);
		}
		for (int i = 0; i < 3; i++)
		{
			printf("��");
			delay(delayNum);
		}
		for (int i = 0; i < 7; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 5; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 5; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("\n");

		//8��° ��
		for (int i = 0; i < 3; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 4; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 4; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 4; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 3; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 9; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 5; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 5; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("\n");

		//9��°��
		for (int i = 0; i < 2; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 6; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 2; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 6; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 2; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 9; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 5; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 5; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("\n");



		//10��°��
		printf("��");
		delay(delayNum);
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 8; i++)
		{
			printf("��");
			delay(delayNum);
		}
		for (int i = 0; i < 2; i++)
		{
			printf("��");
			delay(delayNum);
		}
		for (int i = 0; i < 8; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		printf("��");
		delay(delayNum);
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 3; i++)
		{
			printf("��");
			delay(delayNum);
		}
		for (int i = 0; i < 16; i++)
		{
			printf("��");
			delay(delayNum);
		}
		for (int i = 0; i < 2; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("\n");

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		//11��° ��
		for (int i = 0; i < 44; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("\n");

		//12��° ��
		for (int i = 0; i < 8; i++)
		{
			printf("��");
			delay(delayNum);
		}
		for (int i = 0; i < 15; i++)
		{
			printf("��");
			delay(delayNum);
		}
		for (int i = 0; i < 11; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 9; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("\n");

		//13��° ��
		for (int i = 0; i < 22; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 11; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 9; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("\n");

		//14��° ��
		for (int i = 0; i < 22; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 8; i++)
		{
			printf("��");
			delay(delayNum);
		}
		for (int i = 0; i < 7; i++)
		{
			printf("��");
			delay(delayNum);
		}
		for (int i = 0; i < 6; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("\n");

		//15��° ��
		for (int i = 0; i < 22; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 7; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 7; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 5; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("\n");

		//16��° ��
		for (int i = 0; i < 22; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 6; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 9; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 4; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("\n");

		//17��° �� = 15��° ��
		for (int i = 0; i < 22; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 7; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 7; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 5; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("\n");

		//18��° �� = 14��° ��
		for (int i = 0; i < 22; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("��");
		delay(delayNum);
		for (int i = 0; i < 8; i++)
		{
			printf("��");
			delay(delayNum);
		}
		for (int i = 0; i < 7; i++)
		{
			printf("��");
			delay(delayNum);
		}
		for (int i = 0; i < 6; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("\n");

		//19, 20 ��° ��
		for (int i = 0; i < 44; i++)
		{
			printf("��");
			delay(delayNum);
		}
		printf("\n");
		for (int i = 0; i < 44; i++)
		{
			printf("��");
			delay(delayNum);
		}
		delay(1000);


		//����ٲ�ȿ���ֱ�
		for (int i = 15; i >= 1; i--)
		{
			gotoxy(0, 0);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i);
			//printf("�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�\n�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�\n�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ١ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�\n�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ١ڡڡڡڡ١١١١١١١١١١١١ڡڡڡڡ�\n�ڡڡ١١١١١١١١ڡڡ١١١١١١١١ڡڡ١ڡڡڡڡڡڡڡڡڡ١ڡڡڡڡڡ١ڡڡڡڡ�\n�ڡڡڡڡڡ١١ڡڡڡڡڡڡڡڡ١١ڡڡڡڡڡ١ڡڡڡڡڡڡڡڡڡ١ڡڡڡڡڡ١ڡڡڡڡ�\n�ڡڡڡڡ١ڡڡ١ڡڡڡڡڡڡ١ڡڡ١ڡڡڡڡ١١١ڡڡڡڡڡڡڡ١ڡڡڡڡڡ١ڡڡڡڡ�\n�ڡڡڡ١ڡڡڡڡ١ڡڡڡڡ١ڡڡڡڡ١ڡڡڡ١ڡڡڡڡڡڡڡڡڡ١ڡڡڡڡڡ١ڡڡڡڡ�\n�ڡڡ١ڡڡڡڡڡڡ١ڡڡ١ڡڡڡڡڡڡ١ڡڡ١ڡڡڡڡڡڡڡڡڡ١ڡڡڡڡڡ١ڡڡڡڡ�\n�ڡ١ڡڡڡڡڡڡڡڡ١١ڡڡڡڡڡڡڡڡ١ڡ١ڡڡڡ١١١١١١١١١١١١١١١١ڡ�\n�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ١ڡڡڡڡڡڡڡڡ�\n�ڡڡڡڡڡڡڡڡ١١١١١١١١١١١١١١١ڡڡڡڡڡڡڡڡڡڡڡ١ڡڡڡڡڡڡڡڡ�\n�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ١ڡڡڡڡڡڡڡڡڡڡڡ١ڡڡڡڡڡڡڡڡ�\n�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ١ڡڡڡڡڡڡڡڡ١١١١١١١ڡڡڡڡڡ�\n�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ١ڡڡڡڡڡڡڡ١ڡڡڡڡڡڡڡ١ڡڡڡڡ�\n�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ١ڡڡڡڡڡڡ١ڡڡڡڡڡڡڡڡڡ١ڡڡڡ�\n�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ١ڡڡڡڡڡڡڡ١ڡڡڡڡڡڡڡ١ڡڡڡڡ�\n�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ١ڡڡڡڡڡڡڡڡ١١١١١١١ڡڡڡڡڡ�\n�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�\n�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�");
			printf("�١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١�\n�١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١�\n�١١١١١١١١١١١١١١١١١١١١١١ڡ١١١١١١١١١١١١١١١١١١١١�\n�١١١١١١١١١١١١١١١١١١١١١١ڡ١١١١ڡڡڡڡڡڡڡڡڡڡڡڡ١١١١�\n�١١ڡڡڡڡڡڡڡڡ١١ڡڡڡڡڡڡڡڡ١١ڡ١١١١١١١١١ڡ١١١١١ڡ١١١١�\n�١١١١١ڡڡ١١١١١١١١ڡڡ١١١١١ڡ١١١١١١١١١ڡ١١١١١ڡ١١١١�\n�١١١١ڡ١١ڡ١١١١١١ڡ١١ڡ١١١١ڡڡڡ١١١١١١١ڡ١١١١١ڡ١١١١�\n�١١١ڡ١١١١ڡ١١١١ڡ١١١١ڡ١١١ڡ١١١١١١١١١ڡ١١١١١ڡ١١١١�\n�١١ڡ١١١١١١ڡ١١ڡ١١١١١١ڡ١١ڡ١١١١١١١١١ڡ١١١١١ڡ١١١١�\n�١ڡ١١١١١١١١ڡڡ١١١١١١١١ڡ١ڡ١١١ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ١�\n�١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١ڡ١١١١١١١١�\n�١١١١١١١١ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ١١١١١١١١١١١ڡ١١١١١١١١�\n�١١١١١١١١١١١١١١١١١١١١١١ڡ١١١١١١١١١١١ڡ١١١١١١١١�\n�١١١١١١١١١١١١١١١١١١١١١١ڡ١١١١١١١١ڡڡڡڡڡڡڡ١١١١١�\n�١١١١١١١١١١١١١١١١١١١١١١ڡ١١١١١١١ڡ١١١١١١١ڡ١١١١�\n�١١١١١١١١١١١١١١١١١١١١١١ڡ١١١١١١ڡ١١١١١١١١١ڡ١١١�\n�١١١١١١١١١١١١١١١١١١١١١١ڡ١١١١١١١ڡ١١١١١١١ڡ١١١١�\n�١١١١١١١١١١١١١١١١١١١١١١ڡ١١١١١١١١ڡڡڡڡڡڡڡ١١١١١�\n�١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١�\n�١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١١�");
			delay(150);
		}

		//���ӽ����� ��Ʈ������
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);


		startflag = 0; //�ٽõ��� �ʰ� 0���� �ٲ���
		delay(1000);
		clrscr();
	}
}