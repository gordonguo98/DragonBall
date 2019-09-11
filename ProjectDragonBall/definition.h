#pragma once

#ifndef MYDEFINE_H
#define MYDIFINE_H

#define WIN_WIDTH 1196
#define WIN_HEIGHT 640
#define EASY_NUM 2
#define DIFF_NUM 3
#define MAZE_WIDTH 28//地图的可视宽度
#define MAZE_HEIGHT 20//地图的可视高度
#define EASY_REAL_WIDTH EASY_NUM*MAZE_WIDTH//简单模式地图的实际宽度
#define EASY_REAL_HEIGHT EASY_NUM*MAZE_HEIGHT//简单模式地图的实际高度
#define DIFF_REAL_WIDTH DIFF_NUM*MAZE_WIDTH//困难模式地图的实际宽度
#define DIFF_REAL_HEIGHT DIFF_NUM*MAZE_HEIGHT//困难模式地图的实际高度
#define MAZE_UNIT 32//所有图案的长度和宽度都是32像素
#define FLOOR 0
#define WALL 1
#define BORDER 2
#define DOOR 3
#define STAYING 0
#define WALKING 1

#define MUSICON 1
#define MUSICOFF 0

#define ROLE_MENU_DOWN 1
#define ROLE_MENU_UP 0

#define MENU_POSE 1
#define MENU_GOON 0

#define MAIN_MENU_DOWN 1
#define MAIN_MENU_UP 0

#define PHL 0
#define LL 1
#define YGS 2
#define DS 3
#define MMC 4
#define JNG 5
#define BLS 6
#define HDS 7
#define HLS 8
#define HMS 9
#define GWNUM_EASY 10
#define GWNUM_DIFF 20
#define EASY 0
#define DIFF 1

#define X0 0//主角的起始坐标
#define Y0 0
#define TIME 32
#define ENEMY_STEP 1//敌人走一步相距的像素个数

#endif