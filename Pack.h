#pragma once
#include "Card.h"
#include "Game3.h"



class CPack
{
public:
	CPack(CWnd* pParent);
	~CPack(void);

	// �����(������ ����) �� ������� ������� ������
	CCard* m_pCard[PACKNUMBERCARDS];
private:
	// ���� �������� � ������� ����� ��������
	CWnd* m_pParent;
public:
	void DrawPack(CDC* pDC);

	// ���������, ��� �������� ����� � �������� ��������������
	// ����� � ���������� ��� ����� ������ ���� ���� � ������
	void MouseDown(const CPoint& pt);

	// ����� � ������ �������� ����������� ������ ���� ���� � ������
	void CardBringToFront(int index);

	// ������������ ����� �  ���������� �������� �������������
	void ShufflePack(char* arr);

	// �� ������������� ��������� ������������ ����� � ������
	void GetShufflePack(char arr[52]);
};
