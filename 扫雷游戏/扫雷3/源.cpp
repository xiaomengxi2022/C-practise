#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <graphics.h>
#include<mmsystem.h> //������ý���豸�ӿ�ͷ�ļ�
#pragma comment(lib,"winmm.lib")
int grahight = 415;//ʵ�ʻ�����
int grawidth = 340;//ʵ�ʻ�����
#define allhight 100//�������С
#define allwidth 100//�������С
#define ROWS 102
#define COLS 102
//�м�17*17��415*340������10*10��275*200���߼�20*20��475*400
int hight = 17;//ʵ����Ϸ�����С
int width = 17;//ʵ����Ϸ�����С
int time1 = 0, time2 = 0, time3 = 0;//��¼ʱ�����λ
int first = 1;//�жϵ�һ�ε��
int bomb = 45;//ը������
int runing = 1;//�Ƿ������Ϸ
int smiles = 0;//ͼ�εı仯
int ture = 1;//�ж��Ƿ�ʼ��Ϸ
int a = 1;//������һֱ����ʤ���Ŀ�
int tem = 0;
int map[allhight + 2][allwidth + 2] = { {0} };//����ս�����������к����������������ҳ���Χը����
clock_t start_t, end_t;//��ʼʵ��ʱ�䣬����ʱ����ʱ���
IMAGE block[13];//1-8�Ϳո��ը��ͼƬ
IMAGE back[3];//�˵�����
IMAGE smile[4];//ͼ�εĸ���״̬
IMAGE numbomb[10];//ʱ��ͼƬ
int numbombs = 0;//ը����һλ��
int numbombss = 0;//ը���ڶ�λ��
int win = 0;//�ж��Ƿ�Ӯ
int choice = 0;//���û��Ƿ�ѡ��
void printfmap();//��ӡ��ͼ
void updategame();//�������ݲ��Ҽ���
int findbomb(int a, int b);//���������鸽����ը������
void theloadimage();//����ͼƬ
void playgame();//��ȡ�û��ĵ��
void newgame();//ˢ����Ϸ
void whitchtouch2();//ѡ�� �ѶȽ��溯��
void showbomb();//���ը������ʾ����ը��
void openzero(int m, int n);//�����ո�򿪣��ݹ�˼��
void whitchtouth();//��ʼ�����ѡ����
void save_map(int map[][COLS], int rows);
void load_map(int map[][COLS], int rows);
void printfmaps()
{
	for (int i = 1; i <= hight; i++)
	{
		printf("��%d�У�", i);
		for (int j = 1; j <= width; j++)
		{

			int a = (j - 1) * 20;
			int b = (i - 1) * 20 + 72;
			printf("%3d", map[i][j]);

		}
		printf("\n");
		
	}
}
//����ͨ������̨�������֪���׵�λ��
void before();
int main()
{
	HWND hwnd = initgraph(grawidth, grahight);
	setbkcolor(WHITE);
	cleardevice();
	theloadimage();
	putimage(0, 0, &back[0]);
	mciSendString(L"open ../����/�˵�.mp3 alias BGM", 0, 0, 0);
	mciSendString(L"setaudio BGM volume to 140", 0, 0, 0);//����������С
	mciSendString(L"play BGM repeat", 0, 0, 0);
	//׼������
	while (ture)
	{
		settextcolor(BLACK);
		setbkmode(TRANSPARENT);
		outtextxy(130, 90, L"����Ϸ");
		outtextxy(130, 150, L"������Ϸ");
		outtextxy(130, 210, L"�Ѷ�����");
		outtextxy(15, 280, L"��Ϸ˵��:");
		outtextxy(90, 280, L"����м�ͼ�ο������¿�ʼ����Ϸ");
		outtextxy(90, 305, L"Ĭ���м��Ѷȣ�����ѡ���Ѷ�ֱ��");
		outtextxy(90, 330, L"��ʼ��Ϸ��������Ϸ���Լ����ϴ�");
		outtextxy(90, 355, L"��Ϸ�ļ�¼����ʱ�����õ������");
		outtextxy(90, 380, L"�ص��˵�����");
		whitchtouth();

	}
	cleardevice();
	putimage(0, 0, &back[0]);
	while (choice)
	{
		settextcolor(BLACK);
		setbkmode(TRANSPARENT);
		outtextxy(130, 80, L"����");
		outtextxy(130, 180, L"�м�");
		outtextxy(130, 280, L"�߼�");
		whitchtouch2();
	}
	initgraph(grawidth, grahight,1);
	setbkcolor(WHITE);
	cleardevice();
	//��ʼ
	numbombss = bomb % 10;
	numbombs = bomb / 10;
	updategame();
	system("cls");
	printfmaps();
	while (1)
	{
		printfmap();
		playgame();
		if (win == hight * width - bomb && a)
		{
			a = 0;
			smiles = 3;
			printfmap();
			PlaySound(_T("../��Ч/ʤ��.wav"), NULL, SND_ASYNC);
			MessageBox(hwnd, L"��ϲ���������׳ɹ�~", L"", MB_OK);
		}
		if (smiles == 1)//����м�ͼ�ξͻ����¿�ʼ��Ϸ
		{
			first = 1;
			a = 1;
			win = 0;
			system("cls");
			newgame();
			printfmaps();
		}
		end_t = clock();
		int q = (end_t - start_t) / 1000;//ʱ���
		
		if (runing == 1 && first == 0 && win != hight * width - bomb)
		{
			time1 = q / 100;
			time2 = q / 10 % 10;
			time3 = q % 10;
		}
		if (first == 1)
		{
			time1 = time2 = time3 = 0;
		}
		
	}
	
	closegraph();
	return 0;
}
void printfmap()
{
	settextcolor(BLACK);
	outtextxy(grawidth / 2 - 25, 0, L"����");
	putimage(grawidth / 2 - 20, 30, &smile[smiles]);//�м�ͼ��
	putimage(10, 10, &numbomb[numbombs]);//������¸�������ʾը������ͼƬ
	putimage(40, 10, &numbomb[numbombss]);
	putimage(grawidth - 90, 10, &numbomb[time1]);//������������ʱ��ͼƬ
	putimage(grawidth - 60, 10, &numbomb[time2]);
	putimage(grawidth - 30, 10, &numbomb[time3]);
	for (int i = 1; i <= hight; i++)
	{
		for (int j = 1; j <= width; j++)
		{

			int a = (j - 1) * 20;
			int b = (i - 1) * 20 + 72;
			if (map[i][j] <= 28 && map[i][j] >= 19)//19-28��ʾû�㿪�����Զ��Ƿ���ͼƬ�������ΪʲôҪ��20��ԭ��
				putimage(a, b, &block[0]);
			else if (map[i][j] <= 8 && map[i][j] >= -1)
			{
				switch (map[i][j])
				{
				case -1:
					putimage(a, b, &block[9]);
					break;
				case 0:
					putimage(a, b, &block[10]);
					break;
				case 1:
					putimage(a, b, &block[1]);
					break;
				case 2:
					putimage(a, b, &block[2]);
					break;
				case 3:
					putimage(a, b, &block[3]);
					break;
				case 4:
					putimage(a, b, &block[4]);
					break;
				case 5:
					putimage(a, b, &block[5]);
					break;
				case 6:
					putimage(a, b, &block[6]);
					break;
				case 7:
					putimage(a, b, &block[7]);
					break;
				case 8:
					putimage(a, b, &block[8]);
					break;
				}
			}//
			//�������if���������û����֮���ȥ20�ͱ��-1��8����ʾ����ӦͼƬ
			else if (map[i][j] <= 48 && map[i][j] >= 39)//�Ҽ���20Ȼ�󣬾���39��48֮�䣬��Щ��������ͼ��
				putimage(a, b, &block[11]);
			else if (map[i][j] == -100)//���ǵ㵽��ը��Ҫ���
				putimage(a, b, &block[12]);

		}

	}
}
void updategame()
{
	srand((unsigned int)time(NULL));
	int n = 0;
	while (n < bomb)
	{
		int x = rand() % hight + 1;
		int y = rand() % width + 1;
		if (map[x][y] == 0 && x != 0 && y != 0)
		{
			map[x][y] = -1;
			n++;
		}
	}
	//���õ���
	for (int i = 1; i <= hight; i++)
	{
		for (int j = 1; j <= width; j++)
		{
			if (map[i][j] == 0)
			{
				int sign = 0;
				sign = findbomb(i, j);
				map[i][j] = sign;
			}
		}
	}
	//���Ҳ����׵�������Χ������
	for (int i = 1; i <= hight; i++)
	{
		for (int j = 1; j <= width; j++)
		{
			map[i][j] += 20;
		}
	}
	//����

}
int findbomb(int a, int b)
{
	int all = 0;
	for (int i = a - 1; i <= a + 1; i++)
	{
		for (int j = b - 1; j <= b + 1; j++)
		{
			if (map[i][j] == -1)
			{
				all++;
			}
		}
	}
	return all;
}
void theloadimage()
{
	//����ͼƬ
	loadimage(&back[0], L"../img/����.png", 340, 415);
	loadimage(&block[0], L"../img/��.png", 20, 20);
	loadimage(&block[1], L"../img/1.png", 20, 20);
	loadimage(&block[2], L"../img/2.png", 20, 20);
	loadimage(&block[3], L"../img/3.png", 20, 20);
	loadimage(&block[4], L"../img/4.png", 20, 20);
	loadimage(&block[5], L"../img/5.png", 20, 20);
	loadimage(&block[6], L"../img/6.png", 20, 20);
	loadimage(&block[7], L"../img/7.png", 20, 20);
	loadimage(&block[8], L"../img/8.png", 20, 20);
	loadimage(&block[9], L"../img/��.png", 20, 20);
	loadimage(&block[10], L"../img/����.png", 20, 20);
	loadimage(&block[11], L"../img/���.png", 20, 20);
	loadimage(&block[12], L"../img/����.png", 20, 20);
	loadimage(&smile[0], L"../img/Ц��.png", 30, 30);
	loadimage(&smile[1], L"../img/Ц��2.png", 30, 30);
	loadimage(&smile[2], L"../img/dead.png", 30, 30);
	loadimage(&smile[3], L"../img/cool.png", 30, 30);
	loadimage(&numbomb[0], L"../img/00.png", 30, 50);
	loadimage(&numbomb[1], L"../img/11.png", 30, 50);
	loadimage(&numbomb[2], L"../img/22.png", 30, 50);
	loadimage(&numbomb[3], L"../img/33.png", 30, 50);
	loadimage(&numbomb[4], L"../img/44.png", 30, 50);
	loadimage(&numbomb[5], L"../img/55.png", 30, 50);
	loadimage(&numbomb[6], L"../img/66.png", 30, 50);
	loadimage(&numbomb[7], L"../img/77.png", 30, 50);
	loadimage(&numbomb[8], L"../img/88.png", 30, 50);
	loadimage(&numbomb[9], L"../img/99.png", 30, 50);

}
void playgame()
{
	//��ȡ�û����������
	if (MouseHit())
	{
		MOUSEMSG msg = GetMouseMsg();
		int x = msg.x;
		int y = msg.y;
		switch (msg.uMsg)
		{
		case  WM_LBUTTONDOWN:
			PlaySound(_T("../��Ч/����.wav"), NULL, SND_ASYNC);
			if (x >= 0 && x <= grawidth && y >= 72 && y <= grahight && runing && smiles != 3)
			{
				if (first == 1)//�ж���һ�ε����ʼ��ʱ
				{
					start_t = clock();
					first = 0;
				}
				x = x / 20 + 1;
				y = (y - 72) / 20 + 1;
				if (map[y][x] <= 28 && map[y][x] >= 19)
				{
					if (map[y][x] == 20)//����㿪����0��Ҫչ����Χ8����8������0�ͻ�Ҫչ����ͬʱ��֤�Լ�չ���Ĳ���ը��
					{
						openzero(y, x);
					}
					else
					{
						map[y][x] -= 20;//�����ľ�ֱ��չ��
						win++;
						if (map[y][x] == -1)
							win--;//��ֹ���һ�����ױ������Ӯ������ը��
					}
				}
				if (map[y][x] == -1)
				{
					PlaySound(_T("../��Ч/ʧ��.wav"), NULL, SND_ASYNC);
					map[y][x] = -100;//�㵽��ը��Ҫ��죬���Ե�������һ�����֣���չʾ����ͼƬ
					showbomb();//�㵽ը��Ҫ��ը��ȫ��չ��
					smiles = 2;//���˵Ļ��ͱ����ɫͼ��
					printfmap();
					runing = 0;//���˾Ͳ��ܵ����Ϸ��

				}
				
			}
			else if (x >= grawidth / 2 - 20 && x <= grawidth / 2 + 10 && y >= 20 && y <= 55)
			{
				smiles = 1;
				runing = 1;
			}
			else if (x >= grawidth / 2 - 25 && x <= grawidth / 2 + 10 && y >= 0 && y <= 19)
			{
				before();
			}
			break;
		case WM_LBUTTONUP:
			if (runing == 1 && win != hight * width - bomb)
				smiles = 0;//������Ӯ�͵������������ɫͼƬ
			break;
		case WM_RBUTTONDOWN:
			PlaySound(_T("../��Ч/���.wav"), NULL, SND_ASYNC);
			if (x >= 0 && x <= grawidth && y >= 72 && y <= grahight && runing == 1)//�Ҽ�����
			{
				if (first == 1)
				{
					start_t = clock();
					first = 0;
				}
				x = x / 20 + 1;
				y = (y - 72) / 20 + 1;
				if (map[y][x] <= 28 && map[y][x] >= 19)
				{
					map[y][x] += 20;

				}
				else if (map[y][x] <= 48 && map[y][x] >= 39)
				{
					map[y][x] -= 20;
				}
			}
			break;

		}
		
	}
	save_map(map, ROWS);//�����ͼ
	
}
void openzero(int m, int n)
{
	map[m][n] -= 20;//�򿪱���
	win++;
	for (int i = m - 1; i <= m + 1; i++)
	{
		for (int j = n - 1; j <= n + 1; j++)
		{
			if (i >= 1 && i <= width && j >= 1 && j <= hight)//��֤����Ϸ��
			{
				if (map[i][j] <= 28 && map[i][j] >= 19)//��֤û�з���
				{
					if (map[i][j] != 20)//��֤����0
					{
						map[i][j] -= 20;
						win++;
					}
					else
						openzero(i, j);//�ݹ�

				}
			}
		}
	}
}
void newgame()
{
	for (int i = 1; i <= hight; i++)
	{
		for (int j = 1; j <= width; j++)
		{
			map[i][j] = 0;
		}

	}
	updategame();

}
void whitchtouth()
{
	MOUSEMSG msg = GetMouseMsg();
	int x = msg.x;
	int y = msg.y;
	switch (msg.uMsg)
	{
	case  WM_LBUTTONDOWN:
		PlaySound(_T("../��Ч/����.wav"), NULL, SND_ASYNC);
		if (x >= 80 && x <= 250 && y >= 50 && y <= 120)
		{
			ture = 0;
		}
		else if (x >= 80 && x <= 250 && y >= 130 && y <= 170)
		{
			HWND hwnd = initgraph(grawidth, grahight,1);
			setbkcolor(WHITE);
			cleardevice();
			load_map(map, ROWS);
			ture = 0;
			tem = 1;
			printfmaps();
			while (tem)
			{
				printfmap();
				playgame();
				if (win == hight * width - bomb && a)
				{
					a = 0;
					smiles = 3;
					printfmap();
					PlaySound(_T("../��Ч/ʤ��.wav"), NULL, SND_ASYNC);
					MessageBox(hwnd, L"��ϲ���������׳ɹ�~", L"", MB_OK);
				}
				if (smiles == 1)//���Ц���ͻ����¿�ʼ��Ϸ
				{
					first = 1;
					a = 1;
					win = 0;
					system("cls");
					newgame();
					printfmaps();
				}
				end_t = clock();
				int q = (end_t - start_t ) / 1000;
				if (runing == 1 && first == 0 && win != hight * width - bomb)
				{
					time1 = q / 100;
					time2 = q / 10 % 10;
					time3 = q % 10;
				}
				if (first == 1)
				{
					time1 = time2 = time3 = 0;
				}

			}
			tem = 0;
		}
		
		else if (x >= 80 && x <= 250 && y >= 190 && y <= 230)
		{
			ture = 0;
			choice = 1;
		}
		break;

	}
}
void showbomb()
{
	int i, j;
	for (i = 1; i <= hight; i++)
	{
		for (j = 1; j <= width; j++)
		{
			if (map[i][j] == 19)
			{
				map[i][j] -= 20;
			}
		}
	}
}
void whitchtouch2()
{
	MOUSEMSG msg = GetMouseMsg();
	int x = msg.x;
	int y = msg.y;
	switch (msg.uMsg)
	{
	case  WM_LBUTTONDOWN:
		PlaySound(_T("../��Ч/����.wav"), NULL, SND_ASYNC);
		if (x >= 80 && x <= 250 && y >= 50 && y <= 120)
		{
			choice = 0;
			cleardevice();
			hight = 10;
			width = 10;
			grahight = 275;
			grawidth = 200;
			bomb = 15;

		}
		else if (x >= 80 && x <= 250 && y >= 150 && y <= 220)
		{
			cleardevice();
			choice = 0;
			hight = 17;
			width = 17;
			grahight = 415;
			grawidth = 340;
			bomb = 45;
		}
		else if (x >= 80 && x <= 250 && y >= 250 && y <= 320)
		{
			cleardevice();
			choice = 0;
			hight = 20;
			width = 20;
			grahight = 475;
			grawidth = 400;
			bomb = 60;
		}
		break;

	}
}
void before()
{
	for (int i = 1; i <= hight; i++)
	{
		for (int j = 1; j <= width; j++)
		{
			map[i][j] = 0;
		}

	}
	ture = 1;//���ò���
	choice = 0;
	first = 1;
	smiles = 0;
	runing = 1;
	win = 0;
	a = 1;
	system("cls");
	initgraph(340, 415);
	setbkcolor(WHITE);
	cleardevice();
	putimage(0, 0, &back[0]);
	//׼������
	while (ture)
	{
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		outtextxy(130, 90, L"����Ϸ");
		outtextxy(130, 150, L"������Ϸ");
		outtextxy(130, 210, L"�Ѷ�����");
		outtextxy(15, 280, L"��Ϸ˵��:");
		outtextxy(90, 280, L"����м�ͼ�ο������¿�ʼ����Ϸ");
		outtextxy(90, 305, L"Ĭ���м��Ѷȣ�����ѡ���Ѷ�ֱ��");
		outtextxy(90, 330, L"��ʼ��Ϸ��������Ϸ���Լ����ϴ�");
		outtextxy(90, 355, L"��Ϸ�ļ�¼����ʱ�����õ������");
		outtextxy(90, 380, L"�ص��˵�����");
		whitchtouth();

	}
	cleardevice();
	putimage(0, 0, &back[0]);
	while (choice)
	{
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		outtextxy(130, 80, L"����");
		outtextxy(130, 180, L"�м�");
		outtextxy(130, 280, L"�߼�");
		whitchtouch2();
	}
	initgraph(grawidth, grahight);
	setbkcolor(WHITE);
	cleardevice();
	//��ʼ
	numbombss = bomb % 10;
	numbombs = bomb / 10;
	updategame();
	printfmaps();
}
void save_map(int map[][COLS], int rows) 
{
	FILE* fp = fopen("�浵.txt", "wb");
	for (int i = 0; i < rows; ++i) 
	{
		fwrite(map[i], sizeof(int), COLS, fp);
	}
	fclose(fp);
}

void load_map(int map[][COLS], int rows) 
{
	FILE* fp = fopen("�浵.txt", "rb");
	for (int i = 0; i < rows; ++i) 
	{
		fread(map[i], sizeof(int), COLS, fp);
	}
	fclose(fp);
}