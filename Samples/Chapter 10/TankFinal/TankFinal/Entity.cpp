// TankPlus.cpp : Defines the entry point for the application.
//
/************************************************************************/
/*	������Ϸʵ���������Ĺ��� */
/************************************************************************/
#include "stdafx.h"
#include "Entity.h"

// ���Ʋ���ָ������Ϸʵ��
void Entity::DrawEntity(HDC hdc)
{
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP bmp = a[int(dir)%4];
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, bmp);
	BITMAP bm;
	GetObject(bmp, sizeof(bm), &bm);
	SetStretchBltMode(hdc,STRETCH_HALFTONE);
	// ��Ӷ���Ч�������Ƶ�ǰ֡
	TransparentBlt(hdc,x-s/2, y-s/2, s, s, hdcMem, f*bm.bmWidth/(frames), 0, bm.bmWidth/frames, bm.bmHeight,RGB(255,255,255)); // ��ɫ��Ϊ͸��ɫ��Library Msimg32.lib
	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);
}

// ���ض�ʵ������Ϊ��һ֡
void Entity::NextFrame(int ts)
{
	if(f == frames-1)
		animD = 1;
	else if (f == 0)
		animD = 1;

	f = (f+animD)%frames;
}
// �ж��ض�ʵ���Ƿ�ͱ߽緢����ײ
bool Entity::WallCollide(int w, int h)
{
	bool bC = false;
	switch(dir)
	{
	case UP:
		if ((y - s/2) < 0)	// �ϱ߽�
		{
			bC = true;
			y = s/2;
		}
		break;
	case DOWN:
		if ((y + s/2) > h)	// �±߽�
		{
			bC = true;
			y = h - s/2;
		}
		break;
	case LEFT:
		if((x - s/2) < 0)		// ��߽�
		{
			bC = true;
			x = s/2;
		}
		break;
	case RIGHT:
		if((x + s/2) > w)		// �ұ߽�
		{
			bC = true;
			x = w - s/2;
		}
		break;
	}
	return bC;
}
// ���г�ʼ���Ĺ��캯��
Entity::Entity(int posX, int posY, Dir d, int vel, int size,  int lf, unsigned short fs, bool pause, unsigned short curF, HBITMAP *anim)
	:x(posX),y(posY),dir(d), v(vel),s(size),life(lf),frames(fs),p(pause),f(curF)
{
	if (anim == NULL)
		a[0]=a[1]=a[2]=a[3]=NULL;
	else
		memcpy(a, anim, 4*sizeof(HBITMAP));
}
// ���ض���Ϸʵ������ƶ�
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

int Entity::GetSize()
{
	return s;
}

void Entity::SetX(int posX)
{
	x = posX;
}
void Entity::SetY(int posY)
{
	y = posY;
}
const int Entity::GetX()
{
	return x;
}
const int Entity::GetY()
{
	return y;
}
//////////////////////////////////////////////////////////////////////////
// Tank class

// �ض�ʵ������������
Entity * Tank::Fire(HBITMAP bulletAnim[4], int vel, int fs, int size)
{
	Entity *newBullet = new Entity(x, y,dir, vel,size, 1, fs, false,0,bulletAnim);
	switch(dir)
	{
	case UP:
		newBullet->SetY(newBullet->GetY()-s);
		break;
	case DOWN:
		newBullet->SetY(newBullet->GetY()+s);
		break;
	case LEFT:
		newBullet->SetX(newBullet->GetX()-s);
		break;
	case RIGHT:
		newBullet->SetX(newBullet->GetX()+s);
		break;
	}
	return newBullet;
}

// �ж�����ʵ���Ƿ�����ײ,��������֮�䷢����ײ���ж�
bool Tank::IsCollide(Entity *ent2)
{
	if (x+s/2 <= ent2->GetX()-ent2->GetSize()/2 || x-s/2 >= ent2->GetX() + ent2->GetSize()/2)
		return false;
	if (y+s/2 <= ent2->GetY()-ent2->GetSize()/2 || y-s/2 >= ent2->GetY() + ent2->GetSize()/2)
		return false;
	return true;
}
// ���ø�����ɳ�ʼ��
Tank::Tank(int posX, int posY, Dir d, int vel, int size,  int lf, unsigned short fs, bool pause, unsigned short curF, HBITMAP *anim)
	:Entity(posX,posY,d, vel,size,lf,fs,pause,curF,anim)
{
}
void Tank::SetPos(int posX, int posY, Dir d, bool pause)
{
	x = posX;
	y = posY;
	dir = d;
	p = pause;
}
void Tank::ChangeDir(Dir newD)
{
	dir = newD;
	p = false;
}
int Tank::GetLife()
{
	return life;
}
bool Tank::BeHit()
{
	// ��ǰ̹���Ѿ�����Ч̹�˵�ʱ�򣬷���false
	if (life <= 0)
		return false;
	else
	{
		--life;
		return true;
	}
}
Dir Tank::GetDir()
{
	return dir;
}
void Tank::Pause(bool pause)
{
	p = pause;
}

