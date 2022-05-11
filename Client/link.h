#ifndef __LINK_H__
#define __LINK_H__
#include<string.h>
#include<iostream>
using namespace std;
struct User
{
	
	User* next;
	string uname;
	string upassword;
	User()
	{
		uname = "";
		upassword = "";
		next = NULL;
	}
	User(string& na, string& key,User*ne)
	{
		uname = na;
		upassword = key;
		next = ne;
	}
};
// 单链表类
class LinkList
{
protected:
	//  单链表的数据成员
	User* head;				// 头结点指针
	int length;							// 单链表长度 
public:
	//  单链表的函数成员 
	LinkList();							// 无参数的构造函数
	LinkList(string name[], string key[], int n);		// 有参数的构造函数
	virtual ~LinkList();				// 析构函数
	int GetLength() const;				// 求单链表长度			 
	void Traverse() const;              // 遍历单链表	
	void InsertElem(string& name, string& key);	             // 在表尾插入元素
	void Clear();
};

LinkList::LinkList()
// 操作结果：构造一个空链表
{
	head = new User;		// 构造头结点
	length = 0;						// 初始化单链表长度为0 
}


LinkList::LinkList(string name[],string key[], int n)
// 操作结果：根据数组v中的元素构造单链表
{
	User* p;
	p = head = new User;	// 构造头结点 
	for (int i = 0; i < n; i++) {
		p->next = new User(name[i],key[i], NULL);
		p = p->next;
	}
	length = n;						// 初始化单链表长度为n
}

LinkList::~LinkList()
// 操作结果：销毁单链表
{
	Clear();			// 清空单链表
	delete head;		// 释放头结点所指空间
}


int LinkList::GetLength() const
// 操作结果：返回单链表的长度 
{
	return length;
}



void LinkList::Clear()
// 操作结果：清空单链表,删除单链表中所有元素结点 
{
	User* p = head->next;
	while (p != NULL) {
		head->next = p->next;
		delete p;
		p = head->next;
	}
	length = 0;
}


void LinkList::InsertElem(string& name,string&key)
// 操作结果：在单链表的表尾位置插入元素e
{
	User* p, * q;
	q = new User(name,key, NULL);    // 生成新结点q
	for (p = head; p->next != NULL; p = p->next);	// p指向表尾结点	
	p->next = q;                        // 在单链表的表尾位置插入新结点 
	length++;							// 插入成功后，单链表长度加1 
}

#endif

#pragma once
