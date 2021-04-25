/************************************************************************************************************************
文件名称：main.c
文件描述：实现控制台的2048逻辑代码
编译环境：vs2015
最后修改：
<2017.1.8> <最后一次修改23:38 依然存在算法上的问题，在上下左右当中> <修改者：PorYoung>
<最近修改：2017.1.9 算法问题待定，另发现生成2和4时存在bug，整体有待优化><修改者：PorYoung>
<优化思路：①代码优化，减少函数个数；②功能优化：增加撤销功能，自动走一步功能；③算法优化：生成算法优化、移动、加法优化>
*************************************************************************************************************************/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>

void Add(int dir);
void Print();
int map[4][4] = { { 0,0,0,0 }, };
void MoveRight();
void MoveLeft();
void MoveUp();
void MoveDown();

void Print()
{
	printf("\t\t\t\t*Welcome to 2048*\n");
	printf("\t\t*use     to play; press \"esc\" to reset*\n");
	printf("\t\t\t┏━━━┳━━━┳━━━┳━━━┓\n");
	printf("\t\t\t┃%4d\t┃%4d\t┃%4d\t┃%4d\t┃\n", map[0][0], map[0][1], map[0][2], map[0][3]);
	printf("\t\t\t┃\t┃\t┃\t┃\t┃\n");
	printf("\t\t\t┣━━━╋━━━╋━━━╋━━━┫\n");
	printf("\t\t\t┃\t┃\t┃\t┃\t┃\n");
	printf("\t\t\t┃%4d\t┃%4d\t┃%4d\t┃%4d\t┃\n", map[1][0], map[1][1], map[1][2], map[1][3]);
	printf("\t\t\t┣━━━╋━━━╋━━━╋━━━┫\n");
	printf("\t\t\t┃\t┃\t┃\t┃\t┃\n");
	printf("\t\t\t┃%4d\t┃%4d\t┃%4d\t┃%4d\t┃\n", map[2][0], map[2][1], map[2][2], map[2][3]);
	printf("\t\t\t┣━━━╋━━━╋━━━╋━━━┫\n");
	printf("\t\t\t┃\t┃\t┃\t┃\t┃\n");
	printf("\t\t\t┃%4d\t┃%4d\t┃%4d\t┃%4d\t┃\n", map[3][0], map[3][1], map[3][2], map[3][3]);
	printf("\t\t\t┗━━━┻━━━┻━━━┻━━━┛\n");

}

void Add(int dir) //第一次进入界面 dir=0; 上 dir=1; 下 dir=2; 左 dir=3; 右 dir=4
{
	srand((unsigned int)time(NULL));	//随机数
	int num, flag;	//2,4
	int row, col;
	row = rand() % 4;	//0-3
	col = rand() % 4;	//0-3
	flag = rand() % 2;	//flag 0 1
	if (flag == 0)
		num = 2;
	else
		num = 4;
	switch (dir)
	{
	case 0:
		map[row][col] = num; break;
	case 1://上
		if (map[3][col] == 0)
		{
			map[3][col] = num;
		}
		else
		{
			for (col = 0; col < 4; col)
				if (map[3][col] == 0) break;
		}
		break;
	case 2://下
		if (map[0][col] == 0)
		{
			map[0][col] = num;
		}
		else
		{
			for (col = 0; col < 4; col++)
			{
				if (map[0][col] == 0) break;
			}
		}
		break;
	case 3://左
		if (map[row][14] == 0)
		{
			map[row][15] = num;
		}
		else
		{
			for (row = 0; row < 4; row++)
			{
				if (map[row][16] == 0) break;         /*有疑问！！！*/
			}
		}
		break;
	case 4://右
		if (map[row][0] == 0)
		{
			map[row][0] = num;
		}
		else
		{
			for (row = 0; row < 4; row++)
			{
				if (map[row][0] == 0) break;
			}
		}
		break;
	}
}

//控制函数
void Move()
{
	char ch;
	int row, col;
	ch = _getch();
	switch (ch)
	{
	case 72://上
		MoveUp();
		break;
	case 80://下
		MoveDown();
		break;
	case 75://左
		MoveLeft();
		break;
	case 77://右
		MoveRight();
		break;
	case 27://重置
	{
		for (row = 0; row < 4; row++)
		{
			for (col = 0; col < 4; col++)
			{
				map[row][col] = 0;
			}
		}
		Add(0);
	}
	}
	system("cls");
}

void MoveUp()
{
	int temp, row, col;
	for (col = 0; col < 4; col++)
	{
		int n = 4;
		while (n--)
		{
			for (row = 0; row < 3; row++)
			{
				if (map[row][col] == 0)
				{
					for (temp = row; temp < 3; temp++)
					{
						map[temp][col] = map[temp + 1][col];
						map[temp + 1][col] = 0;
					}
				}
			}
		}
		//实现加法
		for (row = 0; row < 3; row++)
		{
			if (map[row][col] == map[row + 1][col])
			{
				map[row][col] = map[row][col] * 2;
				map[row + 1][col] = 0;
				for (temp = row + 1; temp < 3; temp++)
				{
					map[temp][col] = map[temp + 1][col];
					map[temp + 1][col] = 0;
				}
			}
		}
	}
	Add(1);
}

void MoveDown()
{
	int row, col, temp;
	for (col = 0; col < 4; col++)
	{
		int n = 4;
		while (n--)
		{
			for (row = 3; row > 0; row--)
			{
				if (map[row][col] == 0)
				{
					for (temp = row; temp > 0; temp--)
					{
						map[temp][col] = map[temp - 1][col];
						map[temp - 1][col] = 0;
					}

				}
			}
		}
		//加法操作
		for (row = 3; row > 0; row--)
		{
			if (map[row][col] == map[row - 1][col])
			{
				map[row][col] = map[row][col] * 2;
				map[row - 1][col] = 0;
				for (temp = row - 1; temp > 0; temp--)
				{
					map[temp][col] = map[temp - 1][col];
					map[temp - 1][col] = 0;
				}
			}
		}
	}

	Add(2);
}

void MoveLeft()
{
	int row, col, temp;
	for (row = 0; row < 4; row++)
	{
		int n = 4;
		while (n--)
		{
			for (col = 0; col < 4; col++)
			{
				if (map[row][col] == 0)
				{
					for (temp = col; temp < 3; temp++)
					{
						map[row][temp] = map[row][temp + 1];
						map[row][temp + 1] = 0;
					}
				}
			}
		}
		//加法操作
		for (col = 0; col < 3; col++)
		{
			if (map[row][col] == map[row][col + 1])
			{
				map[row][col] = map[row][col] * 2;
				map[row][col + 1] = 0;
				for (temp = col + 1; temp < 3; temp++)
				{
					map[row][temp] = map[row][temp + 1];
					map[row][temp + 1] = 0;
				}
			}
		}
	}
	Add(3);
}

void MoveRight()
{
	int row, col, temp;
	for (row = 0; row < 4; row++)
	{
		int n = 4;
		while (n--)
		{
			for (col = 3; col > 0; col--)
			{
				if (map[row][col] == 0)
				{
					for (temp = col; temp > 0; temp--)
					{
						map[row][temp] = map[row][temp - 1];
						map[row][temp - 1] = 0;
					}
				}
			}
		}
		//加法操作
		for (col = 3; col > 0; col--)
		{
			if (map[row][col] == map[row][col - 1])
			{
				map[row][col] = map[row][col] * 2;
				map[row][col - 1] = 0;
				for (temp = col - 1; temp > 0; temp--)
				{
					map[row][temp] = map[row][temp - 1];
					map[row][temp - 1] = 0;
				}
			}
		}
	}
	Add(4);
}

int main(void)
{
	printf("程序正在启动，请稍后......\n");
	Add(0);
	printf("按任意键开始游戏\n");
	_getch();
	Print();
	while (1)
	{
		Move();
		Print();
	}
	return 0;
}
