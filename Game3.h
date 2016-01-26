// Game3.h : main header file for the Game3 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// определим необходимые константы 
const int CARDSWIDTH		= 71;
const int CARDSHEIGHT		= 96;
const int PACKNUMBERCARDS	= 52;


// CGame3App:
// See Game3.cpp for the implementation of this class
//

class CGame3App : public CWinApp
{
public:
	CGame3App();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGame3App theApp;