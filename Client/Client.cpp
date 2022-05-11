#include <Winsock2.h>
#include <stdio.h>
#include <cstdio>
#include<iostream>
#include <iomanip>
#include"link.h"
#pragma comment(lib,"ws2_32.lib")
using namespace std;
#define _CRT_SECURE_NO_WARINGS
#define SERVERPORT 6000
# define err(errMsg) cout << "[line" << __LINE__ << "]" << errMsg << WSAGetLastError() << endl;        //定位错误位置

void main() {
	// 打开网络库
	WORD socketVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(socketVersion, &wsaData) != 0)
		return;

	//创建Socket
	SOCKET sockClient = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKADDR_IN addrClient;
	addrClient.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(SERVERPORT);

	char recvBuf[100];
	char sendBuf[100];
	int len = sizeof(SOCKADDR);

	char decision[100];
	char name[10];
	char nameinp[10];
	char pass[10];
	char passinp[10];
	char info[20];      //存储用户验证信息
	strcpy(sendBuf, "Welcome~");
	
		int i = 0;//0
		char Buf[100];
		while (true)
		{
			i++;

			if (i == 2)
			{
				cout << "------------------" << endl;
				cout << "请选择要进行的操作" << endl;
				cout << "------------------" << endl;
				cout << "  1.登录 2.注册" << endl;
				cout << "------------------" << endl;
				cin >> decision;
				strcpy(name, decision);
				if (sendto(sockClient, name, sizeof(name), 0, (SOCKADDR*)&addrClient, len) == SOCKET_ERROR)
					err("send error!");
				if (strcmp(decision, "1") == 0) //登录
				{
					//用户验证
				cout << "请输入用户名:";
				cin >> nameinp;
				strcpy(name, nameinp);
				if (sendto(sockClient, name, sizeof(name), 0, (SOCKADDR*)&addrClient, len) == SOCKET_ERROR)
					err("send error!");
				if (recvfrom(sockClient, info, sizeof(info), 0, (SOCKADDR*)&addrClient, &len) == SOCKET_ERROR)
					//err("*recieve error!");
				cout << info << endl;
				//if (info == "No such user!")
				//	cout << "no!" << endl;         //不能这样比较
				if (strcmp(info, "No such user!") == 0)
				{
					system("pause");
					return;
				}
				else
				{
					cout << "请输入密码:";
					cin >> passinp;
					strcpy(pass, passinp);
					if (sendto(sockClient, pass, sizeof(pass), 0, (SOCKADDR*)&addrClient, len) == SOCKET_ERROR)
						err("send error!");
					if (recvfrom(sockClient, info, sizeof(info), 0, (SOCKADDR*)&addrClient, &len) == SOCKET_ERROR)
						//err("recieve error!");
					cout << info << endl;
					if (strcmp(info, "密码错误！") == 0)
					{
						system("pause");
						return;
					}
				}
				}
				else if (strcmp(decision, "2") == 0) //注册
				{
					cout << "请输入新增用户名:";
					cin >> nameinp;
					strcpy(name, nameinp);
					if (sendto(sockClient, name, sizeof(name), 0, (SOCKADDR*)&addrClient, len) == SOCKET_ERROR)
						err("send error!");
					cout << "请输入新增用户的密码:";
					cin >> passinp;
					strcpy(pass, passinp);
					if (sendto(sockClient, pass, sizeof(pass), 0, (SOCKADDR*)&addrClient, len) == SOCKET_ERROR)
						err("send error!");
				}

				
			}

			else if (i == 3)
			{
				cout << "send(Input 'q' to disconnect.)>";
				getchar();
				gets_s(sendBuf);
				if (sendto(sockClient, sendBuf, sizeof(sendBuf), 0, (SOCKADDR*)&addrClient, len) == SOCKET_ERROR)
					err("send error!");
				if (*sendBuf == 'q')
					break;
				recvfrom(sockClient, recvBuf, 100, 0, (SOCKADDR*)&addrClient, &len);
				//cout << "The message from server is:" << recvBuf << endl;
			}
			else
			{
				cout << "send(Input 'q' to disconnect.)>";
				gets_s(sendBuf);
				if (sendto(sockClient, sendBuf, sizeof(sendBuf), 0, (SOCKADDR*)&addrClient, len) == SOCKET_ERROR)
					err("send error!");
				if (*sendBuf == 'q')
					break;
				if (recvfrom(sockClient, recvBuf, 100, 0, (SOCKADDR*)&addrClient, &len) == SOCKET_ERROR)
					err("recieve error!");
				//recvfrom(sockClient, recvBuf, 100, 0, (SOCKADDR*)&addrClient, &len);
				//cout << "The message from server is : " << recvBuf << endl;
			}

			if (i == 1) {
				int port = atoi(recvBuf);
				cout << "port:" << port << endl;
				addrClient.sin_port = htons(port);
			}
		}
		cout << "客户断开连接" << endl;
	

	closesocket(sockClient);
	WSACleanup();
	system("pause");
	return;

}
