#ifndef BYTER_H
#define BYTER_H

#include <cstdlib>
#include <base.h>
#include <frameworkdummy.h>
// for cpp:
#include <iostream>
#include <fstream>

namespace ExpertMultimediaBase {
	class Byter {
	private:
		int iPlace;
		int usedByteCount;
		int iMaxLength;
	public:
		byte* arrbyData;
		string sFile;

		bool OpenRead(string sFileNow);
		bool OpenRead(const char* szFile);
		int Place();
		int Length();
		bool EndOfFile();
		Byter();
		~Byter();
		bool OpenWrite(string sFileNow);
		bool OpenWrite(string sFileNow, int iSizeTo);
		bool Read(void* hDummyA, void* Dest, Uint32 u32BytesToRead, Uint32 &u32ReturnBytesRead, void* hDummyB);
		void Read(void* Dest, Uint32 u32BytesToRead);
		void Read(byte& val);
		void Read(ushort& val);
		bool ReadAscii(string &sReturn, Uint32 u32BytesToRead, Uint32 &u32BytesRead);
		bool Write(byte* lpbySrc, Uint32 u32BytesToWrite, Uint32 &u32BytesWritten);
		void Write(byte* lpbySrc, Uint32 u32BytesToWrite);
		bool Write(byte& val);
		void Write(ushort& val);
		bool WriteAscii(string val, Uint32 &u32BytesWritten);
		bool Save();
		bool Save(string sFileNow);
		bool SetLength(int iSizeTo);
		bool SetMax(int iSizeTo);
		int BytesLeftUsed();
		bool Position(int iNewPosition);
		bool Advance(int iRelativePos);
	};
}//end namespace
#endif
