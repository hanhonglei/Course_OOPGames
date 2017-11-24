#ifndef _CLASSES_H_
#define  _CLASSES_H_

#define MAX_ENEMY 16			// 最大敌人数目
#define MAX_BULLETS 32			// 最大子弹数目
const int timeStep = 20;		// 定时器时间间隔
const int sz = 50;				// 坦克尺寸
const int velf = 4;				// 快速坦克速率
const int vels = 2;				// 慢速坦克速率
const int szb = 20;				// 子弹尺寸
const int velb = 6;				// 子弹速率
const int enemyFirePer = 300;	// 敌人发射子弹的随机比例
const int enemyDir = 200;		// 敌人改变方向的随机比例
extern int wndWidth;	extern int wndHeight;  // 窗口尺寸
extern bool bInit;				// 是否已经完成初始化
#define ALL_ENTITIES MAX_ENEMY+MAX_BULLETS+MAX_BULLETS+1

enum Dir{UP, DOWN, LEFT, RIGHT};// 枚举类型，运动物体可能的运动方向

class Entity	// 基类，用来表示游戏里面的所有物体，包括子弹和坦克
{
public:
	static int number;
protected:
	int x, y;				// 位置
	Dir dir;				// 方向
	int v;					// 速率
	int s;					// 边长，正方形
	int b;					// 是否是子弹
	int p;					// 是否停止,只有玩家才可能停止
	int e;					// 是否是敌人
	COLORREF c;				// 颜色
	bool isValid;			//
public:
	// 默认构造函数
	Entity();
	// 对特定游戏实体依据朝向和速率进行移动
	void Move(int ts);
	// 判断两个实体是否发生碰撞,以正方形之间发生碰撞来判断
	int IsCollide(const Entity *other);
	// 判断特定实体是否和边界发生碰撞
	int WallCollide();
	// 绘制参数指定的游戏实体
	virtual void DrawEntity(HDC hdc);
	// 为该实体设定特定参数
	void Set(int sz, COLORREF cl, Dir direction, int px, int py, bool bv = true);
	void SetValid(bool bv);
	bool IsValid();
};

class Tank:public Entity		// 坦克基类，继承自游戏实体类
{
public:
	// 绘制坦克，按照类型不同，绘制不同颜色
	void DrawEntity(HDC hdc);
	// 对坦克进行初始化
	void Init();
};

class PlayerTank:public Tank	// 玩家坦克，继承自坦克类
{
public:
	bool bFire;					// 玩家是否处于射击状态
	int nLife;					// 玩家生命
	int nScore;					// 玩家得分
	static int nBullet;			// 玩家打出的子弹数量
	// 重置玩家信息,恢复到初始状态
	void ResetPlayer();
	// 默认构造函数
	PlayerTank();
	// 处理用户输入
	void Interaction(WPARAM wParam);
	// 特定实体进行射击
	void Fire();

};

class EnemyTank:public Tank		// 敌人坦克，继承自坦克类
{
public:
	static int nEnemy;			// 当前的敌人数量
	static int nEnemyBullet;
	void RandDir();
	// 特定实体进行射击
	void Fire();
};

extern EnemyTank enemys[MAX_ENEMY];		// 敌人数组
extern PlayerTank player;				// 玩家
extern Entity bullets[MAX_BULLETS];		// 玩家子弹数组
extern Entity enemyBullets[MAX_BULLETS];// 敌人子弹数组
extern Entity* allEntities[ALL_ENTITIES];

#endif // _CLASSES_H_
