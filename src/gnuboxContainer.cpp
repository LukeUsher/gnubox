//
//	gnuboxContainer.cpp - Gnubox main AppUi container cpp file.
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

#include <eiklabel.h>  // for example label control
#include <coemain.h>   // for iCoeEnv definition
#include <coecntrl.h>  // for ControlEnv() inline
#include <btextnotifiers.h> // for Bluetooth notifier
#include <btregistry.h>  // for accessing BT registry
#include <btmanclient.h>

#ifdef __WINS__
_LIT(KISPName, "NT RAS with stored password");
_LIT(KModemName, "Null Modem 115200bps");
#else
#  ifdef __S60V2__
_LIT(KISPName, "Bt");
_LIT(KModemName, "CSD Modem");
#  else
_LIT(KISPName, "Bt");
_LIT(KModemName, "");
#  endif
#endif

/**
	Information about CommDb
	========================
	CommDb provides system-wide storage for communications-related
	settings. It holds information about Internet Access Providers
	(IAPs), Internet Service Providers (ISPs), GPRS, modems, locations,
	charge-cards, proxies and WAP settings

	CommDb stores the settings in a series of tables in a
	relational database, using the DBMS architecture.
	DBMS is a client/server architecture and provides for safe
	access by multiple clients.

	The CommDb API has two key concepts:
	<b>communications database</b>, and <b>table view</b>.

	The communications database interface allows clients to access
	the database, open its tables, and perform transactions. It is
	provided by CCommsDatabase and its base class CCommsDatabaseBase.

	Most database settings are read and written to by opening the
	appropriate table. Table access is provided by CCommsDbTableView.

	It is possible for several instances of most of the tables
	(e.g. where multiple IAPs have been set up) to exist in the database.
	The Connected preferences table sets a ranking by which such records
	are used. That table is accessed through its own view
	class CCommsDbConnectionPrefTableView.

	A view is simply a subset of the records (or rows) within a table,
	selected according to defined criteria. Tables, records, rows, columns,
	and so on, are DBMS concepts.

	Note that you can open two kinds of CCommsDatabase in 6.0 - EDatabaseTypeIAP
	and EDatabaseTypeISP.  The latter is deprecated in 6.1.

	For a database of type EDatabaseTypeIAP,
	tables are arranged as follows (from /commdb/scdb/cdbdata.cpp):

	LOCAL_D TTable const KTableList[]=
		{
		{IAP, KIAPTable},
		{DIAL_OUT_ISP, KDialOutISPTable},
		{DIAL_IN_ISP, KDialInISPTable},
		{WAP_ACCESS_POINT, KWAPAccessPointTable},
		{WAP_IP_BEARER, KWAPIPBearerTable},
		{WAP_SMS_BEARER, KWAPSMSBearerTable},
		{OUTGOING_GPRS, KGPRSTable},
		{INCOMING_GPRS, KGPRSTable},
		{MODEM, KModemTable},
		{LOCATION, KLocationTable},
		{CHARGECARD, KChargecardTable},
		{PROXIES, KProxiesTable},
		{CONNECTION_PREFERENCES, KConnectionPreferencesTable},
		{AGENT, KAgentTable},
		{GLOBAL_SETTINGS, KGlobalSettings},
		{BT_DEVICE_TABLE, KCBTDeviceTable},
		{BT_DEFAULT_TABLE, KCBTDefaultServiceTable},
		{DEFAULT_GPRS, KDefaultGPRSTable},
		{BT_SECURITY_TABLE, KCBTSecurityTable},
		{ACCESS_TYPE_TABLE, KAccessTypeTable},
		{0}
		};

	Note that the various defines here are in /commdb/inc/cdbcols.h:

	// tables...
	#define IAP												_S("IAP")
	#define DIAL_OUT_ISP									_S("DialOutISP")
	#define MODEM											_S("Modem")
	...

	// DIAL_OUT_ISP table columns
	#define ISP_DESCRIPTION									_S("Description")					// text field for app to use to describe ISP
	#define ISP_TYPE										_S("Type")
	#define ISP_DEFAULT_TEL_NUM								_S("DefaultTelNum")
	...

	// modem table columns
	#define MODEM_PORT_NAME									_S("PortName")
	#define MODEM_TSY_NAME									_S("TSYName")
	#define MODEM_CSY_NAME									_S("CSYName")
	#define MODEM_HANDSHAKING								_S("Handshaking")
	...

	// Bluetooth table columns

	#define BT_DEV_ADD				_S("DeviceAddress")
	#define BT_DEV_DFLT_NAME		_S("DeviceDefaultName")
	#define BT_DEV_FRIENDLY_NAME	_S("DeviceFriendlyName")
	#define BT_DEVICE_CLASS			_S("DeviceClass")
	#define BT_LINKKEY_SIZE			_S("DeviceLinkKeySize")
	#define BT_LINKKEY				_S("DeviceLinkKey")
	#define BT_DEV_STATE			_S("DeviceStatus")
	#define BT_DEVICE_GLOBAL_SECURITY _S("DeviceGlobalSecurity")
	//
	#define BT_DFLT_SER_NAME		_S("DefaultServicename")
	#define BT_DFLT_SER_UUID		_S("DefaultServiceUID")
	#define BT_DFLT_DEV_ADD			_S("DefaultDeviceAddress")
	#define BT_SEC_UID				_S("BTSecurityUID")
	#define BT_SEC_DEVICE_SECURITY	_S("BTSecurityDevice")
	#define BT_DFLT_VIRTUAL_PORT    _S("DefaultVirtualPort")
	#define BT_DFLT_SECURITY_LEVEL  _S("DefaultSecurityLevel")

	Tables we are interested in are DIAL_OUT_ISP () and MODEM ().
	From (from /commdb/scdb/cdbdata.cpp):

	LOCAL_D TColumn const KDialOutISPTable[]=
		{
		{ISP_DESCRIPTION,EDbColText,0},
		...
		{ISP_DEFAULT_TEL_NUM,EDbColText,0},
		...
		{ISP_TYPE,EDbColUint32,0}
		...
		{ISP_IF_NAME,EDbColText,TDbCol::ENotNull},
		...
		};

	LOCAL_D TColumn const KModemTable[]=
		{
		...
		{MODEM_PORT_NAME,EDbColText,0},
		{MODEM_TSY_NAME,EDbColText,0},
		{MODEM_CSY_NAME,EDbColText,0},
		...
		{MODEM_HANDSHAKING,EDbColUint32,0},
		...
		};

	LOCAL_D TColumn const KCBTDeviceTable[]=
  		{
	  	...
  		{BT_DEV_ADD,EDbColText8,TDbCol::ENotNull},
  		{BT_DEV_DFLT_NAME,EDbColLongText16,0},
  		{BT_DEV_FRIENDLY_NAME,EDbColLongText16,0},
  		{BT_DEVICE_CLASS,EDbColUint32,0},
  		{BT_LINKKEY,EDbColText8,0},
		{BT_DEVICE_GLOBAL_SECURITY, EDbColUint32,0},
		{BT_DEV_STATE, EDbColUint32, TDbCol::ENotNull},
  		...
  		};

	LOCAL_D TColumn const KCBTDefaultServiceTable[]=
  		{
	  	...	  	{BT_DFLT_SER_NAME,EDbColText16,0},
	  	{BT_DFLT_SER_UUID,EDbColText8,TDbCol::ENotNull},
  		{BT_DFLT_VIRTUAL_PORT,EDbColUint32,TDbCol::ENotNull},
	  	{BT_DFLT_DEV_ADD,EDbColText8,TDbCol::ENotNull},
		{BT_DFLT_SECURITY_LEVEL,EDbColUint8,0},
		...
		};

	LOCAL_D TColumn const KCBTSecurityTable[]=
  		{
		...
		{BT_DEV_ADD,EDbColText8,TDbCol::ENotNull},
	  	{BT_SEC_DEVICE_SECURITY,EDbColUint32,0},
	  	{BT_SEC_UID,EDbColUint32,0},
	  	...
	  	};

  	Note that you don't actually directly manipulate the BT fields in CommDB.
	The BTRegistry is your friend in this regard.  It acts as a facade over
	the CommDB.  Fine sentiment.  Now all we need is someone to do the same thing
	for the networking bits ;-)

	@TODO: Add more comments here on use cases for each option and IOP/testing.
*/

void CGnuboxContainer::ConstructL(const TRect& aRect, CGnuboxAppUi* aParent)
/**
	CGnuboxContainer::ConstructL(const TRect& aRect)
	EPOC two phased constructor.
	Initialise to default settings.
*/
	{
	FLOG(_L("CGnuboxContainer::ConstructL"));
	CreateWindowL();

	iParent = aParent;

	// This will add further text but it might take time so
	// we've hived it off to a separate active object...
	iLoader = CGnuboxAOLoader::NewL(this);

	iLabel = new (ELeave) CEikLabel;
	iLabel->SetContainerWindowL(*this);

	// Set up the screen text with whatever the default bearer is...
	SetAndDrawScreenTextL(iParent->GetMode());  // MALM

	SetRect(aRect);

	ActivateL();
	}

CGnuboxContainer::~CGnuboxContainer()
/**
	Destructor
*/
	{
	FLOG(_L("CGnuboxContainer::~CGnuboxContainer"));
	delete iLabel;
	iTextLines.Close();
	delete iLoader;
	}

void CGnuboxContainer::SizeChanged()
/**
	CGnuboxContainer::SizeChanged()
	Called by framework when the view size is changed.
	@TODO: Add control resize code etc here needed to resize label properly
*/
	{
	FLOG(_L("CGnuboxContainer::SizeChanged"));
	iLabel->SetExtent(TPoint(10,5),iLabel->MinimumSize());
	}

TInt CGnuboxContainer::CountComponentControls() const
/**
	CGnuboxContainer::CountComponentControls() const
*/
	{
	return 1; // return nbr of controls inside this container
	}

CCoeControl* CGnuboxContainer::ComponentControl(TInt aIndex) const
/**
	CGnuboxContainer::ComponentControl(TInt aIndex) const
*/
	{
	switch ( aIndex )
		{
		case 0:
			return iLabel;
		default:
			return NULL;
		}
	}

void CGnuboxContainer::Draw(const TRect& aRect) const
/**
	CGnuboxContainer::Draw(const TRect& aRect) const
	Note that this redraws itself in different ways
	depending on what the current bearer is.
*/
	{
	FLOG(_L("CGnuboxContainer::Draw"));
	DrawControls(aRect);
	}

void CGnuboxContainer::HandleControlEventL(
	CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
/**
	CGnuboxContainer::HandleControlEventL(
	  CCoeControl* aControl,TCoeEvent aEventType)
	We are not interested in handling control events in
	this program (?).
*/
	{
	// TODO: Add your control event handler code here
	FLOG(_L("CGnuboxContainer::HandleControlEventL"));
	}

void CGnuboxContainer::SwitchModeL(TConnectionMode aMode)
/**
	CGnuboxContainer::SwitchModeL(TConnectionMode aMode)
	Invoked whenever the user has decided to switch from
	one underlying connection mode to another.
	Currently there are five options:
	1) Direct PPP connection over Infrared
	2) Direct PPP connection over Bluetooth
	3) Dialup PPP connection over Infrared
	4) Dialup PPP connection over Bluetooth
	5) Default PPP over CSD/GPRS phone stack
*/
	{
	switch (aMode)
		{
	case EInfraredDirect:
		// First change the relevant CommDB settings
		iEikonEnv->InfoMsg(_L("Infrared Access Point"));
		break;
	case EInfraredDialup:
		// First change the relevant CommDB settings
		iEikonEnv->InfoMsg(_L("Infrared Phone"));
		break;
	case EBluetoothDirect:
		// First change the relevant CommDB settings
		iEikonEnv->InfoMsg(_L("Bluetooth LANP"));
		break;
	case EBluetoothDialup:
		// First change the relevant CommDB settings
		iEikonEnv->InfoMsg(_L("Bluetooth DUNP"));
		break;
	default:
		// First change the relevant CommDB settings
		iEikonEnv->InfoMsg(_L("1 box smartphone"));
		break;
		}
	TBool okToUpdate=UpdateCommDbL(aMode);
	if (okToUpdate)
		{// we are ok to switch mode
		iParent->SetMode(aMode);
		
		////Don't do the StoreRecord here - do it in Loader RunL

		// Then change the container control text strings to
		// ensure their correct updating on redraw
		SetAndDrawScreenTextL(aMode);
		}
	}

void CGnuboxContainer::DrawControls(const TRect& aRect) const
/**
	CGnuboxContainer::DrawControls() const
	Main point at which AppUi controls are drawn
	Note that for 7650 width=176 and height=144
	@TODO: Need to scale the drawing of controls.
*/
	{
	CWindowGc& gc = SystemGc();
	gc.SetPenStyle(CGraphicsContext::ENullPen);
	switch (iParent->GetMode())
		{
	case EBluetoothDirect:
	case EBluetoothDialup:
		{
		TRgb bluish(210,210,255); // appears as pale bluish color
		gc.SetBrushColor(bluish);
		break;
		}
	case EInfraredDirect:
	case EInfraredDialup:
		{
		TRgb reddish(255,200,200); // appears as pale reddish color
		gc.SetBrushColor(reddish);
		break;
		}
	default:
		gc.SetBrushColor(KRgbWhite);
		break;
		}
	gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
	gc.DrawRect(aRect);
	// Now we can draw screen elements
	//TPoint startpoint(0,iH*KHeightScalingFactor*3/5-40);
	TPoint startpoint(KXOffset,20);
	//TPoint endpoint(iW*KWidthScalingFactor,iH*KHeightScalingFactor*3/5-40);
	TPoint endpoint((KWidth-KXOffset),20);
	gc.SetPenStyle(CGraphicsContext::ESolidPen);
	TSize penSizeThicker(1,1);
	gc.SetPenSize(penSizeThicker);
	gc.SetPenColor(KRgbBlack);
	gc.DrawLine(startpoint,endpoint);

	//TFontSpec fs(_L("Arial"),20*7); // 9pt Arial
	TFontSpec fs(_L("Arial"),20*4); // 8pt Arial
	//TFontSpec fs(_L("Arial"),20*11); // 11pt Arial
	//TFontSpec fs(_L("Arial"),20*24); // 24pt Arial
	fs.iFontStyle=
	  TFontStyle(EPostureItalic,EStrokeWeightNormal,EPrintPosNormal);
	CFont* fontUsed=ControlEnv()->CreateScreenFontL(fs);
	gc.UseFont(fontUsed);

	TRect textRect(KXOffset,23,(KWidth-KXOffset),KHeight);
	gc.DrawRect(textRect); // just to see our limits...

	// Now draw the lines of text - you must do this per redraw
	for(TInt count = 0; count < iTextLines.Count(); count++)
		gc.DrawText(iTextLines[count],TPoint(KTextOffset,35+(count*KLineGap)));

	gc.DiscardFont();
	ControlEnv()->ReleaseScreenFont(fontUsed);
	//
	gc.SetBrushStyle(CGraphicsContext::ENullBrush);
	}

void CGnuboxContainer::SetAndDrawScreenTextL(const TConnectionMode aMode)
/**
	CGnubox::SetAndDrawScreenTextL(TConnectionMode aMode)
	This function does two things:
	1) Changes the main label text
	2) Updates screen with new CommDb settings
	To do the latter, it invokes the iLoader active object.
*/
	{
	FLOG(_L("CGnuboxContainer::SetScreenTextL"));
	TBuf<22> txtBuf;
	txtBuf.FillZ();
	switch (aMode)
		{
	case EInfraredDirect:
		txtBuf=KDirectOverIRTxt;
		txtBuf.SetMax();

		iLabel->SetTextL(txtBuf);
		iTextLines.Close();
		User::LeaveIfError(iTextLines.Append(_L("Loading CommDb")));
		User::LeaveIfError(iTextLines.Append(_L("Infrared settings...")));
		iLoader->Fire();
		break;
	case EInfraredDialup:
		txtBuf=KDialupOverIRTxt;
		txtBuf.SetMax();
	
		iLabel->SetTextL(txtBuf);
		iTextLines.Close();
		User::LeaveIfError(iTextLines.Append(_L("Loading CommDb")));
		User::LeaveIfError(iTextLines.Append(_L("Infrared settings...")));
		iLoader->Fire();
		break;
	case EBluetoothDirect:
		txtBuf=KDirectOverBTTxt;
		txtBuf.SetMax();
	
		iLabel->SetTextL(txtBuf);
		iTextLines.Close();
		User::LeaveIfError(iTextLines.Append(_L("Loading CommDb")));
		User::LeaveIfError(iTextLines.Append(_L("Bluetooth settings...")));
		iLoader->Fire();
		break;
	case EBluetoothDialup:
		txtBuf=KDialupOverBTTxt;
		txtBuf.SetMax();

		iLabel->SetTextL(txtBuf);
		iTextLines.Close();
		User::LeaveIfError(iTextLines.Append(_L("Loading CommDb")));
		User::LeaveIfError(iTextLines.Append(_L("Bluetooth settings...")));
		iLoader->Fire();
		break;
	default:
		txtBuf=KPPPoverCSDGPRSTxt;
		txtBuf.SetMax();
	
		iLabel->SetTextL(txtBuf);
		iTextLines.Close();
		User::LeaveIfError(iTextLines.Append(_L("Loading CommDb")));
		User::LeaveIfError(iTextLines.Append(_L("CSD/GPRS settings...")));
		iLoader->Fire();
		break;
		}
	}

void CGnuboxContainer::ReadTextFieldL(CCommsDbTableView* aTable, const TDesC& aRecord)
/**
	CGnuboxContainer::ReadTextFieldL(CCommsDbTableView* aTable, const TDesC& aRecord)
	Reads a particular field and appends it to iTextLines.
	@TODO: Need to add scrolling text window support - migrate to CTextView?
*/
	{
	TBuf<KCommsDbSvrMaxFieldLength> printBuffer;
	TBuf<KCommsDbSvrMaxFieldLength-10> recBuffer;

	printBuffer=_L(" ");
	printBuffer.Append(aRecord);
	printBuffer.Append(_L(": "));
	aTable->ReadTextL(aRecord,recBuffer);

	// Need to store the value in dummyBuffer too...
	//If the array is already at the maximum size, delete the first line of text
	if (iTextLines.Count() >= KMaxTextLines)
		{
		iTextLines.Remove(0);
		}
	if (printBuffer.Length() + recBuffer.Length() > KMaxLineLength)
		{
		iTextLines.Append(printBuffer);
		TBuf<KCommsDbSvrMaxFieldLength> tempBuffer=_L("  ");
		tempBuffer.Append(recBuffer);
		iTextLines.Append(tempBuffer);
		}
	else
		{
		printBuffer.Append(recBuffer);
		iTextLines.Append(printBuffer);
		}

	if ((iParent->GetMode() == ECSD_GPRS))
		{// Need to store these away because they are important
		// @TODO: Only do the following if it is different from current record...
		TBuf<KCommsDbSvrMaxFieldLength-10> currentBuffer;
		iParent->GetDocument()->RetrieveRecordL(aRecord,currentBuffer);
		if (currentBuffer != recBuffer)
			iParent->GetDocument()->StoreRecordL(aRecord,recBuffer);
		}
	}

void CGnuboxContainer::LoadCommDbSettingsL()
/**
	CGnuboxContainer::LoadCommDbSettingsL()
	This function loads the relevant CommDb settings

	Note that we store a couple of tables.
	Namely the MODEM table and the DIAL_OUT_ISP table.
	@TODO: Need to be pulling out the "Nokia CSD" IAP settings
	@TODO: There is a bug here relating to DIAL_OUT_ISP - what if we're GPRS??!!
*/
	{
	FLOG(_L("**** CGnuboxContainer::LoadCommDbSettingsL ****"));

	// UPDATE THE CURRENT MODE IN - need to do this to pick up changes
	TConnectionMode mode=iParent->GetMode();
	iParent->GetDocument()->StoreRecordL(KModeTxt,TInt(mode));

	// CREATE NEW COMMDB HANDLE
	if (iCommDb) 
		User::Leave(KErrInUse);

	iCommDb=CCommsDatabase::NewL(EDatabaseTypeIAP);
	iCommDb->ShowHiddenRecords();

	CleanupStack::PushL(iCommDb);

	// DIAL_OUT_ISP TABLE
	CCommsDbTableView* table=iCommDb->OpenTableLC(TPtrC(DIAL_OUT_ISP));
	TBuf<KCommsDbSvrMaxColumnNameLength> tableName;
	table->GetTableName(tableName);
	// Now you need to do something with it...
	iTextLines.Append(tableName);
	TBool found=FindName(table, KISPName);
	if (!found) {
		TBuf<20> msg;
		msg.Format(_L("No rec"));
		iTextLines.Append(msg);
		User::Leave(1);
	}
	TInt ret;
	TRAP(ret, ReadTextFieldL(table,TPtrC(ISP_DESCRIPTION)));
	if (ret!=KErrNone) {
		TBuf<20> msg;
		msg.Format(_L("No desc %d"), ret);
		iTextLines.Append(msg);
		User::Leave(ret);
	}

	//ReadTextFieldL(table,TPtrC(ISP_TYPE));
	TRAP(ret, ReadTextFieldL(table,TPtrC(ISP_DEFAULT_TEL_NUM)));
	if (ret!=KErrNone) {
		TBuf<20> msg;
		msg.Format(_L("No telnum %d"), ret);
		iTextLines.Append(msg);
		User::Leave(ret);
	}
	CleanupStack::PopAndDestroy();  // table

	// MODEM
#ifndef __S60V2__
	table=iCommDb->OpenTableLC(TPtrC(MODEM));
#else
	table=iCommDb->OpenTableLC(TPtrC(MODEM_BEARER));
#endif
	table->GetTableName(tableName);
	// Now you need to do something with it...
	iTextLines.Append(tableName);
	found=FindName(table, KModemName);
	if (!found) {
		iTextLines.Append(_L("No Nokia modem"));
		User::Leave(ret);
	}
	ReadTextFieldL(table,TPtrC(MODEM_PORT_NAME));
	ReadTextFieldL(table,TPtrC(MODEM_CSY_NAME));
	ReadTextFieldL(table,TPtrC(MODEM_TSY_NAME));
	CleanupStack::PopAndDestroy();  // table

	// GET RID OF COMMDB HANDLE
	CleanupStack::PopAndDestroy();  // dbs
	iCommDb = NULL;

	// **** The (re)drawing bit ****

	//TRect textRect(KXOffset,23,(KWidth-KXOffset),KHeight);
	//SetRect(textRect);
	DrawNow();
	}

TBool CGnuboxContainer::FindName(CCommsDbTableView* table, const TDesC& name)
{
	TBool found=EFalse;
	while (!found && table->GotoNextRecord()==KErrNone) {
		TBuf<100> n;
		table->ReadTextL(TPtrC(COMMDB_NAME), n);
		if (!n.Compare(name) ) found=ETrue;
	}
	return found;
}

TBool CGnuboxContainer::UpdateCommDbL(const TConnectionMode aMode)
/**
	TBool CGnubox::UpdateCommDbL(TConnectionMode aMode)
	Here we update the CommDb MODEM record to reflect the
	change in connection mode we have solicited.
	Currently there are five options:
	1) Direct PPP connection over Infrared
	2) Direct PPP connection over Bluetooth
	3) Dialup PPP connection over Infrared
	4) Dialup PPP connection over Bluetooth
	5) Default PPP over CSD/GPRS phone stack

	@TODO: Need to work out how to restore CSD/GPRS settings
*/
	{
	FLOG(_L("**** CGnuboxContainer::UpdateCommDbL ****"));

	TBool ok=TRUE;
	// CREATE NEW COMMDB HANDLE
	if(iCommDb)
		User::Leave(KErrInUse);
	iCommDb=CCommsDatabase::NewL(EDatabaseTypeIAP);
	iCommDb->ShowHiddenRecords();
	CleanupStack::PushL(iCommDb);

	// LOCK COMMDB - only needed for writes
	TInt ret = iCommDb->BeginTransaction();
	if (ret != 0)
		{
		iEikonEnv->InfoMsg(_L("Failed to lock CommDB"));
		User::Leave(KErrGeneral);
		}

	// Populate templates - in ProfCfg at this point...
	// ...
	//TUint32 id;
	//CCommsDbTableView* table=iCommDb->OpenTableLC(TPtrC(IAP));
	// Goto first record
	//User::LeaveIfError(table->GotoFirstRecord());
	// Update it with the relevant fields
	//User::LeaveIfError(table->UpdateRecord(id));
	//table->WriteTextL(TPtrC(COMMDB_NAME),_L("Direct PPP over IR"));
	//table->WriteUintL(TPtrC(IAP_SERVICE),NTRasISPId);
	//table->WriteTextL(TPtrC(IAP_SERVICE_TYPE),TPtrC(DIAL_OUT_ISP));
	//test(table->PutRecordChanges()==KErrNone);

	switch (aMode)
		{
	case EInfraredDirect:
		{// Modify default (ie. first) ISP record: Direct PPP over IR
		 // DIRECT PPP OVER IR:
		 // 1) insert NULL ISP_DEFAULT_TEL_NUM and MODEM_TSY_NAME
		 // 2) insert IrCOMM::0 into CSY slot
		CCommsDbTableView* table=iCommDb->OpenTableLC(TPtrC(DIAL_OUT_ISP));
		User::LeaveIfError(table->GotoFirstRecord());
		User::LeaveIfError(table->UpdateRecord());
		table->WriteTextL(TPtrC(ISP_DESCRIPTION),KDirect2BoxTxt);
		//table->WriteUintL(TPtrC(ISP_TYPE),(TUint32)EIspTypeInternetOnly);
		// Insert NULL ISP telephone number to kick into direct PPP mode
		table->WriteTextL(TPtrC(ISP_DEFAULT_TEL_NUM),KNullDesC);
		User::LeaveIfError(table->PutRecordChanges());
		CleanupStack::PopAndDestroy();  // table
		// Modify default (ie. first) MODEM record: IrCOMM::0
		table=iCommDb->OpenTableLC(TPtrC(MODEM));
		TBool found=FindName(table, KModemName);
		if (!found) {
			iTextLines.Append(_L("No Nokia modem"));
			User::Leave(ret);
		}
		User::LeaveIfError(table->UpdateRecord());
		table->WriteTextL(TPtrC(MODEM_PORT_NAME),KIrCOMMPortNumTxt);
		table->WriteTextL(TPtrC(MODEM_CSY_NAME),KIrCOMMTxt);
		// Just leave the TSY as was...
		// Trying to set it to NULL won't work as it appears
		// to default to GSMBSC anyways...
		//table->WriteTextL(TPtrC(MODEM_TSY_NAME),KNullDesC);
		User::LeaveIfError(table->PutRecordChanges());
		CleanupStack::PopAndDestroy();  // table
		break;
		}
	case EInfraredDialup:
		{// Modify default (ie. first) ISP record: Direct PPP over IR
		 // DIRECT PPP OVER IR (IR access point):
		 // 1) insert GSMBSC ISP TEL
		 // 2) insert IrCOMM::0 into CSY slot
		CCommsDbTableView* table=iCommDb->OpenTableLC(TPtrC(DIAL_OUT_ISP));
		User::LeaveIfError(table->GotoFirstRecord());
		User::LeaveIfError(table->UpdateRecord());
		table->WriteTextL(TPtrC(ISP_DESCRIPTION),KDialup2BoxTxt);
		//table->WriteUintL(TPtrC(ISP_TYPE),(TUint32)EIspTypeInternetOnly);
		
		TBuf<20> telnum;
		iParent->GetDocument()->RetrieveRecordL(KISPTelNumTxt,telnum);	
		table->WriteTextL(TPtrC(ISP_DEFAULT_TEL_NUM),telnum); // freeserve @TODO: RESTORE PROPERLY from file store!!

		//table->WriteBoolL(TPtrC(ISP_PROMPT_FOR_LOGIN),ETrue);  // pass in your login
		User::LeaveIfError(table->PutRecordChanges());
		CleanupStack::PopAndDestroy();  // table
		// Modify default (ie. first) MODEM record: IrCOMM::0
		table=iCommDb->OpenTableLC(TPtrC(MODEM));
		User::LeaveIfError(table->GotoFirstRecord());
		User::LeaveIfError(table->UpdateRecord());
		table->WriteTextL(TPtrC(MODEM_PORT_NAME),KIrCOMMPortNumTxt);
		table->WriteTextL(TPtrC(MODEM_CSY_NAME),KIrCOMMTxt);
		// You have to change the default TSY to GSMBSC...
		// It seems that doing this does the trick...
		table->WriteTextL(TPtrC(MODEM_TSY_NAME),KNullDesC);
		User::LeaveIfError(table->PutRecordChanges());
		CleanupStack::PopAndDestroy();  // table
		break;
		}
	case EBluetoothDirect:
		{// Modify default (ie. first) ISP record: Dialup PPP over BT
		 // DIRECT PPP OVER Bluetooth (LANP):
		 // 1) insert NULL ISP_DEFAULT_TEL_NUM and MODEM_TSY_NAME
		 // 2) insert BTCOMM::0 into CSY slot

		// Throw up a dialog here to allow the user
		// to select the device they want to try and do LANP with...
		TRAPD(err,SelectBluetoothDeviceL(ELANAccess));
		if (err != 0) {
			FLOG(_L("Failed to select BT"));
			ok=FALSE;
			break; // ensures we unlock and NULL iCommDb
		}
		CCommsDbTableView* table=iCommDb->OpenTableLC(TPtrC(DIAL_OUT_ISP));
		TBool found=FindName(table, KISPName);
		if (!found) {
			FLOG(_L("Bt AP not found"));
			User::Leave(1);
		}
		FLOG(_L("Updating ISP"));
		User::LeaveIfError(table->UpdateRecord());
		table->WriteTextL(TPtrC(ISP_DESCRIPTION),KDirect2BoxTxt);
		//table->WriteUintL(TPtrC(ISP_TYPE),(TUint32)0);
		// Insert NULL ISP telephone number to kick into direct PPP mode
		table->WriteTextL(TPtrC(ISP_DEFAULT_TEL_NUM),KNullDesC);
		User::LeaveIfError(table->PutRecordChanges());
		CleanupStack::PopAndDestroy();  // table
		// Modify default (ie. first) MODEM record: BTCOMM::0
#ifndef __S60V2__
		table=iCommDb->OpenTableLC(TPtrC(MODEM));
#else
		table=iCommDb->OpenTableLC(TPtrC(MODEM_BEARER));
#endif
		found=FindName(table, KModemName);
		if (!found) {
			FLOG(_L("No Nokia modem"));
			User::Leave(ret);
		}
		FLOG(_L("Updating modem"));
		User::LeaveIfError(table->UpdateRecord());
		table->WriteTextL(TPtrC(MODEM_PORT_NAME),KBTCOMMPortNumTxt);
		table->WriteTextL(TPtrC(MODEM_CSY_NAME),KBTCOMMTxt);
		// Just leave the TSY as was...
		// Trying to set it to NULL won't work as it appears
		// to default to GSMBSC anyways...
		//table->WriteTextL(TPtrC(MODEM_TSY_NAME),KNullDesC);
		User::LeaveIfError(table->PutRecordChanges());
		CleanupStack::PopAndDestroy();  // table
		break;
		}
	case EBluetoothDialup:
		{// Modify default (ie. first) ISP record: Direct PPP over Bluetooth
		 // DIRECT PPP OVER Bluetooth (DUNP):
		 // 1) insert GSMBSC ISP TEL
		 // 2) insert BTCOMM::0 into CSY slot

		// Throw up a dialog here to allow the user
		// to select the device they want to try and do DUNP with...
		TRAPD(ret,SelectBluetoothDeviceL(EDialupNetworking));
		if (ret != 0)
			{// We failed to select a device - user probably didn't choose one
			ok=FALSE;
			break; // ensures we unlock and NULL iCommDb
			}
		CCommsDbTableView* table=iCommDb->OpenTableLC(TPtrC(DIAL_OUT_ISP));
		User::LeaveIfError(table->GotoFirstRecord());
		User::LeaveIfError(table->UpdateRecord());
		table->WriteTextL(TPtrC(ISP_DESCRIPTION),KDialup2BoxTxt);
		//table->WriteUintL(TPtrC(ISP_TYPE),(TUint32)EIspTypeInternetOnly);

		TBuf<20> telnum;
		iParent->GetDocument()->RetrieveRecordL(KISPTelNumTxt,telnum);	
		table->WriteTextL(TPtrC(ISP_DEFAULT_TEL_NUM),telnum); // freeserve @TODO: RESTORE PROPERLY from file store!!

		//table->WriteBoolL(TPtrC(ISP_PROMPT_FOR_LOGIN),EFalse);  // pass in your login
		User::LeaveIfError(table->PutRecordChanges());
		CleanupStack::PopAndDestroy();  // table
		// Modify default (ie. first) MODEM record: BTCOMM::0
		table=iCommDb->OpenTableLC(TPtrC(MODEM));
		TBool found=FindName(table, KModemName);
		if (!found) {
			iTextLines.Append(_L("No Nokia modem"));
			User::Leave(ret);
		}
		User::LeaveIfError(table->UpdateRecord());
		table->WriteTextL(TPtrC(MODEM_PORT_NAME),KBTCOMMPortNumTxt);
		table->WriteTextL(TPtrC(MODEM_CSY_NAME),KBTCOMMTxt);
		// It seems that doing this does the trick...
		table->WriteTextL(TPtrC(MODEM_TSY_NAME),KNullDesC);
		User::LeaveIfError(table->PutRecordChanges());
		CleanupStack::PopAndDestroy();  // table
		break;
		}
	default:
		{// Ideally you need to be restoring here from the defaults you stowed
		 // away earlier in document...

		 // ie. ISP_DESCRIPTION, ISP_DEFAULT_TEL_NUM

		CCommsDbTableView* table=iCommDb->OpenTableLC(TPtrC(DIAL_OUT_ISP));
		TBool found=FindName(table, KISPName);
		if (!found) {
			iTextLines.Append(_L("Bt AP not found"));
			User::Leave(1);
		}
		User::LeaveIfError(table->UpdateRecord());
		table->WriteTextL(TPtrC(ISP_DESCRIPTION),KNullDesC);
		table->WriteUintL(TPtrC(ISP_TYPE),(TUint32)EIspTypeInternetAndWAP);

		//table->WriteTextL(TPtrC(ISP_DEFAULT_TEL_NUM),_L("08450796699")); // freeserve @TODO: RESTORE PROPERLY from file store!!		
		TBuf<20> telnum;
		iParent->GetDocument()->RetrieveRecordL(KISPTelNumTxt,telnum);	
		table->WriteTextL(TPtrC(ISP_DEFAULT_TEL_NUM),telnum); // freeserve @TODO: RESTORE PROPERLY from file store!!

		//table->WriteBoolL(TPtrC(ISP_PROMPT_FOR_LOGIN),ETrue);  // pass in your login
		User::LeaveIfError(table->PutRecordChanges());
		CleanupStack::PopAndDestroy();  // table
		// Modify default (ie. first) MODEM record: DATAPORT::0
		table=iCommDb->OpenTableLC(TPtrC(MODEM));
		found=FindName(table, KModemName);
		if (!found) {
			iTextLines.Append(_L("No Nokia modem"));
			User::Leave(ret);
		}
		User::LeaveIfError(table->UpdateRecord());
		table->WriteTextL(TPtrC(MODEM_PORT_NAME),KDataportPortNumTxt);
		table->WriteTextL(TPtrC(MODEM_CSY_NAME),KDataportTxt);
		// You have to change back to the default TSY...
		table->WriteTextL(TPtrC(MODEM_TSY_NAME),KPhoneTSYTxt);
		User::LeaveIfError(table->PutRecordChanges());
		CleanupStack::PopAndDestroy();  // table
		break;
		}
		}

	// COMMIT AND UNLOCK THE COMMDB
	ret=iCommDb->CommitTransaction();
	if(ret != KErrNone)
		{
		iEikonEnv->InfoMsg(_L("Failed to unlock CommDB"));
		FLOG(_L("Failed to update CommDB"));
		User::Leave(KErrGeneral);
		}

	// GET RID OF COMMDB HANDLE
	CleanupStack::PopAndDestroy();  // dbs
	FLOG(_L("Updated CommDB"));
	iCommDb = NULL;
	return ok;
	}

void CGnuboxContainer::SelectBluetoothDeviceL(TBTProfile aProfile)
/**
	CGnuboxContainer::SelectBluetoothDeviceL()
	Deals with Bluetooth device selection.
*/
	{
	FLOG(_L("CGnuboxContainer::SelectBluetoothDevice"));
	TBTDeviceResponseParamsPckg pckg;
	TBTDeviceSelectionParamsPckg resPckg;

	RNotifier not;
	User::LeaveIfError(not.Connect());

	TRequestStatus stat;
	not.StartNotifierAndGetResponse(stat,KDeviceSelectionNotifierUid,resPckg,pckg);

	User::WaitForRequest(stat);
	// check response
	// and if it's positive then slap the resp into CommDb BT settings...

	not.CancelNotifier(KDeviceSelectionNotifierUid);
	not.Close();
	// Currently we're leaving if the user presses Cancel...
	User::LeaveIfError(stat.Int());
	iDevAddr = pckg().BDAddr();
	iDevName = pckg().DeviceName();

	SetupCSYL(aProfile);
	}

void CGnuboxContainer::SetupCSYL(TBTProfile aProfile)
	{
	TBTCommPortSettings	settings;

	settings.SetEncryption();
	settings.SetAuthentication();
	settings.SetUUID(TUint(aProfile));  // The service - e.g. DUNP=0x1103
	settings.SetPort(0);				// n where BTCOMM::n
	settings.SetBTAddr(iDevAddr);

	TBuf<12> addrAsText;
	addrAsText.Zero();
	addrAsText.Format(_L("%02x%02x%02x%02x%02x%02x"),
			iDevAddr[0], iDevAddr[1], iDevAddr[2], iDevAddr[3], iDevAddr[4], iDevAddr[5]);

	//FLOG(addrAsText);
	//FLOG(_L("BTCOMM CSY Configured\n"));

	CBTRegistry* registry=CBTRegistry::NewL();
	CleanupStack::PushL(registry);

	//User::LeaveIfError(registry->SetDefaultCommPort(settings));
	iEikonEnv->InfoMsg(_L("Set registry here"));
	
	FLOG(_L("BTCOMM CSY Configured\n"));
	CleanupStack::PopAndDestroy(registry);
	}




