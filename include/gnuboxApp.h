//
// gnuboxApp.h
// 28th August 2002
//
// Copyright: Mal Minhas
//
// Description:
//    Main Gnubox App header file
//

#ifndef GNUBOXAPP_H
#define GNUBOXAPP_H

// Includes
#include <aknapp.h>

// Constants

// UID of the application
const TUid KUidGnubox = { 0x097764f0 };

// Class declarations

class CGnuboxApp : public CAknApplication
/**
	CGnuboxApp application class.
	Provides factory to create concrete document object.
*/
	{
public: // Functions from base classes
private:
	/**
	   From CApaApplication, creates CGnuboxDocument document object.
	   @return A pointer to the created document object.
	*/
	CApaDocument* CreateDocumentL();
	/**
	   From CApaApplication, returns application's UID (KUidGnubox).
	   @return The value of KUidGnubox.
	*/
	TUid AppDllUid() const;
	};

#endif

