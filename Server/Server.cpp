#include <stdio.h>
#include <cstdio>
#include<sstream>
#include<iostream>
#include<string.h>
#include<WinSock2.h>
#include<string>
#include <iomanip>
#include"link.h"
using namespace std;

#pragma comment(lib,"ws2_32.lib")

const int PORT = 8000;
#define MaxBufSize 1024
#define _CRT_SECURE_NO_WARINGS
#define err(errMsg) cout << "[line" << __LINE__ << "]" << errMsg << WSAGetLastError() << endl;        //��λ����λ��
int COUNTS=3;

void countschange(int& n)
{
	COUNTS = n;
}

User* users = new User[9999];

void adduser(string &name, string& key)
{
	users[COUNTS].uname = name;
	users[COUNTS].upassword = key;
}


DWORD WINAPI ServerThread(LPVOID lpParameter) {  //DWORD���������ͣ��������Ƿ���ֵ��WINAPI�Ǻ����ĵ�����ʽ

	int counts = COUNTS;
	char name[100];
	char pass[100];
	char info[200];
	char decision[10];
	int port = *(int*)lpParameter;
	port--;
	SOCKET sockClient = socket(AF_INET, SOCK_DGRAM, 0);
	//������ַ�ṹ��.
	SOCKADDR_IN addrClient;
	addrClient.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(port);
	//���׽��ֺ͵�ַ.
	bind(sockClient, (SOCKADDR*)&addrClient, sizeof(SOCKADDR));
	char recvBuf[100];
	char sendBuf[100];
	memset(recvBuf, 0, sizeof(recvBuf) / sizeof(char));
	memset(sendBuf, 0, sizeof(sendBuf) / sizeof(char));
	int len = sizeof(SOCKADDR);
	
	//��֤�û����
	if (recvfrom(sockClient, decision, sizeof(decision), 0, (SOCKADDR*)&addrClient, &len) == SOCKET_ERROR)
		err("receive error!");
	//cout <<"ѡ��"<< decision<<endl;
	if (strcmp(decision, "1") == 0)
	{

		if (recvfrom(sockClient, name, sizeof(name), 0, (SOCKADDR*)&addrClient, &len) == SOCKET_ERROR)
			err("receive error!");
		cout << name << "�����¼..." << endl;
		//�����е�3���û���֤
		int u;
		for (u = 0; u < 3; u++)
		{
			if (name == users[u].uname)
				break;
		}
		if (u >= 3)
		{
			sprintf(info, "No such user!");
			if (sendto(sockClient, info, sizeof(info), 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR)) == SOCKET_ERROR)
				err("send error!");
		}
		else
		{
			sprintf(info, "�������������~");
			if (sendto(sockClient, info, sizeof(info), 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR)) == SOCKET_ERROR)
				err("send error!");
			if (recvfrom(sockClient, pass, sizeof(pass), 0, (sockaddr*)&addrClient, &len) == SOCKET_ERROR)
				err("recieve error");
			if (pass == users[u].upassword)
			{
				cout << "��ӭ��¼��" << endl;
				sprintf(info, "��ӭ��¼��");
				if (sendto(sockClient, info, sizeof(info), 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR)) == SOCKET_ERROR)
					err("send error!");
			}
			else
			{
				cout << "�������" << endl;
				sprintf(info, "�������");
				if (sendto(sockClient, info, sizeof(info), 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR)) == SOCKET_ERROR)
					err("send error!");
				cout << "The connection with " << port << " ends!" << endl;
				closesocket(sockClient);
				return 0;
			}
		}
	}
	else if (strcmp(decision, "2") == 0)
	{
		cout << "�����û�����ע��...��" << endl;
		//if (sendto(sockClient, info, sizeof(info), 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR)) == SOCKET_ERROR)
			//err("send error!");
		if (recvfrom(sockClient, name, sizeof(name), 0, (sockaddr*)&addrClient, &len) == SOCKET_ERROR)
			err("recieve error");
		cout << "�������û���Ϊ:" << name << endl;
		if (recvfrom(sockClient, pass, sizeof(pass), 0, (sockaddr*)&addrClient, &len) == SOCKET_ERROR)
			err("recieve error"); 
		cout << "�����û�������Ϊ:" << pass << endl;
		counts++;
		adduser((string)name=name, (string)pass=pass);
		cout << "=================================" << endl;
		cout << "        �ִ��û�" <<COUNTS+1<<"��"<< endl;
		cout << "=================================" << endl;
		for (int i = 0; i < counts;i++)
		{
			cout << "�û���:" << std::left << setw(8) << users[i].uname << "����:" << std::left << setw(10) << users[i].upassword << endl;
		}
		cout << "=================================" << endl;
		countschange(counts);
	}

	//������Ϣ
	while (true) {
		recvfrom(sockClient, recvBuf, 100, 0, (SOCKADDR*)&addrClient, &len);
		cout << port << " says:" << recvBuf << endl;
		if (*recvBuf == 'q') {
			sendto(sockClient, "q", 1, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR));
			cout << "The connection with " << port << " ends!" << endl;
			break;
		}
		ostringstream oss;
		oss << "echo:" << recvBuf;
		strcpy(sendBuf, oss.str().c_str());
		//cout << "The data returned to " << port << " is: " << sendBuf << endl;
		//��������.
		sendto(sockClient, sendBuf, 100, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR));
	}
	closesocket(sockClient);
	return 0;
}

int main() {
	//�������
	WORD socketVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(socketVersion, &wsaData) != 0)
	{
		err("WSA start error!")
			return 0;
	}

	//�����׽���
	SOCKET sockSrv = socket(AF_INET, SOCK_DGRAM, 0);
	//������ַ�ṹ��.
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);
	//���׽��ֺ͵�ַ.
	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	char recvBuf[100];
	char sendBuf[100];
	char filename[100];         //�����ļ��������ļ�������С������
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);
	cout << "main����" << endl;

	users[0].uname = "1";
	users[0].upassword = "1234";
	users[1].uname = "2";
	users[1].upassword = "1234";
	users[2].uname = "3";
	users[2].upassword = "1234";



	int i = 6000;
	cout << "listening..." << endl;
	while (1) {
		i++;
		//���տͻ�����������
		if (recvfrom(sockSrv, recvBuf, 100, 0, (SOCKADDR*)&addrClient, &len) == SOCKET_ERROR)
		   err("receive error!");
		
		cout << "һ���ͻ��������ӵ����������¶˿��ǣ�" << i << endl;
		string i_s;
		stringstream ss;
		ss << i;
		ss >> i_s;
		strcpy(sendBuf, i_s.c_str());

		sendto(sockSrv, sendBuf, 100, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR));
		CreateThread(NULL, 0, &ServerThread, &i, 0, NULL);
		//// << "main��counts:"<<COUNTS<<endl;
	}//while
	closesocket(sockSrv);
	WSACleanup();
	return(0);
}//main









