#pragma once
#include "Game3.h"


// ����� �������� ������ �� ����
class CSendData
{
public:
	CSendData(void);
	~CSendData(void);
	// �������������� ��������� �� ����
	enum {m_eNumberPlayres=1, m_eNewGame, m_eGoPlayer, m_eTake,  m_eNextPlayer} m_idMSG;

	// ������� �����
	char m_goingPlayer;

	// ���������� �������
	char m_CountPlayers;

	// �������� ����� ���������� �� ����
	char m_Value;

	// ����� ����� ���������� �� ����
	char m_Mast;

	// ������ ��� �������� �� ���� 
	// ��������� ������������� ����
	char arr[PACKNUMBERCARDS];
};
