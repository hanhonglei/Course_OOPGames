#pragma once
#include "Entity.h"
#include "GameScene.h"

class GameManage
{
public:
	static GameScene *curScene;		// ��ǰ
	static int wndHeight;			// ���ڳߴ�
	static int wndWidth;

	static HINSTANCE hInst;			// current instance,�����ļ��е�ȫ�ֱ���
	static bool reset_time;			// �Ƿ���µ�ǰ�ؿ��ļ�ʱ��

	static HBITMAP logo, succ_pic, fail_pic;	// logoͼƬ�ͳɹ�ʧ��ͼƬ

	static int timeStep;			// ��ʱ��ʱ����
	static int animStep;			// ����֡���¶�ʱ��ʱ����
	static int birthTime;			// ���˵ĳ���ʱ����

	static void EnemyBirth();		// ���ն�ʱ�����ɵ���
	static void NextFrame(int step);// ����ǰ���ж�����������Ϊ��һ֡
	static void EnterKey(int key);	// �����û���������
	static void Destroy();			// ���ٵ�ǰ�ؿ�
	static void ChangeWndSize(int w, int h);// �ı䴰�ڳߴ�
	static void ChangeLevel(LEVEL newL);	// ת���ؿ�
	static void Update(int ts);		// ���¸�����Ϸ��Ϣ,��ʱ���ᴥ���������
	static void Draw(HDC hdc, HWND hWnd);	// ���Ƶ�ǰ�ؿ�

	GameManage(void);
	~GameManage(void);
};

