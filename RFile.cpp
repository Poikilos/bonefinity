#ifndef BYTER_CPP
#define BYTER_CPP

#include <RFile.h>

using namespace std;

namespace ExpertMultimediaBase {
	bool bByterFirstRun=true;
	Byter::Byter() {
		arrbyData=nullptr;
		iPlace=0;
		usedByteCount=0;
		iMaxLength=1024;
	}
	Byter::~Byter() {
		SafeFree(arrbyData, "~Byter");
	}
	bool Byter::Exists(const std::string& filename) {
		struct stat buf;
		return (stat(filename.c_str(), &buf) == 0);
		//if (stat(filename.c_str(), &buf) != -1) return true;
		//return false;
	}
	bool Byter::IsDir(const std::string& filename) {
		struct stat info;
		if ( stat( filename.c_str(), &info ) != 0 ) return false;  // cannot access
		return ( info.st_mode & S_IFDIR );  // S_ISDIR() doesn't exist on my windows
		//else ;  // is no directory
	}
	bool Byter::OpenRead(string sFileNow) {
		bool bGood=true;
		sFile=sFileNow;
		string sMsg="starting";
		try {
			sMsg="converting string";
			const char* szFile=sFile.c_str();
			sMsg="calling OpenRead on converted string";
			bGood=OpenRead(szFile);
			sMsg="exiting";
		}
		catch (exception& exn) { bGood=false; ShowExn(exn,"Byter::OpenRead");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Failed to convert char string into OpenRead.",sMsg);
		}
		return bGood;
	}//end Byter::OpenRead(string sFileNow)
	bool Byter::OpenWrite(string sFileNow) {
		return OpenWrite(sFileNow, iMaxLength);
	}
	bool Byter::OpenWrite(string sFileNow, int iSizeTo) {
		bool bGood=true;
		sFile=sFileNow;
		bool bExn=false;
		string sMsg="starting";
		try {
			sMsg="setting virtual length";
			bGood=SetLength(0);
			iPlace=0;
			if (bGood) {
				sMsg="setting max";
				bGood=SetMax(iSizeTo);
			}
			else {
				Console::Error.Write("Byter::OpenWrite error: failed to reset length");
				Console::Error.Flush();
			}
		}
		catch (exception& exn) { bGood=false; ShowExn(exn,"Byter::OpenWrite"); bExn=true;
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("OpenWrite",sMsg);
			bExn=true;
		}
		if (!bGood && !bExn) {
			ShowError(sMsg,"OpenWrite");
		}
		return bGood;
	}//end Byter::OpenWrite(string sFileNow, int iSizeTo)
	bool Byter::OpenRead(const char* szFile) {

		string sMsg="starting openread";
		bool bGood=true;
		int iFileSize=0;
		try {
			sFile.assign(szFile);
			if (bByterFirstRun) Console::Error.Write("Byter::OpenRead(\""+sFile+"\")...");
			//const char* szFile=sFileNow.c_str();
			sMsg="openread char*...";
			if (bByterFirstRun) Console::Error.Write(sMsg);
			iFileSize=FileSize(szFile);
			sMsg="done getting filesize [";
			sMsg+=RString_ToString(iFileSize);
			sMsg+="].  Loading all data...";
			if (bByterFirstRun) Console::Error.Write(sMsg);
			usedByteCount=iFileSize;
			iMaxLength=usedByteCount;
			if (iFileSize>0) {
			//ifstream ifData(szFile, ios::out | ios::in | ios::binary);//OFSTRUCT file_data; //the file data information
				sMsg="create file structure";
				std::ifstream ifData(szFile, std::ios::in | std::ios::binary);
				sMsg="checking if file is open";
				if (ifData.is_open()) {// ((file_handle=OpenFile("file.raw",&file_data,OF_READ))==-1) //open the file if it exists
					//std::istringstream issData;

					//issData + ifData.rdbuf();
					//std::string sData = issData.str();
					//iFileSize=sData.length();
					//const char* szData=sData.c_str();
					sMsg="creating data array";
					arrbyData=(byte*)malloc(iFileSize);
					sMsg="creating file structure";
					//ifstream ifData(szFile, ios::out | ios::in | ios::binary);//OFSTRUCT file_data; //the file data information
					sMsg="reading to file structure";
					ifData.read((char*)arrbyData, iFileSize); //_lread(file_handle, array3d,256*256*256);
					sMsg="closing file structure";
					ifData.close();//_lclose(file_handle);


					//byte* arrbySrc=(byte*)szData;
					//for (int iNow; iNow<iFileSize; iNow++) {
					//	arrbyData[iNow]=arrbySrc[iNow];
					//}
					//ifData.read((char*)array3d, 256*256*256); //_lread(file_handle, array3d,256*256*256);
					//ifData.close();//_lclose(file_handle);
					//arrbySrc=nullptr;
					//free(szData);
					sMsg="done...";
					if (bByterFirstRun) Console::Error.Write(sMsg);
				}
				else {
					sMsg="ifstream fails to open file";
					bGood=false;
				}
			}//end if iFileSize>0
			else {
				sMsg="read from file.";
				bGood=false;
			}
		}
		catch (exception& exn) { bGood=false; ShowExn(exn,"Byter::OpenRead");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("OpenRead");
		}
		if (!bGood) {
			if (ShowError()) {
				if (szFile!=nullptr) {
					Console::Error.WriteLine("Exception error in Byter::OpenRead(\""+RString_ToString(szFile)+"\"): "+sMsg);
				}
				else {
					Console::Error.WriteLine("Exception error in Byter::OpenRead(nullptr char*) (failed to read the exception): "+sMsg);
				}
			}
		}
		bByterFirstRun=false;
		return bGood;
	}//end Byter::OpenRead
	int Byter::Place() {
		return iPlace;
	}
	int Byter::Length() {
		return usedByteCount;
	}
	bool Byter::EndOfFile() {
		return iPlace>=usedByteCount;
	}
	void Byter::Read(void* Dest, Uint32 u32BytesToRead) {
		UInt32 u32Test;
		Read(nullptr,Dest,u32BytesToRead,u32Test,nullptr);
	}
	bool Byter::Read(void* hDummyA, void* Dest, Uint32 u32BytesToRead, Uint32 &u32NumBytesRead, void* hDummyB) {
		string sMsg="init";
		bool bGood=true;
		u32NumBytesRead=0;
		try {
			sMsg="setting pointers";
			byte* byDest=(byte*)Dest;
			byte* bySrc=(byte*)&arrbyData[iPlace];
			sMsg="reading data";
			for (Uint32 iNow=0; iNow<u32BytesToRead; iNow++) {
				if (iPlace<usedByteCount) {
					*byDest=*bySrc;
					u32NumBytesRead++;
					iPlace++;
					if (iNow<u32BytesToRead) {
						byDest++;
						bySrc++;
					}
				}
				else {
					sMsg="tried to read at ";
					sMsg+=RString_ToString(iPlace);
					sMsg+=" from ";
					sMsg+=RString_ToString(usedByteCount);
					sMsg+=" byte file.";
					bGood=false;
					break;
				}
			}
		}
		catch (exception& exn) { bGood=false; ShowExn(exn,"Byter::Read");
		}
		catch (...) {
			ShowUnknownExn("Byter::Read byte array");
		}
		if (!bGood) {
			if (ShowErr()) {
				Console::Error.WriteLine("Byter::Read("+RString_ToString(u32BytesToRead)+" bytes from \""+sFile+"\") Error: "+sMsg);
			}
		}
		return (bGood)?(u32NumBytesRead==u32BytesToRead):false;
	}//end Read to void*
	void Byter::Read(byte& val) {
		val=0;
		if (iPlace>=0 && iPlace<usedByteCount) {
			try {
				val=arrbyData[iPlace];
				iPlace+=1;
			}
			catch (exception& exn) { ShowExn(exn,"Byter::Read");
			}
			catch (...) {
				ShowUnknownExn("Byter::Read byte");
			}
		}
		if (iPlace>usedByteCount) usedByteCount=iPlace;
	}//end Byter::Read byte
	void Byter::Read(ushort& val) { //does force little endian storage
		val=0;
		if (iPlace>=0 && iPlace<usedByteCount) {
			try {
				val=arrbyData[iPlace];
				iPlace+=1;
				if (iPlace<usedByteCount) {
					val+=((ushort)arrbyData[iPlace])*256;
					iPlace+=1;
				}
			}
			catch (exception& exn) { ShowExn(exn,"Byter::Read");
			}
			catch (...) {
				ShowUnknownExn("Byter::Read ushort");
			}
		}
		if (iPlace>usedByteCount) usedByteCount=iPlace;
	}//end Byter::Read ushort
	bool Byter::ReadAscii(string &sReturn, Uint32 u32BytesToRead, Uint32 &u32NumBytesRead) {
		string sMsg="init";
		bool bGood=true;
		u32NumBytesRead=0;
		sReturn="";
		try {
			sMsg="setting pointers";
			int iDest=0;//byte* byDest=(byte*)Dest;
			int iSrc=iPlace;//byte* bySrc=(byte*)&arrbyData[iPlace];
			sMsg="reading data";
			char szNow[2];
			szNow[0]='\0';
			szNow[1]='\0';
			string sNow="";
			for (Uint32 iNow=0; iNow<u32BytesToRead; iNow++) {
				if (iPlace<usedByteCount) {
					szNow[0]=arrbyData[iSrc];
					sNow.assign(szNow);
					sReturn+=sNow;//*byDest=*bySrc;
					u32NumBytesRead++;
					iPlace++;
					if (iNow<u32BytesToRead) {
						iDest++;//byDest++;
						iSrc++;//bySrc++;
					}
				}
				else {
					sMsg="tried to read at ";
					sMsg+=RString_ToString(iPlace);
					sMsg+=" from ";
					sMsg+=RString_ToString(usedByteCount);
					sMsg+=" byte file.";
					bGood=false;
					break;
				}
			}
		}
		catch (exception& exn) {
			bGood=false;
			sMsg.assign(exn.what());
			ShowExn(exn,"ReadAscii");
		}
		catch (...) {
			bGood=false;
			sMsg="Unknown Exception";
			ShowUnknownExn("ReadAscii");
		}
		//catch (char* sExn) {
		//	try {
		//		bGood=false;
		//		sMsg.assign(sExn);
		//		free(sExn);
		//	}
		//	catch(...) {
		//		bGood=false;
		//		sMsg="Exception";
		//	}
		//}
		if (!bGood) {
			if (ShowErr()) {
				Console::Error.WriteLine("Byter::ReadAscii("+RString_ToString(u32BytesToRead)+" bytes from \""+sFile+"\") Error: "+sMsg);
			}
		}
		return (bGood)?(u32NumBytesRead==u32BytesToRead):false;
	}//end ReadAscii
	void Byter::Write(byte* lpbySrc, Uint32 u32BytesToWrite) {
		UInt32 u32Test;
		Write(lpbySrc,u32BytesToWrite,u32Test);
	}
	bool Byter::Write(byte* lpbySrc, Uint32 u32BytesToWrite, Uint32 &u32BytesWritten) {
		string sMsg="Initialization";
		bool bGood=true;
		try {
			u32BytesWritten=0;
			for (Uint32 iNow=0; iNow<u32BytesToWrite; iNow++) {
				if (iPlace>=iMaxLength) {
					bGood=SetMax(IROUNDF((long double)iMaxLength*(long double)1.25));
					if (!bGood) {
						sMsg="Couldn't expand.";
						break;
					}
				}
				if (iPlace<iMaxLength) {
					arrbyData[iPlace]=lpbySrc[iNow];
					u32BytesWritten++;
					iPlace++;
					if (iPlace>usedByteCount) usedByteCount=iPlace;
				}
				else {
					bGood=false;
					sMsg="Tried to write beyond array size.";
					break;
				}
			}
		}
		catch (exception& exn) { ShowExn(exn,"Byter::Write byte");
			bGood=false;
			sMsg=exn.what();
		}
		catch (...) {
			bGood=false;
			sMsg="unknown exception";
		}
		if (!bGood) {
			if (ShowErr()) Console::Error.WriteLine("Error in Byter::Write from byte pointer: "+sMsg);
		}
		return bGood;
	}//end write byte array
	bool Byter::Write(byte& val) {
		bool bGood=false;
		if (iPlace>=0) {
			if (iPlace<iMaxLength) {
				try {
					arrbyData[iPlace]=val;
					bGood=true;
					iPlace+=1;
					if (iPlace>iMaxLength) {
						iPlace=iMaxLength;
					}
					else bGood=true;
				}
				catch (exception& exn) { ShowExn(exn,"Byter::Write byte");
				}
				catch (...) {
					ShowUnknownExn("Byter::Write byte");
				}
			}
			else {
				if (iMaxLength==0) Console::Error.Write("1^0");
				else Console::Error.Write("2^#");//Console::Error.Write("Byter::Write byte error: place was "+RString_ToString(iPlace)+" of "+RString_ToString(usedByteCount));
				Console::Error.Flush();
			}
		}
		else {
			Console::Error.Write("Byter::Write byte error: place was "+RString_ToString(iPlace));
			Console::Error.Flush();
		}
		if (iPlace>usedByteCount) usedByteCount=iPlace;
		return bGood;
	}//end Byter::Write byte
	void Byter::Write(ushort& val) { //does force little endian storage
		if (iPlace>=0) {
			if (iPlace<iMaxLength) {
				try {
					arrbyData[iPlace]=(byte)val;
					iPlace+=1;
					if (iPlace<iMaxLength) {
						arrbyData[iPlace]=(byte)(val/256); //(>>8)
						iPlace+=1;
					}
				}
				catch (exception& exn) { ShowExn(exn,"Byter::Write ushort");
				}
				catch (...) {
					ShowUnknownExn("Byter::Write ushort");
				}
			}
			else {
				if (iMaxLength==0) Console::Error.Write("2^0");
				else Console::Error.Write("1^#");//Console::Error.Write("Byter::Write byte error: place was "+RString_ToString(iPlace)+" of "+RString_ToString(usedByteCount));
				Console::Error.Flush();
			}
		}
		else {
			Console::Error.Write("Byter::Write byte error: place was "+RString_ToString(iPlace));
			Console::Error.Flush();
		}
		if (iPlace>usedByteCount) usedByteCount=iPlace;
	}//end Byter::Write ushort
	bool Byter::WriteAscii(string val, Uint32 &u32BytesWritten) {
		string sMsg="Initialization";
		bool bGood=true;
		try {
			Uint32 u32BytesToWrite=val.length();
			u32BytesWritten=0;

			char* szSrc=nullptr;
			if (val.length()>0) {
				szSrc=(char*)malloc(val.length()*sizeof(char));
				for (Uint32 iNow=0; iNow<u32BytesToWrite; iNow++) {
					if (iPlace>=iMaxLength) {
						bGood=SetMax(IROUNDF((long double)iMaxLength*(long double)1.25));
						if (!bGood) {
							sMsg="Couldn't expand.";
							break;
						}
					}
					if (iPlace<iMaxLength) {
						arrbyData[iPlace]=(byte)szSrc[iNow];
						u32BytesWritten++;
						iPlace++;
						if (iPlace>=usedByteCount) usedByteCount++;
					}
					else {
						bGood=false;
						sMsg="Tried to write beyond array size.";
						break;
					}
				}//end for character iNow
				SafeFree(szSrc, "WriteAscii");
			}//end if length()>0
		}
		catch (exception& exn) {
			bGood=false;
			sMsg=exn.what();
			ShowExn(exn,"Byter::WriteAscii");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Byter::WriteAscii");
			sMsg="Unknown Exception";
		}
		if (!bGood) {
			if (ShowErr()) Console::Error.WriteLine("Error in Byter::Write: "+sMsg);
		}
		return bGood;
	}//end WriteAscii

	bool Byter::Save() {
		bool bGood=false;
		string sMsg="initialization";
		try {
			const char* szFile=sFile.c_str();
			std::ofstream ofData(szFile, std::ios::binary);//std::ofstream ofData(szFile, std::ios::out | std::ios::binary);
			sMsg="checking if file is open";
			if (ofData.is_open()) {
				sMsg="writing byte array";
				ofData.write((char*)arrbyData,usedByteCount);
				sMsg="closing file";
				ofData.close();
				bGood=true;
			}
		}
		catch (exception& exn) {
			bGood=false;
			sMsg=exn.what();
			ShowExn(exn,"Byter::Save()");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Byter::Save()");
			sMsg="Unknown Exception";
		}
		if (!bGood) {
			if (ShowErr()) Console::Error.WriteLine("Error in Byter::Save() \""+sFile+"\": "+sMsg);
		}
		return bGood;
	}//end Byter::Save()
	bool Byter::Save(string sFileNow) {
		sFile=sFileNow;
		return Save();
	}
	bool Byter::SetLength(int iSizeTo) {
		bool bGood=true;
		if (iSizeTo>iMaxLength) {
			bGood=SetMax(iSizeTo);
			if (bGood) {
				usedByteCount=iSizeTo;
			}
		}
		else usedByteCount=iSizeTo;
		return bGood;
	}//
	bool Byter::SetMax(int iSizeTo) {
		bool bGood=true;
		string sMsg="initialization";
		try {
			if (iSizeTo>iMaxLength) {
				byte* arrbyDataNew=(byte*)malloc(iSizeTo);
				for (int iNow=0; iNow<usedByteCount; iNow++) {
					arrbyDataNew[iNow]=arrbyData[iNow];
				}
				byte* arrbyDataOld=arrbyData;
				arrbyData=arrbyDataNew;
				iMaxLength=iSizeTo;
				free(arrbyDataOld);//done last to prevent crashes from affecting result.
			}
			//string sFileNow="";
			//sFileNow.assign();
			//const char* szFile=sFileNow.c_str();
			//bGood=OpenRead(szFile);
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"SetMax");
			sMsg.assign(exn.what());
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("SetMax");
			sMsg="Unknown Exception";
		}
		//catch (char* sExn) {
		//	try {
		//		bGood=false;
		//		sMsg.assign(sExn);
		//		free(sExn);
		//	}
		//	catch(...) {
		//		bGood=false;
		//		sMsg="Exception";
		//	}
		//}
		if (!bGood) {
			if (ShowErr()) Console::Error.WriteLine("Error in Byter::ResizeTo(): "+sMsg);
		}
		return bGood;
	}//end Byter::ResizeTo
	int Byter::BytesLeftUsed() {
		return (Length()-Place()) + 1;
	}
	bool Byter::Position(int iNewPosition) {
		bool bGood=false;
		if (iNewPosition>Length()) {
			iPlace=Length();
		}
		else if (iNewPosition<0) {
			iPlace=0;
		}
		else {
			iPlace=iNewPosition;
			bGood=true;
		}
		return bGood;
	}
	bool Byter::Advance(int iRelativePos) {
		return Position(Place()+iRelativePos);
	}
}//end namespace
#endif
