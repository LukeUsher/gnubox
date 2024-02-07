//
//	gnuboxDocument.cpp - main Gnubox document file
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
//  09/01/03:   Added CPermanentFileStore stuff to Gnubox

#include "gnuboxDocument.h"
#include "gnuboxAppUi.h"
#include "debug.h"

CGnuboxDocument::CGnuboxDocument(CEikApplication& aApp)
	:CAknDocument(aApp)
/**
	C'tor
*/
	{
	}

CGnuboxDocument::~CGnuboxDocument()
/**
	Destructor
*/
	{
	iFs.Close();
	}

void CGnuboxDocument::ConstructL()
/**
	Default constructor can leave.
*/
	{
	FLOG(_L("**** CGnuboxDocument::ConstructL ****"));

	TInt ret=iFs.Connect();
	User::LeaveIfError(ret);

	CFileStore* store;
	TParse	filestorename;
	iFs.Parse(KFullNameOfFileStore,filestorename);

	RFile file;
	ret=file.Open(iFs,filestorename.FullName(),EFileShareAny);

	//IMPORT_C TInt Open(RFs& aFs,const TDesC& aName,TUint aFileMode);

	if (ret != 0)
		{// File store does not exist so we need to make it....
		iFs.MkDirAll(KFullNameOfFileStore);

		// Create (and replace, if it exists) the permanent file store
		store = CPermanentFileStore::ReplaceLC(iFs,filestorename.FullName(),EFileWrite);
		// Must say what kind of file store.
		store->SetTypeL(KPermanentFileStoreLayoutUid);

		// Create a TGnuboxIndex object to hold the stream ids of all
		// the component streams
		TGnuboxIndex index;
		// Construct the CGnuboxProfile object, write it to its own stream 
		// and save the stream id in the index.
		CGnuboxProfile* gProfile = CGnuboxProfile::NewLC(*store);
		index.iIndex = gProfile->StoreL();
		CleanupStack::PopAndDestroy();

		//Now write the index itself to its own stream ...
		TStreamId id  = index.StoreL(*store);
		// ... and make this stream the root stream
		store->SetRootL(id);
		// Now commit all changes to the store
		store->CommitL();
		// Destroy the permanent file store object (closes the file)
		CleanupStack::PopAndDestroy();
		}
	else
		{// File store already exists so use it....
		file.Close();
		store = CPermanentFileStore::OpenLC(iFs,filestorename.FullName(),EFileRead|EFileWrite);

		// Restore the index of streamids so that we can access (restore)
		// all the other objects. The index is found in the root stream 
		TGnuboxIndex index;
		index.RestoreL(*store,store->Root());
				
		// Restore the CGnuboxProfile object and then
		// destroy the CGnuboxProfile object.
		CGnuboxProfile* gProfile = 0;
		gProfile=CGnuboxProfile::NewLC(*store,index.iIndex);
		CleanupStack::PopAndDestroy();
	
		// Destroy the permanent file store object (closes the file)
		CleanupStack::PopAndDestroy();
		}
	}

CGnuboxDocument* CGnuboxDocument::NewL(
		CEikApplication& aApp)
/**
	Two-phased constructor.
	Note the passing through of refercence to application.
*/
	{
	FLOG(_L("**** CGnuboxDocument::NewL() ****"));
	CGnuboxDocument* self = new (ELeave) CGnuboxDocument(aApp);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

CEikAppUi* CGnuboxDocument::CreateAppUiL()
/**
	CGnuboxDocument::CreateAppUiL()
	constructs CGnuboxAppUi
*/
	{
	//CEikAppUi *appui = new (ELeave) CGnuboxAppUi;
	CGnuboxAppUi *appui = new (ELeave) CGnuboxAppUi;
	appui->SetDocument(this);
	iAppUi = appui;
	return appui;
	}

void CGnuboxDocument::StoreRecordL(const TDesC& aRecord,const TDesC& aValue)
/**
	CGnuboxDocument::StoreRecordL(const TDesC& aRecord,const TDesC& aValue)
	The record is internalised into a persistent store which is held in 
	the file c:/System/Apps/Gnubox/Gnubox.dat.  
*/
	{
	_LIT(KTxtPrint,"  ==== CGnuboxDocument::StoreRecordL %S %S ==== \n");
	FPrint(KTxtPrint,&aRecord,&aValue);

	TParse	filestorename;
	iFs.Parse(KFullNameOfFileStore,filestorename);
	CFileStore* store = CPermanentFileStore::OpenLC(iFs,filestorename.FullName(),EFileRead|EFileWrite);
	// Restore the index of streamids so that we can access (restore)
	// all the other objects (there's actually only one in TGnuboxIndex). 
	// The index is found in the root stream.
	TGnuboxIndex index;
	index.RestoreL(*store,store->Root());
				
	// Restore the CGnuboxProfile object, update with the changed value and
	// update the stream to reflect the changed content.
	// If updating the stream succeeds, commit the changes to the store
	// otherwise attempt to revert the store to its state at the last
	// commit point.  Destroy the CGnuboxProfile object.	

	CGnuboxProfile *gProfile = CGnuboxProfile::NewLC(*store,index.iIndex);
	gProfile->ChangeDataL(aRecord,aValue);
	CleanupStack::PopAndDestroy();

	// Destroy the permanent file store object (closes the file) 
	CleanupStack::PopAndDestroy();
	}

void CGnuboxDocument::StoreRecordL(const TDesC& aRecord,const TInt aValue)
/**
	CGnuboxDocument::StoreRecordL(const TDesC& aRecord,const TDesC& aValue)
	The record is internalised into a persistent store which is held in 
	the file c:/System/Apps/Gnubox/Gnubox.dat.  
*/
	{
	_LIT(KTxtPrint,"  ==== CGnuboxDocument::StoreRecordL %S %d ==== \n");
	FPrint(KTxtPrint,&aRecord,aValue);

	TParse	filestorename;
	iFs.Parse(KFullNameOfFileStore,filestorename);
	CFileStore* store = CPermanentFileStore::OpenLC(iFs,filestorename.FullName(),EFileRead|EFileWrite);
	// Restore the index of streamids so that we can access (restore)
	// all the other objects (there's actually only one in TGnuboxIndex). 
	// The index is found in the root stream.
	TGnuboxIndex index;
	index.RestoreL(*store,store->Root());
				
	// Restore the CGnuboxProfile object, update with the changed value and
	// update the stream to reflect the changed content.
	// If updating the stream succeeds, commit the changes to the store
	// otherwise attempt to revert the store to its state at the last
	// commit point.  Destroy the CGnuboxProfile object.	

	CGnuboxProfile *gProfile = CGnuboxProfile::NewLC(*store,index.iIndex);
	gProfile->ChangeDataL(aRecord,aValue);
	CleanupStack::PopAndDestroy();

	// Destroy the permanent file store object (closes the file) 
	CleanupStack::PopAndDestroy();
	}

void CGnuboxDocument::RetrieveRecordL(const TDesC& aRecord,TDes& aValue)
/**
*/
	{
	_LIT(KTxtPrint,"  ==== CGnuboxDocument::RetrieveRecordL %S ==== \n");
	FPrint(KTxtPrint,&aRecord);

	TParse	filestorename;
	iFs.Parse(KFullNameOfFileStore,filestorename);
	CFileStore* store = CPermanentFileStore::OpenLC(iFs,filestorename.FullName(),EFileRead);
	// Restore the index of streamids so that we can access (restore)
	// all the other objects (there's actually only one in TGnuboxIndex).
	// The index is found in the root stream.
	TGnuboxIndex index;
	index.RestoreL(*store,store->Root());
	
	// Restore the CGnuboxProfile object, fill the Record, 
	// then destroy the object
	CGnuboxProfile *gProfile = CGnuboxProfile::NewLC(*store,index.iIndex);
	gProfile->RetrieveDataL(aRecord,aValue);
	CleanupStack::PopAndDestroy();

	// Destroy the permanent file store object (closes the file) 
	CleanupStack::PopAndDestroy();
	}

TInt CGnuboxDocument::RetrieveRecordL(const TDesC& aRecord)
/**
*/
	{
	_LIT(KTxtPrint,"  ==== CGnuboxDocument::RetrieveRecordL %S ==== \n");
	FPrint(KTxtPrint,&aRecord);

	TParse	filestorename;
	iFs.Parse(KFullNameOfFileStore,filestorename);
	CFileStore* store = CPermanentFileStore::OpenLC(iFs,filestorename.FullName(),EFileRead);
	// Restore the index of streamids so that we can access (restore)
	// all the other objects (there's actually only one in TGnuboxIndex).
	// The index is found in the root stream.
	TGnuboxIndex index;
	index.RestoreL(*store,store->Root());
	
	// Restore the CGnuboxProfile object, fill the Record, 
	// then destroy the object
	CGnuboxProfile *gProfile = CGnuboxProfile::NewLC(*store,index.iIndex);
	TInt retvalue=gProfile->RetrieveDataL(aRecord);
	CleanupStack::PopAndDestroy();

	// Destroy the permanent file store object (closes the file) 
	CleanupStack::PopAndDestroy();
	return retvalue;
	}

//**********************************************************************************************

CGnuboxProfile::CGnuboxProfile(CStreamStore& aStore)
	: iStore(aStore)
	{}

CGnuboxProfile::CGnuboxProfile(CStreamStore& aStore,TStreamId anId)
	: iStore(aStore), iId(anId)
	{}

CGnuboxProfile* CGnuboxProfile::NewLC(CStreamStore& aStore)
	{
	CGnuboxProfile* self = new (ELeave) CGnuboxProfile(aStore);
	CleanupStack::PushL(self);
	self->Construct();
	return self;
	}

CGnuboxProfile* CGnuboxProfile::NewLC(CStreamStore& aStore, TStreamId anId)
	{
	CGnuboxProfile* self = new (ELeave) CGnuboxProfile(aStore,anId);
	CleanupStack::PushL(self);
	self->RestoreL();
	return self;
	}

void CGnuboxProfile::Construct()
	{
	iISPphonenum = KDefaultTelNum;   // set it to this
	iMode = ECSD_GPRS;				 // set it to this
	}

void CGnuboxProfile::ChangeDataL(const TDesC& aRecord,const TDesC& aValue)
/**	
*/
	{
	if (aRecord == KISPTelNumTxt)
		iISPphonenum = aValue;
	//else if (aRecord == KDescriptionTxt)
	//	iDescription = aValue;
	//else if (aRecord == KPortNameTxt)
	//	iPortName = aValue;
	//else if (aRecord == KCSYNameTxt)
	//	iCSYName = aValue;
	else
		return;

	TRAPD(error,UpdateStoreL());
	if (error!=KErrNone)
		{
		iStore.Revert();
		User::Leave(error);
		}
	}

void CGnuboxProfile::ChangeDataL(const TDesC& aRecord,const TInt aValue)
/**	
*/
	{
	if (aRecord == KModeTxt)
		iMode = TConnectionMode(aValue);
	else
		return;

	TRAPD(error,UpdateStoreL());
	if (error!=KErrNone)
		{
		iStore.Revert();
		User::Leave(error);
		}
	}

void CGnuboxProfile::RetrieveDataL(const TDesC& aRecord,TDes& aValue)
	{
	if (aRecord == KISPTelNumTxt)
		aValue = iISPphonenum;
	//else if (aRecord == KPortNameTxt)
	//	aValue = iPortName;
	//else if (aRecord == KCSYNameTxt)
	//	aValue = iCSYName;
	else
		return;
	}
	
TInt CGnuboxProfile::RetrieveDataL(const TDesC& aRecord)
	{
	if (aRecord == KModeTxt)
		return TInt(iMode);
	else
		return -1;
	}
	
void CGnuboxProfile::RestoreL()
	{
	RStoreReadStream instream;
	instream.OpenLC(iStore,iId);
	InternalizeL(instream);			// or we can say instream >> *this;
	CleanupStack::PopAndDestroy();	
	}

TStreamId CGnuboxProfile::StoreL()
	{
	RStoreWriteStream outstream;
	TStreamId id = outstream.CreateLC(iStore);
	ExternalizeL(outstream);		// or we can say outstream << *this;
	outstream.CommitL();
	CleanupStack::PopAndDestroy();	
	return id;
	}

void CGnuboxProfile::UpdateStoreL()
	{
	RStoreWriteStream outstream;
	outstream.ReplaceLC(iStore,iId);
	ExternalizeL(outstream);		// or we can say outstream << *this;
	outstream.CommitL();
	CleanupStack::PopAndDestroy();
	iStore.CommitL();// commit the changes to the store
	}

void CGnuboxProfile::ExternalizeL(RWriteStream& aStream) const
	{
	aStream.WriteInt32L(TInt(iMode));
	aStream << iISPphonenum;
	}  
 
void CGnuboxProfile::InternalizeL(RReadStream& aStream)
	{
	iMode  = TConnectionMode(aStream.ReadInt32L());
	aStream >> iISPphonenum;
	}  

//**********************************************************************************************

TStreamId TGnuboxIndex::StoreL(CStreamStore& aStore)
	{
	RStoreWriteStream outstream;
	TStreamId id = outstream.CreateLC(aStore);
	ExternalizeL(outstream);		// or we can say outstream << *this;
	outstream.CommitL();
	CleanupStack::PopAndDestroy();	
	return id;
	}

void TGnuboxIndex::RestoreL(CStreamStore& aStore, TStreamId anId)
	{
	RStoreReadStream instream;
	instream.OpenLC(aStore,anId);
	InternalizeL(instream);
	CleanupStack::PopAndDestroy();	
	}

void TGnuboxIndex::ExternalizeL(RWriteStream& aStream) const
	{
	aStream << iIndex;
	}  

void TGnuboxIndex::InternalizeL(RReadStream& aStream)
	{
	aStream >> iIndex;
	}  

