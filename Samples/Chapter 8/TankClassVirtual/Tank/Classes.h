#ifndef _CLASSES_H_
#define  _CLASSES_H_

#define MAX_ENEMY 16			// ��������Ŀ
#define MAX_BULLETS 32			// ����ӵ���Ŀ
const int timeStep = 20;		// ��ʱ��ʱ����
const int sz = 50;				// ̹�˳ߴ�
const int velf = 4;				// ����̹������
const int vels = 2;				// ����̹������
const int szb = 20;				// �ӵ��ߴ�
const int velb = 6;				// �ӵ�����
const int enemyFirePer = 300;	// ���˷����ӵ����������
const int enemyDir = 200;		// ���˸ı䷽����������
extern int wndWidth;	extern int wndHeight;  // ���ڳߴ�
extern bool bInit;				// �Ƿ��Ѿ���ɳ�ʼ��
#define ALL_ENTITIES MAX_ENEMY+MAX_BULLETS+MAX_BULLETS+1

enum Dir{UP, DOWN, LEFT, RIGHT};// ö�����ͣ��˶�������ܵ��˶�����

class Entity	// ���࣬������ʾ��Ϸ������������壬�����ӵ���̹��
{
public:
	static int number;
protected:
	int x, y;				// λ��
	Dir dir;				// ����
	int v;					// ����
	int s;					// �߳���������
	int b;					// �Ƿ����ӵ�
	int p;					// �Ƿ�ֹͣ,ֻ����Ҳſ���ֹͣ
	int e;					// �Ƿ��ǵ���
	COLORREF c;				// ��ɫ
	bool isValid;			//
public:
	// Ĭ�Ϲ��캯��
	Entity();
	// ���ض���Ϸʵ�����ݳ�������ʽ����ƶ�
	void Move(int ts);
	// �ж�����ʵ���Ƿ�����ײ,��������֮�䷢����ײ���ж�
	int IsCollide(const Entity *other);
	// �ж��ض�ʵ���Ƿ�ͱ߽緢����ײ
	int WallCollide();
	// ���Ʋ���ָ������Ϸʵ��
	virtual void DrawEntity(HDC hdc);
	// Ϊ��ʵ���趨�ض�����
	void Set(int sz, COLORREF cl, Dir direction, int px, int py, bool bv = true);
	void SetValid(bool bv);
	bool IsValid();
};

class Tank:public Entity		// ̹�˻��࣬�̳�����Ϸʵ����
{
public:
	// ����̹�ˣ��������Ͳ�ͬ�����Ʋ�ͬ��ɫ
	void DrawEntity(HDC hdc);
	// ��̹�˽��г�ʼ��
	void Init();
};

class PlayerTank:public Tank	// ���̹�ˣ��̳���̹����
{
public:
	bool bFire;					// ����Ƿ������״̬
	int nLife;					// �������
	int nScore;					// ��ҵ÷�
	static int nBullet;			// ��Ҵ�����ӵ�����
	// ���������Ϣ,�ָ�����ʼ״̬
	void ResetPlayer();
	// Ĭ�Ϲ��캯��
	PlayerTank();
	// �����û�����
	void Interaction(WPARAM wParam);
	// �ض�ʵ��������
	void Fire();

};

class EnemyTank:public Tank		// ����̹�ˣ��̳���̹����
{
public:
	static int nEnemy;			// ��ǰ�ĵ�������
	static int nEnemyBullet;
	void RandDir();
	// �ض�ʵ��������
	void Fire();
};

extern EnemyTank enemys[MAX_ENEMY];		// ��������
extern PlayerTank player;				// ���
extern Entity bullets[MAX_BULLETS];		// ����ӵ�����
extern Entity enemyBullets[MAX_BULLETS];// �����ӵ�����
extern Entity* allEntities[ALL_ENTITIES];

#endif // _CLASSES_H_
