#ifndef _ENTITY_H_ // ����Ϊ�˷�ֹͷ�ļ����ظ�������
#define _ENTITY_H_

enum Dir{UP, DOWN, LEFT, RIGHT, NONE};	// �˶�������ܵ��˶�����
class Entity		// ��Ϸ�е�ʵ��ṹ��
{
protected:
	int x, y;				// λ��
	Dir dir;				// ����
	int v;					// ����
	int s;					// �߳���������
	bool p;					// �Ƿ�ֹͣ,ֻ����Ҳſ���ֹͣ
	HBITMAP a[4];			// λͼ,4������
	int life;				// ���浱ǰ����ֵ������ֵΪ0ʱ����Ҫɾ��
	unsigned short f;		// ��ǰ֡
	unsigned short frames;	// ��֡��
	int animD;				// ���Ƶ�ǰ����֡����һ֡����
public:
	// ���캯�����Գ�Ա�������г�ʼ��
	Entity(int posX = 0, int posY = 0, Dir d = NONE, int vel = 0, int size = 1,  int lf = 1,unsigned short fs = 1, bool pause = true, unsigned short curF = 0, HBITMAP *anim = NULL);
	// ���Ʋ���ָ������Ϸʵ��
	void DrawEntity(HDC hdc);
	// ���ض���Ϸʵ������ƶ�
	void Move(int ts);
	// �ж��ض�ʵ���Ƿ�ͱ߽緢����ײ
	bool WallCollide(int w, int h);
	// ���ض�ʵ������Ϊ��һ֡
	void NextFrame(int ts);
	// ��ȡ�ض��ĳ�Ա����
	const int GetX();
	const int GetY();
	int GetSize();
	void SetX(int posX);
	void SetY(int posY);
};
// ̹����̳�����ͨ��Entity��Ϸʵ����
class Tank:public Entity
{
public:
	// ����̹�˵�λ�������Ϣ
	void SetPos(int posX, int posY, Dir d = UP, bool pause = true);
	// �ı�̹���н�����
	void ChangeDir(Dir newD);
	// ��õ�ǰ̹�˵�����ֵ
	int GetLife();
	// ̹���ܵ�����������ֵΪfalse��ʱ��֤����ǰ̹�˵�����ֵ�Ѿ���0
	bool BeHit();
	// ��õ�ǰ̹�˵ĳ���
	Dir GetDir();
	// ���õ�ǰ̹���Ƿ��˶�
	void Pause(bool pause);
	// �ض�ʵ������������
	Entity* Fire(HBITMAP bulletAnim[4], int vel, int fs, int size);
	// �ж�����ʵ���Ƿ�����ײ,��������֮�䷢����ײ���ж�
	bool IsCollide(Entity *ent2);
	// ���ø��෢��̹����Ĺ���ͳ�Ա��ʼ��
	Tank(int posX = 0, int posY = 0, Dir d = NONE, int vel = 0, int size = 1,  int lf = 1,unsigned short fs = 1, bool pause = true, unsigned short curF = 0, HBITMAP *anim = NULL);

};
#endif
