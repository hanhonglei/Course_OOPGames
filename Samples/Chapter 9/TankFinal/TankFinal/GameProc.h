#ifndef _GAME_PROC_H_ // ����Ϊ�˷�ֹͷ�ļ����ظ�������
#define _GAME_PROC_H_

#include "Entity.h"

// ��Ϸ�Ĺؿ�
enum LEVEL {OPEN, LEVEL1, LEVEL2_OPEN, LEVEL2, SUCCEED, FAIL};
// ������
void EnterKey(int key);
// �½����������ͷ���
void NewEntity();
// �ı䴰�ڴ�С��֪ͨ����
void ChangeWndSize(int w, int h);
// ת���ؿ�
void ChangeLevel(LEVEL newL);
// ���������Ϣ
void ResetPlayer(Entity *player);
// ��ɶ���Ϸ�Ļ���
void Draw(HDC hdc, HWND hWnd);
// ����������Ϸ����,�����е��ø��ԵĻ��ƺ�����ɻ���
void DrawScene(HDC hdc, LEVEL curL);
// �����Ϸ���ݵĻ���
void DrawGameScene(HDC hdc);
// ���¸�����Ϸ��Ϣ,��ʱ���ᴥ���������
void Update(int ts);
// ����һ������
void EnemyBirth();
// ����̬��������ݽ����ͷ�
void Destroy();
// ������Ϸ�����еĶ���Ԫ��
void NextFrame(int ts);

#endif
