#ifndef _INIT_H_ // ����Ϊ�˷�ֹͷ�ļ����ظ�������
#define _INIT_H_

#include "GameProc.h"
// ���ض���֡����ʱ����
int GetAnimStep();
// ���ص�ǰ��֡ʱ����
int GetTimeStep();
// ͨ��ini�ļ���ȡ��ǰ�ؿ��Ĳ��ֳ�ʼ����Ϣ
void ReadIni(LEVEL l);
// ��ȡ��ͼ�����ļ�,���Ϊ1�ĵط����ϰ���
void InitMap(LEVEL l, Entity *headBlock);
// ���ݵ�ǰ�����Ĳ�ͬ����Ϸ���ݽ��г�ʼ��
void Init(LEVEL l);
// ��ָ���Ĳ������г�ʼ��
void InitEntity(Entity *headEnemy, Entity *headBlock, Entity *headBullet, Entity *headEnemyBullet, Entity *player, LEVEL l);
// ��ȡ��ǰ�ؿ��ı���ͼƬ���
HBITMAP GetBackPic();
// �����������
int RandomFire();
// ��������ı䷽��
int RandomDir();
// ��ȡ������ɼ���
int GetBirthTime();
// ���ݵ�����������һ������,����ӵ�����������
void EnemyBirth(Entity* headEnemy, int type, int w);
// ��ȡͨ�ط���
int GetPassScore();
// ����logoͼƬ���
HBITMAP GetLogoPic();
// ����ʤ��ͼƬ
HBITMAP GetSuccPic();
// ����ʧ��ͼƬ
HBITMAP GetFailPic();
#endif