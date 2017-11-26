#include "stdafx.h"
#include "Classes.h"

int wndWidth = 0;	int wndHeight = 0;  // ���ڳߴ�
bool bInit = false;						// �Ƿ��Ѿ���ɳ�ʼ��


list<EnemyTank*> enemys;	// ��������
PlayerTank player;			// ���
list<Entity*> bullets;		// ����ӵ�����
list<Entity*> enemyBullets; // �����ӵ�����
list<Entity*> allEntities;

Entity::Entity()
{
	b = 1;
	e = 0;
	p = 0;
	isValid = true;
}
// ���ض���Ϸʵ�����ݳ�������ʽ����ƶ�
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

// �ж�����ʵ���Ƿ�����ײ,��������֮�䷢����ײ���ж�
int Entity::IsCollide(const Entity *other)
{

	if (x+s/2 <= other->x-other->s/2 || x-s/2 >= other->x + other->s/2)
		return 0;
	if (y+s/2 <= other->y-other->s/2 || y-s/2 >= other->y + other->s/2)
		return 0;
	return 1;
}
// �ж��ض�ʵ���Ƿ�ͱ߽緢����ײ
int Entity::WallCollide()
{
	int bC = 0;
	switch(dir)
	{
	case UP:
		if ((y - s/2) < 0)			// �ϱ߽�
		{
			bC = 1;
			y = s/2;
		}
		break;
	case DOWN:
		if ((y + s/2) > wndHeight)	// �±߽�
		{
			bC = 1;
			y = wndHeight - s/2;
		}
		break;
	case LEFT:
		if((x - s/2) < 0)				// ��߽�
		{
			bC = 1;
			x = s/2;
		}
		break;
	case RIGHT:
		if((x + s/2) > wndWidth)		// �ұ߽�
		{
			bC = 1;
			x = wndWidth - s/2;
		}
		break;
	}
	if (bC)
	{
		if (e)				// ������˺ͱ߽緢����ײ,����������µ��˶�����
			dir = Dir((dir+1+rand()%3)%4);
		else					// �������Һͱ߽緢����ײ,�����ֹͣ
			p = true;
	}
	return bC;
}
// ���Ʋ���ָ������Ϸʵ��
void Entity::DrawEntity(HDC hdc)
{
	if (!IsValid())
		return;
	HBRUSH brush;
	brush = CreateSolidBrush(c);		// ����ʵ��ָ������ɫ������ˢ
	RECT rc;								// ʵ�峤����
	rc.top = y-s/2;
	rc.left = x-s/2;
	rc.bottom = y+s/2;
	rc.right = x+s/2;
	FillRect(hdc, &rc, brush);				// ����ʵ������

	DeleteObject(brush);			// ��ʹ����ı�ˢɾ��
}
bool Entity::IsValid()
{
	return isValid;
}
void Entity::Set(int sz, COLORREF cl, Dir direction, int px, int py, bool bv)
{
	s = sz;
	c = cl;	// ���˷����ڵ�����ҷ����Ĳ�һ��
	dir = direction;
	v = velb;
	x = px;
	p = 0;
	y = py;
	isValid = bv;

	switch(dir)	// �ڵ�������Ǵ�ʱ�������ĳ���
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
	brush = CreateSolidBrush(c);		// ����ʵ��ָ������ɫ������ˢ
	RECT rc;								// ʵ�峤����
	rc.top = y-s/2;
	rc.left = x-s/2;
	rc.bottom = y+s/2;
	rc.right = x+s/2;
	FillRect(hdc, &rc, brush);				// ����ʵ������
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
	DeleteObject(brush);			// ��ʹ����ı�ˢɾ��
}

void Tank::Init()
{
	s = sz;
	b = 0;
	e = 1;
	dir = Dir(UP+rand()%4);		// ���ѡ��һ��ǰ������
	v = rand()%2==0?velf:vels;	// �������ٶ�֮����50%�ļ������ѡ��һ��
	c = v == velf?RGB(0,122,122):RGB(0,60,30);	// �����ٶ�̹�˵���ɫ��һ��
	x = (rand()%3)*(wndWidth-sz)/2 + sz/2;	// ���������
	y = sz;
	p = 0;
}


// ���������Ϣ,�ָ�����ʼ״̬
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
	nLife = 3;				// �������
	nScore = 0;				// ��ҵ÷�
	bFire = false;
}
void PlayerTank::Interaction(WPARAM wParam)
{
	switch (wParam)	// ��������������Ϣ������ҿ��Ƶ�̹��״̬
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
	case VK_SPACE:	// ���
		bFire = 1;
		break;
	}
}
// �ض�ʵ��������
void PlayerTank::Fire()
{
	if (bFire)
	{
		bFire = false;
		Entity *pBulletes = new Entity;	// �ж��ǵ��˻�����ҷ����ڵ�
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
// �ض�ʵ��������
void EnemyTank::Fire()
{
	if ((rand()%enemyFirePer) == 0)
	{
		Entity *pBulletes = new Entity;	// �ж��ǵ��˻�����ҷ����ڵ�
		pBulletes->Set(szb, RGB(0,0,255), dir, x, y);
		enemyBullets.push_back(pBulletes);
	}
}