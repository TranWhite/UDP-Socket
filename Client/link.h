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
// ��������
class LinkList
{
protected:
	//  ����������ݳ�Ա
	User* head;				// ͷ���ָ��
	int length;							// �������� 
public:
	//  ������ĺ�����Ա 
	LinkList();							// �޲����Ĺ��캯��
	LinkList(string name[], string key[], int n);		// �в����Ĺ��캯��
	virtual ~LinkList();				// ��������
	int GetLength() const;				// ��������			 
	void Traverse() const;              // ����������	
	void InsertElem(string& name, string& key);	             // �ڱ�β����Ԫ��
	void Clear();
};

LinkList::LinkList()
// �������������һ��������
{
	head = new User;		// ����ͷ���
	length = 0;						// ��ʼ����������Ϊ0 
}


LinkList::LinkList(string name[],string key[], int n)
// �����������������v�е�Ԫ�ع��쵥����
{
	User* p;
	p = head = new User;	// ����ͷ��� 
	for (int i = 0; i < n; i++) {
		p->next = new User(name[i],key[i], NULL);
		p = p->next;
	}
	length = n;						// ��ʼ����������Ϊn
}

LinkList::~LinkList()
// ������������ٵ�����
{
	Clear();			// ��յ�����
	delete head;		// �ͷ�ͷ�����ָ�ռ�
}


int LinkList::GetLength() const
// ������������ص�����ĳ��� 
{
	return length;
}



void LinkList::Clear()
// �����������յ�����,ɾ��������������Ԫ�ؽ�� 
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
// ����������ڵ�����ı�βλ�ò���Ԫ��e
{
	User* p, * q;
	q = new User(name,key, NULL);    // �����½��q
	for (p = head; p->next != NULL; p = p->next);	// pָ���β���	
	p->next = q;                        // �ڵ�����ı�βλ�ò����½�� 
	length++;							// ����ɹ��󣬵������ȼ�1 
}

#endif

#pragma once
