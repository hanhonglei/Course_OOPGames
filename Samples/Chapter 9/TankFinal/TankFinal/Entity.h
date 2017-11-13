#ifndef _ENTITY_H_ // ����Ϊ�˷�ֹͷ�ļ����ظ�������
#define _ENTITY_H_

enum Dir{UP, DOWN, LEFT, RIGHT, NONE};	// �˶�������ܵ��˶�����
typedef struct	entity		// ��Ϸ�е�ʵ��ṹ��
{
	int x, y;				// λ��
	Dir dir;				// ����
	int v;					// ����
	int s;					// �߳���������
	int p;					// �Ƿ�ֹͣ,ֻ����Ҳſ���ֹͣ
	HBITMAP a[4];			// λͼ,4������
	int life;				// ���浱ǰ����ֵ������ֵΪ0ʱ����Ҫɾ��
	struct	entity *next;	// �����������ָ�������е���һ������
	unsigned short f;		// ��ǰ֡
	unsigned short frames;	// ��֡��
	int animD;				// ���Ƶ�ǰ����֡����һ֡����
}Entity;

// ���������е�ÿ��ʵ��
void DrawEntities(HDC hdc, const Entity *head);
// ���Ʋ���ָ������Ϸʵ��
void DrawEntity(HDC hdc, const Entity *ent);
// �������е�ÿ��ʵ������ƶ�
void MoveEntities(Entity *head, int ts);
// ���ض���Ϸʵ������ƶ�
void Move(Entity *ent, int ts);
// �ض�ʵ������������
void Fire(const Entity* ent, Entity *pHeadB);
// ���ض���ʵ���������ɾ��
void Destroy(Entity *pHead, Entity* ent);
// ɾ��ͷ��������������
void Destroy(Entity *pHead);
// �ж�����ʵ���Ƿ�����ײ,��������֮�䷢����ײ���ж�
int IsCollide(const Entity *ent1, const Entity *ent2);
// �ж��ض�ʵ���Ƿ�ͱ߽緢����ײ
int WallCollide(Entity *ent, int w, int h);
// �������е�ʵ�嵱ǰ֡����Ϊ��һ֡
void NextFrameEntites(Entity *head, int ts);
// ���ض�ʵ������Ϊ��һ֡
void NextFrame(Entity *ent, int ts);

#endif
