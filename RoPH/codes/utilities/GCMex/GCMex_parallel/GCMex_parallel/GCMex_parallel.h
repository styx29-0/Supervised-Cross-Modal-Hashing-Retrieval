// GCMex_parallel.h : main header file for the GCMex_parallel DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CGCMex_parallelApp
// See GCMex_parallel.cpp for the implementation of this class
//

class CGCMex_parallelApp : public CWinApp
{
public:
	CGCMex_parallelApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
