#pragma once
#ifndef DRAGONBALL_H
#define DRAGONBALL_H

struct gw//怪物的结构体
{
	int x;
	int y;
	int direction;
	int status = 1;//怪物的状态
	int id;//怪物的种类
	double blood=100.00;//怪物的血量
};

class dragonball
{
public:
	dragonball();
	void reset();
	void yxjs();//游戏介绍
	void rwjs();//角色切换
	void welcome();//欢迎界面
	bool IsInRect(int, int, RECT);//判断鼠标是否在矩形区域内
	void DrawDerivedMenu();//画出子菜单
	void single();//单人模式
	void multiple();
	void easy();//简单模式
	void difficult();//困难模式
	void init_map(int[][EASY_REAL_WIDTH]);//初始化地图
	void init_map_diff(int map[][DIFF_REAL_WIDTH]);
	void createmap(int, int, int[][EASY_REAL_WIDTH]);//创造地图
	void createmap_diff(int x, int y, int map[][DIFF_REAL_WIDTH]);
	void drawmap(int[][EASY_REAL_WIDTH]);//画地图
	void drawmap_diff(int map[][DIFF_REAL_WIDTH]);
	void DrawMenuItems();//画出菜单
	void display();//展示信息
	void radar(int);//画出雷达
	void mapact();
	void showeasymap();//展示地图
	void showeasymap_diff();
	void showblood();
	void createball(int);//生成龙珠
	void drawball();//画出龙珠
	void createrole();//生成角色
	void drawrole();//画出角色
	void roleact(char);//角色的动作
	void roleact_diff(char ch);
	void creategw();//生成怪物
	void creategw_diff();
	void gw1act(gw&);//怪物的动作
	void gw1act_diff(gw&);
	void drawgw1(gw);//画出怪物
	//bool touchmap(Hero &, Map[][EASY_REAL_WIDTH]);
	//bool isTouch(POINT *p1, POINT *p2);
	void gameover();
	void gamewin();
	void createTLS(int);
	void drawTLS();
	void createzd();
	void zdact(int);
	void drawzd();
};

#endif