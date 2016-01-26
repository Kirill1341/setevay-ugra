////////////////////////////////////////////////////////////////////////////////////////////////////
//
//							http://www.interestprograms.ru 
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

// MySocket.cpp : implementation file
//

#include "stdafx.h"
#include "Game3.h"
#include "MySocket.h"
#include ".\mysocket.h"


// CMySocket

CMySocket::CMySocket()
: m_pParent(NULL)
{
	
}

CMySocket::~CMySocket()
{
}

// сообщение об установленном контакте
	// происходит на стороне клиента
void CMySocket::OnConnect(int nErrorCode)
{
	// сообщаем родителю об событии соединения
	SendMessage(m_pParent->m_hWnd, WM_CONNECT, 0, 0);

	
	CAsyncSocket::OnConnect(nErrorCode);
}

// событие закрытия соединения
	// происходит на обоих сторонах
void CMySocket::OnClose(int nErrorCode)
{
	// даем звук приотключении
	Beep(100, 300);

	CAsyncSocket::OnClose(nErrorCode);
}

// извлечение данных по сети
void CMySocket::OnReceive(int nErrorCode)
{
	// сообщаем родителю об событии возможности извлечения данных
	SendMessage(m_pParent->m_hWnd, WM_RECIEVE, 0, 0);
	
	CAsyncSocket::OnReceive(nErrorCode);
}

// принятие клиентов сервером для работы по сети
void CMySocket::OnAccept(int nErrorCode)
{
	// сообщаем родителю об событии возможности принятия клиента
	SendMessage(m_pParent->m_hWnd, WM_ACCEPT, 0, 0);
	
	
	CAsyncSocket::OnAccept(nErrorCode);
}

// установка родителького окна
void CMySocket::SetParent(CWnd* pParent)
{
	// назначение родительского окна, чтобы знать кому отсылать сообщения
	m_pParent = pParent;
}
