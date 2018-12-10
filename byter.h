#ifndef BYTER_H
#define BYTER_H

#include <cstdlib>
#include <base.h>
//#include "C:\My Documents\Projects-cpp\Base\base.h"

using namespace std;

namespace ExpertMultimediaBase {
	class Byter {
	private:
		int iPlace;
		int iLength;
		int iMaxLength;
	public:
		byte* byarrData;
		string sFile;

		bool OpenRead(string sFileNow);
		bool OpenRead(const char* carrFile);
		int Place();
		int Length();
		bool EndOfFile();
		Byter();
		~Byter();
		bool OpenWrite(string sFileNow);
		bool OpenWrite(string sFileNow, int iSizeTo);
		bool Read(void* hDummyA, void* Dest, Uint32 dwBytesToRead, Uint32 &dwReturnBytesRead, void* hDummyB);
		void Read(void* Dest, Uint32 dwBytesToRead);
		void Read(byte& val);
		void Read(ushort& val);
		bool ReadAscii(string &sReturn, Uint32 dwBytesToRead, Uint32 &dwBytesRead);
		bool Write(byte* lpbySrc, Uint32 dwBytesToWrite, Uint32 &dwBytesWritten);
		void Write(byte* lpbySrc, Uint32 dwBytesToWrite);
		bool Write(byte& val);
		void Write(ushort& val);
		bool WriteAscii(string val, Uint32 &dwBytesWritten);
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
