/*************************************
工程名称：2048图形界面版
创建时间：2017.1.19
编译环境：VS2015
创建者：PorYoung
**************************************/


#include "graphics.h"
#include "conio.h"
#include "stdlib.h"
#include "time.h"
#include "windows.h"
#include "resource.h" 
#include "stdio.h"

IMAGE bkImg;
int maps[4][4] = { 0 };
char str[4];
int temp = 0;		//游戏状态	

void gameShow();
int numShow();	//随机生成2或4
void gamePlay();
//void judge();

//4*4界面
void gameShow()
{
	if (temp == 0)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				maps[i][j] = 0;
		int x, y;
		x = rand() % 10 % 4;
		y = rand() % 10 % 4;
		maps[x][y] = numShow();
		temp = 1;
	}
	BeginBatchDraw();
	//putimage(0, 0, &bkImg);
	for (int i = 1; i < 4; i++)
	{
		setlinecolor(RGB(125, 125, 125));
		line(86, 40 + 80 * i, 415, 40 + 80 * i);
		line(88 + 82 * i, 40, 88 + 82 * i, 358);
	}
	for (int i = 0, x = 106, y = 60; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			sprintf(str, "%d", maps[i][j]);
			setbkmode(TRANSPARENT);
			settextcolor(RGB(25, 65, 125));
			settextstyle(40, 10, "微软雅黑");
			outtextxy(x, y, str);
			x += 80;
			if (x >= 400)
			{
				y += 82;
				x = 106;
			}
		}
	}
	EndBatchDraw();
}

int numShow()
{
	int r = rand() % 10, num;
	if (r < 5)	num = 2;
	else num = 4;
	return num;
}

void gamePlay()
{
	void upPlay();
	void downPlay();
	void leftPlay();
	void rightPlay();
	Sleep(100);
	char ch = _getch();
	switch (ch)
	{
	case 72:
		upPlay();
		break;	//上
	case 80:
		downPlay();
		break;	//下
	case 75:
		leftPlay();
		break;	//左
	case 77:
		rightPlay();
		break;	//右
	case 'r':
	case 'R':
		temp = 0;
		break;
	}
}

void upPlay()
{
	int row, col;

	//移动	
	for (col = 0; col < 4; col++)
	{
		int i = 3;
		while (i--)
		{
			for (row = 0; row < 3; row++)
			{
				if (maps[row][col] == 0)
				{
					maps[row][col] = maps[row + 1][col];
					maps[row + 1][col] = 0;
				}
			}
		}
	}
	//相加
	for (col = 0; col < 4; col++)
	{
		int i = 3;
		while (i--)
		{
			for (row = 0; row < 3; row++)
			{
				if (maps[row][col] == maps[row + 1][col])
				{
					maps[row][col] *= 2;
					maps[row + 1][col] = 0;
				}
			}
		}
	}
	//移动
	for (col = 0; col < 4; col++)
	{
		int i = 3;
		while (i--)
		{
			for (row = 0; row < 3; row++)
			{
				if (maps[row][col] == 0)
				{
					maps[row][col] = maps[row + 1][col];
					maps[row + 1][col] = 0;
				}
			}
		}
	}
	//新数字
	row = 3;		//加不加无所谓
	do
	{
		col = rand() % 10 % 4;
	} while (maps[row][col] != 0);
	maps[row][col] = numShow();
}

void downPlay()
{
	int row, col;

	//移动	
	for (col = 0; col < 4; col++)
	{
		int i = 3;
		while (i--)
		{
			for (row = 3; row > 0; row--)
			{
				if (maps[row][col] == 0)
				{
					maps[row][col] = maps[row - 1][col];
					maps[row - 1][col] = 0;
				}
			}
		}
	}
	//相加
	for (col = 0; col < 4; col++)
	{
		int i = 3;
		while (i--)
		{
			for (row = 3; row > 0; row--)
			{
				if (maps[row][col] == maps[row - 1][col])
				{
					maps[row][col] *= 2;
					maps[row - 1][col] = 0;
				}
			}
		}
	}
	//移动
	for (col = 0; col < 4; col++)
	{
		int i = 3;
		while (i--)
		{
			for (row = 3; row > 0; row--)
			{
				if (maps[row][col] == 0)
				{
					maps[row][col] = maps[row - 1][col];
					maps[row - 1][col] = 0;
				}
			}
		}
	}
	//新数字
	row = 0;
	do
	{
		col = rand() % 10 % 4;
	} while (maps[row][col] != 0);
	maps[row][col] = numShow();
}

void leftPlay()
{
	int row, col;

	//移动	
	for (row = 0; row < 4; row++)
	{
		int i = 3;
		while (i--)
		{
			for (col = 0; col < 3; col++)
			{
				if (maps[row][col] == 0)
				{
					maps[row][col] = maps[row][col + 1];
					maps[row][col + 1] = 0;
				}
			}
		}
	}
	//相加
	for (row = 0; row < 4; row++)
	{
		int i = 3;
		while (i--)
		{
			for (col = 0; col < 3; col++)
			{
				if (maps[row][col] == maps[row][col + 1])
				{
					maps[row][col] *= 2;
					maps[row][col + 1] = 0;
				}
			}
		}
	}
	//移动
	for (row = 0; row < 4; row++)
	{
		int i = 3;
		while (i--)
		{
			for (col = 0; col < 3; col++)
			{
				if (maps[row][col] == 0)
				{
					maps[row][col] = maps[row][col + 1];
					maps[row][col + 1] = 0;
				}
			}
		}
		//新数字
		col = 3;		//加不加无所谓
		do
		{
			row = rand() % 10 % 4;
		} while (maps[row][col] != 0);
		maps[row][col] = numShow();
	}
}

void rightPlay()
{
	int row, col;

	//移动	
	for (row = 0; row < 4; row++)
	{
		int i = 3;
		while (i--)
		{
			for (col = 3; col > 0; col--)
			{
				if (maps[row][col] == 0)
				{
					maps[row][col] = maps[row][col - 1];
					maps[row][col - 1] = 0;
				}
			}
		}
	}
	//相加
	for (row = 0; row < 4; row++)
	{
		int i = 3;
		while (i--)
		{
			for (col = 3; col > 0; col--)
			{
				if (maps[row][col] == maps[row][col - 1])
				{
					maps[row][col] *= 2;
					maps[row][col - 1] = 0;
				}
			}
		}
	}
	//移动
	for (row = 0; row < 4; row++)
	{
		int i = 3;
		while (i--)
		{
			for (col = 3; col > 0; col--)
			{
				if (maps[row][col] == 0)
				{
					maps[row][col] = maps[row][col - 1];
					maps[row][col - 1] = 0;
				}
			}
		}
	}
	//新数字
	col = 0;
	do
	{
		row = rand() % 10 % 4;
	} while (maps[row][col] != 0);
	maps[row][col] = numShow();
}

/*void judge()
{
	//判断是否结束
	int row, col, num = 0;
	for (row = 0; row < 4; row += 3)
		for (col = 0; col < 4; col++)
		{
			if (maps[row][col] != 0) num++;
		}
	for (col = 0; col < 4; col += 3)
		for (row = 0; row < 4; row++)
		{
			if (maps[row][col] != 0) num++;
		}
	if (num == 16)
	{
		setbkcolor(RGB(255, 255, 255));
		clearrectangle(88, 48, 412, 358);
		settextcolor(RGB(255, 100, 155));
		outtextxy(200, 170, "游戏结束！");
		Sleep(3000);
		temp = 0;
		clearrectangle(88, 48, 412, 358);
	}
}*/                //有bug,可不要

int main()
{
	initgraph(500, 400);
	loadimage(&bkImg, "IMAGE", MAKEINTRESOURCE(IDR_IMAGE1), 500, 400);
	putimage(0, 0, &bkImg);
	srand((unsigned int)time(NULL));
	while (1)
	{
		gameShow();
		gamePlay();
		setbkcolor(RGB(255, 255, 255));
		clearrectangle(88, 48, 412, 358);
		setlinecolor(RED);
		rectangle(235, 365, 260, 385);	//准备增加点击事件
	}
}
