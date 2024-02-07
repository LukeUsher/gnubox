// debug.h
//
// Central place for nice debug-type macros & functions
//
// Copyright: Mal Minhas
//

#ifndef GNUBOX_DEBUG_H
#define GNUBOX_DEBUG_H

#define _DEBUG 1
#undef __FLOGGING__
#ifdef _DEBUG
#define __FLOGGING__
#endif

#include <e32std.h>

static const TInt KL2CAPDebugOptionMask = 0xffff;      ///< SetOpt for debugging modes in L2CAP.
static const TUint KL2CAPDebugSendRequestsTwice = 1;  ///< Send L2CAP requests twice.

_LIT(KLogFile,"gnubox.txt");
//_LIT(KLogFile,"mm0.txt");
//const TInt KLogIndexOffset=2;
_LIT(KLogDirFullName,"c:\\logs\\gnubox\\");
_LIT(KLogDir,"gnubox");

#include <f32file.h>
#include <flogger.h>

#define FLOG(a) {FPrint(a);}
#define FTRACE(a) {a;}


#ifdef __FLOGGING__
inline void FPrint(const TRefByValue<const TDesC> aFmt, ...)
	{
	VA_LIST list;
	VA_START(list,aFmt);
	TBuf<32> logFile(KLogFile);
	// don't bother with the next line - it numbers the log files but you
	// need to add support in the code to set up Tls integer correctly.
	//logFile[KLogIndexOffset]=TText('0'+(TInt)Dll::Tls());
	RFileLogger::WriteFormat(KLogDir, logFile, EFileLoggingModeAppend, aFmt, list);
#ifdef MEMTRACE
	TInt size;
	User::Heap().AllocSize(size);

	RFileLogger::WriteFormat(KLogDir, logFile, EFileLoggingModeAppend,
							 _L("Mem usage: %d high: %d"), size, User::Heap().Size());
#endif
	}

inline void FHex(const TUint8* aPtr, TInt aLen)
	{
	RFileLogger::HexDump(KLogDir, KLogFile, EFileLoggingModeAppend, 0, 0, aPtr, aLen);
	}

inline void FHex(const TDesC8& aDes)
	{
	FHex(aDes.Ptr(), aDes.Length());
	}

#else // i.e. __FLOGGING__ undefined

inline void FPrint(const TRefByValue<const TDesC> /*aFmt*/,...)
	{
#ifdef _DEBUG // to quell linker warnings
	RFileLogger dummy;
#endif
	}

inline void FHex(const TUint8* /*aPtr*/, TInt /*aLen*/)
	{
	}

inline void FHex(const TDesC8& /*aDes*/)
	{
	}

#endif //	__FLOGGING__

#endif // __GNUBOX_DEBUG_H__
