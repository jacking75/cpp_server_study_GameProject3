#pragma once

#include "Platform.h"

#ifdef _MSC_BUILD
#define WIN32_LEAN_AND_MEAN
#include <ws2tcpip.h>
#include <mswsock.h>
#include <Mstcpip.h>
#endif



namespace ServerEngine
{
#define SOCKET_ERROR 		(-1)

//设置套接字为可重用状态
bool		SetSocketReuseable(SOCKET hSocket);

//设置套接字为非阻塞状态
bool		SetSocketNonblock(SOCKET hSocket);

//设置套接字为阻塞状态
bool		SetSocketBlock(SOCKET hSocket);

bool		SetSocketNoDelay(SOCKET hSocket);

bool		SetSocketKeepAlive( SOCKET hSocket, int nKeepInterval, int nKeepCount, int nKeepIdle );

//初始化网络
bool		InitNetwork();

//反初始化网络
bool		UninitNetwork();

SOCKET		CreateSocket( int af = AF_INET, int type = SOCK_STREAM, int protocol = 0);

bool		BindSocket( SOCKET hSocket, const struct sockaddr* pAddr, int nNamelen);

bool		ListenSocket( SOCKET hSocket, int nBacklog);

bool		ConnectSocket(SOCKET hSocket, const char* pAddr, short sPort);

INT32		GetSocketLastError();

bool		IsSocketValid(SOCKET hSocket);

//关闭套接字发送
void		ShutDownSend(SOCKET hSocket);

//关闭套接字接收
void		ShutDownRecv(SOCKET hSocket);

//关闭套接字
void		CloseSocket(SOCKET hSocket);

//取本机IP地址
std::string GetLocalIP();

std::string GetLastErrorStr(INT32 nError);

UINT32		IpAddrStrToInt(CHAR* pszIpAddr);

UINT32		IpAddrStrToInt(const CHAR* pszIpAddr);

std::string IpAddrIntToStr(UINT32 dwIpAddr);

bool		SetSocketBuffSize(SOCKET hSocket, INT32 nRecvSize, INT32 nSendSize);

#ifdef _MSC_BUILD
bool		ConnectSocketEx(SOCKET hSocket, const char* pAddr, short sPort, LPOVERLAPPED lpOverlapped);

bool		AcceptSocketEx(SOCKET hListenSocket, LPOVERLAPPED lpOverlapped);
#endif
}


