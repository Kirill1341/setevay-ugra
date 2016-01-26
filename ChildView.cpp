
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//							http://www.interestprograms.ru 
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "Game3.h"
#include "ChildView.h"
#include ".\childview.h"
#include "ConnectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView
// ����������� �����������, 
// ������������ ����������� �������������
CChildView::CChildView()
: m_pPack(NULL)
, m_CountPlayers(0)
, m_GOINGPLAYER(0)
, m_Mode(0)
, m_CountClient(0)
, m_LabelMessage(_T("��� ������� � ����"))
, m_Name1("Down")
, m_Name2("Top")
, m_strCongratulate(_T(""))
{
	// ������������� ����� �������
	m_Score[0] = "0";
	m_Score[1] = "0";

	// � ������������ ������ ������� ������ 
	// � �������� ��������� �� ������������� ���� ������ ������
	m_pPack = new CPack(this);
}

CChildView::~CChildView()
{
	// ����������� ���������� ������ ������� ����
	if(m_pPack != NULL)
	{
		delete m_pPack;
		m_pPack = NULL;
	}
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_FILE_BEGIN, OnFileBegin)
	ON_WM_SIZE()
	ON_COMMAND(ID_NET_SERVER, OnNetServer)
	ON_MESSAGE(WM_ACCEPT, OnAccept)
	ON_MESSAGE(WM_CONNECT, OnConnect)
	ON_MESSAGE(WM_RECIEVE, OnRecieve)
	ON_COMMAND(ID_NET_CLIENT, OnNetClient)
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;

	// ����� ���� ������������� � 0, ����� �������������� ��������� ����������� ����
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), NULL/*reinterpret_cast<HBRUSH>(COLOR_WINDOW+1)*/, NULL);

	return TRUE;
}

// ������� ���������� ��������� ���������� ����
void CChildView::OnPaint() 
{
	// DC - �������� ���������� � ������� ���� ��������,
	// � ������ ������ ��� ���� ����
	CPaintDC dc(this); // device context for painting
	// (�������� ���������� ��� ���������, ��������� ���������� ������ )
	// m_wndView

	CRect rc;
	// �������� ������� ���� ����������� ����� ����
	GetClientRect(&rc);

	// ��������� ������������� ���� ������� ������
	// ������� ����� ������� �  ������������ ������� �� ������� ���� ���������� �����
	CBrush brush(RGB(0,150,0)); // �����-������� ����� (RGB(150,0,0)), �����-����� (RGB(0,0,150));
	
	// � ������ ��������� ���������� � ���� ��� ��������� ����� �����
	// ������ ���������� ��� �������� ��������
	CBrush* poldBrush = dc.SelectObject(&brush);

	// ��������� ������������� ����� ���� 
	// ��������� ������
	dc.Rectangle(&rc);

	// �������� ����, ���������� ����������������� ���������
	// �.�. � ����� ������ �����
	// ������������ ���������, �������� ��� windows-�� ������ ���� XP
	dc.SelectObject(poldBrush);


	// ������ ������ ����
	m_pPack->DrawPack(&dc);


	////////////////////////////////////////////////////////////////////////////////////////////////
	// ���������� �������������� ��������� �����
	////////////////////////////////////////////////////////////////////////////////////////////////

	// ����� ������ ���������
	//          � - ����������,  y-����������
	dc.TextOut(rc.right/2 + 100, rc.bottom/2 + 100, m_LabelMessage);

	// ����� ������������
	dc.TextOut(rc.right/2, rc.bottom/2 - 30, m_strCongratulate);

	// ������� ��������� ����� ���� � ����� ������
	dc.TextOut(m_ptPlayerCards[0].x + 100, m_ptPlayerCards[0].y + 50, m_Name1);
	dc.TextOut(m_ptTakeCards[0].x, m_ptTakeCards[0].y-20, m_Score[0]);

	// ���� � ���� ����� ���� �������
	if(m_CountPlayers > 1)
	{
		dc.TextOut(m_ptTakeCards[1].x, m_ptTakeCards[1].y+116, m_Score[1]);
		dc.TextOut(m_ptPlayerCards[1].x + 100, 50, m_Name2);
	}
	
	// Do not call CWnd::OnPaint() for painting messages
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ��� ������ ������������� � ���� �����, �.�. ��� ���� ������ �����

	// ��������� �������� ��� �������
	// ����� ����� ���� �������� ���������
	m_Socket.SetParent(this);
	for(int i = 0; i < 2; i++)
	{
		m_secondSock[i].SetParent(this);
	}

	return 0;
}


// ���������� ������� ������ ����� ������ ����
// ���������� �������� ������������ �� ��������
void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// ���� ���� �������� ��������� �������� ������������
	// � ���� ������
	if(IsEndGame() == true) 
	{
		Beep(3500,100);
		return;
	}

	// ���� ���� �� ������� ����� ������
	if(IsGoEnd() == false)
	{
		// ���� ��� ����� �������(�������) ������
		// ���� ����� �� �����
		// �������� ����� ������� �����
		// ���� ��� ������� ������
		// ����� ����� ������������ �� ����
		for(int i = PACKNUMBERCARDS-1; i >= 0 ; i--)
		{
			if(	m_pPack->m_pCard[i]->m_Player == 1 &&
				m_pPack->m_pCard[i]->m_StateCard == CCard::e_Player)
				if(m_pPack->m_pCard[i]->MouseDown(point) == TRUE)
					if(m_GOINGPLAYER == 1)
					{
						GoPlayer(m_pPack->m_pCard[i], i);
						break;
					}
		}

		// ����� ������� ���� ��������� - �� �������� �� ����
		// ��� ��������� ��������� ����� � ��������������� ����������
		if(IsGoEnd() == true)
		{
			m_LabelMessage = "������";
			Invalidate();
		}

	}// if(IsGoEnd() == false)
	else // ���� ���� ��������, ��� ������ ���� ����� �� ������
	{
		CardsToTake();
	}// else if(IsGoEnd() == false)
	

	CWnd::OnLButtonDown(nFlags, point);
}

// ������� ���� 
// ������ ����� ����
void CChildView::OnFileBegin()
{
	// TODO: Add your command handler code here
	NewGame();
}

// ����� ����
void CChildView::DistrCards(void)
{
	// ����� ���� ���������� ������ �� ���������� ����� ��������
	for(int i = 0; i < PACKNUMBERCARDS; i++)
	{
		m_pPack->m_pCard[i]->m_StateCard = CCard::e_Player; 
		if( (m_GOINGPLAYER == 1 && i%2 == 0) ||
			(m_GOINGPLAYER == 2 && i%2 != 0)
			)
		{
			m_pPack->m_pCard[i]->m_Player = 2;
			m_pPack->m_pCard[i]->SetPos(m_ptPlayerCards[1]);
		}
		else
		{
			m_pPack->m_pCard[i]->m_Player = 1;
			m_pPack->m_pCard[i]->SetPos(m_ptPlayerCards[0]);
		}
	}
	Invalidate();

}

// ������ ����� ����
void CChildView::NewGame(void)
{
	// ��-������, ���� � ���� ������ ���� �������, ������ ���� ������
	if(m_CountPlayers < 2)
	{
		Beep(900,100);
		return;
	}

	//  ����� ����������� ������
	// ������� ������ ������������ � ������� ����
	m_strCongratulate = ""; // ��� ������ ������ m_strCongratulate.Empty();

	// ����� ����� ��� ����� ����
	m_Score[0] = "0";
	m_Score[1] = "0";



	// �������� ������ ������� ��������������� �����
	 srand( (unsigned)time( NULL ) );

	// ���� ��� �� ��������� ����,
	// ������� ���������� "�������������" ������
	if(m_GOINGPLAYER == 0)
		m_GOINGPLAYER = 1 + rand()%2;// �������� ������ ��������� ����� 0 ��� 1
	
	
	// �������� �������� ��������������
	char arr[PACKNUMBERCARDS];
	m_pPack->ShufflePack(arr);

	// ����� ����� 
	DistrCards();

	// �������� ������ ������� ������
	// � ������ ����� ���� ������� �������
	CSendData data;
	// ������������� ����� ����
	data.m_idMSG = CSendData::m_eNewGame;

	// �������� �������� �������������
	for(int i = 0; i < PACKNUMBERCARDS; i++)
	{
		data.arr[i] = arr[i];
	}
	data.m_goingPlayer = m_GOINGPLAYER;
	SendData(data);

	// ���� ������ ����� �� ���� ������� ������� ��� "��� ���"
	if(m_GOINGPLAYER == 1)
		m_LabelMessage = "��� ���!";
	else 
		if(m_Mode == 1)
			m_LabelMessage = "����� ������!";
		else
			m_LabelMessage = "����� ������!";
}


// ��������� ������ ��� ������ ����� ����
// ����������� ��������� �������� ������ ������� 
// ������ ����� ����
void CChildView::NewGame_Receive(const CSendData& recv_data)
{

	// ������� ������ ������������
	m_strCongratulate.Empty(); // ��� ������ ������ m_strCongratulate = "";
	m_Score[0] = "0";
	m_Score[1] = "0";

	// ��� ������ �������� ������ ��� "��������������"
	// �.�. ���� ��� �� �����, �� ����� ������
	switch(recv_data.m_goingPlayer)
	{
	case 1:
		m_GOINGPLAYER = 2;
		break;
	case 2:
		m_GOINGPLAYER = 1;
		break;
	}

	// �������� �������� �������������
	// ����� ����� � ������ ������������� ����� ��� � �� ������ �������
	char arr[PACKNUMBERCARDS];
	for(int i = 0; i < PACKNUMBERCARDS; i++)
	{
		arr[i] = recv_data.arr[i];
	}

	// ��������� ����� �� ������������ ���������
	m_pPack->GetShufflePack(arr);

	// ������ ����������� �����
	DistrCards();

	// ���� ������ ����� �� ���� ������ ������� ��� "��� ���"
	if(m_GOINGPLAYER == 1)
		m_LabelMessage = "��� ���!";
	else 
		// ���� �� �� ������� �������
		if(m_Mode == 1)
			m_LabelMessage = "����� ������!";
		else
			m_LabelMessage = "����� ������!";
}


// ����������� ������� ��������� �������� ����
void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// ������ ������ ������������� ������ ����� CPoint
	m_ptPlayerCards[0].x = cx/2 - 71/2;
	m_ptPlayerCards[0].y = cy - 101;

	m_ptPlayerCards[1].x = cx/2 - 71/2;
	m_ptPlayerCards[1].y = 5;

	// ������ ������ ������������� ������ ����� CPoint
	// ����������� ������������
	m_ptTakeCards[0] = CPoint(m_ptPlayerCards[0].x - 100, m_ptPlayerCards[0].y + 40);
	m_ptTakeCards[1] = CPoint(m_ptPlayerCards[1].x - 100, m_ptPlayerCards[1].y - 40);

	m_ptTableCards[0] = CPoint(cx/2 - 71/2, cy/2);
	m_ptTableCards[1] = CPoint(cx/2 - 71/2, cy/2 - 96);

	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// ������ �� ������������� ������������ ���� � ����

	// ������ �� ���������� ������������� ���� � ������ �� �����
	for(int i = 0; i < PACKNUMBERCARDS; i++)
	{
		if(	m_pPack->m_pCard[i] != NULL &&
			m_pPack->m_pCard[i]->m_StateCard == CCard::e_Player)
		{
			if(	m_pPack->m_pCard[i]->m_Player == 1)
				m_pPack->m_pCard[i]->SetPos(m_ptPlayerCards[0]);
			if(m_pPack->m_pCard[i]->m_Player == 2)
				m_pPack->m_pCard[i]->SetPos(m_ptPlayerCards[1]);
		}
	}

	// ������ �� ���������� ������������� ���� �� �����
	for(int i = 0; i < PACKNUMBERCARDS; i++)
	{
		if(	m_pPack->m_pCard[i] != NULL &&
			m_pPack->m_pCard[i]->m_StateCard == CCard::e_Table)
		{
			if(	m_pPack->m_pCard[i]->m_Player == 1)
				m_pPack->m_pCard[i]->SetPos(m_ptTableCards[0]);
			if(m_pPack->m_pCard[i]->m_Player == 2)
				m_pPack->m_pCard[i]->SetPos(m_ptTableCards[1]);
		}
	}

	// ������ �� ���������� ������������� ���� �� ������� �������
	for(int i = 0; i < PACKNUMBERCARDS; i++)
	{
		if(	m_pPack->m_pCard[i] != NULL &&
			m_pPack->m_pCard[i]->m_StateCard == CCard::e_Take)
		{
			if(	m_pPack->m_pCard[i]->m_Player == 1)
				m_pPack->m_pCard[i]->SetPos(m_ptTakeCards[0]);
			if(m_pPack->m_pCard[i]->m_Player == 2)
				m_pPack->m_pCard[i]->SetPos(m_ptTakeCards[1]);
		}
	}


}





// �������� ����� ���������� �������� ������
void CChildView::NextPlayer(void)
{
	m_GOINGPLAYER++;
	// ���� ����� ���������� �� �������
	// ����������� ���������� ������� ������� �������
	if(m_GOINGPLAYER > m_CountPlayers )
		m_GOINGPLAYER = 1;

	CSendData data;
	data.m_idMSG = CSendData::m_eNextPlayer;
	data.m_goingPlayer = m_GOINGPLAYER;
	SendData(data);


	// ������� ����� ��� ������������ ����������
	m_LabelMessage = "";

	// ���� ������ ���� ������
	// ���� ��� ����� ��������� �����
	if(IsGoEnd() == true) m_LabelMessage = "������";
	else 
		if(m_Mode == 1)
			m_LabelMessage = "����� ������!";
		else
			m_LabelMessage = "����� ������!";

	Invalidate();	
	
}

// �������� ����� �������� ������
void CChildView::NextPlayer_Receive(const CSendData& recv_data)
{

	// ������ ������ �������� ������
	// �.�. �������������� �������
	switch(recv_data.m_goingPlayer)
	{
	case 1:
		m_GOINGPLAYER = 2;
		break;
	case 2:
		m_GOINGPLAYER = 1;
		break;
	}

	// ������� ����� ��� ������������ ����������
	m_LabelMessage = "";

	// ���� ������ ����� �� ���� ������� ������� ��� "��� ���"
	if(IsGoEnd() == true) m_LabelMessage = "������";
	else 
		if(m_GOINGPLAYER == 1)
			m_LabelMessage = "��� ���!";
		

	Invalidate();	
}


// ��� ������ � �������� ������ ����
void CChildView::GoPlayer(CCard* pCard, int i)
{
	// ����� �������, ��� ����� �� �����
	pCard->m_StateCard = CCard::e_Table;
	CPoint pt = pCard->m_Point;
	pCard->SetPos(m_ptTableCards[0]);

	pCard->OpenCard(true);
	m_pPack->CardBringToFront(i);
	InvalidateRect(CRect(pt, CSize(71,96)));

	// ������� ������ ����
	CSendData data;
	data.m_idMSG = CSendData::m_eGoPlayer;
	data.m_Value = pCard->m_Value;
	data.m_Mast = pCard->m_Mast;
	data.m_goingPlayer = m_GOINGPLAYER;
	SendData(data);
	NextPlayer();

	// ����������� ��������� � ����
	Invalidate();

}

// ��������� ������ ����
void CChildView::GoPlayer_Receive(const CSendData& recv_data)
{
	// �������� ����� � �� �� ���� ��� ����
	// � ������������ � ����� �� ������(�� ����) �������
	for(int i = 0; i < PACKNUMBERCARDS; i++)
	{
		if(	m_pPack->m_pCard[i] != NULL &&
			m_pPack->m_pCard[i]->m_Value == recv_data.m_Value &&
			m_pPack->m_pCard[i]->m_Mast  == recv_data.m_Mast)
		{
			// ����� �������, ��� ����� �� �����
			m_pPack->m_pCard[i]->m_StateCard = CCard::e_Table;
			CPoint pt = m_pPack->m_pCard[i]->m_Point;
			m_pPack->m_pCard[i]->SetPos(m_ptTableCards[m_pPack->m_pCard[i]->m_Player-1]);
			m_pPack->m_pCard[i]->OpenCard(true);
			m_pPack->CardBringToFront(i);
			InvalidateRect(CRect(pt, CSize(71,96)));
			break;
		}
	}

	// ����������� ��������� � ����
	Invalidate();
}


// ����� �� ������ � �������� ����������� ������
void CChildView::CardsToTake(void)
{
	// ����� �� ������ � ����� �����
	m_LabelMessage = ""; // ��� m_LabelMessage.Empty()

	//////////////////////////////////////////////////////////////////////////////////
	//			��� ����� �������, ��� ����� � �����
	///////////////////////////////////////////////////////////////////////////////////

	// �������� ����� ������� ������ ������� �� �����
	CCard* temp1 = NULL;
	for(int i = 0; i < PACKNUMBERCARDS; i++)
	{
		if(	m_pPack->m_pCard[i] != NULL &&
			m_pPack->m_pCard[i]->m_Player == 1 &&
			m_pPack->m_pCard[i]->m_StateCard == CCard::e_Table)
		{
			temp1 = m_pPack->m_pCard[i];
			break;
		}
	}// for(int i = 0; i < PACKNUMBERCARDS; i++)

	// �������� ����� ������� ������ ������� �� �����
	CCard* temp2 = NULL;
	for(int i = 0; i < PACKNUMBERCARDS; i++)
	{
		if(	m_pPack->m_pCard[i] != NULL &&
			m_pPack->m_pCard[i]->m_Player == 2 &&
			m_pPack->m_pCard[i]->m_StateCard == CCard::e_Table)
		{
			temp2 = m_pPack->m_pCard[i];
			break;
		}
	}// for(int i = 0; i < PACKNUMBERCARDS; i++)


	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// �������� ����� ��������
	// ���� �������� ������ ������ �����, �� ����� ����� �������� ������ �����
	// ���� ������ ����� �� �������� ������, ����� ������ ������ �����
	// ���� ����� �� �������� ����� ����� ����� ����� ������ ����� (������, ����, �����, ����)
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	int winner_player = -1;

	// �������� �� ������������ ��������
	if(	temp1 != NULL && temp2 != NULL)
	{
		if(temp1->m_Value > temp2->m_Value)		// ������ ����� ����� ������
		{
			winner_player = 1;
		}
		else 
			if(temp2->m_Value > temp1->m_Value) // ��� ������
			{
				winner_player = 2;
			}
			else // ���� ����� �����
			{
				if(temp1->m_Mast < temp2->m_Mast)	// ��� ����� ����?
				{
					winner_player = 1;
				}
				else
				{
					winner_player = 2;
				}// else

			}// else // ���� ����� �����

	}// if(	temp1 != NULL && temp2 != NULL)

	m_GOINGPLAYER = winner_player;

	// ��������������� ���������� ����� �� ������
	if(winner_player != -1)
	{
		int scores = 0;
		for(int i = 0; i < PACKNUMBERCARDS; i++)
		{
			if(	m_pPack->m_pCard[i] != NULL &&
				m_pPack->m_pCard[i]->m_StateCard == CCard::e_Table)
			{
				// ����� ����������� �������������, ��� ����� �� �����
				m_pPack->m_pCard[i]->m_StateCard = CCard::e_Take;
				// ����� ����� ����������� ����������
				m_pPack->m_pCard[i]->m_Player = winner_player;
				// ���������� �� ����� ��� ������
				m_pPack->m_pCard[i]->SetPos(m_ptTakeCards[winner_player-1]);
				

				// �������� ������ � ������
				// ������� ������
				CSendData data;
				data.m_idMSG = CSendData::m_eTake;
				data.m_Value = m_pPack->m_pCard[i]->m_Value;
				data.m_Mast  = m_pPack->m_pCard[i]->m_Mast;
				data.m_goingPlayer = m_GOINGPLAYER;
				SendData(data);

				// ������� ����� ������
				scores += m_pPack->m_pCard[i]->m_Scores;

			}// if(	m_pPack->m_pCard[i]->m_Point == m_pt

		}// for(int i = 0; i < PACKNUMBERCARDS; i++)

		int old_scores = atoi(m_Score[winner_player-1]);
		// ������ ���� + �����
		scores += old_scores;
		m_Score[winner_player-1].Format("%d", scores);

		if(m_GOINGPLAYER == 1)
			m_LabelMessage = "��� ���!";
		else 
			// ���� ����� �������
			if(m_Mode == 1)
				m_LabelMessage = "����� ������!";
			else
				m_LabelMessage = "����� ������!";


	}// if(winner_player != -1)

	// ���� ���� ���������
	if(IsEndGame() == true)
	{
		m_LabelMessage = "���� ���������! ����� �����!";

		// ���� ������ ����� ������ ������ �����
		// ������� ������
		if(atoi(m_Score[0]) > atoi(m_Score[1]))
		{
			if(m_GOINGPLAYER == 1) m_strCongratulate = "�� ��������!";
			else m_strCongratulate = "������� " + m_Name1;


			Beep(400, 150);
			Beep(900, 150);
			Beep(1200, 150);
			Beep(2800, 150);
			Beep(3500, 250);
		}
		else 
			// ���� ������ ����� ������ ������ �����
			// ������� ������
			if(atoi(m_Score[0]) < atoi(m_Score[1]))
			{
				if(m_GOINGPLAYER == 1) m_strCongratulate = "�� ��������!";
				else m_strCongratulate = "������� " + m_Name2;


				Beep(2500, 100);
				Beep(1500, 100);
				Beep(900, 100);
				Beep(300, 300);
			}
			else 
				// �� "������ ������"
				m_strCongratulate = "�����";

	}// if(IsEndGame() == true)
	
	
	Invalidate();

}

// ��������� ������ ������ ������
void CChildView::CardsToTake_Receive(CSendData recv_data)
{
	// �� ������� ����� �� ����� �� ������
	// �������� ������� ���������� CardsToTake()

	m_LabelMessage = "";
	int scores = 0;
	int player = recv_data.m_goingPlayer;
	// �������������� ������
	if(player == 1)
		player = 2;
	else if(player == 2)
			player = 1;

	m_GOINGPLAYER = player;

	for(int i = 0; i < PACKNUMBERCARDS; i++)
	{
		if(	m_pPack->m_pCard[i] != NULL &&
			m_pPack->m_pCard[i]->m_StateCard == CCard::e_Table)
		{
			// ���������� ��������� ����� - �� ������
			m_pPack->m_pCard[i]->m_StateCard = CCard::e_Take;

			m_pPack->m_pCard[i]->m_Player = player;
			
			m_pPack->m_pCard[i]->SetPos(m_ptTakeCards[player-1]);
			// ������� ������
			scores += m_pPack->m_pCard[i]->m_Scores;
			
		}
	}

	int old_scores = atoi(m_Score[player-1]);
	scores += old_scores;
	m_Score[player-1].Format("%d", scores);

	if(m_GOINGPLAYER == 1)
		m_LabelMessage = "��� ���!";
	else 
		if(m_Mode == 1)
			m_LabelMessage = "����� ������!";
		else
			m_LabelMessage = "����� ������!";


	if(IsEndGame() == true)
	{
		m_LabelMessage = "���� ���������! ����� �����!";

		if(atoi(m_Score[0]) > atoi(m_Score[1]))
		{
			if(m_GOINGPLAYER == 1) m_strCongratulate = "�� ��������!";
			else m_strCongratulate = "������� " + m_Name1;

			Beep(400, 150);
			Beep(900, 150);
			Beep(1200, 150);
			Beep(2800, 150);
			Beep(3500, 250);
		}
		else 
			if(atoi(m_Score[0]) < atoi(m_Score[1]))
			{
				if(m_GOINGPLAYER == 1) m_strCongratulate = "�� ��������!";
				else m_strCongratulate = "������� " + m_Name2;
				Beep(2500, 100);
				Beep(1500, 100);
				Beep(900, 100);
				Beep(300, 300);
			}
			else 
				m_strCongratulate = "�����";
	}

	Invalidate();
}

// �������� �� ���� �����
bool CChildView::IsGoEnd(void)
{
	int count_table_cards = 0;
	for(int i = 0; i < PACKNUMBERCARDS; i++)
	{
		if(m_pPack->m_pCard[i] != NULL)
			if(	m_pPack->m_pCard[i]->m_StateCard == CCard::e_Table)
			{
				count_table_cards++;
			}
	}// for(int i = 0; i < PACKNUMBERCARDS; i++)

	// ���� ���������� ���� �� ����� ����� ����� ������� ��� ��������
	// ���� ����� ������
	if(count_table_cards == 2)
		return true;

	return false;
}

// �������� ��������� �� ����
bool CChildView::IsEndGame(void)
{
	// ���� ��� ����� �� ������� ���� ��������
	// ��� ����� �������� ���� ���� ���� ����� ���� � ������� ���� �� ��������
	// ��������� � ����� ������ ������
	for(int i = 0; i < PACKNUMBERCARDS; i++)
	{
		// ���� ���� ���� ����� �� �� ������
		// ������ ���� �� ��������
		if(	m_pPack->m_pCard[i] != NULL &&
			m_pPack->m_pCard[i]->m_StateCard != CCard::e_Take)
		{
			return false;
		}
	}


	return true;
}



//////////////////////////////////////////////////////////////////////////
//							������� �������
//////////////////////////////////////////////////////////////////////////
// ������� ������
void CChildView::OnNetServer()
{
	// ������ ����� �������
	m_Mode = 1;
	CConnectDlg dlg;

	// ���� ������������ ����� �� ��������� ������
	if(dlg.DoModal() == IDOK)
	{
		// ��� ��������� ������� ��� ����� ������ ����� �����
		int port_number = dlg.m_PortNumber; 
		// ���� ����� �� ������
		if(!m_Socket.Create(port_number)){
			// ��� ���������� ������
		}
		// ���� ������������� �� ����������
		else if(!m_Socket.Listen())
		{
			// ��� ���������� ������
		}
		else
		{
			// ��� ��������� ����� ���� ������������
			m_Name1 = " ������ ";
			m_Name2 = " ������ ";

			m_LabelMessage = "���� ����� � ����";
		}

	
		// �������������� ���� ��� �������� ���������
		Invalidate();
	}// if(dlg.DoModal() == IDOK)
}


// ������� �������
void CChildView::OnNetClient()
{
	// ������ ����� �������
	m_Mode = 2;
	
	CConnectDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		// �� ���� ������� �������� ����� �����
		// � ����� �������
		int port_number = dlg.m_PortNumber;
		CString strIP = dlg.m_IP;

		// ���� ����� �� ������
		if(!m_Socket.Create()){
			// ��� ���������� ������
		}
		
		// ���� ���������� �� �������
		if(!m_Socket.Connect(strIP, port_number)){
			// ��� ���������� ������
		}
		else
		{
			//AfxGetMainWnd()->SetWindowText("Connect OK!");
		}

	}
}

// ��������� ������ �� ��������� �������
LRESULT CChildView::OnAccept(WPARAM wParam, LPARAM lParam)
{
	if(m_CountClient == 1) return 0;// ������ ������ �� ���������� �������

	m_Socket.Accept(m_secondSock[m_CountClient++]);
	m_CountPlayers = m_CountClient + 1; // ���� ��� ������

	// ����� �������� ���������� ������� �������
	CSendData data;
	data.m_idMSG = CSendData::m_eNumberPlayres;
	data.m_CountPlayers = m_CountPlayers;
	SendData(data);


	m_LabelMessage = "��� ������ � ����";

	Invalidate();

	return LRESULT();
}

LRESULT CChildView::OnConnect(WPARAM wParam, LPARAM lParam)
{
	//AfxGetMainWnd()->SetWindowText("Server connect!");
	// ����� ���������� ������� ���������� � �������� �� ���������� �������
	return LRESULT();
}



// ��������� ������ ������������ �� ����
LRESULT CChildView::OnRecieve(WPARAM wParam, LPARAM lParam)
{

	// ���� �� �� ������� �������
	if(m_Mode == 1) 
	{
		CSendData data;
		for(int i = 0; i < m_CountPlayers; i++)
		{
			if(m_secondSock[i] != INVALID_SOCKET)
			{
				m_secondSock[i].Receive(&data, sizeof(CSendData));
				if(data.m_idMSG != 0)
					break;
			}
		}

		// ��������� ������������� ���������
		switch(data.m_idMSG)
		{
		case CSendData::m_eNewGame:
			NewGame_Receive(data);
			break;
		case CSendData::m_eNextPlayer:
			NextPlayer_Receive(data);
			break;
		case CSendData::m_eGoPlayer:
			GoPlayer_Receive(data);
			break;
		case CSendData::m_eTake:
			CardsToTake_Receive(data);
			break;
		}

	}
	else // ���� �� �� ������� �������
	{
		CSendData data;
		m_Socket.Receive(&data, sizeof(CSendData));

		// ��������� ������������� ���������
		switch(data.m_idMSG)
		{
		case CSendData::m_eNewGame:
			NewGame_Receive(data);
			break;
		case CSendData::m_eNextPlayer:
			NextPlayer_Receive(data);
			break;
		case CSendData::m_eNumberPlayres:
			m_CountPlayers = data.m_CountPlayers;
			// ��������� ���������
			// � ������� ����� "��������������"
			m_Name2 = " ������ ";
			m_Name1 = " ������ ";

			m_LabelMessage = "��� ������ � ����";

			// �������������� ����
			Invalidate();

			break;
		case CSendData::m_eGoPlayer:
			GoPlayer_Receive(data);
			break;
		case CSendData::m_eTake:
			CardsToTake_Receive(data);
			break;
		}
	}
	
	return LRESULT();
}


// ��� ��������� ������� ������� �������� ������ �� ����
void CChildView::SendData(CSendData send_data)
{
	// ���� �� �� ������� �������
	// ���������� ������ �������
	// � ��������
	if(m_Mode == 1)
	{
		for(int i = 0; i < m_CountPlayers; i++)
		{
			m_secondSock[i].Send(&send_data, sizeof(send_data));
		}
	}
	else
	{
		m_Socket.Send(&send_data, sizeof(send_data));
	}
}


