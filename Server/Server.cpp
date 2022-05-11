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
#define err(errMsg) cout << "[line" << __LINE__ << "]" << errMsg << WSAGetLastError() << endl;        //定位错误位置
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


DWORD WINAPI ServerThread(LPVOID lpParameter) {  //DWORD是数据类型，在这里是返回值，WINAPI是函数的调用形式

	int counts = COUNTS;
	char name[100];
	char pass[100];
	char info[200];
	char decision[10];
	int port = *(int*)lpParameter;
	port--;
	SOCKET sockClient = socket(AF_INET, SOCK_DGRAM, 0);
	//创建地址结构体.
	SOCKADDR_IN addrClient;
	addrClient.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(port);
	//绑定套接字和地址.
	bind(sockClient, (SOCKADDR*)&addrClient, sizeof(SOCKADDR));
	char recvBuf[100];
	char sendBuf[100];
	memset(recvBuf, 0, sizeof(recvBuf) / sizeof(char));
	memset(sendBuf, 0, sizeof(sendBuf) / sizeof(char));
	int len = sizeof(SOCKADDR);
	
	//验证用户身份
	if (recvfrom(sockClient, decision, sizeof(decision), 0, (SOCKADDR*)&addrClient, &len) == SOCKET_ERROR)
		err("receive error!");
	//cout <<"选择"<< decision<<endl;
	if (strcmp(decision, "1") == 0)
	{

		if (recvfrom(sockClient, name, sizeof(name), 0, (SOCKADDR*)&addrClient, &len) == SOCKET_ERROR)
			err("receive error!");
		cout << name << "请求登录..." << endl;
		//用已有的3个用户认证
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
			sprintf(info, "请继续输入密码~");
			if (sendto(sockClient, info, sizeof(info), 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR)) == SOCKET_ERROR)
				err("send error!");
			if (recvfrom(sockClient, pass, sizeof(pass), 0, (sockaddr*)&addrClient, &len) == SOCKET_ERROR)
				err("recieve error");
			if (pass == users[u].upassword)
			{
				cout << "欢迎登录！" << endl;
				sprintf(info, "欢迎登录！");
				if (sendto(sockClient, info, sizeof(info), 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR)) == SOCKET_ERROR)
					err("send error!");
			}
			else
			{
				cout << "密码错误！" << endl;
				sprintf(info, "密码错误！");
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
		cout << "【有用户正在注册...】" << endl;
		//if (sendto(sockClient, info, sizeof(info), 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR)) == SOCKET_ERROR)
			//err("send error!");
		if (recvfrom(sockClient, name, sizeof(name), 0, (sockaddr*)&addrClient, &len) == SOCKET_ERROR)
			err("recieve error");
		cout << "新增的用户名为:" << name << endl;
		if (recvfrom(sockClient, pass, sizeof(pass), 0, (sockaddr*)&addrClient, &len) == SOCKET_ERROR)
			err("recieve error"); 
		cout << "新增用户的密码为:" << pass << endl;
		counts++;
		adduser((string)name=name, (string)pass=pass);
		cout << "=================================" << endl;
		cout << "        现存用户" <<COUNTS+1<<"人"<< endl;
		cout << "=================================" << endl;
		for (int i = 0; i < counts;i++)
		{
			cout << "用户名:" << std::left << setw(8) << users[i].uname << "密码:" << std::left << setw(10) << users[i].upassword << endl;
		}
		cout << "=================================" << endl;
		countschange(counts);
	}

	//发送信息
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
		//发送数据.
		sendto(sockClient, sendBuf, 100, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR));
	}
	closesocket(sockClient);
	return 0;
}

int main() {
	//打开网络库
	WORD socketVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(socketVersion, &wsaData) != 0)
	{
		err("WSA start error!")
			return 0;
	}

	//创建套接字
	SOCKET sockSrv = socket(AF_INET, SOCK_DGRAM, 0);
	//创建地址结构体.
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);
	//绑定套接字和地址.
	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	char recvBuf[100];
	char sendBuf[100];
	char filename[100];         //接收文件，包含文件名、大小和日期
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);
	cout << "main函数" << endl;

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
		//接收客户端连接请求
		if (recvfrom(sockSrv, recvBuf, 100, 0, (SOCKADDR*)&addrClient, &len) == SOCKET_ERROR)
		   err("receive error!");
		
		cout << "一个客户端已连接到服务器，新端口是：" << i << endl;
		string i_s;
		stringstream ss;
		ss << i;
		ss >> i_s;
		strcpy(sendBuf, i_s.c_str());

		sendto(sockSrv, sendBuf, 100, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR));
		CreateThread(NULL, 0, &ServerThread, &i, 0, NULL);
		//// << "main中counts:"<<COUNTS<<endl;
	}//while
	closesocket(sockSrv);
	WSACleanup();
	return(0);
}//main









