#pragma once
#include "Entity.h"
#include "GameScene.h"

class GameManage
{
public:
	static GameScene *curScene;		// 当前
	static int wndHeight;			// 窗口尺寸
	static int wndWidth;

	static HINSTANCE hInst;			// current instance,在主文件中的全局变量
	static bool reset_time;			// 是否更新当前关卡的计时器

	static HBITMAP logo, succ_pic, fail_pic;	// logo图片和成功失败图片

	static int timeStep;			// 定时器时间间隔
	static int animStep;			// 动画帧更新定时器时间间隔
	static int birthTime;			// 敌人的出生时间间隔

	static void EnemyBirth();		// 按照定时器生成敌人
	static void NextFrame(int step);// 将当前所有动画物体设置为下一帧
	static void EnterKey(int key);	// 处理用户按键输入
	static void Destroy();			// 销毁当前关卡
	static void ChangeWndSize(int w, int h);// 改变窗口尺寸
	static void ChangeLevel(LEVEL newL);	// 转换关卡
	static void Update(int ts);		// 更新各种游戏信息,定时器会触发这个函数
	static void Draw(HDC hdc, HWND hWnd);	// 绘制当前关卡

	GameManage(void);
	~GameManage(void);
};

