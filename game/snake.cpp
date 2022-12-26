#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
// 画布参数
const int N = 10;	// 行数
const int M = 10;	// 列数
const int L = 40;	// 像素点大小
const int T = 400;	// 间隔
// 方向枚举
enum Direction
{
	RIGHT = 39,
	LEFT = 37,
	DOWN = 40,
	UP = 38,
};
// 点结构体
struct Point
{
	int x, y;
	int X() {return x*L;}
	int Y() {return y*L;}
};
// 蛇结构体
struct Snake
{
	int num;
	Point body[N*M];
	Direction dir;
} snake;
// 食物结构体
struct Food
{
	Point pos;
} food;
// 功能函数
void initSnake();	// 初始化小蛇
void initFood();	// 初始化食物
void drawBoard();	// 绘制网格
void drawSnake();	// 绘制小蛇
void drawFood();	// 绘制食物
void drawAll();		// 全部绘制
void snakeMove();	// 小蛇移动
void eatFood();		// 吃到食物
bool snakeDie();	// 小蛇撞死
void keyDown();		// 按键处理

// -------------------主函数----------------------
int main()
{
	initgraph(M*L, N*L);
	begin:
	initSnake();
	initFood();
	while (true) {
		if (kbhit()) {	// 监听键盘按键
			keyDown();
		}
		snakeMove();
		if (snakeDie()) {
			MessageBox(getHWnd(),"Game Over!","rePlay",MB_OK);
			goto begin;
		}
		eatFood();
		drawAll();
		Sleep(T);
	}
	closegraph();
	return 0;
}

void initSnake()
{
	snake.num = 3;
	snake.dir = RIGHT;
	snake.body[2].x = 1;
	snake.body[2].y = 1;
	snake.body[1].x = 2;
	snake.body[1].y = 1;
	snake.body[0].x = 3;
	snake.body[0].y = 1;
}

void initFood()
{
	bool flag;
	do {
		flag = false;
		food.pos.x = rand()%(M-2) + 1;
		food.pos.y = rand()%(N-2) + 1;
		// 如果食物在蛇身上则重新投放
		for (int i = 0; i < snake.num; i++) {
			int x = snake.body[i].x, y = snake.body[i].y;
			if (x == food.pos.x && y == food.pos.y) {
				flag = true;
				break;
			}
		}
	} while (flag);
}

void drawBoard()
{
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			setcolor(BLUE);
			setfillcolor(BLACK);
			fillrect(i*L, j*L, i*L+L, j*L+L);
		}
	}
	for (int i = 0; i < N; i++) {
		setcolor(BLUE);
		setfillcolor(EGERGB(0, 0, 139));
		fillrect(0, i*L, L, i*L+L);
		fillrect((M-1)*L, i*L, M*L, i*L+L);
	}
	for (int i = 0; i < M; i++) {
		setcolor(BLUE);
		setfillcolor(EGERGB(0, 0, 139));
		fillrect(i*L, 0, i*L+L, L);
		fillrect(i*L, (N-1)*L, i*L+L, N*L);
	}
}

void drawSnake()
{
	for (int i = 0; i < snake.num; i++) {
		setcolor(BLUE);
		setfillcolor(EGERGB(0,100,0));
		fillrect(snake.body[i].X(),snake.body[i].Y(),snake.body[i].X()+L,snake.body[i].Y()+L);
	}
}

void drawFood()
{
	setcolor(BLUE);
	setfillcolor(RED);
	fillrect(food.pos.X(),food.pos.Y(),food.pos.X()+L,food.pos.Y()+L);
}

void drawAll()
{
	drawBoard();
	drawFood();
	drawSnake();
}

void snakeMove()
{
	// 更新蛇身
	for (int i = snake.num-1; i > 0; i--) {
		snake.body[i].x = snake.body[i-1].x;
		snake.body[i].y = snake.body[i-1].y;
	}
	// 更新蛇头
	switch (snake.dir) {
	case RIGHT:
		snake.body[0].x++;
		break;
	case LEFT:
		snake.body[0].x--;
		break;
	case DOWN:
		snake.body[0].y++;
		break;
	case UP:
		snake.body[0].y--;
		break;
	}
}

void eatFood()
{
	int x = snake.body[0].x;
	int y = snake.body[0].y;
	if (food.pos.x == x && food.pos.y == y) {
		// 食物重新投放
		bool flag;
		do {
			flag = false;
			food.pos.x = rand()%(M-2) + 1;
			food.pos.y = rand()%(N-2) + 1;
			for (int i = 0; i < snake.num; i++) {
				int x = snake.body[i].x, y = snake.body[i].y;
				if (x == food.pos.x && y == food.pos.y) {
					flag = true;
					break;
				}
			}
		} while (flag);
		// 蛇身增长
		snake.body[snake.num].x = snake.body[snake.num-1].x;
		snake.body[snake.num].y = snake.body[snake.num-1].y;
		snake.num++;
	}
}

bool snakeDie()
{
	int x = snake.body[0].x;
	int y = snake.body[0].y;
	// 超出边框
	if (x >= (M-1) || y >= (N-1) || x <= 0 || y <= 0) {
		return true;
	}
	// 迟到自身
	for (int i = 1; i < snake.num; i++) {
		if (x == snake.body[i].x && y == snake.body[i].y) {
			return true;
		}
	}
	return false;
}

void keyDown()
{
	char userKey = 0;
	userKey = getch();
	switch (userKey) {
	case LEFT:
		if (snake.dir != RIGHT) {
			snake.dir = LEFT;
		}
		break;
	case RIGHT:
		if (snake.dir != LEFT) {
			snake.dir = RIGHT;
		}
		break;
	case UP:
		if (snake.dir != DOWN) {
			snake.dir = UP;
		}
		break;
	case DOWN:
		if (snake.dir != UP) {
			snake.dir = DOWN;
		}
	}
}
