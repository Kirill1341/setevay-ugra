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

// ��������� �� ������������� ��������
	// ���������� �� ������� �������
void CMySocket::OnConnect(int nErrorCode)
{
	// �������� �������� �� ������� ����������
	SendMessage(m_pParent->m_hWnd, WM_CONNECT, 0, 0);

	
	CAsyncSocket::OnConnect(nErrorCode);
}

// ������� �������� ����������
	// ���������� �� ����� ��������
void CMySocket::OnClose(int nErrorCode)
{
	// ���� ���� �������������
	Beep(100, 300);

	CAsyncSocket::OnClose(nErrorCode);
}

// ���������� ������ �� ����
void CMySocket::OnReceive(int nErrorCode)
{
	// �������� �������� �� ������� ����������� ���������� ������
	SendMessage(m_pParent->m_hWnd, WM_RECIEVE, 0, 0);
	
	CAsyncSocket::OnReceive(nErrorCode);
}

// �������� �������� �������� ��� ������ �� ����
void CMySocket::OnAccept(int nErrorCode)
{
	// �������� �������� �� ������� ����������� �������� �������
	SendMessage(m_pParent->m_hWnd, WM_ACCEPT, 0, 0);
	
	
	CAsyncSocket::OnAccept(nErrorCode);
}

// ��������� ������������ ����
void CMySocket::SetParent(CWnd* pParent)
{
	// ���������� ������������� ����, ����� ����� ���� �������� ���������
	m_pParent = pParent;
}
