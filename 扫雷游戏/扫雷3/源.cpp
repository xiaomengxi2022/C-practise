#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <graphics.h>
#include<mmsystem.h> //包含多媒体设备接口头文件
#pragma comment(lib,"winmm.lib")
int grahight = 415;//实际画布高
int grawidth = 340;//实际画布宽
#define allhight 100//总数组大小
#define allwidth 100//总数组大小
#define ROWS 102
#define COLS 102
//中级17*17：415*340，初级10*10：275*200，高级20*20：475*400
int hight = 17;//实际游戏区域大小
int width = 17;//实际游戏区域大小
int time1 = 0, time2 = 0, time3 = 0;//记录时间的三位
int first = 1;//判断第一次点击
int bomb = 45;//炸弹数量
int runing = 1;//是否结束游戏
int smiles = 0;//图形的变化
int ture = 1;//判断是否开始游戏
int a = 1;//不让它一直弹出胜利的框
int tem = 0;
int map[allhight + 2][allwidth + 2] = { {0} };//布局战场都出的两行和两个列用来辅助找出周围炸弹数
clock_t start_t, end_t;//开始实际时间，结束时间算时间差
IMAGE block[13];//1-8和空格和炸弹图片
IMAGE back[3];//菜单背景
IMAGE smile[4];//图形的各种状态
IMAGE numbomb[10];//时间图片
int numbombs = 0;//炸弹第一位数
int numbombss = 0;//炸弹第二位数
int win = 0;//判断是否赢
int choice = 0;//看用户是否选择
void printfmap();//打印地图
void updategame();//更新数据并且加密
int findbomb(int a, int b);//找正常方块附近的炸弹数量
void theloadimage();//加载图片
void playgame();//获取用户的点击
void newgame();//刷新游戏
void whitchtouch2();//选择 难度界面函数
void showbomb();//点击炸弹后显示所有炸弹
void openzero(int m, int n);//遇到空格打开，递归思想
void whitchtouth();//开始界面的选择函数
void save_map(int map[][COLS], int rows);
void load_map(int map[][COLS], int rows);
void printfmaps()
{
	for (int i = 1; i <= hight; i++)
	{
		printf("第%d行：", i);
		for (int j = 1; j <= width; j++)
		{

			int a = (j - 1) * 20;
			int b = (i - 1) * 20 + 72;
			printf("%3d", map[i][j]);

		}
		printf("\n");
		
	}
}
//可以通过看后台输出文字知道雷的位置
void before();
int main()
{
	HWND hwnd = initgraph(grawidth, grahight);
	setbkcolor(WHITE);
	cleardevice();
	theloadimage();
	putimage(0, 0, &back[0]);
	mciSendString(L"open ../音乐/菜单.mp3 alias BGM", 0, 0, 0);
	mciSendString(L"setaudio BGM volume to 140", 0, 0, 0);//调节音量大小
	mciSendString(L"play BGM repeat", 0, 0, 0);
	//准备过程
	while (ture)
	{
		settextcolor(BLACK);
		setbkmode(TRANSPARENT);
		outtextxy(130, 90, L"新游戏");
		outtextxy(130, 150, L"继续游戏");
		outtextxy(130, 210, L"难度设置");
		outtextxy(15, 280, L"游戏说明:");
		outtextxy(90, 280, L"点击中间图形可以重新开始，游戏");
		outtextxy(90, 305, L"默认中级难度，可以选择难度直接");
		outtextxy(90, 330, L"开始游戏，继续游戏可以继续上次");
		outtextxy(90, 355, L"游戏的记录，但时间重置点击返回");
		outtextxy(90, 380, L"回到菜单界面");
		whitchtouth();

	}
	cleardevice();
	putimage(0, 0, &back[0]);
	while (choice)
	{
		settextcolor(BLACK);
		setbkmode(TRANSPARENT);
		outtextxy(130, 80, L"初级");
		outtextxy(130, 180, L"中级");
		outtextxy(130, 280, L"高级");
		whitchtouch2();
	}
	initgraph(grawidth, grahight,1);
	setbkcolor(WHITE);
	cleardevice();
	//开始
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
			PlaySound(_T("../音效/胜利.wav"), NULL, SND_ASYNC);
			MessageBox(hwnd, L"恭喜旅行者排雷成功~", L"", MB_OK);
		}
		if (smiles == 1)//点击中间图形就会重新开始游戏
		{
			first = 1;
			a = 1;
			win = 0;
			system("cls");
			newgame();
			printfmaps();
		}
		end_t = clock();
		int q = (end_t - start_t) / 1000;//时间差
		
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
	outtextxy(grawidth / 2 - 25, 0, L"返回");
	putimage(grawidth / 2 - 20, 30, &smile[smiles]);//中间图形
	putimage(10, 10, &numbomb[numbombs]);//这个和下个都是显示炸弹数量图片
	putimage(40, 10, &numbomb[numbombss]);
	putimage(grawidth - 90, 10, &numbomb[time1]);//下面三个都是时间图片
	putimage(grawidth - 60, 10, &numbomb[time2]);
	putimage(grawidth - 30, 10, &numbomb[time3]);
	for (int i = 1; i <= hight; i++)
	{
		for (int j = 1; j <= width; j++)
		{

			int a = (j - 1) * 20;
			int b = (i - 1) * 20 + 72;
			if (map[i][j] <= 28 && map[i][j] >= 19)//19-28表示没点开的所以都是方块图片。这就是为什么要加20的原因
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
			//上面这个if就是用来用户点击之后减去20就变成-1到8就显示出对应图片
			else if (map[i][j] <= 48 && map[i][j] >= 39)//右键加20然后，就在39到48之间，这些都是镖旗图标
				putimage(a, b, &block[11]);
			else if (map[i][j] == -100)//这是点到的炸弹要变红
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
	//布置地雷
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
	//查找不是雷的区域周围的雷数
	for (int i = 1; i <= hight; i++)
	{
		for (int j = 1; j <= width; j++)
		{
			map[i][j] += 20;
		}
	}
	//加密

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
	//加载图片
	loadimage(&back[0], L"../img/背景.png", 340, 415);
	loadimage(&block[0], L"../img/空.png", 20, 20);
	loadimage(&block[1], L"../img/1.png", 20, 20);
	loadimage(&block[2], L"../img/2.png", 20, 20);
	loadimage(&block[3], L"../img/3.png", 20, 20);
	loadimage(&block[4], L"../img/4.png", 20, 20);
	loadimage(&block[5], L"../img/5.png", 20, 20);
	loadimage(&block[6], L"../img/6.png", 20, 20);
	loadimage(&block[7], L"../img/7.png", 20, 20);
	loadimage(&block[8], L"../img/8.png", 20, 20);
	loadimage(&block[9], L"../img/雷.png", 20, 20);
	loadimage(&block[10], L"../img/翻开.png", 20, 20);
	loadimage(&block[11], L"../img/标记.png", 20, 20);
	loadimage(&block[12], L"../img/红雷.png", 20, 20);
	loadimage(&smile[0], L"../img/笑脸.png", 30, 30);
	loadimage(&smile[1], L"../img/笑脸2.png", 30, 30);
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
	//获取用户的鼠标输入
	if (MouseHit())
	{
		MOUSEMSG msg = GetMouseMsg();
		int x = msg.x;
		int y = msg.y;
		switch (msg.uMsg)
		{
		case  WM_LBUTTONDOWN:
			PlaySound(_T("../音效/翻开.wav"), NULL, SND_ASYNC);
			if (x >= 0 && x <= grawidth && y >= 72 && y <= grahight && runing && smiles != 3)
			{
				if (first == 1)//判读第一次点击开始计时
				{
					start_t = clock();
					first = 0;
				}
				x = x / 20 + 1;
				y = (y - 72) / 20 + 1;
				if (map[y][x] <= 28 && map[y][x] >= 19)
				{
					if (map[y][x] == 20)//如果点开的是0需要展开周围8个，8个中有0就还要展开，同时保证自己展开的不是炸弹
					{
						openzero(y, x);
					}
					else
					{
						map[y][x] -= 20;//正常的就直接展开
						win++;
						if (map[y][x] == -1)
							win--;//防止最后一个是雷变成又是赢，又有炸弹
					}
				}
				if (map[y][x] == -1)
				{
					PlaySound(_T("../音效/失败.wav"), NULL, SND_ASYNC);
					map[y][x] = -100;//点到的炸弹要标红，所以单独设置一个数字，来展示这张图片
					showbomb();//点到炸弹要把炸弹全部展开
					smiles = 2;//输了的话就变成紫色图形
					printfmap();
					runing = 0;//输了就不能点击游戏区

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
				smiles = 0;//除了输赢和点击否则都是青绿色图片
			break;
		case WM_RBUTTONDOWN:
			PlaySound(_T("../音效/标记.wav"), NULL, SND_ASYNC);
			if (x >= 0 && x <= grawidth && y >= 72 && y <= grahight && runing == 1)//右键镖旗
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
	save_map(map, ROWS);//保存地图
	
}
void openzero(int m, int n)
{
	map[m][n] -= 20;//打开本身
	win++;
	for (int i = m - 1; i <= m + 1; i++)
	{
		for (int j = n - 1; j <= n + 1; j++)
		{
			if (i >= 1 && i <= width && j >= 1 && j <= hight)//保证在游戏区
			{
				if (map[i][j] <= 28 && map[i][j] >= 19)//保证没有翻开
				{
					if (map[i][j] != 20)//保证不是0
					{
						map[i][j] -= 20;
						win++;
					}
					else
						openzero(i, j);//递归

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
		PlaySound(_T("../音效/翻开.wav"), NULL, SND_ASYNC);
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
					PlaySound(_T("../音效/胜利.wav"), NULL, SND_ASYNC);
					MessageBox(hwnd, L"恭喜旅行者排雷成功~", L"", MB_OK);
				}
				if (smiles == 1)//点击笑脸就会重新开始游戏
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
		PlaySound(_T("../音效/翻开.wav"), NULL, SND_ASYNC);
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
	ture = 1;//重置参数
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
	//准备过程
	while (ture)
	{
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		outtextxy(130, 90, L"新游戏");
		outtextxy(130, 150, L"继续游戏");
		outtextxy(130, 210, L"难度设置");
		outtextxy(15, 280, L"游戏说明:");
		outtextxy(90, 280, L"点击中间图形可以重新开始，游戏");
		outtextxy(90, 305, L"默认中级难度，可以选择难度直接");
		outtextxy(90, 330, L"开始游戏，继续游戏可以继续上次");
		outtextxy(90, 355, L"游戏的记录，但时间重置点击返回");
		outtextxy(90, 380, L"回到菜单界面");
		whitchtouth();

	}
	cleardevice();
	putimage(0, 0, &back[0]);
	while (choice)
	{
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		outtextxy(130, 80, L"初级");
		outtextxy(130, 180, L"中级");
		outtextxy(130, 280, L"高级");
		whitchtouch2();
	}
	initgraph(grawidth, grahight);
	setbkcolor(WHITE);
	cleardevice();
	//开始
	numbombss = bomb % 10;
	numbombs = bomb / 10;
	updategame();
	printfmaps();
}
void save_map(int map[][COLS], int rows) 
{
	FILE* fp = fopen("存档.txt", "wb");
	for (int i = 0; i < rows; ++i) 
	{
		fwrite(map[i], sizeof(int), COLS, fp);
	}
	fclose(fp);
}

void load_map(int map[][COLS], int rows) 
{
	FILE* fp = fopen("存档.txt", "rb");
	for (int i = 0; i < rows; ++i) 
	{
		fread(map[i], sizeof(int), COLS, fp);
	}
	fclose(fp);
}