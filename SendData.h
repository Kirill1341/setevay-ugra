#pragma once
#include "Game3.h"


// класс отправки данных по сети
class CSendData
{
public:
	CSendData(void);
	~CSendData(void);
	// идентификаторы сообщений по сети
	enum {m_eNumberPlayres=1, m_eNewGame, m_eGoPlayer, m_eTake,  m_eNextPlayer} m_idMSG;

	// ходящий игрок
	char m_goingPlayer;

	// количество игроков
	char m_CountPlayers;

	// значение карты переданное по сети
	char m_Value;

	// масть карты переданное по сети
	char m_Mast;

	// массив для передачи по сети 
	// алгоритма перемешивания карт
	char arr[PACKNUMBERCARDS];
};
