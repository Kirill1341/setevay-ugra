#include "StdAfx.h"
#include ".\card.h"




// конструктор класса карты
// инициализирующий переменные определенными 
// значениями
// чтобы этот конструктор заработал необходимо
// создать объект класса в динамической памяти
// иначе сработает конструктор по умолчанию
CCard::CCard(UINT face, UINT back, CWnd* pParent)
: m_Point(0)
, m_pParent(pParent)
//, m_Z(0)
, m_bOpenCard(false)
, m_Player(0)
//, m_IndexZ(0)
, m_Value(0)
, m_Mast(0)
, m_Scores(0)
{
	// загрузка битовых рисунков из ресурсов 
	// по идентификатору
	m_Face.LoadBitmap(face);
	m_Back.LoadBitmap(back);
}


// деструктор класса
// удобное место для освобождения памяти
// занимаемой переменными класса
CCard::~CCard(void)
{
	if(m_Face.m_hObject != NULL)
		m_Face.DeleteObject();
	if(m_Back.m_hObject != NULL)
		m_Back.DeleteObject();
}


// получить положение прямоугольника карты
CRect CCard::GetRectCard(void)
{
	return CRect(m_Point.x,m_Point.y,m_Point.x+71,m_Point.y+96);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//							http://www.interestprograms.ru 
//
/////////////////////////////////////////////////////////////////////////////////////////////////////





// прорисовка карты в контексте родителя
void CCard::Draw(CDC* pDC)
{
	// если разрешено открыть карту
	// прорисовываем лицо, иначе рисуем в прямоугольнике карты
	// рисунок рубашки
	if(m_bOpenCard == true)
		pDC->DrawState(m_Point, CSize(71,96), &m_Face, DST_BITMAP);
	else
		pDC->DrawState(m_Point, CSize(71,96), &m_Back, DST_BITMAP);
}

// псевособытие щелчка кнопки мыши
BOOL CCard::MouseDown(const CPoint& pt)
{
	// если указатель мыши во время щелчка находился
	// в пределах прямоугольника карты
	// возвратим TRUE(истину)
	BOOL result = GetRectCard().PtInRect(pt);
	return result;
}

// расположим карту в указанной точке
void CCard::SetPos(CPoint pt)
{
	this->m_Point = pt;
	m_pParent->InvalidateRect( CRect(m_Point.x,m_Point.y,m_Point.x+71,m_Point.y+96) );
}

// открыть или закрыть карту
void CCard::OpenCard(bool open)
{
	m_bOpenCard = open;
	// перерисуем прямоугольник карты
	m_pParent->InvalidateRect( CRect(m_Point.x,m_Point.y,m_Point.x+71,m_Point.y+96) );
}
