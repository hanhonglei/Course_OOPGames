#include <iostream>
using namespace std;
char GetGlobal(); 
char Next(); 
char Last(); 
char Get(); 
char c = 'G';				// ����ȫ�ֱ���c
int main() 
{ 
	char c = 'L';			// ����ֲ�����c

	cout << "Local c in main function is: \n" << c << endl;	// �ڵ�ǰ������ʹ�õı���c�Ǿֲ�����

	cout << "Global c is: \n" << GetGlobal() << endl;		// ��ȡȫ�ֱ���

	cout << "Static c in file2 is: \n" << Get() << endl;	// ��ȡ�ļ�2�еľ�̬����c
	// �����еľ�̬�����ᱣ��
	cout << "Previous values of static c in file2 are:\n" << Last() << Last() << Last() << endl;	
	cout << "Next values of static c in file2 are: \n" << Next() << Next() << Next() << endl;

	cin >> c;	// ������ͣ���ȴ��û����룬�Ա����û�����ǰ���������
	return 0;
} 