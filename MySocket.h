#pragma once

// класс сокетов для упрощения работы по сети
// наследуется от класса асинхронных сокетов CAsyncSocket (MFC)

// CMySocket command target

class CMySocket : public  CAsyncSocket //CSocket
{
public:
	CMySocket();
	virtual ~CMySocket();

	// сообщение об установленном контакте
	// происходит на стороне клиента
	virtual void OnConnect(int nErrorCode);

	// событие закрытия соединения
	// происходит на обоих сторонах
	virtual void OnClose(int nErrorCode);

	// извлечение данных по сети
	virtual void OnReceive(int nErrorCode);

	// принятие клиентов сервером для работы по сети
	virtual void OnAccept(int nErrorCode);

	// установка родителького окна
	void SetParent(CWnd* pParent);
private:
	CWnd* m_pParent;
};


