//
// gnuboxContainer.h
// 28th August 2002
//
// Copyright: Mal Minhas
//
// Description:
//    Main Gnubox Container header file
//

#ifndef GNUBOXCONTAINER_H
#define GNUBOXCONTAINER_H

// Includes
#include <coecntrl.h>
#include "gnuboxAppUi.h"
#include <commdb.h>
#include <btdevice.h>

// Globals
_LIT(KDirectOverIRTxt,  "Direct over IR        ");
_LIT(KDialupOverIRTxt,  "Dialup over IR        ");
_LIT(KDirectOverBTTxt,  "Direct over BT        ");
_LIT(KDialupOverBTTxt,  "Dialup over BT		   ");
_LIT(KPPPoverCSDGPRSTxt,"PPP over CSD/GPRS     ");


//GLDEF_D RTest test(_L("CommDb Testcode"));

// Defines
//#define IAP								_S("IAP")

//#define DIAL_OUT_ISP					_S("DialOutISP")
//#define ISP_DESCRIPTION					_S("Description")					// text field for app to use to describe ISP
//#define ISP_TYPE						_S("Type")
//#define ISP_DEFAULT_TEL_NUM				_S("DefaultTelNum")
//#define ISP_IF_NAME						_S("IfName")

//#define MODEM							_S("Modem")
//#define MODEM_PORT_NAME					_S("PortName")
//#define MODEM_TSY_NAME					_S("TSYName")
//#define MODEM_CSY_NAME					_S("CSYName")
//#define MODEM_HANDSHAKING				_S("Handshaking")


// Constants
_LIT(KCommsDbFileName,"C:\\System\\Data\\Cdbv2.dat");
const TInt KMaxScreenMessageLength = 100;
const TUint8 KLineGap = 13;
const TUint8 KXOffset = 1;
const TUint8 KWidth = 176;
const TUint8 KHeight = 144;
const TUint8 KMaxTextLines = 8; // max no. lines we can have in screen
const TUint8 KMaxLineLength = 25;
const TUint8 KTextOffset = 5;   // offset of text from left

// Typedefs
typedef TBuf<KMaxScreenMessageLength> TTextLine;

// enums
enum TBTProfile
	{
	ESerialPortProfile=0x1101,
	ELANAccess=0x1102,
	EDialupNetworking=0x1103,
	};

// Forward declarations
class CEikLabel;        // for iLabel
class CGnuboxAOLoader;	// for iLoader

// Class declarations
class CGnuboxContainer : public CCoeControl, MCoeControlObserver
/**
	CGnuboxContainer  container control class.
*/
	{
public:  // Constructors and destructor
	/**
	   EPOC default constructor.
	   @param aRect    Frame rectangle for container.
	   @param aParent  The object that created us.
	*/
	void ConstructL(const TRect& aRect, CGnuboxAppUi* aParent);
	/**
	   Destructor.
	*/
	~CGnuboxContainer();
public: // New functions
	void SwitchModeL(const TConnectionMode aBearer);
	void LoadCommDbSettingsL();
private:
	void DrawControls(const TRect& aRect) const;
	void SetAndDrawScreenTextL(const TConnectionMode aMode);
	TBool UpdateCommDbL(const TConnectionMode aMode);
	void ReadTextFieldL(CCommsDbTableView* aTable, const TDesC& aRecord);
	void SelectBluetoothDeviceL(TBTProfile aProfile);
	TBool FindName(CCommsDbTableView* table, const TDesC& name);
public: // Functions from base classes

private: // Functions from base classes
	/**
	   From CCoeControl,SizeChanged.
	*/
	void SizeChanged();
	/**
	   From CCoeControl,CountComponentControls.
	*/
	TInt CountComponentControls() const;
	/**
	   From CCoeControl,ComponentControl.
	*/
	CCoeControl* ComponentControl(TInt aIndex) const;
	/**
	   From CCoeControl,Draw.
	*/
	void Draw(const TRect& aRect) const;
	/**
	   From MCoeControlObserver,HandleControlEventL
	*/
	void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);
	//
	void SetupCSYL(TBTProfile aProfile);
private:
	CGnuboxAppUi* iParent;          ///< Parent CAknAppUi.
	CEikLabel* iLabel;              ///< Main label.
	CGnuboxAOLoader* iLoader;
	RArray <TTextLine> iTextLines;  ///< The lines of text being displayed on the screen.
	CCommsDatabase* iCommDb;        ///< The default CommDB in "C:\system\data\Cdbv2.dat".
	TBTDevAddr iDevAddr;            ///< The Bluetooth Device Address.
	TBTDeviceName iDevName;         ///< The Bluetooth Device Name.
	};


class CGnuboxAOLoader : public CActive
/**
	CGnubox active object CommDb loader class
*/
	{
public:
	~CGnuboxAOLoader();
	static CGnuboxAOLoader *NewL(CGnuboxContainer *aParent);
	// inherited from CActive
	virtual void RunL();
	virtual void DoCancel();
	void InitL(CGnuboxContainer *aParent);
	void Fire();
private:
	CGnuboxAOLoader();
private:
	CGnuboxContainer *iParent;
	};

#endif
