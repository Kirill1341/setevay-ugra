////////////////////////////////////////////////////////////////////////////////////////////////////
//
//							http://www.interestprograms.ru 
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include ".\pack.h"
#include "Game3.h"


CPack::CPack(CWnd* pParent)
: m_pParent(pParent)
{
	//создание колоды карт
	
	
	HINSTANCE hRes = NULL;

	// извлекаем картинки карт из windows-ской системной папки,
	// из динамической библиотеки "cards.dll",
	// такая библиотека есть во многих видах windows-ов
	hRes=  ::LoadLibrary("cards.dll");
	if(hRes == NULL)
	{
		// не нашли библиотеку "cards.dll"
		Beep(300,1000);
	}
	else
		// теперь ресурсы приложение будет брать
		// из карточной библиотеки
		AfxSetResourceHandle(hRes); 

	// созаем карты, присваиваем значения, и назначаем масти,
	// загружаем картинки лица и рубашки
	for(int card = 0; card < PACKNUMBERCARDS; card++)
	{
		// начинаем с треф - крести
		if(card < 13)
		{
			m_pCard[card] = NULL;
			m_pCard[card] = new CCard(card+1, 54, pParent);
			m_pCard[card]->m_Point = CPoint((int)(card*0.2), (int)(card*0.2));
			m_pCard[card]->m_Value = card;

			if(card == 0)
				m_pCard[card]->m_Value = 13;

			m_pCard[card]->m_Mast = 1;
		}

		// бубна четвертая масть
		if(card >= 11 && card < 26)
		{
			m_pCard[card] = NULL;
			m_pCard[card] = new CCard(card+1, 54, pParent);
			m_pCard[card]->m_Point = CPoint((int)(card*0.2), (int)(card*0.2));
			m_pCard[card]->m_Value = card - 13;

			if(card == 13)
				m_pCard[card]->m_Value = 13;

			m_pCard[card]->m_Mast = 4;
		}

		// черви третья масть
		if(card >= 26 && card < 39)
		{
			m_pCard[card] = NULL;
			m_pCard[card] = new CCard(card+1, 54, pParent);
			m_pCard[card]->m_Point = CPoint((int)(card*0.2), (int)(card*0.2));
			m_pCard[card]->m_Value = card - 26;

			if(card == 26)
				m_pCard[card]->m_Value = 13;

			m_pCard[card]->m_Mast = 3;
		}

		// пик вторая масть
		if(card >= 39 && card < 52)
		{
			m_pCard[card] = NULL;
			m_pCard[card] = new CCard(card+1, 54, pParent);
			m_pCard[card]->m_Point = CPoint(card*0.2, card*0.2);
			m_pCard[card]->m_Value = card - 39;

			if(card == 39)
				m_pCard[card]->m_Value = 13;

			m_pCard[card]->m_Mast = 2;
		}

	}// for(int card = 0; card < PACKNUMBERCARDS; card++)

	for(int i = PACKNUMBERCARDS - 1; i >= 0; i--)
	{
		if(m_pCard[i] == NULL) break;

		if(m_pCard[i]->m_Value == 13)// туз
			m_pCard[i]->m_Scores = 11;

		if(m_pCard[i]->m_Value == 12)
			m_pCard[i]->m_Scores = 10;

		if(m_pCard[i]->m_Value == 11)
			m_pCard[i]->m_Scores = 10;

		if(m_pCard[i]->m_Value == 10) // валет
			m_pCard[i]->m_Scores = 10;

		if(m_pCard[i]->m_Value == 9)//  10
			m_pCard[i]->m_Scores = 10;

		if(m_pCard[i]->m_Value == 8)//  9
			m_pCard[i]->m_Scores = 9;

		if(m_pCard[i]->m_Value == 7)// 8
			m_pCard[i]->m_Scores = 8;

		if(m_pCard[i]->m_Value == 6)// 7
			m_pCard[i]->m_Scores = 7;

		if(m_pCard[i]->m_Value == 5)// 6
			m_pCard[i]->m_Scores = 6;

		if(m_pCard[i]->m_Value == 4)// 5
			m_pCard[i]->m_Scores = 5;

		if(m_pCard[i]->m_Value == 3)// 4
			m_pCard[i]->m_Scores = 4;

		if(m_pCard[i]->m_Value == 2)// 3
			m_pCard[i]->m_Scores = 3;

		if(m_pCard[i]->m_Value == 1)// 2
			m_pCard[i]->m_Scores = 2;

	}

	//resource is App
	AfxSetResourceHandle(AfxGetApp()->m_hInstance);
	::FreeLibrary(hRes);
	
}

CPack::~CPack(void)
{
	// освободим память занятую картами
	for(int i = 0; i < PACKNUMBERCARDS; i++)
	{
		if(m_pCard[i] != NULL)
		{
			delete m_pCard[i];
			m_pCard[i] = NULL;
		}
	}// for(int i = 0; i < PACKNUMBERCARDS; i++)
}

void CPack::DrawPack(CDC* pDC)
{
	// прорисуем каждую карту колоду
	for(int i = 0; i < PACKNUMBERCARDS; i++)
	{
		if(m_pCard[i] != NULL)
			m_pCard[i]->Draw(pDC);
	}// for(int i = 0; i < PACKNUMBERCARDS; i++)
}


void CPack::MouseDown(const CPoint& pt)
{
	// определение события щелчка кнопки мыши точно
	// в пределах прямоугольника карты
	for(int i = PACKNUMBERCARDS - 1; i >= 0; i--)
	{
		if(m_pCard[i] != NULL)
			if(m_pCard[i]->MouseDown(pt) == TRUE)
			{
				// на случай модернизации игры
				// CardBringToFront(i);

				break;
			}
	}
	
}

// карту с данным индексом расположим поверх
// других карт в колоде
void CPack::CardBringToFront(int index)
{
	
	CCard* temp = m_pCard[index];
	if(temp == NULL) return;

	for(int p = index; p < PACKNUMBERCARDS; p++)
	{
		if(m_pCard[p] != NULL)
			m_pCard[p] = m_pCard[p+1];
	}

	m_pCard[PACKNUMBERCARDS-1] = temp;
	m_pParent->InvalidateRect(m_pCard[PACKNUMBERCARDS-1]->GetRectCard());

}

// размешаем колоду и запомним алгоритм перемешивания
void CPack::ShufflePack(char* arr)
{
	srand( (unsigned)time( NULL ) );
	char r;
	for(int i = PACKNUMBERCARDS - 1; i >= 0; i--)
	{
			r = (char)(rand()%PACKNUMBERCARDS);
			arr[i] = r;
			CardBringToFront(r);
	}
	
}

// по установленному алгоритму перемешиваем карты
void CPack::GetShufflePack(char arr[PACKNUMBERCARDS])
{
	char r;
	for(int i = PACKNUMBERCARDS -1; i >= 0; i--)
	{
			r = arr[i];
			CardBringToFront(r);
	}
	
}