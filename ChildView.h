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
	
	
	int m_CountPlayers;// счетчик игроков
	//int m_CoorX[4];// координата расположения карт игрока № 1 по Х
	//int m_CoorY[4];// координата расположения карт игрока № 1 по Y

	// CPoint класс позиции x, y
	CPoint m_ptPlayerCards[2];	// координаты карт на руках
	CPoint m_ptTableCards[2];	// координаты карт на столе
	CPoint m_ptTakeCards[2];	// координаты карт во взятке

	// координаты взятки
	//int m_CoorTakeX[2];
	//int m_CoorTakeY[2];


	// сдать карты
	void DistrCards(void);

	// будем следить за изменением окна
	afx_msg void OnSize(UINT nType, int cx, int cy);

	// ходящий игрок
	int m_GOINGPLAYER;

	


	// принимает серверную сторону
	afx_msg void OnNetServer();
	// принимаем сторону клиента
	afx_msg void OnNetClient();

	// наши сетевые гнезда
	CMySocket m_Socket;
	CMySocket m_secondSock[2];


	// режим программы 1-сервер, 2-клиент
	int m_Mode;


	// принимаем нового игрока(ничего не поделашь)
	LRESULT OnAccept(WPARAM wParam, LPARAM lParam);

	// соединение происходит на клиентской стороне
	LRESULT OnConnect(WPARAM wParam, LPARAM lParam);

	// извлекаем сообщения по сети
	LRESULT OnRecieve(WPARAM wParam, LPARAM lParam);
	
	// для упрощения отправки сообщения
	void SendData(CSendData send_data);

	// начало игры
	void NewGame(void);

	// принимаем необходимые данные для новой игры
	void NewGame_Receive(const CSendData& recv_data);

	// переводим ход на другого игрока
	void NextPlayer(void);
	// извлекаемданные для нового хода
	void NextPlayer_Receive(const CSendData& recv_data);

	// считаем новых клиентов
	int m_CountClient;

	// ход игрока и отправка необходимых данных
	void GoPlayer(CCard* pCard, int i);
	// прием данных для хода игрока
	void GoPlayer_Receive(const CSendData& recv_data);
	// текстовая метка "Взятка" или "Ваш ход"
	CString m_LabelMessage;

	// имена игроков CString класс строк типа длинных строк в Делфи
	CString m_Name1;
	CString m_Name2;

	// карты во взятку
	void CardsToTake(void);
	// получаем данные для забора взятки
	void CardsToTake_Receive(CSendData recv_data);

	// строки содержащие очки игроков
	CString m_Score[2];

	// закончен ли ход
	bool IsGoEnd(void);

	// проверим закончена ли игра
	bool IsEndGame(void);

	// строка поздравления
	CString m_strCongratulate;
};

