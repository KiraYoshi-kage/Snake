#include<iostream>
#include<graphics.h>
#include<mmsystem.h>
#include<vector>
#include<ctime>
#include<conio.h>
#include<string>
#pragma comment(lib,"winmm.lib")

int speed = 200;
int point = 0;
int _time1 = 0, _time2 = 0;
int life = 3, num = 0;

//���ñ�������
void bgm()
{
	mciSendString("open ./music/WhatIsLove.mp3 alias BGM", 0, 0, 0);
	mciSendString("play BGM repeat", 0, 0, 0);
	mciSendString("setaudio BGM volume to 100", 0, 0, 0);

}

//��ʳ����Ч
void eatmusic()
{
	mciSendString("open ./music/eat.mp3 alias eat", 0, 0, 0);
	mciSendString("setaudio eat volume to 100", 0, 0, 0);
	mciSendString("play eat ", 0, 0, 0);
}

//���ý�������
void failmusic()
{
	mciSendString("open ./music/fail.mp3 alias fail", 0, 0, 0);
	mciSendString("setaudio fail volume to 100", 0, 0, 0);
	mciSendString("play fail ", 0, 0, 0);
}

//��Ϸ��ʾ
void tip()
{
	HWND h = GetHWnd();//���
	int isok = MessageBox(h, "�������ҿ��ƣ��ո����", "��ʾ����", MB_OK);
}

//��������
class Node
{
public:
	Node() :Node(0, 0, WHITE) {};
	Node(int x, int y, COLORREF color) :m_x(x), m_y(y), m_color(color) {};

	void draw()//������
	{
		setlinecolor(BLACK);
		setlinestyle(SOLID_FILL, 2);
		setfillcolor(m_color);
		fillrectangle(m_x, m_y, m_x + 10, m_y + 10);
	}

	//������������Ƿ���ײ
	bool collision(const Node& other1, const Node& other2)
	{
		return other1.m_x == other2.m_x && other1.m_y == other2.m_y;
	}

	//����λ��
	void move(int dx, int dy)
	{
		m_x += dx;
		m_y += dy;
		if (m_x == -10)m_x = 490;
		if (m_x == 500)m_x = 0;
		if (m_y == -10)m_y = 490;
		if (m_y == 500)m_y = 0;
	}


protected:
	int m_x;
	int m_y;
	COLORREF m_color;
};

//��
class Snake :public Node
{
public:
	Snake() :Snake(0, 0) {};
	Snake(int x, int y) :Node(x, y, WHITE) {};

	int dir = VK_LEFT;//����

	//��ʼ��
	void creat()
	{
		nodes.push_back(Snake(50, 50));
		nodes.push_back(Snake(60, 50));
		nodes.push_back(Snake(70, 50));
	}

	//�������
	void add()
	{
		nodes.push_back(Snake(5000, 5000));
	}

	void dec()
	{
		nodes.pop_back();
	}

	//�����˶�
	void link()
	{
		switch (dir)
		{
		case VK_UP:
			nodes[0].move(0, -10);
			break;
		case VK_DOWN:
			nodes[0].move(0, 10);
			break;
		case VK_LEFT:
			nodes[0].move(-10, 0);
			break;
		case VK_RIGHT:
			nodes[0].move(10, 0);
			break;

		default:break;
		}
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i].draw();
		}

		for (int i = nodes.size() - 1; i > 0; i--)
		{
			nodes[i] = nodes[i - 1];
		}
	}

	//����ͷ��
	Node head()
	{
		return nodes[0];
	}

	//������ײ
	void poom()
	{
		for (int i = 3; i < nodes.size(); i++)
		{
			if (collision(nodes[0], nodes[i]))
			{
				point = nodes.size();
				life = 0;
			}
		}
	}

protected:
	std::vector<Node> nodes;
};

//ʳ��
class Food :public Node
{
public:
	Food()//��ʼ��
	{
		f();
	}

	void f()
	{
		m_x = 10 + rand() % 48 * 10;
		m_y = 10 + rand() % 48 * 10;
		m_color = RED;
	}

};

//ʳ��
class Bomb :public Node
{
public:
	Bomb()//��ʼ��
	{
		f();
	}

	void f()
	{
		m_x = 10 + rand() % 48 * 10;
		m_y = 10 + rand() % 48 * 10;
		m_color = BLACK;
	}

};

void _life();

//��Ϸ����
class Gameback :public Snake
{
private:
	Snake snake;
	std::vector<Food> food;
	std::vector<Bomb> bomb;
public:

	Gameback()
	{
		food.push_back(Food());
		food.push_back(Food());
		food.push_back(Food());
		bomb.push_back(Bomb());
	}
	void a()
	{
		snake.creat();
	}

	void fbdraw()
	{
		for (int i = 0; i < food.size(); i++)
		{
			food[i].draw();
		}

		for (int i = 0; i < bomb.size(); i++)
		{
			bomb[i].draw();
		}
	}

	void eatfb()
	{
		for (int i = 0; i < food.size(); i++)
		{
			if (collision(snake.head(), food[i]))
			{
				snake.add();
				food[i].f();
				eatmusic();
				_time1 = 0;
				num++;
			}
		}

		for (int i = 0; i < bomb.size(); i++)
		{
			if (collision(snake.head(), bomb[i]))
			{
				snake.dec();
				bomb[i].f();
				mciSendString("open ./music/bomb.mp3 alias bomb", 0, 0, 0);
				mciSendString("setaudio bomb volume to 100", 0, 0, 0);
				mciSendString("play bomb ", 0, 0, 0);
				life--;
				_time2 = 0;
			}
		}
	}

	void run()
	{
		BeginBatchDraw();
		cleardevice();
		snake.link();
		fbdraw();
		eatfb();
		snake.poom();
		_life();
		EndBatchDraw();

		if (num == 5)
		{
			num = 0;
			bomb.push_back(Bomb());
		}

		ExMessage msg = { 0 };

		while (peekmessage(&msg, EM_KEY))
		{
			GetMsg(msg);
		}

		if (GetAsyncKeyState(32) & 0x80000 ? 1 : 0)
		{
			speed = 20;
		}
		else speed = 200;
	}



	void GetMsg(const ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
		{
			if (msg.vkcode == VK_UP || msg.vkcode == VK_DOWN || msg.vkcode == VK_LEFT || msg.vkcode == VK_RIGHT)
			{
				if (snake.dir == VK_UP && msg.vkcode != VK_DOWN)snake.dir = msg.vkcode;
				if (snake.dir == VK_DOWN && msg.vkcode != VK_UP)snake.dir = msg.vkcode;
				if (snake.dir == VK_LEFT && msg.vkcode != VK_RIGHT)snake.dir = msg.vkcode;
				if (snake.dir == VK_RIGHT && msg.vkcode != VK_LEFT)snake.dir = msg.vkcode;
			}
		}
	}
};

//����
void _life()
{
	int x = 50;
	int y = 50;

	settextcolor(BLACK);
	settextstyle(13,0,"��������");
	outtextxy(0, 50, "����ֵ��");
	outtextxy(0, 0, "�÷�Ϊ��");

	for (int i = 0; i < life; i++)
	{
		setfillcolor(RED);
		fillrectangle(x, y, x + 5, y + 5);
		x += 10;
	}
}

//��Ϸ����
void gameover()
{
	std::string a = "�÷�Ϊ��" + std::to_string(point);
	const TCHAR* c = a.c_str();

	settextcolor(BLACK);
	settextstyle(100, 0, "Consolas");
	outtextxy(60, 100, "gameover");
	mciSendString("close BGM", 0, 0, 0);
	failmusic();

	settextstyle(50, 0, "Consolas");
	outtextxy(100, 250, c);
}

//��Ϸ����
void gamerun()
{
	initgraph(500, 500);
	setbkcolor(WHITE);
	cleardevice();
	tip();
	bgm();
	srand(time(0));
	Gameback scene;
	scene.a();
	while (1)
	{

		scene.run();

		Sleep(speed);
		if (life == 0)
		{
			cleardevice();
			gameover();
			break;
		}
		_time1++;
		_time2++;
		if (_time1 == 5)mciSendString("close eat", 0, 0, 0);
		if (_time2 == 5)mciSendString("close bomb", 0, 0, 0);

	}

	system("pause");
	closegraph();
}

int main()
{
	gamerun();
	return 0;
}