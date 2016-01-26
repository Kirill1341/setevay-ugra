#pragma once
#include "Card.h"
#include "Game3.h"



class CPack
{
public:
	CPack(CWnd* pParent);
	~CPack(void);

	// карты(массив карт) из которых состоит колода
	CCard* m_pCard[PACKNUMBERCARDS];
private:
	// окно родителя в котором будем рисовать
	CWnd* m_pParent;
public:
	void DrawPack(CDC* pDC);

	// определим, что щелкнули точно в пределах прямоугольника
	// карты и переместим эту карту поверх всех карт в колоде
	void MouseDown(const CPoint& pt);

	// карту с данным индексом расположить поверх всех карт в колоде
	void CardBringToFront(int index);

	// перемешиваем карты и  запоминаем алгоритм перемешивания
	void ShufflePack(char* arr);

	// по определенному алгоритму перемешиваем карты в колоде
	void GetShufflePack(char arr[52]);
};
