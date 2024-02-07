//
// gnuboxDocument.h
// 28th August 2002
//
// Copyright: Mal Minhas
//
// Description:
//    Main Gnubox Document header file
//

#ifndef GNUBOXDOCUMENT_H
#define GNUBOXDOCUMENT_H

// Includes
#include <akndoc.h>
#include <f32file.h>
#include <s32file.h>
//#include "gnuboxappui.h"

// Constants
// The file name, extension and path for the file store
_LIT(KFullNameOfFileStore,"C:\\System\\Apps\\Gnubox\\gnubox.dat");
_LIT(KISPTelNumTxt,"DefaultTelNum");
_LIT(KDefaultTelNum,""); // empty number by default...
_LIT(KModeTxt,"Mode");
_LIT(KDirect2BoxTxt,"Direct 2box");
_LIT(KDialup2BoxTxt,"Dialup 2box");
_LIT(KIrCOMMPortNumTxt,"IrCOMM::0");
_LIT(KIrCOMMTxt,"IrCOMM");
_LIT(KBTCOMMPortNumTxt,"BTCOMM::0");
_LIT(KBTCOMMTxt,"BTCOMM");
_LIT(KDataportPortNumTxt,"DATAPORT::0");
_LIT(KDataportTxt,"DATAPORT");
_LIT(KPhoneTSYTxt,"PHONETSY");

// Forward declarations
class  CEikAppUi;
class  CGnuboxAppUi;

// Class declarations
enum TConnectionMode {ECSD_GPRS,
					  EInfraredDirect,
					  EInfraredDialup,
					  EBluetoothDirect,
					  EBluetoothDialup};

class TGnuboxIndex
	{
public :
	TStreamId StoreL(CStreamStore& aStore);
	void      RestoreL(CStreamStore& aStore, TStreamId anId);
	void      ExternalizeL(RWriteStream& aStream) const;
	void      InternalizeL(RReadStream& aStream);
public :
	TStreamId iIndex;
	};

class CGnuboxProfile : public CBase
	{
public  :
	static CGnuboxProfile* NewLC(CStreamStore& aStore);
	static CGnuboxProfile* NewLC(CStreamStore& aStore, TStreamId anId);
public  :
	CGnuboxProfile(CStreamStore& aStore);
	CGnuboxProfile(CStreamStore& aStore,TStreamId anId);
public  :
	TStreamId     StoreL();
	void          UpdateStoreL();
	void          RestoreL();
	void          ExternalizeL(RWriteStream& aStream) const;
	void          InternalizeL(RReadStream& aStream);
	void          ChangeDataL(const TDesC& aRecord,const TDesC& aValue);
	void		  ChangeDataL(const TDesC& aRecord,const TInt aValue);
	void		  RetrieveDataL(const TDesC& aRecord,TDes& aValue);
	TInt		  RetrieveDataL(const TDesC& aRecord);
private :
	void          Construct();
public :
	TBuf<32>		 iISPphonenum;
	TConnectionMode	 iMode;
	CStreamStore&	 iStore; // Store to/Restore from this store
	TStreamId        iId;    // Restore from/replace this stream
	};

class CGnuboxDocument : public CAknDocument
/**
	CGnuboxContainer  container control class.
*/
	{
public:  // Constructors and destructor
	static CGnuboxDocument* NewL(CEikApplication& aApp);
	virtual ~CGnuboxDocument();
public: // New functions
	void StoreRecordL(const TDesC& aRecord, const TDesC& aValue);
	void StoreRecordL(const TDesC& aRecord, const TInt aValue);
	void RetrieveRecordL(const TDesC& aRecord,TDes& aValue);
	TInt RetrieveRecordL(const TDesC& aRecord);
	//
//public:	// from CEikDocument
//	CFileStore* OpenFileL(TBool aDoOpen, const TDesC& aFilename, RFs& aFs);
protected:  // New functions

protected:  // Functions from base classes

private:
	/**
	   EPOC default constructor.
	*/
	CGnuboxDocument(CEikApplication& aApp);
	void ConstructL();
private:
	/**
	   From CEikDocument, create CGnuboxAppUi "App UI" object.
	*/
	CEikAppUi* CreateAppUiL();
private:
	CGnuboxAppUi* iAppUi;
	RFs iFs;
	RFile iFile;
	};

#endif

