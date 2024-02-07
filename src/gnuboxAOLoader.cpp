//
//	gnuboxAOLoader.cpp - Gnubox Active Object Loader cpp file.
//	The "AO Loader" is used to ensure that loading of CommDB
//	doesn't block screen drawing.
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

#include "gnuboxContainer.h"
#include "debug.h"

//#include <eiklabel.h>  // for example label control
//#include <coemain.h>   // for iCoeEnv definition
//#include <coecntrl.h>  // for ControlEnv() inline

CGnuboxAOLoader::CGnuboxAOLoader() : CActive(EPriorityStandard)
/**
	CGnuboxAOLoader::CGnuboxAOLoader()
	Default constructor.
*/
	{
	}

CGnuboxAOLoader *CGnuboxAOLoader::NewL(CGnuboxContainer *aParent)
/**
	CGnuboxAOLoader *CGnuboxAOLoader::NewL(CGnuboxContainer *aParent)
*/
	{
	CGnuboxAOLoader *c=new (ELeave) CGnuboxAOLoader;
	CleanupStack::PushL(c);
	c->InitL(aParent);
	CleanupStack::Pop();
	CActiveScheduler::Add(c);
	return c;
	}

void CGnuboxAOLoader::InitL(CGnuboxContainer *aParent)
/**
	CGnuboxAOLoader::InitL(CGnuboxContainer *aParent)
*/
	{
	iParent = aParent;
	}

CGnuboxAOLoader::~CGnuboxAOLoader()
/**
	CGnuboxAOLoader::~CGnuboxAOLoader
*/
	{
	if(IsActive())
		Cancel();
	}

void CGnuboxAOLoader::Fire()
/**
	CGnuboxAOLoader::Fire()
	CGnuboxAOLoader operates like a one-shot.  Could have used
	a CAsyncOneShot here instead but this at least illustrates
	how to do one using an active object for yielding control
	within a thread (ie. cooperative multitasking).
*/
	{
	FLOG(_L("CGnuboxAOLoader::Fire - invoke the one shot active object"));

	TRequestStatus *pStatRef=&iStatus;
	//iParent->InfoMsg(_L("Start Me Up"),EInfoMsgTopLeft);
	SetActive();
	RThread().RequestComplete(pStatRef,KErrNone);
	}

void CGnuboxAOLoader::RunL()
/**
	CGnuboxAOLoader::RunL()
*/
	{
	FLOG(_L("CGnuboxAOLoader::RunL - start loading CommDB"));
	User::LeaveIfError(iStatus.Int());
	iParent->LoadCommDbSettingsL();
	}

void CGnuboxAOLoader::DoCancel()
/**
	CGnuboxAOLoader::DoCancel()
*/
	{
	}
