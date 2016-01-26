
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
// стандартный конструктор, 
// осуществляет необходимые инициализации
CChildView::CChildView()
: m_pPack(NULL)
, m_CountPlayers(0)
, m_GOINGPLAYER(0)
, m_Mode(0)
, m_CountClient(0)
, m_LabelMessage(_T("Нет игроков в сети"))
, m_Name1("Down")
, m_Name2("Top")
, m_strCongratulate(_T(""))
{
	// инициализация очков игроков
	m_Score[0] = "0";
	m_Score[1] = "0";

	// в динамической памяти создаем колоду 
	// и получаем указатель на зафрахтованую нами ячейку памяти
	m_pPack = new CPack(this);
}

CChildView::~CChildView()
{
	// освобождаем занимаемую память колодой карт
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

	// кисть фона устанавливаем в 0, чтобы самостоятельно управлять прорисовкой окна
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), NULL/*reinterpret_cast<HBRUSH>(COLOR_WINDOW+1)*/, NULL);

	return TRUE;
}

// функция получающая сообщение прорисовки окна
void CChildView::OnPaint() 
{
	// DC - контекст устройства в котором надо рисовать,
	// в данном случае это наше окно
	CPaintDC dc(this); // device context for painting
	// (контекст устройства для рисования, ограничен клиентской частью )
	// m_wndView

	CRect rc;
	// получаем размеры окна отображения нашей игры
	GetClientRect(&rc);

	// заполняем прямоугольник окна зеленым цветом
	// создаем кисть окраски и  одновременно придаем ей зеленый цвет карточного стола
	CBrush brush(RGB(0,150,0)); // темно-красная кисть (RGB(150,0,0)), темно-синяя (RGB(0,0,150));
	
	// в данном контексте устройства с этих пор назначаем новую кисть
	// старую запоминаем для возврата настроек
	CBrush* poldBrush = dc.SelectObject(&brush);

	// заполняем прямоугольник всего окна 
	// выбранной кистью
	dc.Rectangle(&rc);

	// окрасили окно, возвращаем предустановленные настройки
	// т.е. в нашем случае кисть
	// обязательная процедура, особенно для windows-ов версии ниже XP
	dc.SelectObject(poldBrush);


	// рисуем колоду карт
	m_pPack->DrawPack(&dc);


	////////////////////////////////////////////////////////////////////////////////////////////////
	// прорисовка информационных текстовых меток
	////////////////////////////////////////////////////////////////////////////////////////////////

	// метка общего сообщения
	//          х - координата,  y-координата
	dc.TextOut(rc.right/2 + 100, rc.bottom/2 + 100, m_LabelMessage);

	// метка поздравления
	dc.TextOut(rc.right/2, rc.bottom/2 - 30, m_strCongratulate);

	// выводим текстовые метки имен и очков взяток
	dc.TextOut(m_ptPlayerCards[0].x + 100, m_ptPlayerCards[0].y + 50, m_Name1);
	dc.TextOut(m_ptTakeCards[0].x, m_ptTakeCards[0].y-20, m_Score[0]);

	// если в игре более двух игроков
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

	// TODO:  эта строка высвечивается в окне задач, т.е. что надо делать здесь

	// назначаем родителя для сокетов
	// чтобы знать кому посылать сообщения
	m_Socket.SetParent(this);
	for(int i = 0; i < 2; i++)
	{
		m_secondSock[i].SetParent(this);
	}

	return 0;
}


// отлавливем событие щелчка левой кнопки мыши
// производим действия взависимости от ситуации
void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// если игра закочена блокируем действия пользователя
	// и даем сигнал
	if(IsEndGame() == true) 
	{
		Beep(3500,100);
		return;
	}

	// если круг не окончен ходим картой
	if(IsGoEnd() == false)
	{
		// если она карта нижнего(первого) игрока
		// если карта на руках
		// выбираем самую верхнюю карту
		// если ход первого игрока
		// тогда карта перемещается на стол
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

		// после каждого хода проверяем - не закончен ли круг
		// для включения текстовой метки с соответствующим сообщением
		if(IsGoEnd() == true)
		{
			m_LabelMessage = "Взятка";
			Invalidate();
		}

	}// if(IsGoEnd() == false)
	else // если круг закончен, при щелчке мыши карты во взятку
	{
		CardsToTake();
	}// else if(IsGoEnd() == false)
	

	CWnd::OnLButtonDown(nFlags, point);
}

// команда меню 
// начало новой игры
void CChildView::OnFileBegin()
{
	// TODO: Add your command handler code here
	NewGame();
}

// сдача карт
void CChildView::DistrCards(void)
{
	// сдача карт начинается всегда со следующего после сдающего
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

// начало новой игры
void CChildView::NewGame(void)
{
	// во-первых, если в игре меньше двух игроков, начать игру нельзя
	if(m_CountPlayers < 2)
	{
		Beep(900,100);
		return;
	}

	//  сброс необходимых данных
	// очищаем строку поздравления с началом игры
	m_strCongratulate = ""; // или другой способ m_strCongratulate.Empty();

	// сброс очков для новой игры
	m_Score[0] = "0";
	m_Score[1] = "0";



	// изменяем начало отсчета псевдослучайных чисел
	 srand( (unsigned)time( NULL ) );

	// если это не следующая игра,
	// выберем случайного "первоходящего" игрока
	if(m_GOINGPLAYER == 0)
		m_GOINGPLAYER = 1 + rand()%2;// получаем псевдо случайное число 0 или 1
	
	
	// запомним алгоритм перемешиваиния
	char arr[PACKNUMBERCARDS];
	m_pPack->ShufflePack(arr);

	// сдаем карты 
	DistrCards();

	// отправим данные другому игроку
	// о начале новой игры другому игрокку
	CSendData data;
	// идентификация новой игры
	data.m_idMSG = CSendData::m_eNewGame;

	// отправим алгоритм перемешивания
	for(int i = 0; i < PACKNUMBERCARDS; i++)
	{
		data.arr[i] = arr[i];
	}
	data.m_goingPlayer = m_GOINGPLAYER;
	SendData(data);

	// если первый игрок на этой стороне напишем что "Ваш ход"
	if(m_GOINGPLAYER == 1)
		m_LabelMessage = "Ваш ход!";
	else 
		if(m_Mode == 1)
			m_LabelMessage = "Ходит Колька!";
		else
			m_LabelMessage = "Ходит Витька!";
}


// извлекаем данные для начала новой игры
// практически повторяем действия прямой функции 
// начала новой игры
void CChildView::NewGame_Receive(const CSendData& recv_data)
{

	// очистим строку поздравления
	m_strCongratulate.Empty(); // или другой способ m_strCongratulate = "";
	m_Score[0] = "0";
	m_Score[1] = "0";

	// при приеме ходящего игрока его "переворачиваем"
	// т.е. если там он снизу, то здесь сверху
	switch(recv_data.m_goingPlayer)
	{
	case 1:
		m_GOINGPLAYER = 2;
		break;
	case 2:
		m_GOINGPLAYER = 1;
		break;
	}

	// извлечем алгоритм перемешивания
	// чтобы карты в колоде располагались также как и на другой стороне
	char arr[PACKNUMBERCARDS];
	for(int i = 0; i < PACKNUMBERCARDS; i++)
	{
		arr[i] = recv_data.arr[i];
	}

	// размешаем карты по извлеченному алгоритму
	m_pPack->GetShufflePack(arr);

	// сдадим размешанные карты
	DistrCards();

	// если первый игрок на этой строне напишем что "Ваш ход"
	if(m_GOINGPLAYER == 1)
		m_LabelMessage = "Ваш ход!";
	else 
		// если мы на стороне сервера
		if(m_Mode == 1)
			m_LabelMessage = "Ходит Колька!";
		else
			m_LabelMessage = "Ходит Витька!";
}


// отслеживаем событие изменения размеров окна
void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// первый способ инициализации класса точки CPoint
	m_ptPlayerCards[0].x = cx/2 - 71/2;
	m_ptPlayerCards[0].y = cy - 101;

	m_ptPlayerCards[1].x = cx/2 - 71/2;
	m_ptPlayerCards[1].y = 5;

	// второй способ инициализации класса точки CPoint
	// посредством конструктора
	m_ptTakeCards[0] = CPoint(m_ptPlayerCards[0].x - 100, m_ptPlayerCards[0].y + 40);
	m_ptTakeCards[1] = CPoint(m_ptPlayerCards[1].x - 100, m_ptPlayerCards[1].y - 40);

	m_ptTableCards[0] = CPoint(cx/2 - 71/2, cy/2);
	m_ptTableCards[1] = CPoint(cx/2 - 71/2, cy/2 - 96);

	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// следим за правильностью расположения карт в окне

	// следим за правильным расположением карт у игрока на руках
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

	// следим за правильным расположением карт на столе
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

	// следим за правильным расположением карт во взятках игроков
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





// отсылаем номер следующего ходящего игрока
void CChildView::NextPlayer(void)
{
	m_GOINGPLAYER++;
	// если номер перескочил за пределы
	// допустимого количества игроков начинем сначала
	if(m_GOINGPLAYER > m_CountPlayers )
		m_GOINGPLAYER = 1;

	CSendData data;
	data.m_idMSG = CSendData::m_eNextPlayer;
	data.m_goingPlayer = m_GOINGPLAYER;
	SendData(data);


	// очистим метку для последующего заполнения
	m_LabelMessage = "";

	// если пришла пора взятки
	// если нет ходит следующий игрок
	if(IsGoEnd() == true) m_LabelMessage = "Взятка";
	else 
		if(m_Mode == 1)
			m_LabelMessage = "Ходит Колька!";
		else
			m_LabelMessage = "Ходит Витька!";

	Invalidate();	
	
}

// получаем номер ходящего игрока
void CChildView::NextPlayer_Receive(const CSendData& recv_data)
{

	// меняем данные ходящего игрока
	// т.е. переворачиваем игроков
	switch(recv_data.m_goingPlayer)
	{
	case 1:
		m_GOINGPLAYER = 2;
		break;
	case 2:
		m_GOINGPLAYER = 1;
		break;
	}

	// очистим метку для последующего заполнения
	m_LabelMessage = "";

	// если первый игрок на этой стороне напишем что "Ваш ход"
	if(IsGoEnd() == true) m_LabelMessage = "Взятка";
	else 
		if(m_GOINGPLAYER == 1)
			m_LabelMessage = "Ваш ход!";
		

	Invalidate();	
}


// ход игрока и отправка данных хода
void CChildView::GoPlayer(CCard* pCard, int i)
{
	// сразу отметим, что карта на столе
	pCard->m_StateCard = CCard::e_Table;
	CPoint pt = pCard->m_Point;
	pCard->SetPos(m_ptTableCards[0]);

	pCard->OpenCard(true);
	m_pPack->CardBringToFront(i);
	InvalidateRect(CRect(pt, CSize(71,96)));

	// отсылка данных хода
	CSendData data;
	data.m_idMSG = CSendData::m_eGoPlayer;
	data.m_Value = pCard->m_Value;
	data.m_Mast = pCard->m_Mast;
	data.m_goingPlayer = m_GOINGPLAYER;
	SendData(data);
	NextPlayer();

	// перерисовка изменений в окне
	Invalidate();

}

// получение данных хода
void CChildView::GoPlayer_Receive(const CSendData& recv_data)
{
	// вычислим карту и ее на стол для хода
	// в соответствии с ходом на другой(по сети) стороне
	for(int i = 0; i < PACKNUMBERCARDS; i++)
	{
		if(	m_pPack->m_pCard[i] != NULL &&
			m_pPack->m_pCard[i]->m_Value == recv_data.m_Value &&
			m_pPack->m_pCard[i]->m_Mast  == recv_data.m_Mast)
		{
			// сразу отметим, что карта на столе
			m_pPack->m_pCard[i]->m_StateCard = CCard::e_Table;
			CPoint pt = m_pPack->m_pCard[i]->m_Point;
			m_pPack->m_pCard[i]->SetPos(m_ptTableCards[m_pPack->m_pCard[i]->m_Player-1]);
			m_pPack->m_pCard[i]->OpenCard(true);
			m_pPack->CardBringToFront(i);
			InvalidateRect(CRect(pt, CSize(71,96)));
			break;
		}
	}

	// перерисовка изменений в окне
	Invalidate();
}


// карты во взятку и отправка необходимых данных
void CChildView::CardsToTake(void)
{
	// карты во взятку и гасим метку
	m_LabelMessage = ""; // или m_LabelMessage.Empty()

	//////////////////////////////////////////////////////////////////////////////////
	//			чья карта сильнее, тот игрок и берет
	///////////////////////////////////////////////////////////////////////////////////

	// запомним карту первого игрока которая на столе
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

	// запомним карту второго игрока которая на столе
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
	// подводим итоги конкурса
	// если значение больше первой карты, то берет игрок владелец первой карты
	// если вторая карта по значению больше, берет взятку второй игрок
	// если карты по значению равны берет карта более высшей масти (крести, вини, черви, буби)
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	int winner_player = -1;

	// проверка на корректность операций
	if(	temp1 != NULL && temp2 != NULL)
	{
		if(temp1->m_Value > temp2->m_Value)		// первый игрок берет взятку
		{
			winner_player = 1;
		}
		else 
			if(temp2->m_Value > temp1->m_Value) // или второй
			{
				winner_player = 2;
			}
			else // если карты равны
			{
				if(temp1->m_Mast < temp2->m_Mast)	// чья масть выше?
				{
					winner_player = 1;
				}
				else
				{
					winner_player = 2;
				}// else

			}// else // если карты равны

	}// if(	temp1 != NULL && temp2 != NULL)

	m_GOINGPLAYER = winner_player;

	// непосредственно перемещаем карты во взятку
	if(winner_player != -1)
	{
		int scores = 0;
		for(int i = 0; i < PACKNUMBERCARDS; i++)
		{
			if(	m_pPack->m_pCard[i] != NULL &&
				m_pPack->m_pCard[i]->m_StateCard == CCard::e_Table)
			{
				// сразу присваиваем идентификатор, что карта на столе
				m_pPack->m_pCard[i]->m_StateCard = CCard::e_Take;
				// карта будет принадлежат победителю
				m_pPack->m_pCard[i]->m_Player = winner_player;
				// расположим на месте для взяток
				m_pPack->m_pCard[i]->SetPos(m_ptTakeCards[winner_player-1]);
				

				// отправка данных о взятке
				// другому игроку
				CSendData data;
				data.m_idMSG = CSendData::m_eTake;
				data.m_Value = m_pPack->m_pCard[i]->m_Value;
				data.m_Mast  = m_pPack->m_pCard[i]->m_Mast;
				data.m_goingPlayer = m_GOINGPLAYER;
				SendData(data);

				// подсчет очков взятки
				scores += m_pPack->m_pCard[i]->m_Scores;

			}// if(	m_pPack->m_pCard[i]->m_Point == m_pt

		}// for(int i = 0; i < PACKNUMBERCARDS; i++)

		int old_scores = atoi(m_Score[winner_player-1]);
		// старые очки + новые
		scores += old_scores;
		m_Score[winner_player-1].Format("%d", scores);

		if(m_GOINGPLAYER == 1)
			m_LabelMessage = "Ваш ход!";
		else 
			// если режим сервера
			if(m_Mode == 1)
				m_LabelMessage = "Ходит Колька!";
			else
				m_LabelMessage = "Ходит Витька!";


	}// if(winner_player != -1)

	// если игра закончена
	if(IsEndGame() == true)
	{
		m_LabelMessage = "Игра закончена! Сдать карты!";

		// если больше очков набрал первый игрок
		// выиграл первый
		if(atoi(m_Score[0]) > atoi(m_Score[1]))
		{
			if(m_GOINGPLAYER == 1) m_strCongratulate = "Вы выиграли!";
			else m_strCongratulate = "Выиграл " + m_Name1;


			Beep(400, 150);
			Beep(900, 150);
			Beep(1200, 150);
			Beep(2800, 150);
			Beep(3500, 250);
		}
		else 
			// если больше очков набрал второй игрок
			// выиграл второй
			if(atoi(m_Score[0]) < atoi(m_Score[1]))
			{
				if(m_GOINGPLAYER == 1) m_strCongratulate = "Вы выиграли!";
				else m_strCongratulate = "Выиграл " + m_Name2;


				Beep(2500, 100);
				Beep(1500, 100);
				Beep(900, 100);
				Beep(300, 300);
			}
			else 
				// на "всякий случай"
				m_strCongratulate = "Ничья";

	}// if(IsEndGame() == true)
	
	
	Invalidate();

}

// получения данных забора взятки
void CChildView::CardsToTake_Receive(CSendData recv_data)
{
	// по команде карты на столе во взятку
	// действие функции аналогично CardsToTake()

	m_LabelMessage = "";
	int scores = 0;
	int player = recv_data.m_goingPlayer;
	// переворачиваем игрока
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
			// записываем состояние карты - во взятке
			m_pPack->m_pCard[i]->m_StateCard = CCard::e_Take;

			m_pPack->m_pCard[i]->m_Player = player;
			
			m_pPack->m_pCard[i]->SetPos(m_ptTakeCards[player-1]);
			// подсчет взятки
			scores += m_pPack->m_pCard[i]->m_Scores;
			
		}
	}

	int old_scores = atoi(m_Score[player-1]);
	scores += old_scores;
	m_Score[player-1].Format("%d", scores);

	if(m_GOINGPLAYER == 1)
		m_LabelMessage = "Ваш ход!";
	else 
		if(m_Mode == 1)
			m_LabelMessage = "Ходит Колька!";
		else
			m_LabelMessage = "Ходит Витька!";


	if(IsEndGame() == true)
	{
		m_LabelMessage = "Игра закончена! Сдать карты!";

		if(atoi(m_Score[0]) > atoi(m_Score[1]))
		{
			if(m_GOINGPLAYER == 1) m_strCongratulate = "Вы выиграли!";
			else m_strCongratulate = "Выиграл " + m_Name1;

			Beep(400, 150);
			Beep(900, 150);
			Beep(1200, 150);
			Beep(2800, 150);
			Beep(3500, 250);
		}
		else 
			if(atoi(m_Score[0]) < atoi(m_Score[1]))
			{
				if(m_GOINGPLAYER == 1) m_strCongratulate = "Вы выиграли!";
				else m_strCongratulate = "Выиграл " + m_Name2;
				Beep(2500, 100);
				Beep(1500, 100);
				Beep(900, 100);
				Beep(300, 300);
			}
			else 
				m_strCongratulate = "Ничья";
	}

	Invalidate();
}

// закончен ли круг ходов
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

	// если количество карт на столе равно числу игроков ход закончен
	// пора брать взятку
	if(count_table_cards == 2)
		return true;

	return false;
}

// проверим закончена ли игра
bool CChildView::IsEndGame(void)
{
	// если все карты во взятках игра закочена
	// или проще наоборот если хоть одна карта есть у игроков игра не окончена
	// проверять в конце забора взятки
	for(int i = 0; i < PACKNUMBERCARDS; i++)
	{
		// если хоть одна карта не во взятке
		// значит игра не окончена
		if(	m_pPack->m_pCard[i] != NULL &&
			m_pPack->m_pCard[i]->m_StateCard != CCard::e_Take)
		{
			return false;
		}
	}


	return true;
}



//////////////////////////////////////////////////////////////////////////
//							СЕТЕВЫЕ ФУНКЦИИ
//////////////////////////////////////////////////////////////////////////
// создаем сервер
void CChildView::OnNetServer()
{
	// Задаем режим сервера
	m_Mode = 1;
	CConnectDlg dlg;

	// если пользователь нажал ОК принимаем данные
	if(dlg.DoModal() == IDOK)
	{
		// для серверной стороны нам нужен только номер порта
		int port_number = dlg.m_PortNumber; 
		// если сокет не создан
		if(!m_Socket.Create(port_number)){
			// код оповещения ошибки
		}
		// если прослушивание не получилось
		else if(!m_Socket.Listen())
		{
			// код оповещения ошибки
		}
		else
		{
			// для упрощения имена даем фиксированые
			m_Name1 = " Витька ";
			m_Name2 = " Колька ";

			m_LabelMessage = "Один игрок в сети";
		}

	
		// перерисовываем окно для принятия изменений
		Invalidate();
	}// if(dlg.DoModal() == IDOK)
}


// создаем клиента
void CChildView::OnNetClient()
{
	// задаем режим клиента
	m_Mode = 2;
	
	CConnectDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		// от окна диалога получаем номер порта
		// и адрес сервера
		int port_number = dlg.m_PortNumber;
		CString strIP = dlg.m_IP;

		// если сокет не создан
		if(!m_Socket.Create()){
			// код оповещения ошибки
		}
		
		// если соединение не удалось
		if(!m_Socket.Connect(strIP, port_number)){
			// код оповещения ошибки
		}
		else
		{
			//AfxGetMainWnd()->SetWindowText("Connect OK!");
		}

	}
}

// принимаем игрока на серверной стороне
LRESULT CChildView::OnAccept(WPARAM wParam, LPARAM lParam)
{
	if(m_CountClient == 1) return 0;// больше одного не подключаем клиента

	m_Socket.Accept(m_secondSock[m_CountClient++]);
	m_CountPlayers = m_CountClient + 1; // один сам сервер

	// сразу отсылаем количество игроков клиенту
	CSendData data;
	data.m_idMSG = CSendData::m_eNumberPlayres;
	data.m_CountPlayers = m_CountPlayers;
	SendData(data);


	m_LabelMessage = "Два игрока в сети";

	Invalidate();

	return LRESULT();
}

LRESULT CChildView::OnConnect(WPARAM wParam, LPARAM lParam)
{
	//AfxGetMainWnd()->SetWindowText("Server connect!");
	// здесь происходит событие соединения с сервером на клиентской стороне
	return LRESULT();
}



// извлекаем данные пересылаемые по сети
LRESULT CChildView::OnRecieve(WPARAM wParam, LPARAM lParam)
{

	// если мы на стороне сервера
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

		// исследуем идентификатор сообщения
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
	else // если мы на стороне клиента
	{
		CSendData data;
		m_Socket.Receive(&data, sizeof(CSendData));

		// исследуем идентификатор сообщения
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
			// принимаем сообщения
			// у клиента имена "переворачиваем"
			m_Name2 = " Витька ";
			m_Name1 = " Колька ";

			m_LabelMessage = "Два игрока в сети";

			// перерисовываем окно
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


// для упрощения создаем функцию отправки данных по сети
void CChildView::SendData(CSendData send_data)
{
	// если мы на стороне сервера
	// отправляем данные клиенту
	// и наоборот
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


