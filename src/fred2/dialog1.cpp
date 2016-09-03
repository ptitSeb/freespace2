/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/FRED2/dialog1.cpp $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * key usage summary screen (actually a dialog box, but it's not very interactive)
 *
 * $Log$
 * Revision 1.3  2002/06/09 04:41:16  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/07 03:16:44  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:08  root
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
 * 2     2/17/97 5:28p Hoffoss
 * Checked RCS headers, added them were missing, changing description to
 * something better, etc where needed.
 *
 * $NoKeywords: $
 */

#include "stdafx.h"
#include "fred.h"
#include "dialog1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dialog1 dialog

dialog1::dialog1(CWnd* pParent /*=NULL*/)
	: CDialog(dialog1::IDD, pParent)
{
	//{{AFX_DATA_INIT(dialog1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void dialog1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dialog1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(dialog1, CDialog)
	//{{AFX_MSG_MAP(dialog1)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dialog1 message handlers

