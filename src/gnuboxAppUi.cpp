//
//	gnuboxAppUi.cpp - main Gnubox AppUi cpp file.
//
//  Gnubox is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  Gnubox is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Gnubox; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//	Copyright (C) 2002 Mal Minhas.
//
//	Revision history:
//	=================
//	28/08/02:	First version of Gnubox started.

#include "gnuboxAppUi.h"
#include "gnuboxContainer.h"
#include <gnubox.rsg>
#include "gnubox.hrh"

#include <avkon.hrh>

void CGnuboxAppUi::ConstructL()
/**
	CGnuboxAppUi::ConstructL()
*/
	{
	BaseConstructL();
	// Need to set right mode after parsing CommDB.
	iMode=TConnectionMode(GetDocument()->RetrieveRecordL(_L("Mode")));

	iAppContainer = new (ELeave) CGnuboxContainer;
	iAppContainer->SetMopParent(this);
	iAppContainer->ConstructL(ClientRect(), this);
	AddToStackL(iAppContainer);
	}

CGnuboxAppUi::~CGnuboxAppUi()
/**
	CGnuboxAppUi::~CGnuboxAppUi()
	Destructor
*/
	{
	if(iAppContainer)
		{
		RemoveFromStack(iAppContainer);
		delete iAppContainer;
		}
   }

void CGnuboxAppUi::DynInitMenuPaneL(
	TInt /*aResourceId*/,CEikMenuPane* /*aMenuPane*/)
/**
	CGnuboxAppUi::DynInitMenuPaneL(TInt aResourceId,
	CEikMenuPane* aMenuPane)
	This function is called by the EIKON framework just
	before it displays a menu pane. Its default implementation
	is empty, and by overriding it, the application can set the
	state of menu items dynamically according
	to the state of application data.
*/
	{
	}

TKeyResponse CGnuboxAppUi::HandleKeyEventL(
	const TKeyEvent& /*aKeyEvent*/,TEventCode /*aType*/)
/**
	CGnuboxAppUi::HandleKeyEventL(
	 const TKeyEvent& aKeyEvent,TEventCode aType)
*/
	{
	return EKeyWasNotConsumed;
	}

void CGnuboxAppUi::HandleCommandL(TInt aCommand)
/**
	CGnuboxAppUi::HandleCommandL(TInt aCommand)
*/
	{
	switch (aCommand)
		{
	case EAknSoftkeyBack:
	case EEikCmdExit:
		Exit();
		break;
	case EGnuboxCmdAppInfraredDirect:
		if (iMode != EInfraredDirect)
			iAppContainer->SwitchModeL(EInfraredDirect);
		else
			iEikonEnv->InfoMsg(_L("Already is Direct Infrared"));
		break;
	case EGnuboxCmdAppBluetoothDirect:
		if (iMode != EBluetoothDirect)
			iAppContainer->SwitchModeL(EBluetoothDirect);
		else
			iEikonEnv->InfoMsg(_L("Already is Direct Bluetooth"));
		break;
	case EGnuboxCmdAppInfraredDialup:
		if (iMode != EInfraredDialup)
			iAppContainer->SwitchModeL(EInfraredDialup);
		else
			iEikonEnv->InfoMsg(_L("Already is Dialup Infrared"));
		break;
	case EGnuboxCmdAppBluetoothDialup:
		if (iMode != EBluetoothDialup)
			iAppContainer->SwitchModeL(EBluetoothDialup);
		else
			iEikonEnv->InfoMsg(_L("Already is Dialup Bluetooth"));
		break;
	case EGnuboxCmdAppCSD_GPRS:
		if (iMode != ECSD_GPRS)
			iAppContainer->SwitchModeL(ECSD_GPRS);
		else
			iEikonEnv->InfoMsg(_L("Already is default CSD/GPRS"));
		break;
	default:
		break;
		}
	}
