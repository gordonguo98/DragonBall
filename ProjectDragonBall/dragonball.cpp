#include<iostream>
#include<graphics.h>
#include<Windows.h>
#include<cstdio>
#include<conio.h>
#include<cstdlib>
#include<fstream>
#include<string>
#include<stack>
#include<vector>
#include<ctime>
#include<thread>
#include"definition.h"
#include"dragonball.h"
#include"mmsystem.h"
#pragma comment(lib,"WINMM.LIB")

/*************************************************************************结构体****************************************************************************/
// 玩家的结构体
struct Hero
{
	int x;//主角的当前坐标（在屏幕上的坐标）
	int y;
	int vx0;//主角的坐标（double型）
	int vy0;
	int x0;//原点坐标
	int y0;
	double vX;//速度
	double vY;
	int turn;//主角运动的方向
	int direction;
	bool isShoot;//主角是否可以射击
	bool died;//主角是否死亡
	bool ending;//主角是否马上就要过关
	bool passed;//主角是否过关
	double blood=100.00;
};

//地图的结构体
struct Map
{
	int mapx;
	int mapy;
	int mapID;
};

//龙珠的结构体
struct LZ
{
	int x;
	int y;
	int id;
	int state = 1;
};

//食物的结构体
struct TLS
{
	int x;
	int y;
	int state;
};

//子弹的结构体
struct ZD
{
	int x;
	int y;
	int state;
	bool touchgw;
	bool touchmap;
	int direction;
	int birthtime;
	int gw;
	int isjust;//就在刚刚
};

/*************************************************************************变量部分**************************************************************************/

//初始化玩家
Hero myHero = { 2 * MAZE_UNIT,2 * MAZE_UNIT,2 * MAZE_UNIT,2 * MAZE_UNIT,0,0,6,6,0,0,false,false,false,false,100.00 };
//初始化食物
TLS tls = { 0,0,1 };
//用于表示七颗龙珠
LZ lz[7];
//用于表示子弹
ZD zd[30];
//简单模式，困难模式的怪物数组
gw gw_easy[GWNUM_EASY];
gw gw_diff[GWNUM_DIFF];

DWORD* p_Screen;
MOUSEMSG mmsg;
int mouseX, mouseY;
bool isMouseDown;
int keyState;//播放音乐
int prekeyState;
int ispose;//是否暂停
int preispose;
int countBall=0;//标记已找到的龙珠
int getball[8];
int istouchball=0;

RECT r_mainmenu;
RECT r_pose;
RECT r_music;
RECT r_role;
RECT r_exit;

IMAGE tlsimg;
IMAGE tlsymtimg;

DWORD buff_Screen[WIN_WIDTH*WIN_HEIGHT];
Map easymap[EASY_REAL_HEIGHT][EASY_REAL_WIDTH];//用于保存地图信息
Map diffmap[DIFF_REAL_HEIGHT][DIFF_REAL_WIDTH];

//定义图片变量
IMAGE roleimg1, roleimg2, roleimg3, roleimg4, roleimg5, roleimg6, roleimg7, roleimg8, roleimg9, roleimg0
, roleimg11, roleimg12, roleimg13, roleimg14, roleimg15, roleimg16, roleimg17, roleimg18, roleimg19, roleimg20, roleimg21, roleimg22
, ball1, ball2, ball3, ball4, ball5, ball6, ball7, ball11, ball22, ball33, ball44, ball55, ball66, ball77, enemyimg, bomb, bombymt, bullet, bulletymt,gwblood,gwbloodymt;

//生成原始地图
int map0[EASY_REAL_HEIGHT][EASY_REAL_WIDTH];
int map1[DIFF_REAL_HEIGHT][DIFF_REAL_WIDTH];

//原始地图的起点坐标
int x = MAZE_HEIGHT - 2;
int y = MAZE_WIDTH - 2;

int status = STAYING;
int STEP = MAZE_UNIT / 2;//玩家走一步的像素大小
int jsd_a = 4;
int zljsd_a = 2;

int hero_iframe = 0;
int enemy_iframe[10] = {};
int radar_iframe = 1;
int bullet_iframe[30] = {};
int score;
int yfszds = 0;//已发射子弹数
int zdoldtime;
int countball = 0;

/*************************************************************************函数部分**************************************************************************/

//初始化
void init()
{
	loadimage(&bomb, _T("..\\res\\bomb.bmp"));
	loadimage(&bombymt, _T("..\\res\\bombymt.bmp"));
	loadimage(&bullet, _T("..\\res\\bullet.bmp"));
	loadimage(&bulletymt, _T("..\\res\\bulletymt.bmp"));
	loadimage(&gwblood, _T("..\\res\\gwblood.bmp"));
	loadimage(&gwbloodymt, _T("..\\res\\gwbloodymt.bmp"));
	keyState = MUSICON;
	mciSendString("open ..\\res\\背景音乐.mp3 alias bgyy", NULL, 0, NULL);
	mciSendString("open ..\\res\\胜利.mp3 alias sl", NULL, 0, NULL);
	mciSendString("open ..\\res\\游戏结束.mp3 alias yxjs", NULL, 0, NULL);
	mciSendString("open ..\\res\\脚步声.mp3 alias jbs", NULL, 0, NULL);
	mciSendString("open ..\\res\\子弹.mp3 alias zd", NULL, 0, NULL);
	mciSendString("open ..\\res\\子弹打到敌人.mp3 alias zddddr", NULL, 0, NULL);
	mciSendString("open ..\\res\\子弹撞墙.mp3 alias zdzq", NULL, 0, NULL);
	mciSendString("open ..\\res\\获得龙珠.mp3 alias hdlz", NULL, 0, NULL);
	mciSendString("open ..\\res\\点击菜单.mp3 alias djcd", NULL, 0, NULL);
	mciSendString("play bgyy repeat", NULL, 0, NULL);

	ispose = 0;

	r_mainmenu.left = 1015;
	r_mainmenu.top = 25;
	r_mainmenu.right = r_mainmenu.left + 60;
	r_mainmenu.bottom = r_mainmenu.top + 20;

	r_pose.left = 1015;
	r_pose.top = 93;
	r_pose.right = r_pose.left + 60;
	r_pose.bottom = r_pose.top + 20;

	r_music.left = 1015;
	r_music.top = 161;
	r_music.right = r_music.left + 60;
	r_music.bottom = r_music.top + 20;

	r_role.left = 1015;
	r_role.top = 229;
	r_role.right = r_role.left + 60;
	r_role.bottom = r_role.top + 20;

	r_exit.left = 1015;
	r_exit.top = 297;
	r_exit.right = r_exit.left + 60;
	r_exit.bottom = r_exit.top + 20;
}
//加载怪物
void cshtx()//初始化怪物
{
	loadimage(&roleimg1, _T("..\\res\\BLS.bmp"));
	loadimage(&roleimg2, _T("..\\res\\BLSymt.bmp"));
	loadimage(&roleimg3, _T("..\\res\\HDS.bmp"));
	loadimage(&roleimg4, _T("..\\res\\HDSymt.bmp"));
	loadimage(&roleimg5, _T("..\\res\\HLS.bmp"));
	loadimage(&roleimg6, _T("..\\res\\HLSymt.bmp"));
	loadimage(&roleimg7, _T("..\\res\\HMS.bmp"));
	loadimage(&roleimg8, _T("..\\res\\HMSymt.bmp"));
	loadimage(&roleimg9, _T("..\\res\\JNG.bmp"));
	loadimage(&roleimg0, _T("..\\res\\JNGymt.bmp"));
	loadimage(&roleimg11, _T("..\\res\\LL.bmp"));
	loadimage(&roleimg12, _T("..\\res\\LLymt.bmp"));
	loadimage(&roleimg13, _T("..\\res\\MMC.bmp"));
	loadimage(&roleimg14, _T("..\\res\\MMCymt.bmp"));
	loadimage(&roleimg15, _T("..\\res\\PHL.bmp"));
	loadimage(&roleimg16, _T("..\\res\\PHLymt.bmp"));
	loadimage(&roleimg17, _T("..\\res\\YGS.bmp"));
	loadimage(&roleimg18, _T("..\\res\\YGSymt.bmp"));
	loadimage(&roleimg19, _T("..\\res\\DS.bmp"));
	loadimage(&roleimg20, _T("..\\res\\DSymt.bmp"));
}
//加载龙珠
void cshlz()
{
	loadimage(&ball1, _T("..\\res\\lz1.bmp"));
	loadimage(&ball2, _T("..\\res\\lz2.bmp"));
	loadimage(&ball3, _T("..\\res\\lz3.bmp"));
	loadimage(&ball4, _T("..\\res\\lz4.bmp"));
	loadimage(&ball5, _T("..\\res\\lz5.bmp"));
	loadimage(&ball6, _T("..\\res\\lz6.bmp"));
	loadimage(&ball7, _T("..\\res\\lz7.bmp"));
	loadimage(&ball11, _T("..\\res\\lz1ymt.bmp"));
	loadimage(&ball22, _T("..\\res\\lz2ymt.bmp"));
	loadimage(&ball33, _T("..\\res\\lz3ymt.bmp"));
	loadimage(&ball44, _T("..\\res\\lz4ymt.bmp"));
	loadimage(&ball55, _T("..\\res\\lz5ymt.bmp"));
	loadimage(&ball66, _T("..\\res\\lz6ymt.bmp"));
	loadimage(&ball77, _T("..\\res\\lz7ymt.bmp"));
}
// 复制屏幕到缓存  
void CopyScreenBuffer()
{
	for (int i = 0; i<WIN_WIDTH*WIN_HEIGHT; i++)
	{
		buff_Screen[i] = p_Screen[i];
	}
}
// 绘制缓存到屏幕  
void DrawScreenBuffer()
{
	for (int i = 0; i < WIN_WIDTH*WIN_HEIGHT; i++)
	{
		p_Screen[i] = buff_Screen[i];
	}
}
// 绝对延时  
void Delay(DWORD ms)
{
	static DWORD oldTime = GetTickCount();
	while (GetTickCount() - oldTime < ms)
		Sleep(1);
	oldTime = GetTickCount();
}
//游戏介绍
void dragonball::yxjs()
{
	//mciSendString("pause bjyy",NULL,0,NULL);
	//keyState = MUSICOFF;
	IMAGE yxjsimg,yxjsymtimg;
	loadimage(&yxjsimg, _T("..\\res\\游戏介绍.bmp"));
	loadimage(&yxjsymtimg, _T("..\\res\\游戏介绍ymt.bmp"));
	putimage(171, 0, &yxjsymtimg,SRCAND);
	putimage(171, 0, &yxjsimg, SRCPAINT);
	FlushBatchDraw();
}
//人物介绍
void dragonball::rwjs()
{
	IMAGE rwjsimg, rwjsymtimg;
	loadimage(&rwjsimg, _T("..\\res\\人物介绍.bmp"));
	loadimage(&rwjsymtimg, _T("..\\res\\人物介绍ymt.bmp"));
	putimage(171, 0, &rwjsymtimg, SRCAND);
	putimage(171, 0, &rwjsimg, SRCPAINT);
	FlushBatchDraw();
}
//是否接触
bool isTouch(POINT *p1, POINT *p2)
{
	for (int i = 0; i < 4; i++)
	{
		if (p1[i].x > p2[0].x&&p1[i].y > p2[0].y&&p1[i].x < p2[1].x&&p1[i].y < p2[1].y)
		{
			return true;
		}
	}
	return false;
}
//判断主角是否接触到地图块
bool touchmap(Hero& role, Map map[][EASY_REAL_WIDTH])
{
	POINT m[2];
	POINT r[4];
	r[0].x = -myHero.x0 + role.x + 1;
	r[0].y = -myHero.y0 + role.y + 1;
	r[1].x = -myHero.x0 + role.x + MAZE_UNIT - 1;
	r[1].y = -myHero.y0 + role.y + 1;
	r[2].x = -myHero.x0 + role.x + 1;
	r[2].y = -myHero.y0 + role.y + MAZE_UNIT - 1;
	r[3].x = -myHero.x0 + role.x + MAZE_UNIT - 1;
	r[3].y = -myHero.y0 + role.y + MAZE_UNIT - 1;

	for (int i = 0; i < EASY_REAL_HEIGHT ; i++)
	{
		for (int j = 0; j < EASY_REAL_WIDTH ; j++)
		{
			if (map[i][j].mapID == WALL||map[i][j].mapID==BORDER)
			{
				m[0].x = map[i][j].mapx*MAZE_UNIT;
				m[0].y = map[i][j].mapy*MAZE_UNIT;
				m[1].x = (map[i][j].mapx + 1)*MAZE_UNIT;
				m[1].y = (map[i][j].mapy + 1)*MAZE_UNIT;
				if (isTouch(r, m))
					return true;
			}
		}
	}
	return false;
}
bool touchmap_diff(Hero& role, Map map[][DIFF_REAL_WIDTH])
{
	POINT m[2];
	POINT r[4];
	r[0].x = -myHero.x0 + role.x + 1;
	r[0].y = -myHero.y0 + role.y + 1;
	r[1].x = -myHero.x0 + role.x + MAZE_UNIT - 1;
	r[1].y = -myHero.y0 + role.y + 1;
	r[2].x = -myHero.x0 + role.x + 1;
	r[2].y = -myHero.y0 + role.y + MAZE_UNIT - 1;
	r[3].x = -myHero.x0 + role.x + MAZE_UNIT - 1;
	r[3].y = -myHero.y0 + role.y + MAZE_UNIT - 1;

	for (int i = 0; i < DIFF_REAL_HEIGHT; i++)
	{
		for (int j = 0; j < DIFF_REAL_WIDTH; j++)
		{
			if (map[i][j].mapID == WALL || map[i][j].mapID == BORDER)
			{
				m[0].x = map[i][j].mapx*MAZE_UNIT;
				m[0].y = map[i][j].mapy*MAZE_UNIT;
				m[1].x = (map[i][j].mapx + 1)*MAZE_UNIT;
				m[1].y = (map[i][j].mapy + 1)*MAZE_UNIT;
				if (isTouch(r, m))
					return true;
			}
		}
	}
	return false;
}
//判断怪物是否接触到地图块
bool touchmap_gw(gw& role, Map map[][EASY_REAL_WIDTH])
{
	if (role.status == 1)
	{
		POINT m[2];
		POINT r[4];
		r[0].x = role.x + 1;
		r[0].y = role.y + 1;
		r[1].x = role.x + MAZE_UNIT - 1;
		r[1].y = role.y + 1;
		r[2].x = role.x + 1;
		r[2].y = role.y + MAZE_UNIT - 1;
		r[3].x = role.x + MAZE_UNIT - 1;
		r[3].y = role.y + MAZE_UNIT - 1;

		for (int i = 0; i < EASY_REAL_HEIGHT; i++)
		{
			for (int j = 0; j < EASY_REAL_WIDTH; j++)
			{
				if (map[i][j].mapID == WALL || map[i][j].mapID == BORDER)
				{
					m[0].x = map[i][j].mapx*MAZE_UNIT;
					m[0].y = map[i][j].mapy*MAZE_UNIT;
					m[1].x = (map[i][j].mapx + 1)*MAZE_UNIT;
					m[1].y = (map[i][j].mapy + 1)*MAZE_UNIT;
					if (isTouch(r, m))
						return true;
				}
			}
		}
		return false;
	}
}
bool touchmap_gw_diff(gw& role, Map map[][DIFF_REAL_WIDTH])
{
	if (role.status == 1)
	{
		POINT m[2];
		POINT r[4];
		r[0].x = role.x + 1;
		r[0].y = role.y + 1;
		r[1].x = role.x + MAZE_UNIT - 1;
		r[1].y = role.y + 1;
		r[2].x = role.x + 1;
		r[2].y = role.y + MAZE_UNIT - 1;
		r[3].x = role.x + MAZE_UNIT - 1;
		r[3].y = role.y + MAZE_UNIT - 1;

		for (int i = 0; i < DIFF_REAL_HEIGHT; i++)
		{
			for (int j = 0; j < DIFF_REAL_WIDTH; j++)
			{
				if (map[i][j].mapID == WALL || map[i][j].mapID == BORDER)
				{
					m[0].x = map[i][j].mapx*MAZE_UNIT;
					m[0].y = map[i][j].mapy*MAZE_UNIT;
					m[1].x = (map[i][j].mapx + 1)*MAZE_UNIT;
					m[1].y = (map[i][j].mapy + 1)*MAZE_UNIT;
					if (isTouch(r, m))
						return true;
				}
			}
		}
		return false;
	}
}
//判断是否接触到龙珠
void touchBall()
{
	if (countBall <= 7)
	{
		for (int i = 0; i < 7; i++)
		{
			if (lz[i].state==1&&-myHero.x0 + myHero.x+1 >= lz[i].x && -myHero.x0 + myHero.x+1 <= lz[i].x + MAZE_UNIT
				&& -myHero.y0 + myHero.y+1 >= lz[i].y && -myHero.y0 + myHero.y+1 <= lz[i].y+ MAZE_UNIT)
			{
				mciSendString("play hdlz from 0", NULL, 0, NULL);
				countBall++;
				lz[i].state = 0;
			}
			else if (lz[i].state == 1 && -myHero.x0 + (myHero.x + MAZE_UNIT)-1 >= lz[i].x && -myHero.x0 + (myHero.x + MAZE_UNIT)-1 <= lz[i].x + MAZE_UNIT
				&& (-myHero.y0 + myHero.y)+1 >= lz[i].y && (-myHero.y0 + myHero.y)+1 <= lz[i].y + MAZE_UNIT)
			{
				mciSendString("play hdlz from 0", NULL, 0, NULL);
				countBall++;
				lz[i].state = 0;
			}
			else if (lz[i].state == 1 && -myHero.x0 + (myHero.x + MAZE_UNIT)-1 >= lz[i].x && -myHero.x0 + (myHero.x + MAZE_UNIT)-1 <= lz[i].x + MAZE_UNIT
				&& -myHero.y0 + (myHero.y + MAZE_UNIT)-1 >= lz[i].y && -myHero.y0 + (myHero.y + MAZE_UNIT)-1 <= lz[i].y + MAZE_UNIT)
			{
				mciSendString("play hdlz from 0", NULL, 0, NULL);
				countBall+=
				lz[i].state = 0;
			}
			else if (lz[i].state == 1 && -myHero.x0 + myHero.x +1>= lz[i].x && -myHero.x0 + myHero.x+1 <= lz[i].x + MAZE_UNIT
				&& -myHero.y0 + (myHero.y + MAZE_UNIT)-1 >= lz[i].y && -myHero.y0 + (myHero.y + MAZE_UNIT)-1 <= lz[i].y + MAZE_UNIT)
			{
				mciSendString("play hdlz from 0", NULL, 0, NULL);
				countBall++;
				lz[i].state = 0;
			}
		}
	}
}
//判断主角是否接触到怪物
void touchGw(int mapkind)
{
	POINT m[2];
	POINT r[4];
	r[0].x = -myHero.x0 + myHero.x + 1;
	r[0].y = -myHero.y0 + myHero.y + 1;
	r[1].x = -myHero.x0 + myHero.x + MAZE_UNIT - 1;
	r[1].y = -myHero.y0 + myHero.y + 1;
	r[2].x = -myHero.x0 + myHero.x + 1;
	r[2].y = -myHero.y0 + myHero.y + MAZE_UNIT - 1;
	r[3].x = -myHero.x0 + myHero.x + MAZE_UNIT - 1;
	r[3].y = -myHero.y0 + myHero.y + MAZE_UNIT - 1;
	if (mapkind == EASY)
	{
		for (int i = 0; i < GWNUM_EASY; i++)
		{
			if (gw_easy[i].status == 1)
			{
				m[0].x = gw_easy[i].x;
				m[0].y = gw_easy[i].y;
				m[1].x = gw_easy[i].x + MAZE_UNIT;
				m[1].y = gw_easy[i].y + MAZE_UNIT;
				if (isTouch(r, m))
				{
					myHero.blood -= 2;
					if (myHero.blood < 0)
						myHero.died = true;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < GWNUM_DIFF; i++)
		{
			if (gw_easy[i].status == 1)
			{
				m[0].x = gw_diff[i].x;
				m[0].y = gw_diff[i].y;
				m[1].x = gw_diff[i].x + MAZE_UNIT;
				m[1].y = gw_diff[i].y + MAZE_UNIT;
				if (isTouch(r, m))
				{
					myHero.blood -= 4;
					if (myHero.blood < 0)
						myHero.died = true;
				}
			}
		}
	}
}
//判断主角是否接触到食物
void touchTLS()
{
	POINT m[2];
	POINT r[4];
	r[0].x = -myHero.x0 + myHero.x + 1;
	r[0].y = -myHero.y0 + myHero.y + 1;
	r[1].x = -myHero.x0 + myHero.x + MAZE_UNIT - 1;
	r[1].y = -myHero.y0 + myHero.y + 1;
	r[2].x = -myHero.x0 + myHero.x + 1;
	r[2].y = -myHero.y0 + myHero.y + MAZE_UNIT - 1;
	r[3].x = -myHero.x0 + myHero.x + MAZE_UNIT - 1;
	r[3].y = -myHero.y0 + myHero.y + MAZE_UNIT - 1;
	m[0].x = tls.x;
	m[0].y = tls.y;
	m[1].x = tls.x + MAZE_UNIT;
	m[1].y = tls.y + MAZE_UNIT;
	if (tls.state==1&&isTouch(r, m))
	{
		mciSendString("play hdlz from 0", NULL, 0, NULL);
		tls.state = 0;
		myHero.isShoot = true;
	}
}
//构造函数
dragonball::dragonball()
{

}
//欢迎界面
void dragonball::welcome()
{
	//隐藏光标
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = FALSE;
	cci.dwSize = sizeof(cci);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(handle, &cci);

	IMAGE img1,img2;
	loadimage(&img1, _T("..\\ProjectDragonBall\\龙珠.jpg"));
	loadimage(&img2, _T("..\\ProjectDragonBall\\龙珠2.jpg"));
	SetWorkingImage(&img1);
	setlinecolor(BLACK);
	if (mouseX >= 600 && mouseX <= 700 && mouseY >= 260 && mouseY <= 300)
		setfillcolor(RGB(169, 169, 169));
	else
		setfillcolor(RGB(220, 220, 220));
	fillrectangle(600, 260, 700, 300);
	if (mouseX >= 600 && mouseX <= 700 && mouseY >= 320 && mouseY <= 360)
		setfillcolor(RGB(169, 169, 169));
	else
		setfillcolor(RGB(220, 220, 220));
	fillrectangle(600, 320, 700, 360);
	// 输出提示语
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(16, 0, _T("宋体"));
	outtextxy(618, 273, _T("进入游戏"));
	outtextxy(618, 333, _T("游戏介绍"));
	SetWorkingImage(NULL);
	//putimage(0, 0, &img1);
	putimage(0, 0, &img2);
}
//是否在矩形内
bool dragonball::IsInRect(int x, int y, RECT r)
{
	if ((x >= r.left && x <= r.right) && (y >= r.top && y <= r.bottom)) return true;
	else return false;
}
//画出子菜单
void dragonball::DrawDerivedMenu()
{
	RECT r;
	r.left = 600;
	r.top = 260;
	r.right = r.left + 100;
	r.bottom = r.top + 40;
	setcolor(RGB(50, 50, 50));
	setbkmode(TRANSPARENT);
	if (IsInRect(mouseX, mouseY, r))
	{
		setfillcolor(RGB(220, 220, 220));
		// 生成按键响应
		if (isMouseDown)
		{
			if (mouseX >= r.left&&mouseX <= r.right&&mouseY >= r.top&&mouseY <= r.bottom)
			{
				isMouseDown = 0;
				easy();
			}
		}
	}
	else
		setfillcolor(RGB(240, 240, 240));
	fillrectangle(r.left, r.top, r.right, r.bottom);
	settextstyle(25, 9, _T("华文新魏"));
	setcolor(RGB(60, 60, 60));
	outtextxy(r.left + 13, r.top + 8, _T("  简单"));
	r.left = 600;
	r.top = 320;
	r.right = r.left + 100;
	r.bottom = r.top + 40;
	setcolor(RGB(50, 50, 50));
	if (IsInRect(mouseX, mouseY, r))
	{
		setfillcolor(RGB(220, 220, 220));
		// 按键响应
		if (isMouseDown)
		{
			if (mouseX >= r.left&&mouseX <= r.right&&mouseY >= r.top&&mouseY <= r.bottom)
			{
				isMouseDown = 0;
				difficult();
			}
		}
	}
	else
		setfillcolor(RGB(240, 240, 240));
	fillrectangle(r.left, r.top, r.right, r.bottom);
	settextstyle(25, 9, _T("华文新魏"));
	setcolor(RGB(60, 60, 60));
	outtextxy(r.left + 13, r.top + 8, _T("  困难"));
}
//进入游戏
void dragonball::single()
{
	while (true)
	{
		MOUSEMSG mmsg;
		while (MouseHit())
		{
			mmsg = GetMouseMsg();
			switch (mmsg.uMsg)
			{
			case WM_MOUSEMOVE:  mouseX = mmsg.x; mouseY = mmsg.y; break;
			case WM_LBUTTONDOWN: isMouseDown = true; break;
			case WM_LBUTTONUP: isMouseDown = false; break;
			default:break;
			}
		}
		DrawDerivedMenu();
		Delay(50);
	}
}
void dragonball::multiple()
{

}
//重置数据
void dragonball::reset()
{
	cleardevice();
	cshtx();
	cshlz();
	init();
	BeginBatchDraw();
	p_Screen = GetImageBuffer();
	init_map(map0);
	createmap(x, y, map0);
	for (int i = 0; i < 500; i++)
	{
		int destroyX = rand() % (EASY_REAL_HEIGHT - 2) + 1;
		int destroyY = rand() % (EASY_REAL_WIDTH - 2) + 1;
		map0[destroyX][destroyY] = 0;
	}
	drawmap(map0);
	showeasymap();
	creategw();
	for (int s = 0; s < GWNUM_EASY; s++)
		drawgw1(gw_easy[s]);
	createball(EASY);
	drawball();
	createTLS(EASY);
	drawTLS();
	display();
	//thread t1(radar);
	//CopyScreenBuffer();
	//DrawScreenBuffer();
	EndBatchDraw();
	myHero = { 2 * MAZE_UNIT,2 * MAZE_UNIT,2 * MAZE_UNIT,2 * MAZE_UNIT,0,0,6,6,0,0,false,false,false,false,100.00 };
	TLS tls = { 0,0,1 };
}
//简单模式
void dragonball::easy()
{
	cleardevice();
	cshtx();
	cshlz();
	init();
	BeginBatchDraw();
	p_Screen = GetImageBuffer();
	init_map(map0);
	createmap(x, y, map0);
	for (int i = 0; i < 500; i++)
	{
		int destroyX = rand() % (EASY_REAL_HEIGHT - 2) + 1;
		int destroyY = rand() % (EASY_REAL_WIDTH - 2) + 1;
		map0[destroyX][destroyY] = 0;
	}
	drawmap(map0);
	showeasymap();
	creategw();
	for (int s = 0; s < GWNUM_EASY; s++)
		drawgw1(gw_easy[s]);
	createball(EASY);
	drawball();
	createTLS(EASY);
	drawTLS();
	display();
	//thread t1(radar);
	//CopyScreenBuffer();
	//DrawScreenBuffer();
	EndBatchDraw();

	while (true)
	{
		while (MouseHit())
		{
			mmsg = GetMouseMsg();
			switch (mmsg.uMsg)
			{
			case WM_MOUSEMOVE:  mouseX = mmsg.x; mouseY = mmsg.y; break;
			case WM_LBUTTONDOWN: isMouseDown = true; break;
			case WM_LBUTTONUP: isMouseDown = false; break;
			default:break;
			}
		}
		
		int ylzb_x = myHero.x;
		int ylzb_y = myHero.y;
		int key = 0;
		//thread t1(radar);
		//控制层
		if (_kbhit())
		{
			//mciSendString("play jbs from 0", NULL, 0, NULL);
			key = _getch();
			status = WALKING;
		}
		//显示画面
		BeginBatchDraw();
		roleact(key);
		for (int i = 0; i < GWNUM_EASY; i++)
			gw1act(gw_easy[i]);
		zdact(EASY);
		showeasymap();
		drawrole();
		for (int i = 0; i<GWNUM_EASY; i++)
			drawgw1(gw_easy[i]);
		touchBall();
		touchGw(EASY);
		touchTLS();
		drawball();
		drawzd();
		display();
		drawTLS();
		radar(EASY);
		setfillstyle(0);
		setfillcolor(WHITE);
		solidcircle(896 + 300 * tls.x / (EASY_REAL_WIDTH*MAZE_UNIT), 340 + 300 * tls.y / (EASY_REAL_HEIGHT*MAZE_UNIT), 2);
		showblood();
		FlushBatchDraw();
		//延迟
		status = STAYING;
		if (myHero.died)
		{
			gameover();
			reset();
		}
		for (int i = 0; i < 7; i++)
		{
			if (lz[i].state == 0)
			{
				countball++;
			}
		}
		if (countball == 7)
		{
			gamewin();
		}
		countball = 0;
		EndBatchDraw();
	}
}
//困难模式
void dragonball::difficult()
{
	cleardevice();
	cshtx();
	cshlz();
	init();
	BeginBatchDraw();
	p_Screen = GetImageBuffer();
	init_map_diff(map1);
	createmap_diff(x, y, map1);
	for (int i = 0; i < 1000; i++)
	{
		int destroyX = rand() % (DIFF_REAL_HEIGHT - 2) + 1;
		int destroyY = rand() % (DIFF_REAL_WIDTH - 2) + 1;
		map1[destroyX][destroyY] = 0;
	}
	drawmap_diff(map1);
	showeasymap_diff();
	creategw_diff();
	for (int t = 0; t < GWNUM_DIFF; t++)
		drawgw1(gw_diff[t]);
	createball(DIFF);
	drawball();
	createTLS(DIFF);
	drawTLS();
	display();
	//thread t1(radar);
	//CopyScreenBuffer();
	//DrawScreenBuffer();
	EndBatchDraw();

	while (true)
	{
		while (MouseHit())
		{
			mmsg = GetMouseMsg();
			switch (mmsg.uMsg)
			{
			case WM_MOUSEMOVE:  mouseX = mmsg.x; mouseY = mmsg.y; break;
			case WM_LBUTTONDOWN: isMouseDown = true; break;
			case WM_LBUTTONUP: isMouseDown = false; break;
			default:break;
			}
		}
		
		int ylzb_x = myHero.x;
		int ylzb_y = myHero.y;
		int key = 0;
		//thread t1(radar);
		//控制层
		if (_kbhit())
		{
			//mciSendString("play jbs from 0",NULL,0,NULL);
			key = _getch();
			status = WALKING;
		}
		//显示画面
		BeginBatchDraw();
		roleact_diff(key);
		for(int k=0;k<GWNUM_DIFF;k++)
			gw1act_diff(gw_diff[k]);
		zdact(DIFF);
		showeasymap_diff();
		drawrole();
		for(int i=0;i<GWNUM_DIFF;i++)
			drawgw1(gw_diff[i]);
		touchBall();
		touchGw(DIFF);
		touchTLS();
		drawball();
		drawzd();
		display();
		drawTLS();
		radar(DIFF);
		setfillstyle(0);
		setfillcolor(WHITE);
		solidcircle(896 + 300 * tls.x / (DIFF_REAL_WIDTH*MAZE_UNIT), 340 + 300 * tls.y / (DIFF_REAL_HEIGHT*MAZE_UNIT), 2);
		showblood();
		FlushBatchDraw();
		//延迟
		status = STAYING;
		if (myHero.died)
		{
			gameover();
			reset();
		}
		for (int i = 0; i < 7; i++)
		{
			if (lz[i].state == 0)
			{
				countball++;
			}
		}
		if (countball == 7)
		{
			gamewin();
		}
		countball = 0;
		EndBatchDraw();
	}
}
//初始化原始地图
void dragonball::init_map(int map[][EASY_REAL_WIDTH])
{
	for (int i = 0; i < EASY_REAL_HEIGHT; i++)
		for (int j = 0; j < EASY_REAL_WIDTH; j++)
		{
			if (i == 0 || i == EASY_REAL_HEIGHT - 1)
				map[i][j] = 2;
			else if (j == 0 || j == EASY_REAL_WIDTH - 1)
				map[i][j] = 2;
			else
				map[i][j] = 1;
		}
}
void dragonball::init_map_diff(int map[][DIFF_REAL_WIDTH])
{
	for (int i = 0; i < DIFF_REAL_HEIGHT; i++)
		for (int j = 0; j < DIFF_REAL_WIDTH; j++)
		{
			if (i == 0 || i == DIFF_REAL_HEIGHT - 1)
				map[i][j] = 2;
			else if (j == 0 || j == DIFF_REAL_WIDTH - 1)
				map[i][j] = 2;
			else
				map[i][j] = 1;
		}
}
//创造地图
void dragonball::createmap(int x, int y, int map[][EASY_REAL_WIDTH])
{
	int c[4][2] = { 0,1,1,0,0,-1,-1,0 };
	int i, j, t;

	for (i = 0; i < 4; i++)
	{
		j = rand() % 4;
		t = c[i][0]; c[i][0] = c[j][0]; c[j][0] = t;
		t = c[i][1]; c[i][1] = c[j][1]; c[j][1] = t;
	}

	map[x][y] = 0;

	for (i = 0; i < 4; i++)
		if (map[x + 2 * c[i][0]][y + 2 * c[i][1]] == 1)
		{
			map[x + c[i][0]][y + c[i][1]] = 0;
			createmap(x + 2 * c[i][0], y + 2 * c[i][1], map);
		}
}
void dragonball::createmap_diff(int x, int y, int map[][DIFF_REAL_WIDTH])
{
	int c[4][2] = { 0,1,1,0,0,-1,-1,0 };
	int i, j, t;

	for (i = 0; i < 4; i++)
	{
		j = rand() % 4;
		t = c[i][0]; c[i][0] = c[j][0]; c[j][0] = t;
		t = c[i][1]; c[i][1] = c[j][1]; c[j][1] = t;
	}

	map[x][y] = 0;

	for (i = 0; i < 4; i++)
		if (map[x + 2 * c[i][0]][y + 2 * c[i][1]] == 1)
		{
			map[x + c[i][0]][y + c[i][1]] = 0;
			createmap_diff(x + 2 * c[i][0], y + 2 * c[i][1], map);
		}
}
//画地图块
void dragonball::drawmap(int map[][EASY_REAL_WIDTH])
{
	SetWorkingImage(NULL);
	for (int i = 0; i<EASY_REAL_HEIGHT; i++)
		for (int j = 0; j < EASY_REAL_WIDTH; j++)
		{
			if ((i == 2 && j == 2) || (i == EASY_REAL_HEIGHT - 3 && j == EASY_REAL_WIDTH - 3))
			{
				easymap[i][j].mapx = j;
				easymap[i][j].mapy = i;
				easymap[i][j].mapID = DOOR;
			}
			else if (map[i][j] == 2)
			{
				easymap[i][j].mapx = j;
				easymap[i][j].mapy = i;
				easymap[i][j].mapID = BORDER;
			}
			else if (map[i][j] == 1)
			{
				easymap[i][j].mapx = j;
				easymap[i][j].mapy = i;
				easymap[i][j].mapID = WALL;
			}
			else
			{
				easymap[i][j].mapx = j;
				easymap[i][j].mapy = i;
				easymap[i][j].mapID = FLOOR;
			}
		}
	/*CopyScreenBuffer();
	DrawScreenBuffer();
	FlushBatchDraw();*/
}
void dragonball::drawmap_diff(int map[][DIFF_REAL_WIDTH])
{
	SetWorkingImage(NULL);
	for (int i = 0; i<DIFF_REAL_HEIGHT; i++)
		for (int j = 0; j < DIFF_REAL_WIDTH; j++)
		{
			if ((i == 2 && j == 2) || (i == DIFF_REAL_HEIGHT - 3 && j == DIFF_REAL_WIDTH - 3))
			{
				diffmap[i][j].mapx = j;
				diffmap[i][j].mapy = i;
				diffmap[i][j].mapID = DOOR;
			}
			else if (map[i][j] == 2)
			{
				diffmap[i][j].mapx = j;
				diffmap[i][j].mapy = i;
				diffmap[i][j].mapID = BORDER;
			}
			else if (map[i][j] == 1)
			{
				diffmap[i][j].mapx = j;
				diffmap[i][j].mapy = i;
				diffmap[i][j].mapID = WALL;
			}
			else
			{
				diffmap[i][j].mapx = j;
				diffmap[i][j].mapy = i;
				diffmap[i][j].mapID = FLOOR;
			}
		}
	/*CopyScreenBuffer();
	DrawScreenBuffer();
	FlushBatchDraw();*/
}
//画主菜单
void dragonball::DrawMenuItems()
{
	RECT r;
	r.left = 600;
	r.top = 260;
	r.right = r.left + 100;
	r.bottom = r.top + 40;
	setcolor(RGB(50, 50, 50));
	setbkmode(TRANSPARENT);
	if (IsInRect(mouseX, mouseY, r))
	{
		setfillcolor(RGB(220, 220, 220));
		// 生成按键响应
		if (isMouseDown)
		{
			if (mouseX >= r.left&&mouseX <= r.right&&mouseY >= r.top&&mouseY <= r.bottom)
			{
				isMouseDown = 0;
				single();
			}
		}
	}
	else
		setfillcolor(RGB(240, 240, 240));
	fillrectangle(r.left, r.top, r.right, r.bottom);
	settextstyle(25, 9, _T("华文新魏"));
	setcolor(RGB(60, 60, 60));
	outtextxy(r.left + 5, r.top + 5, _T(" 进入游戏"));
	r.left = 600;
	r.top = 320;
	r.right = r.left + 100;
	r.bottom = r.top + 40;
	setcolor(RGB(50, 50, 50));
	if (IsInRect(mouseX, mouseY, r))
	{
		setfillcolor(RGB(220, 220, 220));
		// 按键响应
		if (isMouseDown)
		{
			if (mouseX >= r.left&&mouseX <= r.right&&mouseY >= r.top&&mouseY <= r.bottom)
			{
				isMouseDown = 0;
				yxjs();
			}
		}
	}
	else
		setfillcolor(RGB(240, 240, 240));
	fillrectangle(r.left, r.top, r.right, r.bottom);
	settextstyle(25, 9, _T("华文新魏"));
	setcolor(RGB(60, 60, 60));
	outtextxy(r.left + 5, r.top + 5, _T(" 游戏介绍"));
}
//信息板
void dragonball::display()
{
	SetWorkingImage(NULL);
	for (int i = 0; i < 5; i++)
	{
		IMAGE img4;
		loadimage(&img4, _T("..\\ProjectDragonBall\\信息板.jpg"));
		setfillstyle(BS_DIBPATTERN, NULL, &img4);
		fillrectangle(WIN_WIDTH - 300, 68 * i, WIN_WIDTH, 68 * (i + 1));
	}

	setcolor(RGB(0, 0, 0));
	setbkmode(TRANSPARENT);
	if (IsInRect(mouseX, mouseY, r_mainmenu))
	{
		settextcolor(RGB(255, 255, 255));
		// 生成按键响应
		if (isMouseDown)
		{
			if (mouseX >= r_mainmenu.left&&mouseX <= r_mainmenu.right&&mouseY >= r_mainmenu.top&&mouseY <= r_mainmenu.bottom)
			{
				mciSendString("play djcd from 0", NULL, 0, NULL);
				//mciSendString("pause bjyy", NULL, 0, NULL);
				//keyState = MUSICOFF;
				isMouseDown = 0;
				yxjs();
				_getch();
			}
		}
	}
	else
		settextcolor(RGB(128,128,128));
	//fillrectangle(r_mainmenu.left, r_mainmenu.top, r_mainmenu.right, r_mainmenu.bottom);
	settextstyle(20, 0, "华文行楷", 0, 0, 100, false, false, false);
	//setcolor(RGB(0, 0, 0));
	outtextxy(r_mainmenu.left, r_mainmenu.top , _T("游戏介绍"));

	prekeyState = keyState;

	if (IsInRect(mouseX, mouseY, r_pose))
	{
		settextcolor(RGB(255, 255, 255));
		// 生成按键响应
		if (isMouseDown)
		{
			if (mouseX >= r_pose.left&&mouseX <= r_pose.right&&mouseY >= r_pose.top&&mouseY <= r_pose.bottom)
			{
				mciSendString("play djcd from 0", NULL, 0, NULL);
				isMouseDown = 0;
				if (prekeyState == MUSICON)
					keyState = MUSICOFF;
				else
					keyState = MUSICON;
			}
		}
	}
	else
		settextcolor(RGB(128, 128, 128));
	settextstyle(20, 0, "华文行楷", 0, 0, 100, false, false, false);
	
	if (keyState == MUSICON)
	{
		outtextxy(r_pose.left, r_pose.top, _T("音乐：开"));
		if (prekeyState == keyState)
			;
		else
		{
			mciSendString("open ..\\res\\背景音乐.mp3 alias bgyy", NULL, 0, NULL);
			mciSendString("play bgyy repeat", NULL, 0, NULL);
		}

	}
	else
	{
		outtextxy(r_pose.left, r_pose.top, _T("音乐：关"));
		mciSendString("stop bgyy",NULL,0,NULL);
	}

	preispose = ispose;

	if (IsInRect(mouseX, mouseY, r_music))
	{
		settextcolor(RGB(255, 255, 255));
		// 生成按键响应
		if (isMouseDown)
		{
			if (mouseX >= r_music.left&&mouseX <= r_music.right&&mouseY >= r_music.top&&mouseY <= r_music.bottom)
			{
				mciSendString("play djcd", NULL, 0, NULL);
				isMouseDown = 0;
				if (preispose == 0)
					ispose = 1;
				else
					ispose = 0;
			}
		}
	}
	else
		settextcolor(RGB(128, 128, 128));
	//fillrectangle(r_mainmenu.left, r_mainmenu.top, r_mainmenu.right, r_mainmenu.bottom);
	settextstyle(20, 0, "华文行楷", 0, 0, 100, false, false, false);
	//setcolor(RGB(0, 0, 0));
	if (ispose==0)
		outtextxy(r_music.left, r_music.top, _T("查看龙珠"));
	else
	{
		mciSendString("pause bjyy from 0", NULL, 0, NULL);
		//keyState = MUSICOFF;
		outtextxy(r_music.left, r_music.top, _T("按任意键继续"));
		FlushBatchDraw();
		_getch();
		ispose = 0;
	}

	//角色切换
	if (IsInRect(mouseX, mouseY,r_role))
	{
		settextcolor(RGB(255, 255, 255));
		// 生成按键响应
		if (isMouseDown)
		{
			if (mouseX >= r_role.left&&mouseX <= r_role.right&&mouseY >= r_role.top&&mouseY <= r_role.bottom)
			{
				mciSendString("play djcd from 0", NULL, 0, NULL);
				//mciSendString("pause bjyy", NULL, 0, NULL);
				//keyState = MUSICOFF;
				isMouseDown = 0;
				rwjs();
				_getch();
			}
		}
	}
	else
		settextcolor(RGB(128, 128, 128));
	//fillrectangle(r_mainmenu.left, r_mainmenu.top, r_mainmenu.right, r_mainmenu.bottom);
	settextstyle(20, 0, "华文行楷", 0, 0, 100, false, false, false);
	//setcolor(RGB(0, 0, 0));
	outtextxy(r_role.left, r_role.top, _T("人物介绍"));

	if (IsInRect(mouseX, mouseY, r_exit))
	{
		settextcolor(RGB(255, 255, 255));
		// 生成按键响应
		if (isMouseDown)
		{
			if (mouseX >= r_exit.left&&mouseX <= r_exit.right&&mouseY >= r_exit.top&&mouseY <= r_exit.bottom)
			{
				mciSendString("play djcd from 0", NULL, 0, NULL);
				//mciSendString("pause bjyy", NULL, 0, NULL);
				//keyState = MUSICOFF;
				isMouseDown = 0;
				exit(0);
			}
		}
	}
	else
		settextcolor(RGB(128, 128, 128));
	//fillrectangle(r_mainmenu.left, r_mainmenu.top, r_mainmenu.right, r_mainmenu.bottom);
	settextstyle(20, 0, "华文行楷", 0, 0, 100, false, false, false);
	//setcolor(RGB(0, 0, 0));
	outtextxy(r_exit.left, r_exit.top, _T("退出"));
}
//雷达
void dragonball::radar(int mapkind)
{
	SetWorkingImage(NULL);
	IMAGE img5, img6, img7, img8, img9, img10, img11, img12, img13, img14, img15, img16;
	loadimage(&img5, _T("..\\ProjectDragonBall\\雷达1.jpg"));
	loadimage(&img6, _T("..\\ProjectDragonBall\\雷达2.jpg"));
	loadimage(&img7, _T("..\\ProjectDragonBall\\雷达3.jpg"));
	loadimage(&img8, _T("..\\ProjectDragonBall\\雷达4.jpg"));
	loadimage(&img9, _T("..\\ProjectDragonBall\\雷达5.jpg"));
	loadimage(&img10, _T("..\\ProjectDragonBall\\雷达6.jpg"));
	loadimage(&img11, _T("..\\ProjectDragonBall\\雷达7.jpg"));
	loadimage(&img12, _T("..\\ProjectDragonBall\\雷达8.jpg"));
	loadimage(&img13, _T("..\\ProjectDragonBall\\雷达9.jpg"));
	loadimage(&img14, _T("..\\ProjectDragonBall\\雷达10.jpg"));
	loadimage(&img15, _T("..\\ProjectDragonBall\\雷达11.jpg"));
	loadimage(&img16, _T("..\\ProjectDragonBall\\雷达12.jpg"));
	switch (radar_iframe)
	{
	case 1:
		putimage(WIN_WIDTH - 300, 340, &img5);
		radar_iframe++;
		break;
	case 2:
		putimage(WIN_WIDTH - 300, 340, &img6);
		radar_iframe++;
		break;
	case 3:
		putimage(WIN_WIDTH - 300, 340, &img7);
		radar_iframe++;
		break;
	case 4:
		putimage(WIN_WIDTH - 300, 340, &img8);
		radar_iframe++;
		break;
	case 5:
		putimage(WIN_WIDTH - 300, 340, &img9);
		radar_iframe++;
		break;
	case 6:
		putimage(WIN_WIDTH - 300, 340, &img10);
		radar_iframe++;
		break;
	case 7:
		putimage(WIN_WIDTH - 300, 340, &img11);
		radar_iframe++;
		break;
	case 8:
		putimage(WIN_WIDTH - 300, 340, &img12);
		radar_iframe++;
		break;
	case 9:
		putimage(WIN_WIDTH - 300, 340, &img13);
		radar_iframe++;
		break;
	case 10:
		putimage(WIN_WIDTH - 300, 340, &img14);
		radar_iframe++;
		break;
	case 11:
		putimage(WIN_WIDTH - 300, 340, &img15);
		radar_iframe++;
		break;
	case 12:
		putimage(WIN_WIDTH - 300, 340, &img16);
		radar_iframe = 1;
		break;
	}

	if (mapkind == EASY)
	{
		for (int i = 0; i < 7; i++)
		{
			if (lz[i].state == 1)
			{
				setfillstyle(0);
				setfillcolor(YELLOW);
				solidcircle(896 + 300 * lz[i].x / (EASY_REAL_WIDTH*MAZE_UNIT), 340 + 300 * lz[i].y / (EASY_REAL_HEIGHT*MAZE_UNIT), 2);
			}
		}
		setfillstyle(0);
		setfillcolor(RED);
		solidcircle(896 + 300 * (-myHero.x0 + myHero.x) / (EASY_REAL_WIDTH*MAZE_UNIT), 340 + 300 * (-myHero.y0 + myHero.y) / (EASY_REAL_HEIGHT*MAZE_UNIT), 2);
	}
	else
	{
		for (int i = 0; i < 7; i++)
		{
			if (lz[i].state == 1)
			{
				setfillstyle(0);
				setfillcolor(YELLOW);
				solidcircle(896 + 300 * lz[i].x / (DIFF_REAL_WIDTH*MAZE_UNIT), 340 + 300 * lz[i].y / (DIFF_REAL_HEIGHT*MAZE_UNIT), 2);
			}
		}
		setfillstyle(0);
		setfillcolor(RED);
		solidcircle(896 + 300 * (-myHero.x0 + myHero.x) / (DIFF_REAL_WIDTH*MAZE_UNIT), 340 + 300 * (-myHero.y0 + myHero.y) / (DIFF_REAL_HEIGHT*MAZE_UNIT), 2);
	}
	
}

void dragonball::mapact()
{

}
//展示地图
void dragonball::showeasymap()
{
	cleardevice();
	IMAGE door, border, wall, floor;
	loadimage(&door, _T("..\\ProjectDragonBall\\门.jpg"));
	loadimage(&border, _T("..\\ProjectDragonBall\\边界.jpg"));
	loadimage(&wall, _T("..\\ProjectDragonBall\\围墙.jpg"));
	loadimage(&floor, _T("..\\ProjectDragonBall\\地板.jpg"));

	for (int i = 0; i<EASY_REAL_HEIGHT; i++)
		for (int j = 0; j < EASY_REAL_WIDTH; j++)
		{
			if (easymap[i][j].mapID == DOOR)
				putimage(myHero.x0+MAZE_UNIT*j, myHero.y0 + MAZE_UNIT*i, &floor);
			else if (easymap[i][j].mapID == BORDER)
				putimage(myHero.x0+MAZE_UNIT*j, myHero.y0+MAZE_UNIT*i, &border);
			else if (easymap[i][j].mapID == WALL)
				putimage(myHero.x0 + MAZE_UNIT * j, myHero.y0 + MAZE_UNIT * i, &wall);
			else if (easymap[i][j].mapID == FLOOR)
				putimage(myHero.x0 + MAZE_UNIT * j, myHero.y0 + MAZE_UNIT * i, &floor);
		}
}
void dragonball::showeasymap_diff()
{
	cleardevice();

	IMAGE door, border, wall, floor;
	loadimage(&door, _T("..\\ProjectDragonBall\\门.jpg"));
	loadimage(&border, _T("..\\ProjectDragonBall\\边界.jpg"));
	loadimage(&wall, _T("..\\ProjectDragonBall\\围墙.jpg"));
	loadimage(&floor, _T("..\\ProjectDragonBall\\地板.jpg"));

	for (int i = 0; i<DIFF_REAL_HEIGHT; i++)
		for (int j = 0; j < DIFF_REAL_WIDTH; j++)
		{
			if (diffmap[i][j].mapID == DOOR)
				putimage(myHero.x0 + MAZE_UNIT*j, myHero.y0 + MAZE_UNIT*i, &floor);
			else if (diffmap[i][j].mapID == BORDER)
				putimage(myHero.x0 + MAZE_UNIT * j, myHero.y0 + MAZE_UNIT * i, &border);
			else if (diffmap[i][j].mapID == WALL)
				putimage(myHero.x0 + MAZE_UNIT * j, myHero.y0 + MAZE_UNIT * i, &wall);
			else if (diffmap[i][j].mapID == FLOOR)
				putimage(myHero.x0 + MAZE_UNIT * j, myHero.y0 + MAZE_UNIT * i, &floor);
		}
}
//展示血量
void dragonball::showblood()
{
	SetWorkingImage(NULL);
	setbkmode(TRANSPARENT);

	IMAGE bloodimg,bloodymtimg;

	loadimage(&bloodimg, _T("..\\res\\blood.bmp"));
	loadimage(&bloodymtimg, _T("..\\res\\bloodymt.bmp"));

	putimage(0, 0, &bloodymtimg, SRCAND);
	putimage(0, 0, &bloodimg, SRCPAINT);

	setfillstyle(0);
	
	setfillcolor(BLACK);
	solidrectangle(int(50 + 2.5*myHero.blood), 16, 300, 32);

	/*CopyScreenBuffer();
	DrawScreenBuffer();
	FlushBatchDraw();*/
}
//创造龙珠
void dragonball::createball(int mapKind)
{
	int qs_x;
	int qs_y;
	if (mapKind == EASY)
	{
		for (int i = 0; i < 7; i++)
		{
			do 
			{ 
				qs_x = rand() % (EASY_REAL_WIDTH);
				qs_y = rand() % (EASY_REAL_HEIGHT);
			} while (easymap[qs_y][qs_x].mapID == WALL || easymap[qs_y][qs_x].mapID == BORDER);
			lz[i].x = qs_x * MAZE_UNIT;
			lz[i].y = qs_y * MAZE_UNIT;
			lz[i].id = i + 1;
		}
	}
	else
	{
		for (int i = 0; i < 7; i++)
		{
			do
			{
				qs_x = rand() % (DIFF_REAL_WIDTH);
				qs_y = rand() % (DIFF_REAL_HEIGHT);
			} while (diffmap[qs_y][qs_x].mapID == WALL || diffmap[qs_y][qs_x].mapID == BORDER);
			lz[i].x = qs_x * MAZE_UNIT;
			lz[i].y = qs_y * MAZE_UNIT;
			lz[i].id = i + 1;
		}
	}
}
//画出龙珠
void dragonball::drawball()
{
	for (int i = 0; i < 7; i++)
	{
		if (lz[i].id == 1)
		{
			if (lz[i].state == 1)
			{
				putimage(myHero.x0 + lz[i].x, myHero.y0 + lz[i].y, MAZE_UNIT, MAZE_UNIT, &ball11, 0, 0, SRCAND);
				putimage(myHero.x0 + lz[i].x, myHero.y0 + lz[i].y, MAZE_UNIT, MAZE_UNIT, &ball1, 0, 0, SRCPAINT);
			}
			else
			{
				putimage(934, 340, &ball11, SRCAND);
				putimage(934, 340, &ball1, SRCPAINT);
			}
		}
		else if (lz[i].id == 2)
		{
			if (lz[i].state == 1)
			{
				putimage(myHero.x0 + lz[i].x, myHero.y0 + lz[i].y, MAZE_UNIT, MAZE_UNIT, &ball22, 0, 0, SRCAND);
				putimage(myHero.x0 + lz[i].x, myHero.y0 + lz[i].y, MAZE_UNIT, MAZE_UNIT, &ball2, 0, 0, SRCPAINT);
			}
			else
			{
				putimage(934 + MAZE_UNIT, 340, &ball22, SRCAND);
				putimage(934 + MAZE_UNIT, 340, &ball2, SRCPAINT);
			}
		}
		else if (lz[i].id == 3)
		{
			if (lz[i].state == 1)
			{
				putimage(myHero.x0 + lz[i].x, myHero.y0 + lz[i].y, MAZE_UNIT, MAZE_UNIT, &ball33, 0, 0, SRCAND);
				putimage(myHero.x0 + lz[i].x, myHero.y0 + lz[i].y, MAZE_UNIT, MAZE_UNIT, &ball3, 0, 0, SRCPAINT);
			}
			else
			{
				putimage(934 + 2 * MAZE_UNIT, 340, &ball33, SRCAND);
				putimage(934 + 2 * MAZE_UNIT, 340, &ball3, SRCPAINT);
			}
		}
		else if (lz[i].id == 4)
		{
			if (lz[i].state == 1)
			{
				putimage(myHero.x0 + lz[i].x, myHero.y0 + lz[i].y, MAZE_UNIT, MAZE_UNIT, &ball44, 0, 0, SRCAND);
				putimage(myHero.x0 + lz[i].x, myHero.y0 + lz[i].y, MAZE_UNIT, MAZE_UNIT, &ball4, 0, 0, SRCPAINT);
			}
			else
			{
				putimage(934 + 3 * MAZE_UNIT, 340, &ball44, SRCAND);
				putimage(934 + 3 * MAZE_UNIT, 340, &ball4, SRCPAINT);
			}
		}
		else if (lz[i].id == 5)
		{
			if (lz[i].state == 1)
			{
				putimage(myHero.x0 + lz[i].x, myHero.y0 + lz[i].y, MAZE_UNIT, MAZE_UNIT, &ball55, 0, 0, SRCAND);
				putimage(myHero.x0 + lz[i].x, myHero.y0 + lz[i].y, MAZE_UNIT, MAZE_UNIT, &ball5, 0, 0, SRCPAINT);
			}
			else
			{
				putimage(934 + 4* MAZE_UNIT, 340, &ball55, SRCAND);
				putimage(934 + 4* MAZE_UNIT, 340, &ball5, SRCPAINT);
			}
		}
		else if (lz[i].id == 6)
		{
			if (lz[i].state == 1)
			{
				putimage(myHero.x0 + lz[i].x, myHero.y0 + lz[i].y, MAZE_UNIT, MAZE_UNIT, &ball66, 0, 0, SRCAND);
				putimage(myHero.x0 + lz[i].x, myHero.y0 + lz[i].y, MAZE_UNIT, MAZE_UNIT, &ball6, 0, 0, SRCPAINT);
			}
			else
			{
				putimage(934 + 5 * MAZE_UNIT, 340, &ball66, SRCAND);
				putimage(934 + 5 * MAZE_UNIT, 340, &ball6, SRCPAINT);
			}
		}
		else
		{
			if (lz[i].state == 1)
			{
				putimage(myHero.x0 + lz[i].x, myHero.y0 + lz[i].y, MAZE_UNIT, MAZE_UNIT, &ball77, 0, 0, SRCAND);
				putimage(myHero.x0 + lz[i].x, myHero.y0 + lz[i].y, MAZE_UNIT, MAZE_UNIT, &ball7, 0, 0, SRCPAINT);
			}
			else
			{
				putimage(934 + 6 * MAZE_UNIT, 340, &ball77, SRCAND);
				putimage(934 + 6 * MAZE_UNIT, 340, &ball7, SRCPAINT);
			}
		}
	}
}

void dragonball::createrole()
{

}
//画主角
void dragonball::drawrole()
{
	loadimage(&roleimg21, _T("..\\res\\role.bmp"));
	loadimage(&roleimg22, _T("..\\res\\roleymt.bmp"));
	//控制主角的方向
	if (status==STAYING)
	{
		if (myHero.direction == 3)
		{
			putimage(myHero.x, myHero.y, MAZE_UNIT, MAZE_UNIT, &roleimg22, 0, 2 * MAZE_UNIT, SRCAND);
			putimage(myHero.x, myHero.y, MAZE_UNIT, MAZE_UNIT, &roleimg21, 0, 2 * MAZE_UNIT, SRCPAINT);
		}
		else if (myHero.direction == 2)
		{
			putimage(myHero.x, myHero.y, MAZE_UNIT, MAZE_UNIT, &roleimg22, 0, MAZE_UNIT, SRCAND);
			putimage(myHero.x, myHero.y, MAZE_UNIT, MAZE_UNIT, &roleimg21, 0, MAZE_UNIT, SRCPAINT);
		}
		else if (myHero.direction == 0)
		{
			putimage(myHero.x, myHero.y, MAZE_UNIT, MAZE_UNIT, &roleimg22, 0, 3 * MAZE_UNIT, SRCAND);
			putimage(myHero.x, myHero.y, MAZE_UNIT, MAZE_UNIT, &roleimg21, 0, 3 * MAZE_UNIT, SRCPAINT);
		}
		else
		{
			putimage(myHero.x, myHero.y, MAZE_UNIT, MAZE_UNIT, &roleimg22, 0, 0, SRCAND);
			putimage(myHero.x, myHero.y, MAZE_UNIT, MAZE_UNIT, &roleimg21, 0, 0, SRCPAINT);
		}
	}
	else
	{
		if (myHero.direction == 3)
		{
			if (hero_iframe <= 3)
				hero_iframe++;
			else
				hero_iframe = 1;
			putimage(myHero.x, myHero.y, MAZE_UNIT, MAZE_UNIT, &roleimg22, (hero_iframe-1)*MAZE_UNIT, 2*MAZE_UNIT, SRCAND);
			putimage(myHero.x, myHero.y, MAZE_UNIT, MAZE_UNIT, &roleimg21, (hero_iframe-1)*MAZE_UNIT, 2*MAZE_UNIT, SRCPAINT);
		}
		else if(myHero.direction==2)
		{
			if (hero_iframe <= 3)
				hero_iframe++;
			else
				hero_iframe = 1;
			putimage(myHero.x, myHero.y, MAZE_UNIT, MAZE_UNIT, &roleimg22, (hero_iframe - 1)* MAZE_UNIT, MAZE_UNIT, SRCAND);
			putimage(myHero.x, myHero.y, MAZE_UNIT, MAZE_UNIT, &roleimg21, (hero_iframe - 1)* MAZE_UNIT, MAZE_UNIT, SRCPAINT);
		}
		else if (myHero.direction == 0)
		{
			if (hero_iframe <= 3)
				hero_iframe++;
			else
				hero_iframe = 1;
			putimage(myHero.x, myHero.y, MAZE_UNIT, MAZE_UNIT, &roleimg22, (hero_iframe - 1)* MAZE_UNIT, 3*MAZE_UNIT, SRCAND);
			putimage(myHero.x, myHero.y, MAZE_UNIT, MAZE_UNIT, &roleimg21, (hero_iframe - 1)* MAZE_UNIT, 3*MAZE_UNIT, SRCPAINT);
		}
		else
		{
			if (hero_iframe <= 3)
				hero_iframe++;
			else
				hero_iframe = 1;
			putimage(myHero.x, myHero.y, MAZE_UNIT, MAZE_UNIT, &roleimg22, (hero_iframe - 1)* MAZE_UNIT, 0, SRCAND);
			putimage(myHero.x, myHero.y, MAZE_UNIT, MAZE_UNIT, &roleimg21, (hero_iframe - 1)* MAZE_UNIT, 0, SRCPAINT);
		}
	}
}
//主角动作
void dragonball::roleact(char ch)
{
	Hero temp = { myHero.x,myHero.y,myHero.vx0,myHero.vy0 };
	switch (ch)
	{
	case 'w':
	case 'W':
		myHero.direction = 0;
		temp.y -= STEP;
		if (touchmap(temp, easymap))
			return;
		else if(myHero.y<MAZE_HEIGHT*MAZE_UNIT/2)
		{
			if (myHero.y0 < 0)
			{
				myHero.y0 += STEP;
				myHero.direction= 0;
			}
			else
			{
				myHero.y -= STEP;
				myHero.direction= 0;
			}
			//role.y = (int)role.yy;
		}
		else if(myHero.y>=MAZE_HEIGHT*MAZE_UNIT/2)
		{
			if (myHero.y - STEP < MAZE_HEIGHT * MAZE_UNIT / 2 && myHero.y0<0)
			{
				myHero.y0 += STEP;
				myHero.y = MAZE_HEIGHT * MAZE_UNIT / 2;
				myHero.direction= 0;
			}
			else
			{
				myHero.y -= STEP;
				myHero.direction= 0;
			}
		}
		
		break;
	case 'a':
	case 'A':
		myHero.direction = 2;
		temp.x -= STEP;
		//temp.x = (int)temp.xx;
		if (touchmap(temp, easymap))
			return;
		else if(myHero.x<MAZE_WIDTH*MAZE_UNIT/2)
		{
			if (myHero.x0 < 0)
			{
				myHero.x0 += STEP;
				//role.x = (int)role.xx;
				myHero.direction= 2;
			}
			else
			{
				myHero.x -= STEP;
				myHero.direction= 2;
			}
		}
		else if (myHero.x >= MAZE_WIDTH * MAZE_UNIT / 2 )
		{
			if (myHero.x - STEP < MAZE_WIDTH*MAZE_UNIT / 2 && myHero.x0 < 0)
			{
				myHero.x0 += STEP;
				myHero.x = MAZE_WIDTH * MAZE_UNIT / 2;
				myHero.direction=2;
			}
			else
			{
				myHero.x -= STEP;
				//role.x = (int)role.xx;
				myHero.direction=2;
			}
		}
		break;
	case 'd':
	case 'D':
		myHero.direction = 3;
		temp.x += STEP;
		//temp.x = (int)temp.xx;
		if (touchmap(temp, easymap))
			return;

		else if (myHero.x >= MAZE_WIDTH*MAZE_UNIT / 2 )
		{
			if (myHero.x0 == -MAZE_WIDTH * MAZE_UNIT)
			{
				myHero.x += STEP;
				//role.x = (int)role.xx;
				myHero.direction = 3;
			}
			else
			{
				myHero.x0 -= STEP;
				myHero.direction = 3;
			}
		}
		
		else if(myHero.x<MAZE_WIDTH*MAZE_UNIT/2)
		{
			if (myHero.x + STEP > MAZE_WIDTH * MAZE_UNIT / 2)
			{
				myHero.x0 -= STEP;
				myHero.direction = 3;
			}
			else
			{
				myHero.x += STEP;
				//role.x = (int)role.xx;
				myHero.direction= 3;
			}
		}
		break;
	case 's':
	case 'S':
		myHero.direction = 1;
		temp.y += STEP;
		//temp.y = (int)temp.yy;
		if (touchmap(temp, easymap))
			return;

		else if(myHero.y>= MAZE_HEIGHT*MAZE_UNIT / 2)
		{
			if (myHero.y0 <= -MAZE_HEIGHT * MAZE_UNIT)
			{
				myHero.y += STEP;
				myHero.direction= 1;
			}
			else
			{
				myHero.y0 -= STEP;
				myHero.direction = 1;
			}
			    //role.y = (int)role.yy;
		}
		
		else if(myHero.y<MAZE_HEIGHT*MAZE_UNIT/2)
		{
			if (myHero.y + STEP > MAZE_HEIGHT * MAZE_UNIT / 2)
			{
				myHero.y0 -= STEP;
				myHero.direction = 1;
			}
			else
			{
				myHero.y += STEP;
				myHero.direction = 1;
			}
				//role.y = (int)role.yy;
		}

		break;
	case 'k':
	case 'K':
		if (myHero.isShoot&&yfszds<30)
			createzd();
		break;
	default:
		break;
		/*case 'k':
		case 'K':
		shoot();*/
	}
}
void dragonball::roleact_diff(char ch)
{
	Hero temp = { myHero.x,myHero.y,myHero.vx0,myHero.vy0 };
	switch (ch)
	{
	case 'w':
	case 'W':
		myHero.direction = 0;
		temp.y -= STEP;
		if (touchmap_diff(temp, diffmap))
			return;
		else if (myHero.y<MAZE_HEIGHT*MAZE_UNIT / 2)
		{
			if (myHero.y0 < 0)
			{
				myHero.y0 += STEP;
				myHero.direction = 0;
			}
			else
			{
				myHero.y -= STEP;
				myHero.direction = 0;
			}
			//role.y = (int)role.yy;
		}
		else if (myHero.y >= MAZE_HEIGHT * MAZE_UNIT / 2)
		{
			if (myHero.y - STEP < MAZE_HEIGHT * MAZE_UNIT / 2 && myHero.y0<0)
			{
				myHero.y0 += STEP;
				myHero.y = MAZE_HEIGHT * MAZE_UNIT / 2;
				myHero.direction = 0;
			}
			else
			{
				myHero.y -= STEP;
				myHero.direction = 0;
			}
		}

		break;
	case 'a':
	case 'A':
		myHero.direction = 2;
		temp.x -= STEP;
		//temp.x = (int)temp.xx;
		if (touchmap_diff(temp, diffmap))
			return;
		else if (myHero.x<MAZE_WIDTH*MAZE_UNIT / 2)
		{
			if (myHero.x0 < 0)
			{
				myHero.x0 += STEP;
				//role.x = (int)role.xx;
				myHero.direction = 2;
			}
			else
			{
				myHero.x -= STEP;
				myHero.direction = 2;
			}
		}
		else if (myHero.x >= MAZE_WIDTH * MAZE_UNIT / 2)
		{
			if (myHero.x - STEP < MAZE_WIDTH*MAZE_UNIT / 2 && myHero.x0 < 0)
			{
				myHero.x0 += STEP;
				myHero.x = MAZE_WIDTH * MAZE_UNIT / 2;
				myHero.direction = 2;
			}
			else
			{
				myHero.x -= STEP;
				//role.x = (int)role.xx;
				myHero.direction = 2;
			}
		}
		break;
	case 'd':
	case 'D':
		myHero.direction = 3;
		temp.x += STEP;
		//temp.x = (int)temp.xx;
		if (touchmap_diff(temp, diffmap))
			return;

		else if (myHero.x >= MAZE_WIDTH * MAZE_UNIT / 2)
		{
			if (myHero.x0 == -2*MAZE_WIDTH * MAZE_UNIT)
			{
				myHero.x += STEP;
				//role.x = (int)role.xx;
				myHero.direction = 3;
			}
			else
			{
				myHero.x0 -= STEP;
				myHero.direction = 3;
			}
		}

		else if (myHero.x<MAZE_WIDTH*MAZE_UNIT / 2)
		{
			if (myHero.x + STEP > MAZE_WIDTH * MAZE_UNIT / 2)
			{
				myHero.x0 -= STEP;
				myHero.direction = 3;
			}
			else
			{
				myHero.x += STEP;
				//role.x = (int)role.xx;
				myHero.direction = 3;
			}
		}
		break;
	case 's':
	case 'S':
		myHero.direction = 1;
		temp.y += STEP;
		//temp.y = (int)temp.yy;
		if (touchmap_diff(temp, diffmap))
			return;

		else if (myHero.y >= MAZE_HEIGHT * MAZE_UNIT / 2)
		{
			if (myHero.y0 <= -2*MAZE_HEIGHT * MAZE_UNIT)
			{
				myHero.y += STEP;
				myHero.direction = 1;
			}
			else
			{
				myHero.y0 -= STEP;
				myHero.direction = 1;
			}
			//role.y = (int)role.yy;
		}

		else if (myHero.y<MAZE_HEIGHT*MAZE_UNIT / 2)
		{
			if (myHero.y + STEP > MAZE_HEIGHT * MAZE_UNIT / 2)
			{
				myHero.y0 -= STEP;
				myHero.direction = 1;
			}
			else
			{
				myHero.y += STEP;
				myHero.direction = 1;
			}
			//role.y = (int)role.yy;
		}

		break;
	case 'k':
	case 'K':
		if (myHero.isShoot&&yfszds<30)
			createzd();
		break;
	default:
		break;
		/*case 'k':
		case 'K':
		shoot();*/
	}
}
//创造怪物
void dragonball::creategw()
{
	int qs_x;
	int qs_y;

	for (int i = 0; i < GWNUM_EASY; i++)
	{
		do
		{
			qs_x = rand() % (EASY_REAL_WIDTH);
			qs_y = rand() % (EASY_REAL_HEIGHT);
		} while (easymap[qs_y][qs_x].mapID == WALL || easymap[qs_y][qs_x].mapID == BORDER);

		gw_easy[i].x = qs_x*MAZE_UNIT;
		gw_easy[i].y = qs_y*MAZE_UNIT;
		switch (i)
		{
		case 0:
			gw_easy[i].id = BLS;
			break;
		case 1:
			gw_easy[i].id = DS;
			break;
		case 2:
			gw_easy[i].id = HDS;
			break;
		case 3:
			gw_easy[i].id = HLS;
			break;
		case 4:
			gw_easy[i].id = HMS;
			break;
		case 5:
			gw_easy[i].id = JNG;
			break;
		case 6:
			gw_easy[i].id = LL;
			break;
		case 7:
			gw_easy[i].id = MMC;
			break;
		case 8:
			gw_easy[i].id = PHL;
			break;
		case 9:
			gw_easy[i].id = YGS;
			break;
		}
	}
}
void dragonball::creategw_diff()
{
	int qs_x;
	int qs_y;

	for (int i = 0; i < GWNUM_DIFF; i++)
	{
		do
		{
			qs_x = rand() % (DIFF_REAL_WIDTH);
			qs_y = rand() % (DIFF_REAL_HEIGHT);
		} while (diffmap[qs_y][qs_x].mapID == WALL || diffmap[qs_y][qs_x].mapID == BORDER);

		gw_diff[i].x = qs_x*MAZE_UNIT;
		gw_diff[i].y = qs_y*MAZE_UNIT;
		switch (i)
		{
		case 0:
		case 10:
		case 20:
			gw_diff[i].id = BLS;
			break;
		case 1:
		case 11:
		case 21:
			gw_diff[i].id = DS;
			break;
		case 2:
		case 12:
		case 22:
			gw_diff[i].id = HDS;
			break;
		case 3:
		case 13:
		case 23:
			gw_diff[i].id = HLS;
			break;
		case 4:
		case 14:
		case 24:
			gw_diff[i].id = HMS;
			break;
		case 5:
		case 15:
		case 25:
			gw_diff[i].id = JNG;
			break;
		case 6:
		case 16:
		case 26:
			gw_diff[i].id = LL;
			break;
		case 7:
		case 17:
		case 27:
			gw_diff[i].id = MMC;
			break;
		case 8:
		case 18:
		case 28:
			gw_diff[i].id = PHL;
			break;
		case 9:
		case 19:
		case 29:
			gw_diff[i].id = YGS;
			break;
		}
	}
}
//怪物动作
void dragonball::gw1act(gw& gw1)
{
	int gwxl;
	if (myHero.x <= gw1.x + myHero.x0&&myHero.y <= gw1.y + myHero.y0)
	{
		gwxl = rand() % 10;
		if (gwxl >= 0 && gwxl < 4)
			gwxl = 0;
		else if (gwxl >= 4 && gwxl < 8)
			gwxl = 2;
		else if (gwxl == 8)
			gwxl = 1;
		else
			gwxl = 3;
	}
	else if (myHero.x > gw1.x + myHero.x0&&myHero.y <= gw1.y + myHero.y0)
	{
		gwxl = rand() % 10;
		if (gwxl >= 0 && gwxl < 4)
			gwxl = 0;
		else if (gwxl >= 4 && gwxl < 8)
			gwxl = 3;
		else if (gwxl == 8)
			gwxl = 1;
		else
			gwxl = 2;
	}
	else if (myHero.x <= gw1.x + myHero.x0&&myHero.y > gw1.y + myHero.y0)
	{
		gwxl = rand() % 10;
		if (gwxl >= 0 && gwxl < 4)
			gwxl = 1;
		else if (gwxl >= 4 && gwxl < 8)
			gwxl = 2;
		else if (gwxl == 8)
			gwxl = 0;
		else
			gwxl = 3;
	}
	else
	{
		gwxl = rand() % 10;
		if (gwxl >= 0 && gwxl < 4)
			gwxl = 1;
		else if (gwxl >= 4 && gwxl < 8)
			gwxl = 3;
		else if (gwxl == 8)
			gwxl = 0;
		else
			gwxl = 2;
	}
	
	gw temp = { gw1.x,gw1.y,gw1.direction };
	switch (gwxl)
	{
	case 0:
		temp.y -= STEP;
		if (touchmap_gw(temp,easymap))
			return;
		else
		{
			gw1.y -= STEP;
			gw1.direction = 0;
		}
		break;
	case 1:
		temp.y += STEP;
		if (touchmap_gw(temp, easymap))
			return;
		else
		{
			gw1.y += STEP;
			gw1.direction = 1;
		}
		break;
	case 2:
		temp.x -= STEP;
		if (touchmap_gw(temp, easymap))
			return;
		else
		{
			gw1.x -= STEP;
			gw1.direction = 2;
		}
		break;
	case 3:
		temp.x += STEP;
		if (touchmap_gw(temp, easymap))
			return;
		else
		{
			gw1.x += STEP;
			gw1.direction = 3;
		}
		break;
	}
}
void dragonball::gw1act_diff(gw& gw1)
{
	int gwxl;
	if (myHero.x <= gw1.x + myHero.x0&&myHero.y <= gw1.y + myHero.y0)
	{
		gwxl = rand() % 10;
		if (gwxl >= 0 && gwxl < 4)
			gwxl = 0;
		else if (gwxl >= 4 && gwxl < 8)
			gwxl = 2;
		else if (gwxl == 8)
			gwxl = 1;
		else
			gwxl = 3;
	}
	else if (myHero.x > gw1.x + myHero.x0&&myHero.y <= gw1.y + myHero.y0)
	{
		gwxl = rand() % 10;
		if (gwxl >= 0 && gwxl < 4)
			gwxl = 0;
		else if (gwxl >= 4 && gwxl < 8)
			gwxl = 3;
		else if (gwxl == 8)
			gwxl = 1;
		else
			gwxl = 2;
	}
	else if (myHero.x <= gw1.x + myHero.x0&&myHero.y > gw1.y + myHero.y0)
	{
		gwxl = rand() % 10;
		if (gwxl >= 0 && gwxl < 4)
			gwxl = 1;
		else if (gwxl >= 4 && gwxl < 8)
			gwxl = 2;
		else if (gwxl == 8)
			gwxl = 0;
		else
			gwxl = 3;
	}
	else
	{
		gwxl = rand() % 10;
		if (gwxl >= 0 && gwxl < 4)
			gwxl = 1;
		else if (gwxl >= 4 && gwxl < 8)
			gwxl = 3;
		else if (gwxl == 8)
			gwxl = 0;
		else
			gwxl = 2;
	}

	gw temp = { gw1.x,gw1.y,gw1.direction };
	switch (gwxl)
	{
	case 0:
		temp.y -= STEP;
		if (touchmap_gw_diff(temp, diffmap))
			return;
		else
		{
			gw1.y -= STEP;
			gw1.direction = 0;
		}
		break;
	case 1:
		temp.y += STEP;
		if (touchmap_gw_diff(temp, diffmap))
			return;
		else
		{
			gw1.y += STEP;
			gw1.direction = 1;
		}
		break;
	case 2:
		temp.x -= STEP;
		if (touchmap_gw_diff(temp, diffmap))
			return;
		else
		{
			gw1.x -= STEP;
			gw1.direction = 2;
		}
		break;
	case 3:
		temp.x += STEP;
		if (touchmap_gw_diff(temp, diffmap))
			return;
		else
		{
			gw1.x += STEP;
			gw1.direction = 3;
		}
		break;
	}
}
//画出怪物
void dragonball::drawgw1(gw gw1)
{
	if (gw1.status == 1)
	{
		switch (gw1.id)
		{
		case BLS:
			if (gw1.direction == 3)
			{
				if (enemy_iframe[0] <= 3)
					enemy_iframe[0]++;
				else
					enemy_iframe[0] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg2, (hero_iframe - 1)*MAZE_UNIT, 2 * MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg1, (hero_iframe - 1)*MAZE_UNIT, 2 * MAZE_UNIT, SRCPAINT);
			}
			else if (gw1.direction == 2)
			{
				if (enemy_iframe[0] <= 3)
					enemy_iframe[0]++;
				else
					enemy_iframe[0] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg2, (hero_iframe - 1)* MAZE_UNIT, MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg1, (hero_iframe - 1)* MAZE_UNIT, MAZE_UNIT, SRCPAINT);
			}
			else if (gw1.direction == 0)
			{
				if (enemy_iframe[0] <= 3)
					enemy_iframe[0]++;
				else
					enemy_iframe[0] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg2, (hero_iframe - 1)* MAZE_UNIT, 3 * MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg1, (hero_iframe - 1)* MAZE_UNIT, 3 * MAZE_UNIT, SRCPAINT);
			}
			else
			{
				if (enemy_iframe[0] <= 3)
					enemy_iframe[0]++;
				else
					enemy_iframe[0] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg2, (hero_iframe - 1)* MAZE_UNIT, 0, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg1, (hero_iframe - 1)* MAZE_UNIT, 0, SRCPAINT);
			}
			break;
		case DS:
			if (gw1.direction == 3)
			{
				if (enemy_iframe[1] <= 3)
					enemy_iframe[1]++;
				else
					enemy_iframe[1] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg20, (hero_iframe - 1)*MAZE_UNIT, 2 * MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg19, (hero_iframe - 1)*MAZE_UNIT, 2 * MAZE_UNIT, SRCPAINT);
			}
			else if (gw1.direction == 2)
			{
				if (enemy_iframe[1] <= 3)
					enemy_iframe[1]++;
				else
					enemy_iframe[1] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg20, (hero_iframe - 1)* MAZE_UNIT, MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg19, (hero_iframe - 1)* MAZE_UNIT, MAZE_UNIT, SRCPAINT);
			}
			else if (gw1.direction == 0)
			{
				if (enemy_iframe[1] <= 3)
					enemy_iframe[1]++;
				else
					enemy_iframe[1] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg20, (hero_iframe - 1)* MAZE_UNIT, 3 * MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg19, (hero_iframe - 1)* MAZE_UNIT, 3 * MAZE_UNIT, SRCPAINT);
			}
			else
			{
				if (enemy_iframe[1] <= 3)
					enemy_iframe[1]++;
				else
					enemy_iframe[1] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg20, (hero_iframe - 1)* MAZE_UNIT, 0, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg19, (hero_iframe - 1)* MAZE_UNIT, 0, SRCPAINT);
			}
			break;
		case HDS:
			if (gw1.direction == 3)
			{
				if (enemy_iframe[2] <= 3)
					enemy_iframe[2]++;
				else
					enemy_iframe[2] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg4, (hero_iframe - 1)*MAZE_UNIT, 2 * MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg3, (hero_iframe - 1)*MAZE_UNIT, 2 * MAZE_UNIT, SRCPAINT);
			}
			else if (gw1.direction == 2)
			{
				if (enemy_iframe[2] <= 3)
					enemy_iframe[2]++;
				else
					enemy_iframe[2] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg4, (hero_iframe - 1)* MAZE_UNIT, MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg3, (hero_iframe - 1)* MAZE_UNIT, MAZE_UNIT, SRCPAINT);
			}
			else if (gw1.direction == 0)
			{
				if (enemy_iframe[2] <= 3)
					enemy_iframe[2]++;
				else
					enemy_iframe[2] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg4, (hero_iframe - 1)* MAZE_UNIT, 3 * MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg3, (hero_iframe - 1)* MAZE_UNIT, 3 * MAZE_UNIT, SRCPAINT);
			}
			else
			{
				if (enemy_iframe[2] <= 3)
					enemy_iframe[2]++;
				else
					enemy_iframe[2] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg4, (hero_iframe - 1)* MAZE_UNIT, 0, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg3, (hero_iframe - 1)* MAZE_UNIT, 0, SRCPAINT);
			}
			break;
		case HLS:
			if (gw1.direction == 3)
			{
				if (enemy_iframe[3] <= 3)
					enemy_iframe[3]++;
				else
					enemy_iframe[3] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg6, (hero_iframe - 1)*MAZE_UNIT, 2 * MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg5, (hero_iframe - 1)*MAZE_UNIT, 2 * MAZE_UNIT, SRCPAINT);
			}
			else if (gw1.direction == 2)
			{
				if (enemy_iframe[3] <= 3)
					enemy_iframe[3]++;
				else
					enemy_iframe[3] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg6, (hero_iframe - 1)* MAZE_UNIT, MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg5, (hero_iframe - 1)* MAZE_UNIT, MAZE_UNIT, SRCPAINT);
			}
			else if (gw1.direction == 0)
			{
				if (enemy_iframe[3] <= 3)
					enemy_iframe[3]++;
				else
					enemy_iframe[3] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg6, (hero_iframe - 1)* MAZE_UNIT, 3 * MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg5, (hero_iframe - 1)* MAZE_UNIT, 3 * MAZE_UNIT, SRCPAINT);
			}
			else
			{
				if (enemy_iframe[3] <= 3)
					enemy_iframe[3]++;
				else
					enemy_iframe[3] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg6, (hero_iframe - 1)* MAZE_UNIT, 0, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg5, (hero_iframe - 1)* MAZE_UNIT, 0, SRCPAINT);
			}
			break;
		case HMS:
			if (gw1.direction == 3)
			{
				if (enemy_iframe[4] <= 3)
					enemy_iframe[4]++;
				else
					enemy_iframe[4] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg8, (hero_iframe - 1)*MAZE_UNIT, 2 * MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg7, (hero_iframe - 1)*MAZE_UNIT, 2 * MAZE_UNIT, SRCPAINT);
			}
			else if (gw1.direction == 2)
			{
				if (enemy_iframe[4] <= 3)
					enemy_iframe[4]++;
				else
					enemy_iframe[4] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg8, (hero_iframe - 1)* MAZE_UNIT, MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg7, (hero_iframe - 1)* MAZE_UNIT, MAZE_UNIT, SRCPAINT);
			}
			else if (gw1.direction == 0)
			{
				if (enemy_iframe[4] <= 3)
					enemy_iframe[4]++;
				else
					enemy_iframe[4] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg8, (hero_iframe - 1)* MAZE_UNIT, 3 * MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg7, (hero_iframe - 1)* MAZE_UNIT, 3 * MAZE_UNIT, SRCPAINT);
			}
			else
			{
				if (enemy_iframe[4] <= 3)
					enemy_iframe[4]++;
				else
					enemy_iframe[4] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg8, (hero_iframe - 1)* MAZE_UNIT, 0, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg7, (hero_iframe - 1)* MAZE_UNIT, 0, SRCPAINT);
			}
			break;
		case JNG:
			if (gw1.direction == 3)
			{
				if (enemy_iframe[5] <= 3)
					enemy_iframe[5]++;
				else
					enemy_iframe[5] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg0, (hero_iframe - 1)*MAZE_UNIT, 2 * MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg9, (hero_iframe - 1)*MAZE_UNIT, 2 * MAZE_UNIT, SRCPAINT);
			}
			else if (gw1.direction == 2)
			{
				if (enemy_iframe[5] <= 3)
					enemy_iframe[5]++;
				else
					enemy_iframe[5] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg0, (hero_iframe - 1)* MAZE_UNIT, MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg9, (hero_iframe - 1)* MAZE_UNIT, MAZE_UNIT, SRCPAINT);
			}
			else if (gw1.direction == 0)
			{
				if (enemy_iframe[5] <= 3)
					enemy_iframe[5]++;
				else
					enemy_iframe[5] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg0, (hero_iframe - 1)* MAZE_UNIT, 3 * MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg9, (hero_iframe - 1)* MAZE_UNIT, 3 * MAZE_UNIT, SRCPAINT);
			}
			else
			{
				if (enemy_iframe[5] <= 3)
					enemy_iframe[5]++;
				else
					enemy_iframe[5] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg0, (hero_iframe - 1)* MAZE_UNIT, 0, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg9, (hero_iframe - 1)* MAZE_UNIT, 0, SRCPAINT);
			}
			break;
		case LL:
			if (gw1.direction == 3)
			{
				if (enemy_iframe[6] <= 3)
					enemy_iframe[6]++;
				else
					enemy_iframe[6] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg12, (hero_iframe - 1)*MAZE_UNIT, 2 * MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg11, (hero_iframe - 1)*MAZE_UNIT, 2 * MAZE_UNIT, SRCPAINT);
			}
			else if (gw1.direction == 2)
			{
				if (enemy_iframe[6] <= 3)
					enemy_iframe[6]++;
				else
					enemy_iframe[6] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg12, (hero_iframe - 1)* MAZE_UNIT, MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg11, (hero_iframe - 1)* MAZE_UNIT, MAZE_UNIT, SRCPAINT);
			}
			else if (gw1.direction == 0)
			{
				if (enemy_iframe[6] <= 3)
					enemy_iframe[6]++;
				else
					enemy_iframe[6] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg12, (hero_iframe - 1)* MAZE_UNIT, 3 * MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg11, (hero_iframe - 1)* MAZE_UNIT, 3 * MAZE_UNIT, SRCPAINT);
			}
			else
			{
				if (enemy_iframe[6] <= 3)
					enemy_iframe[6]++;
				else
					enemy_iframe[6] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg12, (hero_iframe - 1)* MAZE_UNIT, 0, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg11, (hero_iframe - 1)* MAZE_UNIT, 0, SRCPAINT);
			}
			break;
		case MMC:
			if (gw1.direction == 3)
			{
				if (enemy_iframe[7] <= 3)
					enemy_iframe[7]++;
				else
					enemy_iframe[7] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg14, (hero_iframe - 1)*MAZE_UNIT, 2 * MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg13, (hero_iframe - 1)*MAZE_UNIT, 2 * MAZE_UNIT, SRCPAINT);
			}
			else if (gw1.direction == 2)
			{
				if (enemy_iframe[7] <= 3)
					enemy_iframe[7]++;
				else
					enemy_iframe[7] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg14, (hero_iframe - 1)* MAZE_UNIT, MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg13, (hero_iframe - 1)* MAZE_UNIT, MAZE_UNIT, SRCPAINT);
			}
			else if (gw1.direction == 0)
			{
				if (enemy_iframe[7] <= 3)
					enemy_iframe[7]++;
				else
					enemy_iframe[7] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg14, (hero_iframe - 1)* MAZE_UNIT, 3 * MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg13, (hero_iframe - 1)* MAZE_UNIT, 3 * MAZE_UNIT, SRCPAINT);
			}
			else
			{
				if (enemy_iframe[7] <= 3)
					enemy_iframe[7]++;
				else
					enemy_iframe[7] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg14, (hero_iframe - 1)* MAZE_UNIT, 0, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg13, (hero_iframe - 1)* MAZE_UNIT, 0, SRCPAINT);
			}
			break;
		case PHL:
			if (gw1.direction == 3)
			{
				if (enemy_iframe[8] <= 3)
					enemy_iframe[8]++;
				else
					enemy_iframe[8] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg16, (hero_iframe - 1)*MAZE_UNIT, 2 * MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg15, (hero_iframe - 1)*MAZE_UNIT, 2 * MAZE_UNIT, SRCPAINT);
			}
			else if (gw1.direction == 2)
			{
				if (enemy_iframe[8] <= 3)
					enemy_iframe[8]++;
				else
					enemy_iframe[8] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg16, (hero_iframe - 1)* MAZE_UNIT, MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg15, (hero_iframe - 1)* MAZE_UNIT, MAZE_UNIT, SRCPAINT);
			}
			else if (gw1.direction == 0)
			{
				if (enemy_iframe[8] <= 3)
					enemy_iframe[8]++;
				else
					enemy_iframe[8] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg16, (hero_iframe - 1)* MAZE_UNIT, 3 * MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg15, (hero_iframe - 1)* MAZE_UNIT, 3 * MAZE_UNIT, SRCPAINT);
			}
			else
			{
				if (enemy_iframe[8] <= 3)
					enemy_iframe[8]++;
				else
					enemy_iframe[8] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg16, (hero_iframe - 1)* MAZE_UNIT, 0, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg15, (hero_iframe - 1)* MAZE_UNIT, 0, SRCPAINT);
			}
			break;
		case YGS:
			if (gw1.direction == 3)
			{
				if (enemy_iframe[9] <= 3)
					enemy_iframe[9]++;
				else
					enemy_iframe[9] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg18, (hero_iframe - 1)*MAZE_UNIT, 2 * MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg17, (hero_iframe - 1)*MAZE_UNIT, 2 * MAZE_UNIT, SRCPAINT);
			}
			else if (gw1.direction == 2)
			{
				if (enemy_iframe[9] <= 3)
					enemy_iframe[9]++;
				else
					enemy_iframe[9] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg18, (hero_iframe - 1)* MAZE_UNIT, MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg17, (hero_iframe - 1)* MAZE_UNIT, MAZE_UNIT, SRCPAINT);
			}
			else if (gw1.direction == 0)
			{
				if (enemy_iframe[9] <= 3)
					enemy_iframe[9]++;
				else
					enemy_iframe[9] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg18, (hero_iframe - 1)* MAZE_UNIT, 3 * MAZE_UNIT, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg17, (hero_iframe - 1)* MAZE_UNIT, 3 * MAZE_UNIT, SRCPAINT);
			}
			else
			{
				if (enemy_iframe[9] <= 3)
					enemy_iframe[9]++;
				else
					enemy_iframe[9] = 1;
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg18, (hero_iframe - 1)* MAZE_UNIT, 0, SRCAND);
				putimage(myHero.x0 + gw1.x, myHero.y0 + gw1.y, MAZE_UNIT, MAZE_UNIT, &roleimg17, (hero_iframe - 1)* MAZE_UNIT, 0, SRCPAINT);
			}
			break;
		}
		putimage(myHero.x0 + gw1.x + MAZE_UNIT, myHero.y0 + gw1.y - 4, &gwbloodymt, SRCAND);
		putimage(myHero.x0 + gw1.x + MAZE_UNIT, myHero.y0 + gw1.y - 4, &gwblood, SRCPAINT);
		setfillstyle(0);
		setfillcolor(BLACK);
		solidrectangle(0.4*gw1.blood + myHero.x0 + gw1.x + MAZE_UNIT, myHero.y0 + gw1.y - 4, myHero.x0 + gw1.x + MAZE_UNIT + 40, myHero.y0 + gw1.y);
	}
}
//游戏结束
void dragonball::gameover()
{
	BeginBatchDraw();
	mciSendString("stop bjyy", NULL, 0, NULL);
	mciSendString("play yxjs from 0", NULL, 0, NULL);
	IMAGE gameoverimg, gameoverymtimg;
	loadimage(&gameoverimg, _T("..\\res\\游戏结束.bmp"));
	loadimage(&gameoverymtimg, _T("..\\res\\游戏结束ymt.bmp"));

	putimage(342, 192, &gameoverymtimg, SRCAND);
	putimage(342, 192, &gameoverimg, SRCPAINT);
	FlushBatchDraw();

	Sleep(5000);
}
//游戏胜利
void dragonball::gamewin()
{
	ispose = 1;
	keyState = MUSICOFF;
	mciSendString("stop bjyy", NULL, 0, NULL);
	IMAGE zhsl;
	loadimage(&zhsl, _T("..\\res\\游戏胜利.bmp"));
	int oldtime0 = GetTickCount();
	while (GetTickCount() - oldtime0 < 5000)
	{
		BeginBatchDraw();
		putimage(171, 0, &zhsl,SRCCOPY);
		FlushBatchDraw();
	}
	mciSendString("open ..\\res\\召唤神龙.avi type MPEGVideo alias mymovie", NULL, 0, NULL);
	mciSendString("play mymovie from 0", NULL, 0, NULL);
}
//创造食物
void dragonball::createTLS(int mapkind)
{
	int qs_x, qs_y;
	if (mapkind == EASY)
	{
		tls.state = 1;
		do {
			qs_x = rand() % (EASY_REAL_WIDTH);
			qs_y = rand() % (EASY_REAL_HEIGHT);
		} while (easymap[qs_y][qs_x].mapID == WALL || easymap[qs_y][qs_x].mapID == BORDER);
		tls.x = qs_x * MAZE_UNIT;
		tls.y = qs_y * MAZE_UNIT;
	}
	else
	{
		tls.state = 1;
		do {
			qs_x = rand() % (DIFF_REAL_WIDTH);
			qs_y = rand() % (DIFF_REAL_HEIGHT);
		} while (diffmap[qs_y][qs_x].mapID == WALL || diffmap[qs_y][qs_x].mapID == BORDER);
		tls.x = qs_x * MAZE_UNIT;
		tls.y = qs_y * MAZE_UNIT;
	}
}
//画出食物
void dragonball::drawTLS()
{
	if (tls.state == 1)
	{
		loadimage(&tlsimg, _T("..\\res\\tls.bmp"));
		loadimage(&tlsymtimg, _T("..\\res\\tlsymt.bmp"));
		putimage(myHero.x0+tls.x, myHero.y0+tls.y, &tlsymtimg, SRCAND);
		putimage(myHero.x0+tls.x, myHero.y0+tls.y, &tlsimg, SRCPAINT);
	}
	//FlushBatchDraw();
}
//运行游戏
void run()
{
	dragonball myDragon;
	initgraph(WIN_WIDTH, WIN_HEIGHT);
	// 置随机数种子 
	srand(unsigned(time(NULL)));
	// 初始化设备，加载图片
	myDragon.welcome();
	radar_iframe = 1;
	while (true)
	{
		while (MouseHit())
		{
			mmsg = GetMouseMsg();
			switch (mmsg.uMsg)
			{
			case WM_MOUSEMOVE:  mouseX = mmsg.x; mouseY = mmsg.y; break;
			case WM_LBUTTONDOWN: isMouseDown = true; break;
			case WM_LBUTTONUP: isMouseDown = false; break;
			default:break;
			}
		}
		myDragon.DrawMenuItems();
		Delay(50);
	}
	closegraph();
}

void dragonball::createzd()
{
	zd[yfszds].birthtime = GetTickCount();
	if (myHero.direction == 3)
	{
		zd[yfszds].state = 1;
		zd[yfszds].direction = 3;
		zd[yfszds].x = -myHero.x0 + myHero.x + MAZE_UNIT;
		zd[yfszds].y = -myHero.y0 + myHero.y;
	}
	else if (myHero.direction == 1)
	{
		zd[yfszds].state = 1;
		zd[yfszds].direction = 1;
		zd[yfszds].x = -myHero.x0 + myHero.x;
		zd[yfszds].y = -myHero.y0 + myHero.y + MAZE_UNIT;
	}
	else if (myHero.direction == 0)
	{
		zd[yfszds].state = 1;
		zd[yfszds].direction = 0;
		zd[yfszds].x = -myHero.x0 + myHero.x ;
		zd[yfszds].y = -myHero.y0 + myHero.y - MAZE_UNIT;
	}
	else
	{
		zd[yfszds].state = 1;
		zd[yfszds].direction = 2;
		zd[yfszds].x = -myHero.x0 + myHero.x - MAZE_UNIT;
		zd[yfszds].y = -myHero.y0 + myHero.y ;
	}
	if (yfszds == 29)
	{
		for (int i = 0; i < 30; i++)
		{
			zd[i].birthtime = 0;
			zd[i].state = 0;
		}
		yfszds = 0;
	}
	else
		yfszds++;
}

void dragonball::zdact(int mapkind)
{
	for (int i = 0; i < yfszds; i++)
	{
		if (zd[i].state == 1)
		{
			POINT r[4];
			POINT m[2];
			r[0].x = zd[i].x + 1;
			r[0].y = zd[i].y + 1;
			r[1].x = zd[i].x + MAZE_UNIT - 1;
			r[1].y = zd[i].y + 1;
			r[2].x = zd[i].x + 1;
			r[2].y = zd[i].y + MAZE_UNIT - 1;
			r[3].x = zd[i].x + MAZE_UNIT - 1;
			r[3].y = zd[i].y + MAZE_UNIT - 1;
			if (mapkind == EASY)
			{
				for (int j = 0; j < EASY_REAL_WIDTH; j++)
				{
					for (int k = 0; k < EASY_REAL_HEIGHT; k++)
					{
						if (easymap[k][j].mapID == WALL || easymap[k][j].mapID == BORDER)
						{
							m[0].x = easymap[k][j].mapx*MAZE_UNIT;
							m[0].y = easymap[k][j].mapy*MAZE_UNIT;
							m[1].x = (easymap[k][j].mapx + 1)*MAZE_UNIT;
							m[1].y = (easymap[k][j].mapy + 1)*MAZE_UNIT;
							if (isTouch(r, m))
							{
								zd[i].touchmap = 1;
								break;
							}
						}
					}
				}
				for (int j = 0; j < GWNUM_EASY; j++)
				{
					if (gw_easy[j].status == 1)
					{
						m[0].x = gw_easy[j].x;
						m[0].y = gw_easy[j].y;
						m[1].x = gw_easy[j].x + MAZE_UNIT;
						m[1].y = gw_easy[j].y + MAZE_UNIT;
						if (isTouch(r, m))
						{
							zd[i].touchgw = 1;
							zd[i].gw = j;
							break;
						}
						else
							zd[i].touchgw = 0;
					}
				}
			}
			else
			{
				for (int j = 0; j < DIFF_REAL_WIDTH; j++)
				{
					for (int k = 0; k < DIFF_REAL_WIDTH; k++)
					{
						if (diffmap[k][j].mapID == WALL || diffmap[k][j].mapID == BORDER)
						{
							m[0].x = diffmap[k][j].mapx*MAZE_UNIT;
							m[0].y = diffmap[k][j].mapy*MAZE_UNIT;
							m[1].x = (diffmap[k][j].mapx + 1)*MAZE_UNIT;
							m[1].y = (diffmap[k][j].mapy + 1)*MAZE_UNIT;
							if (isTouch(r, m))
							{
								zd[i].touchmap = 1;
								break;
							}
						}
					}
				}
				for (int j = 0; j < GWNUM_DIFF; j++)
				{
					if (gw_diff[j].status == 1)
					{
						m[0].x = gw_diff[j].x;
						m[0].y = gw_diff[j].y;
						m[1].x = gw_diff[j].x + MAZE_UNIT;
						m[1].y = gw_diff[j].y + MAZE_UNIT;
						if (isTouch(r, m))
						{
							zd[i].touchgw = 1;
							zd[i].gw = j;
							break;
						}
						else
							zd[i].touchgw = 0;
					}
				}
			}
			switch (zd[i].direction)
			{
			case 0:
				if (zd[i].touchmap != 1 && zd[i].touchgw != 1 && GetTickCount() - zd[i].birthtime < 10000)
				{
					zd[i].state = 1;
					zd[i].y -= STEP / 2;
				}
				else
				{
					zd[i].state = 0;
					zd[i].isjust = 1;//就在刚刚碰到了障碍
					if (mapkind == EASY)
					{
						if (zd[i].touchgw)
						{
							gw_easy[zd[i].gw].blood -= 4;
							if (gw_easy[zd[i].gw].blood <= 0)
								gw_easy[zd[i].gw].status = 0;
						}
					}
					else
					{
						if (zd[i].touchgw)
						{
							gw_diff[zd[i].gw].blood -= 4;
							if (gw_diff[zd[i].gw].blood <= 0)
								gw_diff[zd[i].gw].status = 0;
						}
					}
				}
				break;
			case 1:
				if (zd[i].touchmap != 1 && zd[i].touchgw != 1 && GetTickCount() - zd[i].birthtime < 10000)
				{
					zd[i].state = 1;
					zd[i].y += STEP / 2;
				}
				else
				{
					zd[i].state = 0;
					zd[i].isjust = 1;//就在刚刚碰到了障碍
					if (mapkind == EASY)
					{
						if (zd[i].touchgw)
						{
							gw_easy[zd[i].gw].blood -= 4;
							if (gw_easy[zd[i].gw].blood <= 0)
								gw_easy[zd[i].gw].status = 0;
						}
					}
					else
					{
						if (zd[i].touchgw)
						{
							gw_diff[zd[i].gw].blood -= 4;
							if (gw_diff[zd[i].gw].blood <= 0)
								gw_diff[zd[i].gw].status = 0;
						}
					}
				}
				break;
			case 2:
				if (zd[i].touchmap != 1 && zd[i].touchgw != 1 && GetTickCount() - zd[i].birthtime < 10000)
				{
					zd[i].state = 1;
					zd[i].x -= STEP / 2;
				}
				else
				{
					zd[i].state = 0;
					zd[i].isjust = 1;//就在刚刚碰到了障碍
					if (mapkind == EASY)
					{
						if (zd[i].touchgw)
						{
							gw_easy[zd[i].gw].blood -= 4;
							if (gw_easy[zd[i].gw].blood <= 0)
								gw_easy[zd[i].gw].status = 0;
						}
					}
					else
					{
						if (zd[i].touchgw)
						{
							gw_diff[zd[i].gw].blood -= 4;
							if (gw_diff[zd[i].gw].blood <= 0)
								gw_diff[zd[i].gw].status = 0;
						}
					}
				}
				break;
			case 3:
				if (zd[i].touchmap != 1 && zd[i].touchgw != 1 && GetTickCount() - zd[i].birthtime < 10000)
				{
					zd[i].state = 1;
					zd[i].x += STEP / 2;
				}
				else
				{
					zd[i].state = 0;
					zd[i].isjust = 1;//就在刚刚碰到了障碍
					if (mapkind == EASY)
					{
						if (zd[i].touchgw)
						{
							gw_easy[zd[i].gw].blood -= 4;
							if (gw_easy[zd[i].gw].blood <= 0)
								gw_easy[zd[i].gw].status = 0;
						}
					}
					else
					{
						if (zd[i].touchgw)
						{
							gw_diff[zd[i].gw].blood -= 4;
							if (gw_diff[zd[i].gw].blood <= 0)
								gw_diff[zd[i].gw].status = 0;
						}
					}
				}
				break;
			}
		}
	}
}

void dragonball::drawzd()
{
	for (int i = 0; i < yfszds; i++)
	{
		if (zd[i].state == 1)
		{
			putimage(myHero.x0 + zd[i].x, myHero.y0 + zd[i].y, &bulletymt, SRCAND);
			putimage(myHero.x0 + zd[i].x, myHero.y0 + zd[i].y, &bullet, SRCPAINT);
		}
		else
		{
			if (zd[i].isjust == 1)
			{
				putimage(myHero.x0 + zd[i].x, myHero.y0 + zd[i].y, &bombymt, SRCAND);
				putimage(myHero.x0 + zd[i].x, myHero.y0 + zd[i].y, &bomb, SRCPAINT);
				zd[i].isjust = 0;
				zd[i].touchgw = 0;
				zd[i].touchmap = 0;
			}
		}
	}
}

/**********************************************************主函数*******************************************************************************************/
int main()
{
	run();
	return 0;
}