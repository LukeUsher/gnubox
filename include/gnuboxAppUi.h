//
// gnuboxAppUi.h
// 28th August 2002
//
// Copyright: Mal Minhas
//
// Description:
//    Main Gnubox AppUi header file
//

#ifndef GNUBOXAPPUI_H
#define GNUBOXAPPUI_H

// Includes
#include <eikapp.h>
#include <eikdoc.h>
#include <e32std.h>
#include <coeccntx.h>
#include <aknappui.h>

#include "gnuboxDocument.h"

// Forward declarations
class CGnuboxContainer;

// Constants

// Class declarations

class CGnuboxAppUi : public CAknAppUi
/**
	Application UI class.
	Provides support for the following features:
	- EIKON control architecture
*/
	{
public:  // Constructors and destructor
	/**
	   EPOC default constructor.
	*/
	void ConstructL();
	/**
	   Destructor.
	*/
	~CGnuboxAppUi();

public: // New functions
	inline TConnectionMode GetMode() {return iMode;};
	inline void SetMode(const TConnectionMode aMode) {iMode = aMode;};
	inline CGnuboxDocument *GetDocument() {return iDocument;};
	inline void SetDocument(CGnuboxDocument *aDoc) {iDocument = aDoc;};
public: // Functions from base classes

private:
	// From MEikMenuObserver
	void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane);

private:
	/**
	   From CEikAppUi, takes care of command handling.
	   @param aCommand Command to be handled.
	*/
	void HandleCommandL(TInt aCommand);
	/**
	   From CEikAppUi, handles key events.
	   @param aKeyEvent Event to handled.
	   @param aType Type of the key event.
	   @return Response code (EKeyWasConsumed, EKeyWasNotConsumed).
	*/
	virtual TKeyResponse HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
private:
	/**
	   Data - AppUi has a container which holds all the individual
	   controls in the AppUi
	*/
	CGnuboxContainer* iAppContainer;
	TConnectionMode iMode;
	CGnuboxDocument* iDocument;
	};

#endif
