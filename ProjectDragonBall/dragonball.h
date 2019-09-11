#pragma once
#ifndef DRAGONBALL_H
#define DRAGONBALL_H

struct gw//����Ľṹ��
{
	int x;
	int y;
	int direction;
	int status = 1;//�����״̬
	int id;//���������
	double blood=100.00;//�����Ѫ��
};

class dragonball
{
public:
	dragonball();
	void reset();
	void yxjs();//��Ϸ����
	void rwjs();//��ɫ�л�
	void welcome();//��ӭ����
	bool IsInRect(int, int, RECT);//�ж�����Ƿ��ھ���������
	void DrawDerivedMenu();//�����Ӳ˵�
	void single();//����ģʽ
	void multiple();
	void easy();//��ģʽ
	void difficult();//����ģʽ
	void init_map(int[][EASY_REAL_WIDTH]);//��ʼ����ͼ
	void init_map_diff(int map[][DIFF_REAL_WIDTH]);
	void createmap(int, int, int[][EASY_REAL_WIDTH]);//�����ͼ
	void createmap_diff(int x, int y, int map[][DIFF_REAL_WIDTH]);
	void drawmap(int[][EASY_REAL_WIDTH]);//����ͼ
	void drawmap_diff(int map[][DIFF_REAL_WIDTH]);
	void DrawMenuItems();//�����˵�
	void display();//չʾ��Ϣ
	void radar(int);//�����״�
	void mapact();
	void showeasymap();//չʾ��ͼ
	void showeasymap_diff();
	void showblood();
	void createball(int);//��������
	void drawball();//��������
	void createrole();//���ɽ�ɫ
	void drawrole();//������ɫ
	void roleact(char);//��ɫ�Ķ���
	void roleact_diff(char ch);
	void creategw();//���ɹ���
	void creategw_diff();
	void gw1act(gw&);//����Ķ���
	void gw1act_diff(gw&);
	void drawgw1(gw);//��������
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