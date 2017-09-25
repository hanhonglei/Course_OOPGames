#include <iostream>
using namespace std;

class CTime 
{
public:
	CTime(const CTime& another)
	{
		// 1 You cannot call GetTime function in object another, unless GetTime is const
		// Since GetTime is not const, but object another is
		//int *t = another.GetTime();
		//m_hour = t[0];
		//m_min = t[1];
		//m_sec = t[2];
		// delete[] t;

		// 2 But you can access object another's private variables
		// Since protection in C++ is class level, not object level
		// So a method being invoked on one object can access private members of any other instance of the same class.
		m_hour = another.m_hour;
		m_min = another.m_min;
		m_sec = another.m_sec;
	}
	int* GetTime()
	{
		int *t = new int[3];
		t[0] = m_hour;
		t[1] = m_min;
		t[2] = m_sec;
		return t;
	}
	CTime(int hour, int min, int sec)
	{
		m_hour = hour;
		m_min = min;
		m_sec = sec;
	}
	CTime()
	{
		m_hour = 0;
		m_min = 0;
		m_sec = 0;
	}
	void SetTime(int hour, int min, int sec)
	{
		m_hour = hour;
		m_min = min;
		m_sec = sec;
	}

	void ShowTime()	{
		cout << m_hour << ":" << m_min << ":" << m_sec << endl;
	}
private:
	int m_hour;
	int m_min;
	int m_sec;
};


int main()
{
	CTime t;
	t.ShowTime();
	t.SetTime(9, 15, 20);
	t.ShowTime();

	CTime t1(t);
	t1.ShowTime();
	t1.SetTime(12, 0, 0);
	t1.ShowTime();


	char c;
	cin>>c;
	return 0;
}