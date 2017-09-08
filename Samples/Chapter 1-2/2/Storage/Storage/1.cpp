#include <iostream>
using namespace std;
char GetGlobal(); 
char Next(); 
char Last(); 
char Get(); 
char c = 'G';				// 定义全局变量c
int main() 
{ 
	char c = 'L';			// 定义局部变量c

	cout << "Local c in main function is: \n" << c << endl;	// 在当前函数中使用的变量c是局部变量

	cout << "Global c is: \n" << GetGlobal() << endl;		// 获取全局变量

	cout << "Static c in file2 is: \n" << Get() << endl;	// 获取文件2中的静态变量c
	// 函数中的静态变量会保留
	cout << "Previous values of static c in file2 are:\n" << Last() << Last() << Last() << endl;	
	cout << "Next values of static c in file2 are: \n" << Next() << Next() << Next() << endl;

	cin >> c;	// 程序暂停，等待用户输入，以便让用户看到前面的输出结果
	return 0;
} 