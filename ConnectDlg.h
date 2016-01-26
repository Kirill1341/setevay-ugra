
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//							http://www.interestprograms.ru 
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once


// CConnectDlg dialog

class CConnectDlg : public CDialog
{
	DECLARE_DYNAMIC(CConnectDlg)

public:
	CConnectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConnectDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_PortNumber;
	CString m_IP;
};
