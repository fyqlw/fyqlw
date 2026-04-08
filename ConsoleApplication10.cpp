#include <iostream>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
using namespace std;
// 方向定义
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

// 蛇身节点
struct Node {
    int x;
    int y;
    Node* next;
};

Node* head = nullptr;
int foodX, foodY;
int dir = RIGHT;//游戏开始方向向右边
int width = 30;
int height = 20;
bool gameOver = false;
int score1 = 0;
// 光标定位
void gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };//系统自带的结构体
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// 隐藏光标
void hideCursor() {
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 1;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

// 生成食物
void createFood() {
    srand((unsigned)time(NULL));
    foodX = rand() % (width - 2) + 1;
    foodY = rand() % (height - 2) + 1;
}

// 初始化蛇
void initSnake() {
    head = new Node();
    head->x = width / 2;
    head->y = height / 2;
    head->next = nullptr;

    for (int i = 0; i < 2; ++i) {
        Node* n = new Node();
        n->x = head->x - 1 - i;
        n->y = head->y - 1 - i;
        n->next = head;
        head = n;
    }
}

// 画墙
void drawWall() {
    for (int i = 0; i < width; ++i) {
        gotoxy(i, 0);
        cout << "#";
        gotoxy(i, height - 1);
        cout << "#";
    }
    for (int i = 0; i < height; ++i) {
        gotoxy(0, i);
        cout << "#";
        gotoxy(width - 1, i);
        cout << "#";
    }
}

// 画蛇
void drawSnake() {
    Node* p = head;
    while (p != nullptr) {
        gotoxy(p->x, p->y);
        cout << "O";
        p = p->next;
    }
}

// 擦蛇尾
void clearTail() {
    Node* p = head;
    if (p == nullptr) return;
    while (p->next != nullptr && p->next->next != nullptr)
        p = p->next;
    gotoxy(p->next->x, p->next->y);
    cout << " ";
    delete p->next;
    p->next = nullptr;
}

// 按键控制
//_kbhit:键盘检测函数，_getch:获取按键函数
void input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'w': if (dir != DOWN) dir = UP; break;
        case 's': if (dir != UP) dir = DOWN; break;
        case 'a': if (dir != RIGHT) dir = LEFT; break;
        case 'd': if (dir != LEFT) dir = RIGHT; break;
        case 27: gameOver = true; break;
        }
    }
}

// 移动
void move() {
    Node* newHead = new Node();
    newHead->x = head->x;
    newHead->y = head->y;

    switch (dir) {
    case UP: newHead->y--; break;
    case DOWN: newHead->y++; break;
    case LEFT: newHead->x--; break;
    case RIGHT: newHead->x++; break;
    }

    //穿墙
    if (newHead->x <= 0)
    {
        newHead->x = width - 2;
    }
    else if (newHead->x >= width - 1)
    {
        newHead->x = 1;
    }
    else if (newHead->y <= 0)
    {
        newHead->y = height -2;
    }
    else if (newHead->y >= height - 1)
    {
        newHead->y = 1;
    }
    // 撞墙
    /*if (newHead->x <= 0 || newHead->x >= width - 1 ||
        newHead->y <= 0 || newHead->y >= height - 1) {
        gameOver = true;
        delete newHead;
        return;
    }*/

    // 撞自己
    Node* p = head;
    while (p != nullptr) {
        if (p->x == newHead->x && p->y == newHead->y) {
            gameOver = true;
            delete newHead;
            return;
        }
        p = p->next;
    }

    newHead->next = head;
    head = newHead;

    // 吃到食物
    if (newHead->x == foodX && newHead->y == foodY) {
        createFood();
        score1++;
    }
    else {
        clearTail();
    }
}

void score()
{
    gotoxy(width + 3, height / 2);
    cout <<"分数:" << score1;
    
}
// 画食物
void drawFood() {
    gotoxy(foodX, foodY);
    cout << "*";
}

int main() {

    
        hideCursor();
        initSnake();
        createFood();
        drawWall();
        
        while (!gameOver) {
            drawSnake();
            drawFood();
            Sleep(150);
            input();
            move();
           score();
        }

        gotoxy(width / 2 - 5, height / 2);
        cout << "Game Over!";
        gotoxy(width / 2 - 4, height / 2-1);

        gotoxy(0, height);
      

    
    return 0;
}