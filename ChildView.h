// ChildView.h : interface of the CChildView class
//


#pragma once
#include "pack.h"
#include "mysocket.h"
#include "senddata.h"


// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CPack* m_pPack;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnFileBegin();
	
	
	int m_CountPlayers;// ������� �������
	//int m_CoorX[4];// ���������� ������������ ���� ������ � 1 �� �
	//int m_CoorY[4];// ���������� ������������ ���� ������ � 1 �� Y

	// CPoint ����� ������� x, y
	CPoint m_ptPlayerCards[2];	// ���������� ���� �� �����
	CPoint m_ptTableCards[2];	// ���������� ���� �� �����
	CPoint m_ptTakeCards[2];	// ���������� ���� �� ������

	// ���������� ������
	//int m_CoorTakeX[2];
	//int m_CoorTakeY[2];


	// ����� �����
	void DistrCards(void);

	// ����� ������� �� ���������� ����
	afx_msg void OnSize(UINT nType, int cx, int cy);

	// ������� �����
	int m_GOINGPLAYER;

	


	// ��������� ��������� �������
	afx_msg void OnNetServer();
	// ��������� ������� �������
	afx_msg void OnNetClient();

	// ���� ������� ������
	CMySocket m_Socket;
	CMySocket m_secondSock[2];


	// ����� ��������� 1-������, 2-������
	int m_Mode;


	// ��������� ������ ������(������ �� ��������)
	LRESULT OnAccept(WPARAM wParam, LPARAM lParam);

	// ���������� ���������� �� ���������� �������
	LRESULT OnConnect(WPARAM wParam, LPARAM lParam);

	// ��������� ��������� �� ����
	LRESULT OnRecieve(WPARAM wParam, LPARAM lParam);
	
	// ��� ��������� �������� ���������
	void SendData(CSendData send_data);

	// ������ ����
	void NewGame(void);

	// ��������� ����������� ������ ��� ����� ����
	void NewGame_Receive(const CSendData& recv_data);

	// ��������� ��� �� ������� ������
	void NextPlayer(void);
	// ��������������� ��� ������ ����
	void NextPlayer_Receive(const CSendData& recv_data);

	// ������� ����� ��������
	int m_CountClient;

	// ��� ������ � �������� ����������� ������
	void GoPlayer(CCard* pCard, int i);
	// ����� ������ ��� ���� ������
	void GoPlayer_Receive(const CSendData& recv_data);
	// ��������� ����� "������" ��� "��� ���"
	CString m_LabelMessage;

	// ����� ������� CString ����� ����� ���� ������� ����� � �����
	CString m_Name1;
	CString m_Name2;

	// ����� �� ������
	void CardsToTake(void);
	// �������� ������ ��� ������ ������
	void CardsToTake_Receive(CSendData recv_data);

	// ������ ���������� ���� �������
	CString m_Score[2];

	// �������� �� ���
	bool IsGoEnd(void);

	// �������� ��������� �� ����
	bool IsEndGame(void);

	// ������ ������������
	CString m_strCongratulate;
};

