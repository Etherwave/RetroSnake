// 贪吃蛇3.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <graphics.h> //图形界面头文件
#include <time.h>
#include <conio.h>
#include <mmsystem.h>
#include<string>
#include<vector>
#pragma comment(lib,"winmm.lib")
#define SIZE 20
using namespace std;

int survival = 1;
int wallx=900, wally=600;

enum ways { UP = 72, DOWN = 80, LEFT = 75, RIGHT = 77 };  //上下左右四个方向

class position
{
	public:
		int x,y;
};
position tempp;

class Snake
{
	public:
		vector<position> body;
		ways direction;
		int getsize();
	private:
		int bodysize = SIZE;
};
Snake snake;
int Snake::getsize()
{
	return bodysize;
}

class Food
{
	public:
		position foodp;
		int life;
		int getsize();
	private:
		int foodsize = SIZE;
};
int Food::getsize()
{
	return foodsize;
}
Food food;


void GameInit()
{
	//打开一个窗口
	initgraph(wallx, wally);
	srand((unsigned int)time(NULL));
	setbkcolor(RGB(100, 100, 0));
	cleardevice(); //使用设置好的背景颜色刷新屏幕
	//蛇初始化
	snake.body.clear();
	tempp.x = 20;
	tempp.y = 20;
	snake.body.push_back(tempp);
	snake.direction = DOWN;
	food.life = 0; //初始化没有食物
}

void CreateFood()
{
	food.foodp.x = rand() % (wallx / food.getsize()-1)*food.getsize();
	food.foodp.y = rand() % (wally / food.getsize()-1)*food.getsize();

	food.life = 1;
}

void FoodPaint()
{
	setfillcolor(LIGHTRED);
	solidroundrect(food.foodp.x, food.foodp.y, food.foodp.x + food.getsize(),
		food.foodp.y + food.getsize(), food.getsize(), food.getsize());
}

void SnakePaint()
{
	static int cnt = 0;
	cnt++;
	if (cnt % 50 == 0)
		setbkcolor(RGB(rand() % 100 + 156, rand() % 100 + 156, rand() % 100 + 156));
	if (cnt > 500)cnt = 0;

	settextcolor(LIGHTRED);
	settextstyle(50, 0, L"宋体");
	int t = snake.body.size();
	string a;
	a = to_string(t);
	TCHAR wc[MAX_PATH];
	_stprintf_s(wc, MAX_PATH, _T("%S"), a.c_str());//%S宽字符 
	outtextxy(0, 0, wc);

	vector<position>::iterator it;

	for (it = snake.body.begin(); it != snake.body.end(); it++)
	{
		setfillcolor(RGB(rand() % 100 + 56, rand() % 100 + 56, rand() % 100 + 56));
		if(rand()%2==0)
		bar(it->x, it->y, it->x + snake.getsize(), it->y + snake.getsize());
		else
			solidroundrect(it->x, it->y, it->x + snake.getsize(), it->y + snake.getsize(),
				snake.getsize(), snake.getsize());
	}
}

void SnakeMove()
{
	tempp = *snake.body.begin();
	snake.body.erase(snake.body.end() - 1);
	switch (snake.direction)
	{
	case UP:
		tempp.y -= snake.getsize();
		break;
	case DOWN:
		tempp.y += snake.getsize();
		break;
	case LEFT:
		tempp.x -= snake.getsize();
		break;
	case RIGHT:
		tempp.x += snake.getsize();
		break;
	}
	snake.body.insert(snake.body.begin(), tempp);
}

void EatFood()
{
	if (snake.body.begin()->x == food.foodp.x&&snake.body.begin()->y == food.foodp.y)
	{
		tempp = *(snake.body.end() - 1);
		snake.body.push_back(tempp);
		food.life = 0;
	}
}

void ChangeSnakeCh()
{
	switch (_getch())
	{
	case UP:  //UP
		if (snake.direction != DOWN)
			snake.direction = UP;
		break;
	case DOWN:  //DOWN
		if (snake.direction != UP)
			snake.direction = DOWN;
		break;
	case RIGHT:
		if (snake.direction != LEFT)
			snake.direction = RIGHT;
		break;
	case LEFT:
		if (snake.direction != RIGHT)
			snake.direction = LEFT;
		break;
	case VK_SPACE:
		system("pause");
		break;
	}
}

void GameBreak()
{
	void OverGame();
	//撞墙 
	if (snake.body.begin()->y < -1 || snake.body.begin()->y+snake.getsize()>wally 
		|| snake.body.begin()->x < -1 || snake.body.begin()->x + snake.getsize()>wallx)
	{
		//退出
		OverGame();
	}
	//咬自己
	vector<position>::iterator it;
	for (it = snake.body.begin()+1; it != snake.body.end(); it++)
	{
		if (snake.body.begin()->x == it->x&&snake.body.begin()->y == it->y)
		{
			//退出
			OverGame();
			break;
		}
	}
}

void OverGame()
{
	survival = 0;
	cleardevice();
	settextcolor(LIGHTRED);
	settextstyle(50, 0, L"宋体");
	int t = snake.body.size();
	string a;
	a = to_string(t);
	a = "Bureau score:" + a+"!";
	TCHAR wc[MAX_PATH];
	_stprintf_s(wc, MAX_PATH, _T("%S"), a.c_str());//%S宽字符 
	outtextxy(wallx / 3-50, wally / 3-60, wc);
	settextcolor(RED);
	settextstyle(50, 0, L"宋体");
	outtextxy(wallx / 3, wally / 3, L"game over");
	settextcolor(GREEN);
	outtextxy(wallx / 3, wally / 3+50, L"R restart");
	time_t t1, t2;
	t1 = t2 = time(NULL);
	while (t2 - t1 < 4)
	{
		t2 = time(NULL);
		if (_getch() == 'r')
		{
			survival = 1;
			snake.body.clear();
			tempp.x = 20;
			tempp.y = 20;
			snake.body.push_back(tempp);
			snake.direction = DOWN;
			food.life = 0; //初始化没有食物
			cleardevice(); //使用设置好的背景颜色刷新屏幕
			break;
		}
	}
}

int main()
{
	GameInit();  //初始化的函数.
	mciSendString(L"open .\\甩葱歌.mp3 alias scg", 0, 0, 0);
	mciSendString(L"play scg repeat", 0, 0, 0);
	//按键控制 不按键->移动循环 按键-->退出移动循环-->改变方向-->回到移动循环 
	while (survival)
	{
		while (!_kbhit())  //kbhit(检测键盘是否有消息)
		{
			if (food.life == 0)//没有食物
				CreateFood();    //生成一个食物坐标
			cleardevice();     //刷新
			FoodPaint();       //绘制食物
			SnakePaint();      //绘制蛇
			SnakeMove();       //蛇移动
			Sleep(100);        //蛇的移动速度
			GameBreak();       //判断游戏是否结束
			EatFood();         //吃食物
		}
		ChangeSnakeCh();
	}
    return 0;
}

