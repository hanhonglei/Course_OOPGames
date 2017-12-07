#ifndef _ENTITY_H_ // 这是为了防止头文件被重复包含。
#define _ENTITY_H_

enum Dir{UP, DOWN, LEFT, RIGHT, NONE};	// 运动物体可能的运动方向
class Entity		// 游戏中的实体结构体
{
protected:
	int x, y;				// 位置
	Dir dir;				// 方向
	int v;					// 速率
	int s;					// 边长，正方形
	bool p;					// 是否停止,只有玩家才可能停止
	HBITMAP a[4];			// 位图,4个方向
	int life;				// 保存当前生命值，生命值为0时，需要删除
	unsigned short f;		// 当前帧
	unsigned short frames;	// 总帧数
	int animD;				// 控制当前动画帧的下一帧方向
public:
	// 构造函数，对成员变量进行初始化
	Entity(int posX = 0, int posY = 0, Dir d = NONE, int vel = 0, int size = 1,  int lf = 1,unsigned short fs = 1, bool pause = true, unsigned short curF = 0, HBITMAP *anim = NULL);
	// 绘制参数指定的游戏实体
	void DrawEntity(HDC hdc);
	// 对特定游戏实体进行移动
	void Move(int ts);
	// 判断特定实体是否和边界发生碰撞
	bool WallCollide(int w, int h);
	// 将特定实体设置为下一帧
	void NextFrame(int ts);
	// 获取特定的成员变量
	const int GetX();
	const int GetY();
	int GetSize();
	void SetX(int posX);
	void SetY(int posY);
};
// 坦克类继承自普通的Entity游戏实体类
class Tank:public Entity
{
public:
	// 设置坦克的位置相关信息
	void SetPos(int posX, int posY, Dir d = UP, bool pause = true);
	// 改变坦克行进方向
	void ChangeDir(Dir newD);
	// 获得当前坦克的生命值
	int GetLife();
	// 坦克受到攻击，返回值为false的时候，证明当前坦克的生命值已经是0
	bool BeHit();
	// 获得当前坦克的朝向
	Dir GetDir();
	// 设置当前坦克是否运动
	void Pause(bool pause);
	// 特定实体进行射击操作
	Entity* Fire(HBITMAP bulletAnim[4], int vel, int fs, int size);
	// 判断两个实体是否发生碰撞,以正方体之间发生碰撞来判断
	bool IsCollide(Entity *ent2);
	// 利用父类发成坦克类的构造和成员初始化
	Tank(int posX = 0, int posY = 0, Dir d = NONE, int vel = 0, int size = 1,  int lf = 1,unsigned short fs = 1, bool pause = true, unsigned short curF = 0, HBITMAP *anim = NULL);

};
#endif
