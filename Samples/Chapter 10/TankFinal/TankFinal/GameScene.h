#pragma once
//#include "GameManage.h"
#include "Entity.h"
#include <list>
using namespace std;
// 游戏的关卡
enum LEVEL {OPEN, LEVEL1, LEVEL2_OPEN, LEVEL2, SUCCEED, FAIL, NOT_READY};

class GameScene
{
private:
	WCHAR *fileIni;	// 用于保存当前所要载入的Ini文件名
	HBITMAP tank1[4],tank2[4],tank3[4],bulletPic[4],block,background;	// 各种物体的图片

	int nScore;				// 玩家得分
	bool bFire;				// 是否处于开火状态
	LEVEL curL;
	int nLife;				// 玩家生命
	int enemyFirePer;		// 敌人发射子弹的随机比例
	int enemyDirPer;		// 敌人随机改变方向的比例
	int nPassScore;			// 过关分数


	list<Entity *> pHeadBlocks;	// 障碍物链表
	list<Tank *> pHeadEnemys;	// 敌人链表
	list<Entity *> pHeadBullets;// 玩家子弹链表
	list<Entity *> pHeadEnemyBullets;// 敌人子弹链表
	Tank * pPlayer;			// 玩家
private:
	void ReadIni();			// 通过ini文件读取当前关卡的部分初始化信息
	void InitMap();			// 读取地图配置文件,标记为1的地方有障碍物
	int RandomFire();		// 敌人随机开火
	int RandomDir();		// 敌人随机改变方向
	void DrawScene(HDC hdc);// 回执当前关卡
	void DrawGameScene(HDC hdc);// 绘制游戏关卡
	void DrawEntities(HDC hdc);	// 绘制场景中的所有角色
	void MoveEntities(int ts);// 对链表中的每个实体进行移动
public:
	GameScene(LEVEL l);		// 构造函数，生成当前游戏关卡对象
	~GameScene(void);		// 析构函数，需要删除动态分配的对象
	void Init();			// 依据当前场景的不同对游戏内容进行初始化
	HBITMAP GetBackPic();	// 获取当前关卡的背景图片句柄
	int GetBirthTime();		// 获取随机生成几率
	void EnemyBirth();		// 依据敌人类型生成一个敌人,并添加到敌人链表中
	int GetPassScore();		// 获取通关分数
	void EnterKey(int key);	// 处理玩家输入
	void ResetPlayer();		// 玩家角色重置
	void Draw(HDC hdc, HWND hWnd);	// 绘制当前游戏关卡
	void NextFrame(int ts);	// 将动画物体切换到下一帧
	void Update(int ts);	// 更新当前帧

};

