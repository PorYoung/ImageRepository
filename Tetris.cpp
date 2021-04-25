/*************************************
项目名称:俄罗斯方块
编译环境:VS2015 && easyX
建立时间:2017-2-12
创建人:	PorYoung
最近修改:	2017-2-13
遗留问题:	① 变形		②消除		③计分计时		④界面美化
最近修改:	2017-2-14
想法：设计横版玩法——左右堆叠，下落至地则失败
遗留问题:	①优化变形方法		②优化算法，使运行更流畅		③速度越来越慢		④部分方块存在bug
16：52	修改:	修改代码,优化速度越来越慢问题
		问题:	下落中的方块闪动过快
思考: 如何实现智能play	------	自我学习功能(记忆操作步骤)
19:00	修改:	①下落方块闪动		②增加提示下一个方块
		问题:	运行过程存在未知bug,导致方块混乱停止下落或者异常退出
		遗留:	①优化变形方法	②计分计时	③界面美化
**************************************/

#include "resource.h"
#include "graphics.h"
#include "conio.h"
#include "stdlib.h"
#include "time.h"
struct PART
{
	//形状1
	char part1[4][4] =
	{
		{0, 1, 1, 0},
		{0, 1, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	};
	//形状2
	char part2[4][4] =
	{
		{ 0, 1, 1, 0 },
		{ 0, 0, 1, 1 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	};
	char part3[4][4] =
	{
		{0, 0, 1, 0},
		{0, 1, 1, 0},
		{0, 1, 0, 0},
		{0, 0, 0, 0}
	};
	//形状3
	char part4[4][4] =
	{
		{0, 0, 1, 1},
		{0, 1, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	};
	char part5[4][4] =
	{
		{ 0, 1, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 0}
	};
	//形状4
	char part6[4][4] =
	{
		{ 0, 1, 1, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 0 }
	};
	char part7[4][4] =
	{
		{0, 0, 1, 0},
		{1, 1, 1, 0},
		{0,},
		{0,}
	};
	char part8[4][4] =
	{
		{ 0, 1, 0 ,0},
		{ 0, 1, 0 ,0},
		{ 0, 1, 1 ,0},
		{ 0, 0, 0 ,0},
	};
	char part9[4][4] =
	{
		{1, 1, 1, 0},
		{1, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	};
	//形状5
	char part10[4][4] =
	{
		{0, 1, 1, 0},
		{0, 1, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 0, 0}
	};
	char part11[4][4] =
	{
		{1, 1, 1, 0},
		{0, 0, 1, 0},
		{0,},
		{0,}
	};
	char part12[4][4] =
	{
		{0, 0, 1, 0},
		{0, 0, 1, 0},
		{0, 1, 1, 0},
		{0, 0, 0, 0}
	};
	char part13[4][4] =
	{
		{1, 0, 0, 0},
		{1, 1, 1, 0},
		{0,},
		{0,}
	};
	//形状6
	char part14[4][4] =
	{
		{ 0, 1, 0, 0},
		{ 0, 1, 0, 0},
		{ 0, 1, 0, 0},
		{ 0, 1, 0, 0}
	};
	char part15[4][4] =
	{
		{1, 1, 1, 1},
		{0,},
		{0,},
		{0,}
	};
}part;

int temp = 1;		//方块是否停止	0 运动 1 停止
char Map[24][10];
int dir = 0;	//	0 不操作 1左	2右	3下	4上
int gameCore = 0;	//游戏分数
wchar_t grade[2];

int ch;		//	区别方块形状
int chflag = 1;
int chtemp1;
int chtemp2;	//两次方块形状


int flag = 0;	//速度 0: 慢速(1000ms)	1: 加速 (200ms)	2:	正常(700ms)		3:	加速(100ms)	4:快速(200ms)	5:加速(50ms)
int velocity = 1;	//8	:暂停
int tempvel = 0;	//	8	暂停

HANDLE hThread;

struct COOR
{
	int CRow[4] = { 0 };
	int CCol[4] = { 0 };
	int n = 0;
}coor;


void initgame();
void randShow();
void draw();
void move();
void Changemove();
void NumCount();
DWORD WINAPI Thread(LPVOID lpParameter);

void initgame()
{
	//行宽10*30, 列高20*30
	//initgraph(640, 600);
	graphdefaults();
	cleardevice();
	setfillcolor(RGB(86, 86, 86));
	solidrectangle(0, 0, 170, 600);
	solidrectangle(470, 0, 640, 600);
	int row = 0, col = 0;
	for (row = 0; row < 24; row++)
	{
		for (col = 0; col < 10; col++)
		{
			Map[row][col] = 0;
		}
	}
	dir = 0;
	//左边状态栏
	setbkmode(TRANSPARENT);
	setbkcolor(RGB(68, 68, 68));
	settextcolor(RGB(255, 130, 40));
	settextstyle(30, 0, L"华文彩云");
	clearrectangle(30, 50, 150, 340);
	outtextxy(50, 60, L"	变形");
	outtextxy(50, 95, L"	加速");
	outtextxy(50, 130, L"	左移");
	outtextxy(50, 165, L"	右移");
	outtextxy(50, 200, L"W	提速");
	outtextxy(50, 235, L"S	 降速");
	outtextxy(28, 270, L"空格 暂停");
	outtextxy(28, 305, L"回车 重玩");
	settextstyle(20, 8, L"华文彩云");
	outtextxy(10, 350, L"Made By PorYoung");
	outtextxy(45, 385, L"2017-2-13");


	char str2[2];
	InputBox(grade, 2, L"游戏难度\n(0->5 由慢到快)", L"选项", 0, 30, 30, true);
	WideCharToMultiByte(0, 0, grade, 1, str2, 1, 0, 0);
	velocity = atoi(str2);

	gameCore = 0;

	hThread = CreateThread(NULL, 0, Thread, NULL, 0, NULL);

	//第一个随机方块
	srand((unsigned int)time(NULL));
	chtemp1 = rand() % 14 + 1;
}

void randShow()
{
	void initShow(char nowshow[4][4]);
	if (chflag == 0)
	{
		chtemp1 = rand() % 14 + 1;
		ch = chtemp2;
		chflag = 1;
	}
	else
	{
		chtemp2 = rand() % 14 + 1;
		ch = chtemp1;
		chflag = 0;
	}

	switch (ch)
	{
	case 1:
		initShow(part.part1);
		break;
	case 2:
		initShow(part.part2);
		break;
	case 3:
		initShow(part.part3);
		break;
	case 4:
		initShow(part.part4);
		break;
	case 5:
		initShow(part.part5);
		break;
	case 6:
		initShow(part.part6);
		break;
	case 7:
		initShow(part.part7);
		break;
	case 8:
		initShow(part.part8);
		break;
	case 9:
		initShow(part.part9);
		break;
	case 10:
		initShow(part.part10);
		break;
	case 11:
		initShow(part.part11);
		break;
	case 12:
		initShow(part.part12);
		break;
	case 13:
		initShow(part.part13);
		break;
	case 14:
		initShow(part.part14);
		break;
	case 15:
		initShow(part.part15);
		break;
	}
	temp = 0;
}

void initShow(char nowshow[4][4])
{
	int row = 0, col = 0;
	for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			Map[row][col + 4] = nowshow[row][col];
		}
	}
}

void draw()
{
	int num = 0;
	for (int row = 23; row > 3; row--)
	{
		for (int col = 0; col < 10; col++)
		{
			if (Map[row][col] == 2)
			{
				setfillcolor(GREEN);
				fillrectangle(col * 30 + 170, row * 30 - 120, col * 30 + 30 + 170, row * 30 - 90);
			}
			else if (Map[row][col] == 0) num++;
		}
		if (num == 10) break;
		else num = 0;
	}
	coor.n = 0;
}

void showMessage()
{
	void NextShow(char nextshow[4][4]);
	wchar_t str[4];
	char str2[4];
	settextstyle(30, 0, L"华文彩云");
	setbkmode(TRANSPARENT);
	setbkcolor(RGB(68, 68, 68));
	clearrectangle(500, 200, 620, 340);
	itoa(gameCore, str2, 4);
	MultiByteToWideChar(0, 0, str2, 4, str, 4);
	outtextxy(510, 200, L"SCORE:");
	outtextxy(550, 235, str);

	//itoa(velocity, str2, 2);
	//MultiByteToWideChar(0, 0, str2, 2, str, 2);
	outtextxy(510, 270, L"GRADE:");
	outtextxy(550, 305, grade);

	int chtemp;
	if (chflag == 0)
	{
		chtemp = chtemp2;
	}
	else chtemp = chtemp1;
	switch (chtemp)
	{
	case 1:
		NextShow(part.part1);
		break;
	case 2:
		NextShow(part.part2);
		break;
	case 3:
		NextShow(part.part3);
		break;
	case 4:
		NextShow(part.part4);
		break;
	case 5:
		NextShow(part.part5);
		break;
	case 6:
		NextShow(part.part6);
		break;
	case 7:
		NextShow(part.part7);
		break;
	case 8:
		NextShow(part.part8);
		break;
	case 9:
		NextShow(part.part9);
		break;
	case 10:
		NextShow(part.part10);
		break;
	case 11:
		NextShow(part.part11);
		break;
	case 12:
		NextShow(part.part12);
		break;
	case 13:
		NextShow(part.part13);
		break;
	case 14:
		NextShow(part.part14);
		break;
	case 15:
		NextShow(part.part15);
		break;
	}
}

void move()
{
	void unmove();
	switch (ch)
	{
	case 1:
		//能移动
		if (Map[coor.CRow[2] + 1][coor.CCol[2]] == 0 && Map[coor.CRow[3] + 1][coor.CCol[3]] == 0 && coor.CRow[3] < 23)
		{
			Map[coor.CRow[0]][coor.CCol[0]] = 0;
			Map[coor.CRow[1]][coor.CCol[1]] = 0;
			Map[coor.CRow[2] + 1][coor.CCol[2]] = 1;
			Map[coor.CRow[3] + 1][coor.CCol[3]] = 1;
		}
		//不能移动
		else unmove();
		break;
	case 2:
		//能移动
		if (Map[coor.CRow[0] + 1][coor.CCol[0]] == 0 && Map[coor.CRow[2] + 1][coor.CCol[2]] == 0 && Map[coor.CRow[3] + 1][coor.CCol[3]] == 0 && coor.CRow[3] < 23)
		{
			Map[coor.CRow[0]][coor.CCol[0]] = 0;
			Map[coor.CRow[1]][coor.CCol[1]] = 0;
			Map[coor.CRow[3]][coor.CCol[3]] = 0;
			Map[coor.CRow[0] + 1][coor.CCol[0]] = 1;
			Map[coor.CRow[2] + 1][coor.CCol[2]] = 1;
			Map[coor.CRow[3] + 1][coor.CCol[3]] = 1;
		}
		//不能移动
		else unmove();
		break;
	case 3:
		//能移动
		if (Map[coor.CRow[3]][coor.CCol[3] + 1] == 0 && Map[coor.CRow[3] + 1][coor.CCol[3]] == 0 && coor.CRow[3] < 23)
		{
			Map[coor.CRow[0]][coor.CCol[0]] = 0;
			Map[coor.CRow[1]][coor.CCol[1]] = 0;
			Map[coor.CRow[3]][coor.CCol[3] + 1] = 1;
			Map[coor.CRow[3] + 1][coor.CCol[3]] = 1;
		}
		else unmove();
		break;
	case 4:
		//能移动
		if (Map[coor.CRow[3]][coor.CCol[3] + 1] == 0 && Map[coor.CRow[2] + 1][coor.CCol[2]] == 0 && Map[coor.CRow[3] + 1][coor.CCol[3]] == 0 && coor.CRow[3] < 23)
		{
			Map[coor.CRow[0]][coor.CCol[0]] = 0;
			Map[coor.CRow[1]][coor.CCol[1]] = 0;
			Map[coor.CRow[2]][coor.CCol[2]] = 0;
			Map[coor.CRow[3]][coor.CCol[3] + 1] = 1;
			Map[coor.CRow[2] + 1][coor.CCol[2]] = 1;
			Map[coor.CRow[3] + 1][coor.CCol[3]] = 1;
		}
		//不能移动
		else unmove();
		break;
	case 5:
		//能移动
		if (Map[coor.CRow[3]][coor.CCol[3] - 1] == 0 && Map[coor.CRow[3] + 1][coor.CCol[3]] == 0 && coor.CRow[3] < 23)
		{
			Map[coor.CRow[0]][coor.CCol[0]] = 0;
			Map[coor.CRow[2]][coor.CCol[2]] = 0;
			Map[coor.CRow[3]][coor.CCol[3] - 1] = 1;
			Map[coor.CRow[3] + 1][coor.CCol[3]] = 1;
		}
		//不能移动
		else unmove();
		break;
	case 6:
		//能移动
		if (Map[coor.CRow[3] + 1][coor.CCol[3]] == 0 && Map[coor.CRow[2]][coor.CCol[2] - 1] == 0 && coor.CRow[3] < 23)
		{
			Map[coor.CRow[0]][coor.CCol[0]] = 0;
			Map[coor.CRow[1]][coor.CCol[1]] = 0;
			Map[coor.CRow[2]][coor.CCol[2] - 1] = 1;
			Map[coor.CRow[3] + 1][coor.CCol[3]] = 1;
		}
		//不能移动
		else unmove();
		break;
	case 7:
		//能移动
		if (Map[coor.CRow[1] + 1][coor.CCol[1]] == 0 && Map[coor.CRow[2] + 1][coor.CCol[2]] == 0 && Map[coor.CRow[3] + 1][coor.CCol[3]] == 0 && coor.CRow[3] < 23)
		{
			Map[coor.CRow[1] + 1][coor.CCol[1]] = 1;
			Map[coor.CRow[2] + 1][coor.CCol[2]] = 1;
			Map[coor.CRow[3] + 1][coor.CCol[3]] = 1;
			Map[coor.CRow[0]][coor.CCol[0]] = 0;
			Map[coor.CRow[1]][coor.CCol[1]] = 0;
			Map[coor.CRow[2]][coor.CCol[2]] = 0;
		}
		else unmove();
		break;
	case 8:
		//能
		if (Map[coor.CRow[2] + 1][coor.CCol[2]] == 0 && Map[coor.CRow[3] + 1][coor.CCol[3]] == 0 && coor.CRow[3] < 23)
		{
			Map[coor.CRow[2] + 1][coor.CCol[2]] = 1;
			Map[coor.CRow[3] + 1][coor.CCol[3]] = 1;
			Map[coor.CRow[0]][coor.CCol[0]] = 0;
			Map[coor.CRow[3]][coor.CCol[3]] = 0;
		}
		else unmove();
		break;
	case 9:
		if (Map[coor.CRow[3]][coor.CCol[3] + 1] == 0 && Map[coor.CRow[2] + 1][coor.CCol[2]] == 0 && Map[coor.CRow[3] + 1][coor.CCol[3]] == 0 && coor.CRow[3] < 23)
		{
			//与case4判断相同	与case7移动方法相同
			Map[coor.CRow[1] + 1][coor.CCol[1]] = 1;
			Map[coor.CRow[2] + 1][coor.CCol[2]] = 1;
			Map[coor.CRow[3] + 1][coor.CCol[3]] = 1;
			Map[coor.CRow[0]][coor.CCol[0]] = 0;
			Map[coor.CRow[1]][coor.CCol[1]] = 0;
			Map[coor.CRow[2]][coor.CCol[2]] = 0;
		}
		else unmove();
		break;
	case 10:
		//能移动
		if (Map[coor.CRow[3] + 1][coor.CCol[3]] == 0 && Map[coor.CRow[1] + 1][coor.CCol[1]] == 0 && coor.CRow[3] < 23)
		{
			Map[coor.CRow[0]][coor.CCol[0]] = 0;
			Map[coor.CRow[1]][coor.CCol[1]] = 0;
			Map[coor.CRow[2]][coor.CCol[2] + 1] = 1;
			Map[coor.CRow[3] + 1][coor.CCol[3]] = 1;
		}
		//不能移动
		else unmove();
		break;
	case 11:
		//能移动
		if (Map[coor.CRow[1] + 1][coor.CCol[1]] == 0 && Map[coor.CRow[3] + 1][coor.CCol[3]] == 0 && Map[coor.CRow[0] + 1][coor.CCol[0]] == 0 && coor.CRow[3] < 23)
		{
			Map[coor.CRow[0] + 1][coor.CCol[0]] = 1;
			Map[coor.CRow[1] + 1][coor.CCol[1]] = 1;
			Map[coor.CRow[3] + 1][coor.CCol[3]] = 1;
			Map[coor.CRow[0]][coor.CCol[0]] = 0;
			Map[coor.CRow[1]][coor.CCol[1]] = 0;
			Map[coor.CRow[2]][coor.CCol[2]] = 0;
		}
		else unmove();
		break;
	case 12:
		//能
		if (Map[coor.CRow[2] + 1][coor.CCol[2]] == 0 && Map[coor.CRow[3] + 1][coor.CCol[3]] == 0 && coor.CRow[3] < 23)
		{
			Map[coor.CRow[2] + 1][coor.CCol[2]] = 1;
			Map[coor.CRow[3] + 1][coor.CCol[3]] = 1;
			Map[coor.CRow[0]][coor.CCol[0]] = 0;
			Map[coor.CRow[2]][coor.CCol[2]] = 0;
		}
		else unmove();
		break;

	case 13:
		if (Map[coor.CRow[1] + 1][coor.CCol[1]] == 0 && Map[coor.CRow[2] + 1][coor.CCol[2]] == 0 && Map[coor.CRow[3] + 1][coor.CCol[3]] == 0 && coor.CRow[3] < 23)
		{
			Map[coor.CRow[1] + 1][coor.CCol[1]] = 1;
			Map[coor.CRow[2] + 1][coor.CCol[2]] = 1;
			Map[coor.CRow[3] + 1][coor.CCol[3]] = 1;
			Map[coor.CRow[0]][coor.CCol[0]] = 0;
			Map[coor.CRow[2]][coor.CCol[2]] = 0;
			Map[coor.CRow[3]][coor.CCol[3]] = 0;
		}
		else unmove();
		break;
	case 14:
		if (Map[coor.CRow[3] + 1][coor.CCol[3]] == 0 && coor.CRow[3] < 23)
		{
			Map[coor.CRow[0]][coor.CCol[0]] = 0;
			Map[coor.CRow[3] + 1][coor.CCol[3]] = 1;
		}
		else unmove();
		break;
	case 15:
		for (int i = 0; i < 4; i++)
		{
			if (Map[coor.CRow[i] + 1][coor.CCol[i]] == 0)
			{
				if (coor.CRow[3] < 23)
				{
					if (i == 3)
					{
						Map[coor.CRow[0]][coor.CCol[0]] = 0;
						Map[coor.CRow[1]][coor.CCol[1]] = 0;
						Map[coor.CRow[2]][coor.CCol[2]] = 0;
						Map[coor.CRow[3]][coor.CCol[3]] = 0;
						Map[coor.CRow[0] + 1][coor.CCol[0]] = 1;
						Map[coor.CRow[1] + 1][coor.CCol[1]] = 1;
						Map[coor.CRow[2] + 1][coor.CCol[2]] = 1;
						Map[coor.CRow[3] + 1][coor.CCol[3]] = 1;
					}
					continue;
				}
			}
			unmove();
		}
		break;
	}
}

void Changemove()
{
	void unmove();
	//void getCoor();
	//getCoor();
	void drawCoor();
	drawCoor();
	switch (dir)
	{
	case 1:
		switch (ch)
		{
		case 1:
		case 2:
			//能移动
			if (Map[coor.CRow[0]][coor.CCol[0] - 1] == 0 && Map[coor.CRow[2]][coor.CCol[2] - 1] == 0 && coor.CCol[0] > 0)
			{
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
				Map[coor.CRow[1]][coor.CCol[1]] = 0;
				Map[coor.CRow[0]][coor.CCol[0] - 1] = 1;
				Map[coor.CRow[2]][coor.CCol[2] - 1] = 1;
			}
			//不能移动
			break;
		case 3:
			//能移动
			if (Map[coor.CRow[0]][coor.CCol[0] - 1] == 0 && Map[coor.CRow[1]][coor.CCol[1] - 1] == 0 && Map[coor.CRow[3]][coor.CCol[3] - 1] == 0 && coor.CCol[3] > 0)
			{
				Map[coor.CRow[0]][coor.CCol[0]] = 0;
				Map[coor.CRow[2]][coor.CCol[2]] = 0;
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
				Map[coor.CRow[0]][coor.CCol[0] - 1] = 1;
				Map[coor.CRow[1]][coor.CCol[1] - 1] = 1;
				Map[coor.CRow[3]][coor.CCol[3] - 1] = 1;
			}
			break;
		case 4:
			//能移动
			if (Map[coor.CRow[0]][coor.CCol[0] - 1] == 0 && Map[coor.CRow[2]][coor.CCol[2] - 1] == 0 && coor.CCol[2] > 0)
			{
				Map[coor.CRow[1]][coor.CCol[1]] = 0;
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
				Map[coor.CRow[0]][coor.CCol[0] - 1] = 1;
				Map[coor.CRow[2]][coor.CCol[2] - 1] = 1;
			}
			//不能移动
			break;
		case 5:
			//能移动
			if (Map[coor.CRow[0]][coor.CCol[0] - 1] == 0 && Map[coor.CRow[1]][coor.CCol[1] - 1] == 0 && Map[coor.CRow[3]][coor.CCol[3] - 1] == 0 && coor.CCol[0] > 0)
			{
				Map[coor.CRow[0]][coor.CCol[0]] = 0;
				Map[coor.CRow[2]][coor.CCol[2]] = 0;
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
				Map[coor.CRow[0]][coor.CCol[0] - 1] = 1;
				Map[coor.CRow[1]][coor.CCol[1] - 1] = 1;
				Map[coor.CRow[3]][coor.CCol[3] - 1] = 1;
			}
			//不能移动
			break;
		case 6:
			//能移动
			if (Map[coor.CRow[0]][coor.CCol[0] - 1] == 0 && Map[coor.CRow[2]][coor.CCol[2] - 1] == 0 && Map[coor.CRow[3]][coor.CCol[3] - 1] == 0 && coor.CCol[0] > 0)
			{
				Map[coor.CRow[1]][coor.CCol[1]] = 0;
				Map[coor.CRow[2]][coor.CCol[2]] = 0;
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
				Map[coor.CRow[0]][coor.CCol[0] - 1] = 1;
				Map[coor.CRow[2]][coor.CCol[2] - 1] = 1;
				Map[coor.CRow[3]][coor.CCol[3] - 1] = 1;
			}
			//不能移动
			break;
		case 7:
			//能移动
			if (Map[coor.CRow[1]][coor.CCol[1] - 1] == 0 & Map[coor.CRow[0]][coor.CCol[0] - 1] == 0 && coor.CCol[1] > 0)
			{
				Map[coor.CRow[0]][coor.CCol[0]] = 0;
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
				Map[coor.CRow[0]][coor.CCol[0] - 1] = 1;
				Map[coor.CRow[1]][coor.CCol[1] - 1] = 1;
			}
			break;
		case 8:
			//能
			if (Map[coor.CRow[0]][coor.CCol[0] - 1] == 0 && Map[coor.CRow[1]][coor.CCol[1] - 1] == 0 && Map[coor.CRow[2]][coor.CCol[2] - 1] == 0 && coor.CCol[0] > 0)
			{
				Map[coor.CRow[0]][coor.CCol[0] - 1] = 1;
				Map[coor.CRow[1]][coor.CCol[1] - 1] = 1;
				Map[coor.CRow[2]][coor.CCol[2] - 1] = 1;
				Map[coor.CRow[0]][coor.CCol[0]] = 0;
				Map[coor.CRow[1]][coor.CCol[1]] = 0;
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
			}
			break;
		case 9:
			if (Map[coor.CRow[0]][coor.CCol[0] - 1] == 0 && Map[coor.CRow[3]][coor.CCol[3] - 1] == 0 && coor.CCol[0] > 0)
			{
				Map[coor.CRow[0]][coor.CCol[0] - 1] = 1;
				Map[coor.CRow[3]][coor.CCol[3] - 1] = 1;
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
				Map[coor.CRow[2]][coor.CCol[2]] = 0;
			}
			break;
		case 10:
			//能移动
			if (Map[coor.CRow[0]][coor.CCol[0] - 1] == 0 && Map[coor.CRow[3]][coor.CCol[3] - 1] == 0 && Map[coor.CRow[2]][coor.CCol[2] - 1] == 0 && coor.CCol[0] > 0)
			{
				Map[coor.CRow[0]][coor.CCol[0] - 1] = 1;
				Map[coor.CRow[2]][coor.CCol[2] - 1] = 1;
				Map[coor.CRow[3]][coor.CCol[3] - 1] = 1;
				Map[coor.CRow[1]][coor.CCol[1]] = 0;
				Map[coor.CRow[2]][coor.CCol[2]] = 0;
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
			}
			//不能移动
			break;
		case 11:
			//能移动
			if (Map[coor.CRow[0]][coor.CCol[0] - 1] == 0 && Map[coor.CRow[3]][coor.CCol[3] - 1] == 0 && coor.CCol[0] > 0)
			{
				Map[coor.CRow[0]][coor.CCol[0] - 1] = 1;
				Map[coor.CRow[3]][coor.CCol[3] - 1] = 1;
				Map[coor.CRow[2]][coor.CCol[2]] = 0;
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
			}
			break;
		case 12:
			//能
			if (Map[coor.CRow[0]][coor.CCol[0] - 1] == 0 && Map[coor.CRow[1]][coor.CCol[1] - 1] == 0 && Map[coor.CRow[2]][coor.CCol[2] - 1] == 0 && coor.CCol[2] > 0)
			{
				Map[coor.CRow[0]][coor.CCol[0] - 1] = 1;
				Map[coor.CRow[1]][coor.CCol[1] - 1] = 1;
				Map[coor.CRow[2]][coor.CCol[2] - 1] = 1;
				Map[coor.CRow[0]][coor.CCol[0]] = 0;
				Map[coor.CRow[1]][coor.CCol[1]] = 0;
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
			}
			break;
		case 13:
			if (Map[coor.CRow[0]][coor.CCol[0] - 1] == 0 && Map[coor.CRow[1]][coor.CCol[1] - 1] == 0 && coor.CCol[0] > 0)
			{
				Map[coor.CRow[0]][coor.CCol[0] - 1] = 1;
				Map[coor.CRow[1]][coor.CCol[1] - 1] = 1;
				Map[coor.CRow[0]][coor.CCol[0]] = 0;
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
			}
			break;
		case 14:
			for (int i = 0; i < 4; i++)
			{
				if (Map[coor.CRow[i]][coor.CCol[i] - 1] == 0)
				{
					if (coor.CCol[0] > 0)
					{
						if (i == 3)
						{
							Map[coor.CRow[0]][coor.CCol[0]] = 0;
							Map[coor.CRow[1]][coor.CCol[1]] = 0;
							Map[coor.CRow[2]][coor.CCol[2]] = 0;
							Map[coor.CRow[3]][coor.CCol[3]] = 0;
							Map[coor.CRow[0]][coor.CCol[0] - 1] = 1;
							Map[coor.CRow[1]][coor.CCol[1] - 1] = 1;
							Map[coor.CRow[2]][coor.CCol[2] - 1] = 1;
							Map[coor.CRow[3]][coor.CCol[3] - 1] = 1;
						}
						continue;
					}
				}
			}
			break;
		case 15:
			if (Map[coor.CRow[0]][coor.CCol[0] - 1] == 0 && coor.CCol[0] > 0)
			{
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
				Map[coor.CRow[0]][coor.CCol[0] - 1] = 1;
			}
			break;
		}
		break;
	case 2:
		switch (ch)
		{
		case 1:
		case 2:
			//能移动
			if (Map[coor.CRow[1]][coor.CCol[1] + 1] == 0 && Map[coor.CRow[3]][coor.CCol[3] + 1] == 0 && coor.CCol[3] < 9)
			{
				Map[coor.CRow[0]][coor.CCol[0]] = 0;
				Map[coor.CRow[2]][coor.CCol[2]] = 0;
				Map[coor.CRow[1]][coor.CCol[1] + 1] = 1;
				Map[coor.CRow[3]][coor.CCol[3] + 1] = 1;
			}
			//不能移动
			break;
		case 3:
			//能移动
			if (Map[coor.CRow[0]][coor.CCol[0] + 1] == 0 && Map[coor.CRow[2]][coor.CCol[2] + 1] == 0 && Map[coor.CRow[3]][coor.CCol[3] + 1] == 0 && coor.CCol[0] < 9)
			{
				Map[coor.CRow[0]][coor.CCol[0]] = 0;
				Map[coor.CRow[1]][coor.CCol[1]] = 0;
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
				Map[coor.CRow[0]][coor.CCol[0] + 1] = 1;
				Map[coor.CRow[2]][coor.CCol[2] + 1] = 1;
				Map[coor.CRow[3]][coor.CCol[3] + 1] = 1;
			}
			break;
		case 4:
			//能移动
			if (Map[coor.CRow[1]][coor.CCol[1] + 1] == 0 && Map[coor.CRow[3]][coor.CCol[3] + 1] == 0 && coor.CCol[1] < 9)
			{
				Map[coor.CRow[0]][coor.CCol[0]] = 0;
				Map[coor.CRow[2]][coor.CCol[2]] = 0;
				Map[coor.CRow[1]][coor.CCol[1] + 1] = 1;
				Map[coor.CRow[3]][coor.CCol[3] + 1] = 1;
			}
			//不能移动
			break;
		case 5:
			//能移动
			if (Map[coor.CRow[0]][coor.CCol[0] + 1] == 0 && Map[coor.CRow[2]][coor.CCol[2] + 1] == 0 && Map[coor.CRow[3]][coor.CCol[3] + 1] == 0 && coor.CCol[3] < 9)
			{
				Map[coor.CRow[0]][coor.CCol[0]] = 0;
				Map[coor.CRow[1]][coor.CCol[1]] = 0;
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
				Map[coor.CRow[0]][coor.CCol[0] + 1] = 1;
				Map[coor.CRow[2]][coor.CCol[2] + 1] = 1;
				Map[coor.CRow[3]][coor.CCol[3] + 1] = 1;
			}
			//不能移动
			break;
		case 6:
			//能移动
			if (Map[coor.CRow[1]][coor.CCol[1] + 1] == 0 && Map[coor.CRow[2]][coor.CCol[2] + 1] == 0 && Map[coor.CRow[3]][coor.CCol[3] + 1] == 0 && coor.CCol[3] < 9)
			{
				Map[coor.CRow[0]][coor.CCol[0]] = 0;
				Map[coor.CRow[2]][coor.CCol[2]] = 0;
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
				Map[coor.CRow[1]][coor.CCol[1] + 1] = 1;
				Map[coor.CRow[2]][coor.CCol[2] + 1] = 1;
				Map[coor.CRow[3]][coor.CCol[3] + 1] = 1;
			}
			//不能移动
			break;
		case 7:
			//能移动
			if (Map[coor.CRow[3]][coor.CCol[3] + 1] == 0 & Map[coor.CRow[0]][coor.CCol[0] + 1] == 0 && coor.CCol[0] < 9)
			{
				Map[coor.CRow[0]][coor.CCol[0]] = 0;
				Map[coor.CRow[1]][coor.CCol[1]] = 0;
				Map[coor.CRow[0]][coor.CCol[0] + 1] = 1;
				Map[coor.CRow[3]][coor.CCol[3] + 1] = 1;
			}
			break;
		case 8:
			//能
			if (Map[coor.CRow[0]][coor.CCol[0] + 1] == 0 && Map[coor.CRow[1]][coor.CCol[1] + 1] == 0 && Map[coor.CRow[3]][coor.CCol[3] + 1] == 0 && coor.CCol[3] < 9)
			{
				Map[coor.CRow[0]][coor.CCol[0] + 1] = 1;
				Map[coor.CRow[1]][coor.CCol[1] + 1] = 1;
				Map[coor.CRow[3]][coor.CCol[3] + 1] = 1;
				Map[coor.CRow[0]][coor.CCol[0]] = 0;
				Map[coor.CRow[1]][coor.CCol[1]] = 0;
				Map[coor.CRow[2]][coor.CCol[2]] = 0;
			}
			break;
		case 9:
			if (Map[coor.CRow[2]][coor.CCol[2] + 1] == 0 && Map[coor.CRow[3]][coor.CCol[3] + 1] == 0 && coor.CCol[2] < 9)
			{
				Map[coor.CRow[2]][coor.CCol[2] + 1] = 1;
				Map[coor.CRow[3]][coor.CCol[3] + 1] = 1;
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
				Map[coor.CRow[0]][coor.CCol[0]] = 0;
			}
			break;
		case 10:
			//能移动
			if (Map[coor.CRow[1]][coor.CCol[1] + 1] == 0 && Map[coor.CRow[2]][coor.CCol[2] + 1] == 0 && Map[coor.CRow[3]][coor.CCol[3] + 1] == 0 && coor.CCol[1] < 9)
			{
				Map[coor.CRow[1]][coor.CCol[1] + 1] = 1;
				Map[coor.CRow[2]][coor.CCol[2] + 1] = 1;
				Map[coor.CRow[3]][coor.CCol[3] + 1] = 1;
				Map[coor.CRow[0]][coor.CCol[0]] = 0;
				Map[coor.CRow[2]][coor.CCol[2]] = 0;
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
			}
			//不能移动
			break;
		case 11:
			//能移动
			if (Map[coor.CRow[2]][coor.CCol[2] + 1] == 0 && Map[coor.CRow[3]][coor.CCol[3] + 1] == 0 && coor.CCol[3] < 9)
			{
				Map[coor.CRow[2]][coor.CCol[2] + 1] = 1;
				Map[coor.CRow[3]][coor.CCol[3] + 1] = 1;
				Map[coor.CRow[0]][coor.CCol[0]] = 0;
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
			}
			break;
		case 12:
			//能
			if (Map[coor.CRow[0]][coor.CCol[0] + 1] == 0 && Map[coor.CRow[1]][coor.CCol[1] + 1] == 0 && Map[coor.CRow[3]][coor.CCol[3] + 1] == 0 && coor.CCol[0] < 9)
			{
				Map[coor.CRow[0]][coor.CCol[0] + 1] = 1;
				Map[coor.CRow[1]][coor.CCol[1] + 1] = 1;
				Map[coor.CRow[3]][coor.CCol[3] + 1] = 1;
				Map[coor.CRow[0]][coor.CCol[0]] = 0;
				Map[coor.CRow[1]][coor.CCol[1]] = 0;
				Map[coor.CRow[2]][coor.CCol[2]] = 0;
			}
			break;
		case 13:
			if (Map[coor.CRow[0]][coor.CCol[0] + 1] == 0 && Map[coor.CRow[3]][coor.CCol[3] + 1] == 0 && coor.CCol[3] < 9)
			{
				Map[coor.CRow[0]][coor.CCol[0] + 1] = 1;
				Map[coor.CRow[3]][coor.CCol[3] + 1] = 1;
				Map[coor.CRow[0]][coor.CCol[0]] = 0;
				Map[coor.CRow[1]][coor.CCol[1]] = 0;
			}
			break;
		case 14:
			for (int i = 0; i < 4; i++)
			{
				if (Map[coor.CRow[i]][coor.CCol[i] + 1] == 0)
				{
					if (coor.CCol[0] < 9)
					{
						if (i == 3)
						{
							Map[coor.CRow[0]][coor.CCol[0]] = 0;
							Map[coor.CRow[1]][coor.CCol[1]] = 0;
							Map[coor.CRow[2]][coor.CCol[2]] = 0;
							Map[coor.CRow[3]][coor.CCol[3]] = 0;
							Map[coor.CRow[0]][coor.CCol[0] + 1] = 1;
							Map[coor.CRow[1]][coor.CCol[1] + 1] = 1;
							Map[coor.CRow[2]][coor.CCol[2] + 1] = 1;
							Map[coor.CRow[3]][coor.CCol[3] + 1] = 1;
						}
					}
				}
			}
			break;
		case 15:
			if (Map[coor.CRow[3]][coor.CCol[3] + 1] == 0 && coor.CCol[3] < 9)
			{
				Map[coor.CRow[0]][coor.CCol[0]] = 0;
				Map[coor.CRow[3]][coor.CCol[3] + 1] = 1;
			}
			break;
		}
		break;
		//变形
	case 3:
		switch (ch)
		{
		case 2:
			if (Map[coor.CRow[2]][coor.CCol[2] - 1] == 0 && Map[coor.CRow[0] - 1][coor.CCol[0]] == 0 && Map[coor.CRow[1] - 1][coor.CCol[1]] == 0)
			{
				Map[coor.CRow[2]][coor.CCol[2]] = 0;
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
				Map[coor.CRow[1] - 1][coor.CCol[1]] = 1;
				Map[coor.CRow[0] + 1][coor.CCol[0]] = 1;
				ch = 3;
			}
			break;
		case 3:
			if (Map[coor.CRow[0]][coor.CCol[0] + 1] == 0 && Map[coor.CRow[0]][coor.CCol[0] - 1] == 0 && Map[coor.CRow[3]][coor.CCol[3] - 1] == 0 && Map[coor.CRow[1]][coor.CCol[1] - 1] == 0 && coor.CCol[0] < 9)
			{
				Map[coor.CRow[1]][coor.CCol[1]] = 0;
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
				Map[coor.CRow[0]][coor.CCol[0] - 1] = 1;
				Map[coor.CRow[2]][coor.CCol[2] + 1] = 1;
				ch = 2;
			}
			break;
		case 4:
			if (Map[coor.CRow[0]][coor.CCol[0] - 1] == 0 && Map[coor.CRow[1] + 1][coor.CCol[1]] == 0 && Map[coor.CRow[0]][coor.CCol[0] - 1] == 0)
			{
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
				Map[coor.CRow[2]][coor.CCol[2]] = 0;
				Map[coor.CRow[0] - 1][coor.CCol[0]] = 1;
				Map[coor.CRow[1] + 1][coor.CCol[1]] = 1;
				ch = 5;
			}
			break;
		case 5:
			if (Map[coor.CRow[0]][coor.CCol[0] + 1] == 0 && Map[coor.CRow[3]][coor.CCol[3] - 1] == 0 && Map[coor.CRow[1]][coor.CCol[1] - 1] == 0 && coor.CCol[0] > 0)
			{
				Map[coor.CRow[2]][coor.CCol[2]] = 0;
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
				Map[coor.CRow[0]][coor.CCol[0] + 1] = 1;
				Map[coor.CRow[1]][coor.CCol[1] - 1] = 1;
				ch = 4;
			}
			break;
		case 6:
			if (Map[coor.CRow[0] - 1][coor.CCol[0]] == 0 && Map[coor.CRow[0] + 1][coor.CCol[0]] == 0 && Map[coor.CRow[1] - 1][coor.CCol[1]] == 0 && Map[coor.CRow[1]][coor.CCol[1] + 1] == 0 && Map[coor.CRow[2]][coor.CCol[2] + 1] == 0 && Map[coor.CRow[3]][coor.CCol[3] - 1] == 0 && coor.CCol[1] < 9)
			{
				Map[coor.CRow[0]][coor.CCol[0]] = 0;
				Map[coor.CRow[1]][coor.CCol[1]] = 0;
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
				Map[coor.CRow[1]][coor.CCol[1] + 1] = 1;
				Map[coor.CRow[2]][coor.CCol[2] + 1] = 1;
				Map[coor.CRow[2]][coor.CCol[2] - 1] = 1;
				ch = 7;
			}
			break;
		case 7:
			if (Map[coor.CRow[0] - 1][coor.CCol[0] - 1] == 0 && Map[coor.CRow[0] - 1][coor.CCol[0] - 2] == 0 && Map[coor.CRow[0]][coor.CCol[0] - 1] == 0 && Map[coor.CRow[0]][coor.CCol[0] - 2] == 0)
			{
				Map[coor.CRow[0]][coor.CCol[0]] = 0;
				Map[coor.CRow[1]][coor.CCol[1]] = 0;
				Map[coor.CRow[0]][coor.CCol[0] - 1] = 1;
				Map[coor.CRow[0] - 1][coor.CCol[0] - 1] = 1;
				ch = 8;
			}
			break;
		case 8:
			if (Map[coor.CRow[0]][coor.CCol[0] + 1] == 0 && Map[coor.CRow[1]][coor.CCol[1] - 1] == 0 && Map[coor.CRow[1]][coor.CCol[1] + 1] == 0 && Map[coor.CRow[2] + 1][coor.CCol[2]] == 0 && Map[coor.CRow[3] + 1][coor.CCol[3]] == 0 && Map[coor.CRow[2]][coor.CCol[2] - 1] == 0 && coor.CCol[0] > 0)
			{
				Map[coor.CRow[0]][coor.CCol[0]] = 0;
				Map[coor.CRow[2]][coor.CCol[2]] = 0;
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
				Map[coor.CRow[1]][coor.CCol[1] + 1] = 1;
				Map[coor.CRow[1]][coor.CCol[1] - 1] = 1;
				Map[coor.CRow[2]][coor.CCol[2] - 1] = 1;
				ch = 9;
			}
			break;
		case 9:
			if (Map[coor.CRow[0] - 1][coor.CCol[0]] == 0 && Map[coor.CRow[0]][coor.CCol[0] - 1] == 0 && Map[coor.CRow[3]][coor.CCol[3] - 1] == 0 && Map[coor.CRow[3]][coor.CCol[3] + 1] == 0 && Map[coor.CRow[1] - 1][coor.CCol[1]] == 0 && Map[coor.CRow[2] + 1][coor.CCol[2]] == 0)
			{
				Map[coor.CRow[0]][coor.CCol[0]] = 0;
				Map[coor.CRow[2]][coor.CCol[2]] = 0;
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
				Map[coor.CRow[0] - 1][coor.CCol[0]] = 1;
				Map[coor.CRow[1] - 1][coor.CCol[1]] = 1;
				Map[coor.CRow[1] + 1][coor.CCol[1]] = 1;
				ch = 6;
			}
			break;
		case 10:
			if (Map[coor.CRow[0] - 1][coor.CCol[0]] == 0 && Map[coor.CRow[0]][coor.CCol[0] - 1] == 0 && Map[coor.CRow[3]][coor.CCol[3] - 1] == 0 && Map[coor.CRow[2]][coor.CCol[2] + 1] == 0 && Map[coor.CRow[1] - 1][coor.CCol[1]] == 0 && Map[coor.CRow[2]][coor.CCol[2] - 1] == 0 && coor.CCol[0] > 0)
			{
				Map[coor.CRow[2]][coor.CCol[2]] = 0;
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
				Map[coor.CRow[0]][coor.CCol[0] - 1] = 1;
				Map[coor.CRow[1] + 1][coor.CCol[1]] = 1;
				ch = 11;
			}
			break;
		case 11:
			if (Map[coor.CRow[0] - 1][coor.CCol[0]] == 0 && Map[coor.CRow[0] + 1][coor.CCol[0]] == 0 && Map[coor.CRow[1] - 1][coor.CCol[1]] == 0 && Map[coor.CRow[1] + 1][coor.CCol[1]] == 0 && Map[coor.CRow[2] - 1][coor.CCol[2]] == 0 && Map[coor.CRow[2]][coor.CCol[2] + 1] == 0 && Map[coor.CRow[3]][coor.CCol[3] + 1] == 0)
			{
				Map[coor.CRow[0]][coor.CCol[0]] = 0;
				Map[coor.CRow[1]][coor.CCol[1]] = 0;
				Map[coor.CRow[2] - 1][coor.CCol[2]] = 1;
				Map[coor.CRow[3]][coor.CCol[3] - 1] = 1;
				ch = 12;
			}
			break;
		case 12:
			if (Map[coor.CRow[0]][coor.CCol[0] + 1] == 0 && Map[coor.CRow[1]][coor.CCol[1] + 1] == 0 && Map[coor.CRow[2] - 1][coor.CCol[2]] == 0 && Map[coor.CRow[2] + 1][coor.CCol[2]] == 0 && Map[coor.CRow[3] + 1][coor.CCol[3]] == 0 && Map[coor.CRow[3]][coor.CCol[3] + 1] == 0 && coor.CCol[0] < 9)
			{
				Map[coor.CRow[0]][coor.CCol[0]] = 0;
				Map[coor.CRow[1]][coor.CCol[1]] = 0;
				Map[coor.CRow[2] - 1][coor.CCol[2]] = 1;
				Map[coor.CRow[3]][coor.CCol[3] + 1] = 1;
				ch = 13;
			}
			break;
		case 13:
			if (Map[coor.CRow[0]][coor.CCol[0] - 1] == 0 && Map[coor.CRow[0]][coor.CCol[0] + 1] == 0 && Map[coor.CRow[1] + 1][coor.CCol[1]] == 0 && Map[coor.CRow[1]][coor.CCol[1] - 1] == 0 && Map[coor.CRow[2] + 1][coor.CCol[2]] == 0 && Map[coor.CRow[3] + 1][coor.CCol[3]] == 0 && coor.CRow[3] < 23)
			{
				Map[coor.CRow[2]][coor.CCol[2]] = 0;
				Map[coor.CRow[3]][coor.CCol[3]] = 0;
				Map[coor.CRow[0]][coor.CCol[0] + 1] = 1;
				Map[coor.CRow[1] + 1][coor.CCol[1]] = 1;
				ch = 10;
			}
			break;
		case 14:
			if (Map[coor.CRow[0]][coor.CCol[0] + 1] == 0 && Map[coor.CRow[1]][coor.CCol[1] - 1] == 0 && Map[coor.CRow[1]][coor.CCol[1] + 1] == 0 && Map[coor.CRow[2]][coor.CCol[2] - 1] == 0 && Map[coor.CRow[3]][coor.CCol[3] - 1] == 0)
			{
				if (coor.CCol[0] > 1 && Map[coor.CRow[1]][coor.CCol[1] - 2] == 0 && coor.CCol[3] < 9)
				{
					Map[coor.CRow[0]][coor.CCol[0]] = 0;
					Map[coor.CRow[2]][coor.CCol[2]] = 0;
					Map[coor.CRow[3]][coor.CCol[3]] = 0;
					Map[coor.CRow[1]][coor.CCol[1] - 1] = 1;
					Map[coor.CRow[1]][coor.CCol[1] + 1] = 1;
					Map[coor.CRow[1]][coor.CCol[1] - 2] = 1;
					ch = 15;
				}
				else if (coor.CCol[0] < 8 && Map[coor.CRow[1]][coor.CCol[1] + 2] == 0 && coor.CCol[0] > 0)
				{
					Map[coor.CRow[0]][coor.CCol[0]] = 0;
					Map[coor.CRow[2]][coor.CCol[2]] = 0;
					Map[coor.CRow[3]][coor.CCol[3]] = 0;
					Map[coor.CRow[1]][coor.CCol[1] - 1] = 1;
					Map[coor.CRow[1]][coor.CCol[1] + 1] = 1;
					Map[coor.CRow[1]][coor.CCol[1] + 2] = 1;
					ch = 15;
				}
			}
			break;
		case 15:
			if (Map[coor.CRow[0] - 1][coor.CCol[0]] == 0 && Map[coor.CRow[3] + 1][coor.CCol[3]] == 0 && Map[coor.CRow[2] + 1][coor.CCol[2]] == 0 && Map[coor.CRow[2] - 1][coor.CCol[2]] == 0)
			{
				if (coor.CRow[0] < 22 && Map[coor.CRow[2] + 2][coor.CCol[2]] == 0)
				{
					Map[coor.CRow[0]][coor.CCol[0]] = 0;
					Map[coor.CRow[1]][coor.CCol[1]] = 0;
					Map[coor.CRow[3]][coor.CCol[3]] = 0;
					Map[coor.CRow[2] - 1][coor.CCol[2]] = 1;
					Map[coor.CRow[2] + 1][coor.CCol[2]] = 1;
					Map[coor.CRow[2] + 2][coor.CCol[2]] = 1;
					ch = 14;
				}
				else if (coor.CRow[0] == 22 && Map[coor.CRow[2] - 2][coor.CCol[2]] == 0)
				{
					Map[coor.CRow[0]][coor.CCol[0]] = 0;
					Map[coor.CRow[1]][coor.CCol[1]] = 0;
					Map[coor.CRow[3]][coor.CCol[3]] = 0;
					Map[coor.CRow[2] - 1][coor.CCol[2]] = 1;
					Map[coor.CRow[2] + 1][coor.CCol[2]] = 1;
					Map[coor.CRow[2] - 2][coor.CCol[2]] = 1;
					ch = 14;
				}
			}
			break;
		}
		break;
	}
	if (dir != 0)
	{
		for (int j = 0; j < 4; j++)
		{
			clearrectangle(coor.CCol[j] * 30 + 170, coor.CRow[j] * 30 - 120, coor.CCol[j] * 30 + 170 + 30, coor.CRow[j] * 30 - 90);
		}
	}
	dir = 0;
}
void unmove()
{
	for (int i = 0; i < 4; i++)
	{
		Map[coor.CRow[i]][coor.CCol[i]] = 2;
	}
	temp = 1;
}

void drawCoor()
{
	for (int row = 0; row < 24; row++)
	{
		for (int col = 0; col < 10; col++)
		{
			if (Map[row][col] == 1)
			{
				setfillcolor(YELLOW);
				fillrectangle(col * 30 + 170, row * 30 - 120, col * 30 + 30 + 170, row * 30 - 90);
				coor.CRow[coor.n] = row;
				coor.CCol[coor.n] = col;
				coor.n++;
			}
		}
		if (coor.n == 4) break;
	}
	coor.n = 0;
}


void gameVelocity()
{
	switch (velocity)
	{
	case 0:
		if (flag)
		{
			Sleep(100);
			flag--;
		}
		else Sleep(1000);
		break;
	case 1:
		if (flag)
		{
			Sleep(50);
			flag--;
		}
		else Sleep(500);
		break;
	case 2:
		if (flag)
		{
			Sleep(30);
			flag--;
		}
		else Sleep(400);
		break;
	case 3:
		if (flag)
		{
			Sleep(10);
			flag--;
		}
		else Sleep(200);
		break;
	case 4:
		if (flag)
		{
			Sleep(1);
			flag--;
		}
		else Sleep(10);
		break;
	case 5:
		if (flag)
		{
			outtextxy(190, 180, L"别逗了,你以为是神啊⊙﹏⊙b");
			Sleep(1000);
			flag = 0;
		}
		else Sleep(1);
		break;
	case 8:
		setbkmode(TRANSPARENT);
		setbkcolor(RGB(68, 68, 68));
		clearrectangle(30, 50, 150, 200);
		settextcolor(RGB(255, 130, 40));
		settextstyle(30, 0, L"华文彩云");
		settextstyle(50, 6, L"华文彩云");
		while (velocity == 8)
		{
			outtextxy(70, 110, L"暂  停");
		}
		//左边状态栏
		setbkmode(TRANSPARENT);
		setbkcolor(RGB(68, 68, 68));
		settextcolor(RGB(255, 130, 40));
		settextstyle(30, 0, L"华文彩云");
		clearrectangle(30, 50, 150, 200);
		outtextxy(50, 60, L"	变形");
		outtextxy(50, 95, L"	加速");
		outtextxy(50, 130, L"	左移");
		outtextxy(50, 165, L"	右移");
		outtextxy(50, 200, L"W	提速");
		outtextxy(50, 235, L"S	 降速");
		outtextxy(28, 270, L"空格 暂停");
		outtextxy(28, 305, L"回车 重玩");
		settextstyle(20, 8, L"华文彩云");
		outtextxy(10, 350, L"Made By PorYoung");
		outtextxy(45, 385, L"2017-2-13");
	}

}

void NumCount()
{
	int Num[20] = { 0 };
	int i = 0;
	int row, col;
	for (row = 23; row > 3 && i < 20; row--)
	{
		for (col = 0; col < 10; col++)
		{
			if (Map[row][col] == 2)
			{
				Num[i]++;
			}
		}
		i++;
	}

	//消去方块
	int tempRow = 0;
	for (row = 23, i = 0; row > 3; row--)
	{
		if (Num[i] == 10)
		{
			for (col = 0; col < 10; col++)
			{
				for (tempRow = row; tempRow > 3; tempRow--)
				{
					Map[tempRow][col] = Map[tempRow - 1][col];
					Map[tempRow - 1][col] = 0;
				}
			}
			gameCore += 1;
			row++;
		}
		else if (Num[i] == 0) break;
		i++;
	}

	//游戏结束
	for (col = 0; col < 10; col++)
	{
		if (Map[4][col] == 2)
		{
			settextcolor(RED);
			settextstyle(40, 0, L"华文彩云");
			outtextxy(190, 300, L"游戏结束⊙﹏⊙");
			Sleep(3000);
			gameCore = 0;
			cleardevice();
			initgame();
		}
	}
}

void NextShow(char nextshow[4][4])
{
	setbkmode(TRANSPARENT);
	setbkcolor(RGB(68, 68, 68));
	settextcolor(RGB(255, 130, 40));
	settextstyle(20, 0, L"华文彩云");
	clearrectangle(500, 50, 620, 170);
	outtextxy(520, 600, L"Next One");
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			if (nextshow[row][col] == 1)
			{
				setfillcolor(BROWN);
				fillroundrect(col * 30 + 500, row * 30 + 50, col * 30 + 500 + 30, row * 30 + 50 + 30, 10, 10);
			}
		}
	}
}
DWORD WINAPI Thread(LPVOID lpParameter)
{
	while (1)
	{
		int keyMs;

		keyMs = getch();
		switch (keyMs)
		{
		case 75:
			dir = 1;
			break;
		case 77:
			dir = 2;
			break;
		case 72:
			dir = 3;
			break;
		case 80:
			dir = 0;
			flag = 2;
			break;
			//空格暂停
		case 32:
			tempvel = velocity;
			velocity = 8;
			system("pause");
			velocity = tempvel;
			break;
			//重玩
		case 13:
			temp = 1;
			Map[4][0] = 2;
			break;
			//提速
		case 87:
			if (velocity < 5)
			{
				velocity++;
			}
			else
			{
				clearrectangle(50, 430, 120, 480);
				settextstyle(30, 0, L"华文彩云");
				outtextxy(50, 440, L"最大速");
			}
			break;
			//降速
		case 83:
			if (velocity > 0)
			{
				velocity--;
			}
			else
			{
				clearrectangle(30, 430, 120, 480);
				settextstyle(30, 0, L"华文彩云");
				outtextxy(50, 440, L"最小速");
			}
			break;
		}
	}
}
int main()
{
	void showMessage();
	HWND hwnd = GetHWnd();
	SetWindowText(hwnd, L"俄罗斯方块");
	initgraph(640, 600);
	initgame();

	//mciSendString(L"open 1.mp3 alias bgm", NULL, 0, NULL);
	//mciSendString(L"play bgm repeat", 0, 0, 0);

	while (1)
	{
		if (temp == 1)
		{
			flag = 0;
			NumCount();
			draw();
			randShow();
			showMessage();
			clearrectangle(171, 0, 469, 600);
		}
		int i = 1000;
		while (i--)
		{
			Changemove();
		}
		draw();
		move();
		gameVelocity();
		for (int j = 0; j < 4; j++)
		{
			clearrectangle(coor.CCol[j] * 30 + 170, coor.CRow[j] * 30 - 120, coor.CCol[j] * 30 + 170 + 30, coor.CRow[j] * 30 - 90);
		}
	}
	return 0;
	system("pause");
}
