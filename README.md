# Course_OOPGames
Object-Oriented Game Programming

## References
1. C++语言程序设计
- 作者： 郑莉   
- 出版社：清华大学出版社
- http://product.china-pub.com/4716843#ml
- 课件主要参考自郑莉老师
2. 其他参考资源
- 请访问[韩红雷个人网站的资源部分](https://hanhonglei.github.io/links/)

## 课程介绍
1. 绪论（1）
2. C++简单程序设计（3+4）
2. 函数（2+2）
2. 类与对象（4+4）
2. 数据的共享与保护（2+2）
2. 数组、指针与字符串（2+2）
2. 继承与派生（4+4）
2. 多态性（2+2）
2. 群体类和群体数据的组织（4+4）
2. 泛型程序设计与C++标准模板（2+2）
2. 流类库和输入输出（2+2）
2. 异常处理（2+2）
2. 开发一款游戏（2+2）

## 练习程序
请查看`Samples`文件夹

## Some details(sample can be found at (this project)[https://github.com/hanhonglei/Course_OOPGames/tree/master/Samples/Class])
1. The protection in `C++` is class level, not object level. So a method being invoked on one object can access private members of any other instance of the same class. More information please check [here](https://stackoverflow.com/questions/7396846/with-a-private-modifier-why-can-the-member-in-other-objects-be-accessed-directl)
```
class A 
{
private:
    int x;
public:
    A()
    {
        x = 90;
    }
	// a1 and a2 can access private data member x
    A(A a1, A a2)
    {
        a1.x = 10;
        a2.x = 20;
    }
    int getX()
    {
        return this->x;
    }
};
```

2. Const object can only call const functions, even the object is some function's parameter. like the example below, the problem is `rhs` is declared as const, but `getData()` isn't, so it could be modifying rhs when you call it even though rhs is supposedly const. As `getData()` is an accessor, it should be const too.

```
class A {
public:
    //Copy Constructor
    A(const A &rhs);

    //accessor for data
    int getData();
	
	//mutator for data
    void setData(int d);
private:
    int data;
};
//Copy Constructor
A::A(const A &rhs) {
    this->setData(rhs.getData()); 	//error: passing 'const A' as 'this' argument of 'int A::getData()' discards qualifiers
}
//accessor for data
int A::getData() {
    return data;
}

//mutator for data
void A::setData(int d) {
    data = d;
}
```

----

- This project is under [GNU GENERAL PUBLIC LICENSE](https://www.gnu.org/licenses/), please check it in the root folder.