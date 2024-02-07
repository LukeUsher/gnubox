//
//	gnuboxApp.cpp - main app .cpp file for Gnubox application
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

#include "gnuboxApp.h"
#include "gnuboxDocument.h"

TUid CGnuboxApp::AppDllUid() const
/**
	CGnuboxApp::AppDllUid - returns application UID.
*/
	{
	return KUidGnubox;
	}

CApaDocument* CGnuboxApp::CreateDocumentL()
/**
	CGnuboxApp::CreateDocumentL()
	Creates CGnuboxDocument object
*/
	{
	return CGnuboxDocument::NewL(*this);
	}

EXPORT_C CApaApplication* NewApplication()
/**
	NewApplication()
	Constructs CGnuboxApp.
	Returns: Created application object
*/
	{
	return new CGnuboxApp;
	}

GLDEF_C TInt E32Dll(TDllReason)
/**
	E32Dll(TDllReason)
	Entry point function for all Symbian OS based Apps
	Returns: KErrNone: No error
*/
	{
	return KErrNone;
	}

