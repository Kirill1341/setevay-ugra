#pragma once

// ����� ������� ��� ��������� ������ �� ����
// ����������� �� ������ ����������� ������� CAsyncSocket (MFC)

// CMySocket command target

class CMySocket : public  CAsyncSocket //CSocket
{
public:
	CMySocket();
	virtual ~CMySocket();

	// ��������� �� ������������� ��������
	// ���������� �� ������� �������
	virtual void OnConnect(int nErrorCode);

	// ������� �������� ����������
	// ���������� �� ����� ��������
	virtual void OnClose(int nErrorCode);

	// ���������� ������ �� ����
	virtual void OnReceive(int nErrorCode);

	// �������� �������� �������� ��� ������ �� ����
	virtual void OnAccept(int nErrorCode);

	// ��������� ������������ ����
	void SetParent(CWnd* pParent);
private:
	CWnd* m_pParent;
};


