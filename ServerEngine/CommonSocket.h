#pragma once

#define WIN32_LEAN_AND_MEAN
#include <ws2tcpip.h>
#include <mswsock.h>
#include <Mstcpip.h>

#include "Platform.h"


namespace ServerEngine
{
#define SOCKET_ERROR 		(-1)

//设置套接字为可重用状态
BOOL		SetSocketReuseable(SOCKET hSocket);

//设置套接字为非阻塞状态
BOOL		SetSocketUnblock(SOCKET hSocket);

//设置套接字为阻塞状态
BOOL		SetSocketBlock(SOCKET hSocket);

BOOL		SetSocketNoDelay(SOCKET hSocket);

BOOL		SetSocketKeepAlive( SOCKET hSocket, int nKeepInterval, int nKeepCount, int nKeepIdle );

//初始化网络
BOOL		InitNetwork();

//反初始化网络
BOOL		UninitNetwork();

SOCKET		CreateSocket( int af = AF_INET, int type = SOCK_STREAM, int protocol = 0);

BOOL		BindSocket( SOCKET hSocket, const struct sockaddr* pAddr, int nNamelen);

BOOL		ListenSocket( SOCKET hSocket, int nBacklog);

BOOL		ConnectSocket(SOCKET hSocket, const char* pAddr, short sPort);

INT32		GetSocketLastError();

BOOL		IsSocketValid(SOCKET hSocket);

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

BOOL		SetSocketBuffSize(SOCKET hSocket, INT32 nRecvSize, INT32 nSendSize);

#ifdef _MSC_BUILD
BOOL		ConnectSocketEx(SOCKET hSocket, const char* pAddr, short sPort, LPOVERLAPPED lpOverlapped);

BOOL		AcceptSocketEx(SOCKET hListenSocket, LPOVERLAPPED lpOverlapped);
#endif
}


