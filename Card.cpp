#include "StdAfx.h"
#include ".\card.h"




// ����������� ������ �����
// ���������������� ���������� ������������� 
// ����������
// ����� ���� ����������� ��������� ����������
// ������� ������ ������ � ������������ ������
// ����� ��������� ����������� �� ���������
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
	// �������� ������� �������� �� �������� 
	// �� ��������������
	m_Face.LoadBitmap(face);
	m_Back.LoadBitmap(back);
}


// ���������� ������
// ������� ����� ��� ������������ ������
// ���������� ����������� ������
CCard::~CCard(void)
{
	if(m_Face.m_hObject != NULL)
		m_Face.DeleteObject();
	if(m_Back.m_hObject != NULL)
		m_Back.DeleteObject();
}


// �������� ��������� �������������� �����
CRect CCard::GetRectCard(void)
{
	return CRect(m_Point.x,m_Point.y,m_Point.x+71,m_Point.y+96);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//							http://www.interestprograms.ru 
//
/////////////////////////////////////////////////////////////////////////////////////////////////////





// ���������� ����� � ��������� ��������
void CCard::Draw(CDC* pDC)
{
	// ���� ��������� ������� �����
	// ������������� ����, ����� ������ � �������������� �����
	// ������� �������
	if(m_bOpenCard == true)
		pDC->DrawState(m_Point, CSize(71,96), &m_Face, DST_BITMAP);
	else
		pDC->DrawState(m_Point, CSize(71,96), &m_Back, DST_BITMAP);
}

// ������������ ������ ������ ����
BOOL CCard::MouseDown(const CPoint& pt)
{
	// ���� ��������� ���� �� ����� ������ ���������
	// � �������� �������������� �����
	// ��������� TRUE(������)
	BOOL result = GetRectCard().PtInRect(pt);
	return result;
}

// ���������� ����� � ��������� �����
void CCard::SetPos(CPoint pt)
{
	this->m_Point = pt;
	m_pParent->InvalidateRect( CRect(m_Point.x,m_Point.y,m_Point.x+71,m_Point.y+96) );
}

// ������� ��� ������� �����
void CCard::OpenCard(bool open)
{
	m_bOpenCard = open;
	// ���������� ������������� �����
	m_pParent->InvalidateRect( CRect(m_Point.x,m_Point.y,m_Point.x+71,m_Point.y+96) );
}
