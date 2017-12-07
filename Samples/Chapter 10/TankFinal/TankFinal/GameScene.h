#pragma once
//#include "GameManage.h"
#include "Entity.h"
#include <list>
using namespace std;
// ��Ϸ�Ĺؿ�
enum LEVEL {OPEN, LEVEL1, LEVEL2_OPEN, LEVEL2, SUCCEED, FAIL, NOT_READY};

class GameScene
{
private:
	WCHAR *fileIni;	// ���ڱ��浱ǰ��Ҫ�����Ini�ļ���
	HBITMAP tank1[4],tank2[4],tank3[4],bulletPic[4],block,background;	// ���������ͼƬ

	int nScore;				// ��ҵ÷�
	bool bFire;				// �Ƿ��ڿ���״̬
	LEVEL curL;
	int nLife;				// �������
	int enemyFirePer;		// ���˷����ӵ����������
	int enemyDirPer;		// ��������ı䷽��ı���
	int nPassScore;			// ���ط���


	list<Entity *> pHeadBlocks;	// �ϰ�������
	list<Tank *> pHeadEnemys;	// ��������
	list<Entity *> pHeadBullets;// ����ӵ�����
	list<Entity *> pHeadEnemyBullets;// �����ӵ�����
	Tank * pPlayer;			// ���
private:
	void ReadIni();			// ͨ��ini�ļ���ȡ��ǰ�ؿ��Ĳ��ֳ�ʼ����Ϣ
	void InitMap();			// ��ȡ��ͼ�����ļ�,���Ϊ1�ĵط����ϰ���
	int RandomFire();		// �����������
	int RandomDir();		// ��������ı䷽��
	void DrawScene(HDC hdc);// ��ִ��ǰ�ؿ�
	void DrawGameScene(HDC hdc);// ������Ϸ�ؿ�
	void DrawEntities(HDC hdc);	// ���Ƴ����е����н�ɫ
	void MoveEntities(int ts);// �������е�ÿ��ʵ������ƶ�
public:
	GameScene(LEVEL l);		// ���캯�������ɵ�ǰ��Ϸ�ؿ�����
	~GameScene(void);		// ������������Ҫɾ����̬����Ķ���
	void Init();			// ���ݵ�ǰ�����Ĳ�ͬ����Ϸ���ݽ��г�ʼ��
	HBITMAP GetBackPic();	// ��ȡ��ǰ�ؿ��ı���ͼƬ���
	int GetBirthTime();		// ��ȡ������ɼ���
	void EnemyBirth();		// ���ݵ�����������һ������,����ӵ�����������
	int GetPassScore();		// ��ȡͨ�ط���
	void EnterKey(int key);	// �����������
	void ResetPlayer();		// ��ҽ�ɫ����
	void Draw(HDC hdc, HWND hWnd);	// ���Ƶ�ǰ��Ϸ�ؿ�
	void NextFrame(int ts);	// �����������л�����һ֡
	void Update(int ts);	// ���µ�ǰ֡

};

