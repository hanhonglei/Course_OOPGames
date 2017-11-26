#include "stdafx.h"
#include "Classes.h"

int wndWidth = 0;	int wndHeight = 0;  // 窗口尺寸
bool bInit = false;						// 是否已经完成初始化


list<EnemyTank*> enemys;	// 敌人数组
PlayerTank player;			// 玩家
list<Entity*> bullets;		// 玩家子弹数组
list<Entity*> enemyBullets; // 敌人子弹数组
list<Entity*> allEntities;

Entity::Entity()
{
	b = 1;
	e = 0;
	p = 0;
	isValid = true;
}
// 对特定游戏实体依据朝向和速率进行移动
void Entity::Move(int ts)
{
	if (p)
		return;
	switch(dir)
	{
	case UP:
		y -= v * ts;
		break;
	case DOWN:
		y += v * ts;
		break;
	case LEFT:
		x -= v * ts;
		break;
	case RIGHT:
		x += v * ts;
		break;
	}
}
void Entity::SetValid(bool bv)
{
	isValid = bv;
}

// 判断两个实体是否发生碰撞,以正方形之间发生碰撞来判断
int Entity::IsCollide(const Entity *other)
{

	if (x+s/2 <= other->x-other->s/2 || x-s/2 >= other->x + other->s/2)
		return 0;
	if (y+s/2 <= other->y-other->s/2 || y-s/2 >= other->y + other->s/2)
		return 0;
	return 1;
}
// 判断特定实体是否和边界发生碰撞
int Entity::WallCollide()
{
	int bC = 0;
	switch(dir)
	{
	case UP:
		if ((y - s/2) < 0)			// 上边界
		{
			bC = 1;
			y = s/2;
		}
		break;
	case DOWN:
		if ((y + s/2) > wndHeight)	// 下边界
		{
			bC = 1;
			y = wndHeight - s/2;
		}
		break;
	case LEFT:
		if((x - s/2) < 0)				// 左边界
		{
			bC = 1;
			x = s/2;
		}
		break;
	case RIGHT:
		if((x + s/2) > wndWidth)		// 右边界
		{
			bC = 1;
			x = wndWidth - s/2;
		}
		break;
	}
	if (bC)
	{
		if (e)				// 如果敌人和边界发生碰撞,则随机生成新的运动方向
			dir = Dir((dir+1+rand()%3)%4);
		else					// 如果是玩家和边界发生碰撞,则玩家停止
			p = true;
	}
	return bC;
}
// 绘制参数指定的游戏实体
void Entity::DrawEntity(HDC hdc)
{
	if (!IsValid())
		return;
	HBRUSH brush;
	brush = CreateSolidBrush(c);		// 按照实体指定的颜色创建笔刷
	RECT rc;								// 实体长方形
	rc.top = y-s/2;
	rc.left = x-s/2;
	rc.bottom = y+s/2;
	rc.right = x+s/2;
	FillRect(hdc, &rc, brush);				// 绘制实体主体

	DeleteObject(brush);			// 将使用完的笔刷删除
}
bool Entity::IsValid()
{
	return isValid;
}
void Entity::Set(int sz, COLORREF cl, Dir direction, int px, int py, bool bv)
{
	s = sz;
	c = cl;	// 敌人发出炮弹和玩家发出的不一样
	dir = direction;
	v = velb;
	x = px;
	p = 0;
	y = py;
	isValid = bv;

	switch(dir)	// 炮弹方向就是此时射击物体的朝向
	{
	case UP:
		y -= s;
		break;
	case DOWN:
		y += s;
		break;
	case LEFT:
		x -= s;
		break;
	case RIGHT:
		x += s;
		break;
	}
}

int Entity::number = 0;


void Tank::DrawEntity(HDC hdc)
{
	if (!IsValid())
		return;
	HBRUSH brush;
	brush = CreateSolidBrush(c);		// 按照实体指定的颜色创建笔刷
	RECT rc;								// 实体长方形
	rc.top = y-s/2;
	rc.left = x-s/2;
	rc.bottom = y+s/2;
	rc.right = x+s/2;
	FillRect(hdc, &rc, brush);				// 绘制实体主体
	switch(dir)
	{
	case UP:
		rc.bottom = rc.top;
		rc.top = rc.bottom - s/2;
		rc.left = rc.left + s/4;
		rc.right = rc.right - s/4;
		break;
	case DOWN:
		rc.top = rc.bottom;
		rc.bottom = rc.bottom + s/2;
		rc.left = rc.left + s/4;
		rc.right = rc.right - s/4;
		break;
	case LEFT:
		rc.right = rc.left;
		rc.left = rc.left - s/2;
		rc.bottom = rc.bottom - s/4;
		rc.top = rc.top + s/4;
		break;
	case RIGHT:
		rc.left = rc.right;
		rc.right = rc.right + s/2;
		rc.bottom = rc.bottom - s/4;
		rc.top = rc.top + s/4;
		break;
	}
	FillRect(hdc, &rc, brush);
	DeleteObject(brush);			// 将使用完的笔刷删除
}

void Tank::Init()
{
	s = sz;
	b = 0;
	e = 1;
	dir = Dir(UP+rand()%4);		// 随机选择一个前进方向
	v = rand()%2==0?velf:vels;	// 在两种速度之间以50%的几率随机选择一种
	c = v == velf?RGB(0,122,122):RGB(0,60,30);	// 两种速度坦克的颜色不一样
	x = (rand()%3)*(wndWidth-sz)/2 + sz/2;	// 随机出生点
	y = sz;
	p = 0;
}


// 重置玩家信息,恢复到初始状态
void PlayerTank::ResetPlayer()
{
	s = sz;
	b = 0;
	c = RGB(122,30,0);
	dir = UP;
	v = vels;
	x = wndWidth/2;
	y = wndHeight-sz;
	p = 1;
	e = 0;
}
PlayerTank::PlayerTank()
{
	nLife = 3;				// 玩家生命
	nScore = 0;				// 玩家得分
	bFire = false;
}
void PlayerTank::Interaction(WPARAM wParam)
{
	switch (wParam)	// 依据玩家输入的信息调整玩家控制的坦克状态
	{
	case VK_LEFT:
		dir = LEFT;
		p = 0;
		break;
	case VK_RIGHT:
		dir = RIGHT;
		p = 0;
		break;
	case VK_UP:
		dir = UP;
		p = 0;
		break;
	case VK_DOWN:
		dir = DOWN;
		p = 0;
		break;
	case VK_SPACE:	// 射击
		bFire = 1;
		break;
	}
}
// 特定实体进行射击
void PlayerTank::Fire()
{
	if (bFire)
	{
		bFire = false;
		Entity *pBulletes = new Entity;	// 判断是敌人还是玩家发射炮弹
		pBulletes->Set(szb, RGB(255,0,0), dir, x, y);
		bullets.push_back(pBulletes);
		PlayerTank::nBullet++;
	}
}


int PlayerTank::nBullet = 0;

void EnemyTank::RandDir()
{
	if (rand()%enemyDir == 0)
		dir = Dir((dir+1+rand()%3)%4);
}
// 特定实体进行射击
void EnemyTank::Fire()
{
	if ((rand()%enemyFirePer) == 0)
	{
		Entity *pBulletes = new Entity;	// 判断是敌人还是玩家发射炮弹
		pBulletes->Set(szb, RGB(0,0,255), dir, x, y);
		enemyBullets.push_back(pBulletes);
	}
}