/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/FRED2/OperatorArgTypeSelect.cpp $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Dialog box handling code for selecting an argument return type of an SEXP.
 * Changes to SEXPs made this no longer needed, but just in case more changes
 * cause it to be needed again, it's still around.
 *
 * $Log$
 * Revision 1.3  2002/06/09 04:41:16  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/07 03:16:44  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:09  root
 * Initial import.
 *
 * 
 * 2     10/07/98 6:28p Dave
 * Initial checkin. Renamed all relevant stuff to be Fred2 instead of
 * Fred. Globalized mission and campaign file extensions. Removed Silent
 * Threat specific code.
 * 
 * 1     10/07/98 3:01p Dave
 * 
 * 1     10/07/98 3:00p Dave
 * 
 * 3     2/17/97 5:28p Hoffoss
 * Checked RCS headers, added them were missing, changing description to
 * something better, etc where needed.
 *
 * $NoKeywords: $
 */

#include "stdafx.h"
#include "fred.h"
#include "operatorargtypeselect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OperatorArgTypeSelect dialog


OperatorArgTypeSelect::OperatorArgTypeSelect(CWnd* pParent /*=NULL*/)
	: CDialog(OperatorArgTypeSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(OperatorArgTypeSelect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void OperatorArgTypeSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OperatorArgTypeSelect)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(OperatorArgTypeSelect, CDialog)
	//{{AFX_MSG_MAP(OperatorArgTypeSelect)
	ON_BN_CLICKED(ID_BOOLEAN, OnBoolean)
	ON_BN_CLICKED(ID_NUMBERS, OnNumbers)
	ON_BN_CLICKED(ID_SHIPS, OnShips)
	ON_BN_CLICKED(ID_WINGS, OnWings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OperatorArgTypeSelect message handlers

void OperatorArgTypeSelect::OnBoolean() 
{
	EndDialog(ID_BOOLEAN);
}

void OperatorArgTypeSelect::OnNumbers() 
{
	EndDialog(ID_NUMBERS);
}

void OperatorArgTypeSelect::OnShips() 
{
	EndDialog(ID_SHIPS);
}

void OperatorArgTypeSelect::OnWings() 
{
	EndDialog(ID_WINGS);
}

