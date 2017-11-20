#include <iostream>
#include <cstdlib>
using namespace std;
#include<iostream>
using namespace std;

// 用户自定义数据类型
class RealData
{
	private: 
	int score;
	int id;
	char s;
public:
	friend ostream& operator << (ostream& lhs, const RealData& rhs);
	bool operator == (const RealData& rhs);
	RealData(int sc, int i, char ss):score(sc),id(i),s(ss){};
};
ostream& operator << (ostream& lhs, const RealData& rhs)
{
	lhs << "("<<rhs.id << ","<<rhs.s << ","<<rhs.score<<")";
	return lhs;
}
bool RealData::operator == (const RealData& rhs)
{
	return(rhs.id == id && rhs.s == s && rhs.score == score);
}

// 结点类，使用模板，可以支持任意类型结点
template<class T>
class Node{
private:
	T data;
	Node<T>* next;
public:
	Node(const T& d):data(d){next = NULL;};
	void InsertAfter(Node<T> *p);
	Node<T>* DeleteAfter();
	Node<T>* NextNode();
	T& GetData();
};
template<class T>
T& Node<T>::GetData()
{
	return data;
}
template<class T>
void Node<T>::InsertAfter(Node<T> *p)
{
		p->next = this->next;
		this->next = p;
}
template<class T>
Node<T>* Node<T>::DeleteAfter()
{
	if (NULL == this->next)
		return NULL;
	Node<T>* p = this->next;
	this->next = p->next;
	return p;
}
template<class T>
Node<T>* Node<T>::NextNode()
{
	return this->next;
}

// 链表类，使用模板
template<class T>
class LinkedList
{
private:
	Node<T>* head;
public:
	LinkedList(Node<T> *h);
	Node<T>* FindNode(T data);
	void InsertAfter(T data, Node<T>* p);
	Node<T>* DeleteNode(T data);
	void PrintList();
	void FreeList();
};
template<class T>
LinkedList<T>::LinkedList(Node<T> *h)
{
	head = h;
}
template<class T>
Node<T>* LinkedList<T>::FindNode(T data)
{
	Node<T> *p = head;
	while(p!=NULL)
	{
		if (p->GetData() == data)
			return p;
		p = p->NextNode();
	}
	return NULL;
}
template<class T>
void LinkedList<T>::InsertAfter(T data, Node<T>* p)
{
	Node<T> *c = FindNode(data);
	if (NULL == c)
		return;
	c->InsertAfter(p);
}
template<class T>
Node<T>* LinkedList<T>::DeleteNode(T data)
{
	Node<T>* p = head;
	if (head->GetData() == data)
	{
		head = head->NextNode();
		return p;
	}
	else
	{
		while(p->NextNode() != NULL)
		{
			if(p->NextNode()->GetData() == data)
				return p->DeleteAfter();
		}
	}
	return NULL;
}
template<class T>
void LinkedList<T>::PrintList()
{
	Node<T>* p = head;
	while(p!=NULL)
	{
		cout << "--"<<p->GetData();
		p = p->NextNode();
	}
	cout << endl;
}
template<class T>
void LinkedList<T>::FreeList()
{
	Node<T>* p = head;
	while(p!=NULL)
	{
		Node<T>* c = p;
		p = p->NextNode();
		delete c;
	}
}

int main()	
{
	// 实例化两种类型的链表，分别保存整型和用户自定义类型结点
	Node<int> *n1 = new Node<int>(3);
	LinkedList<int> *l = new LinkedList<int>(n1);
	RealData r(98, 1,'M');
	Node<RealData> *n2 = new Node<RealData>(r);
	LinkedList<RealData>* l2 = new LinkedList<RealData>(n2);

	RealData r2(60, 2,'F');
	Node<RealData> *n3 = new Node<RealData>(r2);

	l->PrintList();
	l2->PrintList();
	// 测试插入结点功能
	l->InsertAfter(3, new Node<int>(5));
	l->InsertAfter(3, new Node<int>(4));
	l2->InsertAfter(r, n3);

	l->PrintList();
	l2->PrintList();
	// 测试删除结点功能
	delete l2->DeleteNode(r);

	l2->PrintList();

	l->FreeList();
	l2->FreeList();
	delete l, l2;
	char cc;
	cin >> cc;
}