/*************************************
项目名称: 计时器
编译环境: VS2015
建立时间: 2017-1-08
创建人:    PorYoung
*************************************/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

int h, m, s, e, d = 0;
void updata();
void display();
void delay();
void gotoxy(int x, int y);

void gotoxy(int x, int y)       //将光标移动到坐标为(x,y)的地方
{
    CONSOLE_SCREEN_BUFFER_INFO    csbiInfo;
    HANDLE    hConsoleOut;
    hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsoleOut, &csbiInfo);
    csbiInfo.dwCursorPosition.X = x;
    csbiInfo.dwCursorPosition.Y = y;
    SetConsoleCursorPosition(hConsoleOut, csbiInfo.dwCursorPosition);
}
/*这是功能实现的关键，必须要能在同一个位置显示才行( ⊙ o ⊙ )啊~*/

void updata()            //更新秒、分、时
{

    if (s == 60)
    {
        s = 0;
        m++;
    }
    if (m == 60)
    {
        m = 0;
        h++;
    }

    if (h == 24)
    {
        h = 0;
        d++;
        exit(0);
    }
}

void delay()
/*这里就是控制更新时间间隔为1s啦，考虑整个程序运行的时间，最后感觉sleep(9)误差小一些即延迟900ms，童鞋也可自测(*^__^*) 嘻嘻……*/
{
    int i;
    int j;

    for (i = 0; i <= 100; i++)
    {
        Sleep(9);
        //for(j=0;j<10000000;j++);
        switch (i / 10)
        {
        case 0: printf("\b\b\b0%d)", i); break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:printf("\b\b\b%d)", i); break;
        case 10:printf("\b\b\b00)"); s++; break;
        }
    }
    printf("\n\t\t╠点击屏幕暂停,按任意键继续\t\t\t    ╣");
    printf("\n\t\t╚─────────────────────────╝");
}

void display()                   //显示时间
{
    gotoxy(18, 3);          //成败在此一举~~~~
    if (h < 10) printf("\t\t      0%d\:", h);
    else printf("\n\t\t╠\t\t%d\:", h);
    if (m < 10) printf("0%d\:", m);
    else printf("%d\:", m);
    if (s < 10) printf("0%d", s);
    else printf("%d", s);
    printf("(00)");

}

int main(void)
{
    printf("\t\t╔┄┄┄┄┄┄┄┄┄*****计时器*****┄┄┄┄┄┄┄┄╗");
    printf("\n\t\t╠**************************************************╣");
    printf("\n\t\t╠时间输入:(时 分 秒)                               ╣");
    printf("\n\t\t╠                                                  ╣");
    gotoxy(18, 3);
    scanf_s("%d%d%d", &h, &m, &s);
    while (1)
    {
        display();
        delay();
        updata();
    }
    return 0;
}
