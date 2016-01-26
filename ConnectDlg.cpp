// ConnectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Game3.h"
#include "ConnectDlg.h"


// CConnectDlg dialog

IMPLEMENT_DYNAMIC(CConnectDlg, CDialog)
CConnectDlg::CConnectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectDlg::IDD, pParent)
	, m_PortNumber(3000)
	, m_IP(_T("127.0.0.1"))
{
}

CConnectDlg::~CConnectDlg()
{
}

void CConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PORT, m_PortNumber);
	DDX_Text(pDX, IDC_EDIT2, m_IP);
}


BEGIN_MESSAGE_MAP(CConnectDlg, CDialog)
END_MESSAGE_MAP()


// CConnectDlg message handlers
