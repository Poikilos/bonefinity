#ifndef BYTER_CPP
#define BYTER_CPP

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <byter.h>
//#include "C:\My Documents\Projects-cpp\Base\byter.h"

using namespace std;

namespace ExpertMultimediaBase {
	bool bByterFirstRun=true;
	Byter::Byter() {
		byarrData=NULL;
		iPlace=0;
		iLength=0;
		iMaxLength=1024;
	}
	Byter::~Byter() {
		SafeFree(byarrData);
	}
	bool Byter::OpenRead(string sFileNow) {
		bool bGood=true;
		sFile=sFileNow;
		string sMsg="starting";
		try {
			sMsg="converting string";
			const char* carrFile=sFile.c_str();
			sMsg="calling OpenRead on converted string";
			bGood=OpenRead(carrFile);
			sMsg="exiting";
		}
		catch (char* carrExn) {
			bGood=false;
			ShowAndDeleteException(carrExn,"Failed to convert char string into OpenRead.",sMsg);
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("Failed to convert char string into OpenRead.",sMsg);
		}
		return bGood;
	}
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
			if (bGood) {
				sMsg="setting max";
				SetMax(iSizeTo);
			}
		}
		catch (char* carrExn) {
			bGood=false;
			ShowAndDeleteException(carrExn,"OpenWrite",sMsg);
			bExn=true;
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("OpenWrite",sMsg);
			bExn=true;
		}
		if (!bGood && !bExn) {
			ShowError(sMsg,"OpenWrite");
		}
		return bGood;
	}
	bool Byter::OpenRead(const char* carrFile) {
		
		string sMsg="starting openread";
		bool bGood=true;
		int iFileSize=0;
		try {
			sFile.assign(carrFile);
			if (bByterFirstRun) Console.Write("Byter::OpenRead(\""+sFile+"\")...");
			//const char* carrFile=sFileNow.c_str();
			sMsg="openread char*...";
       		if (bByterFirstRun) Console.Write(sMsg);
			iFileSize=FileSize(carrFile);
			sMsg="done getting filesize [";
			sMsg+=ToString(iFileSize);
			sMsg+="].  Loading all data...";
       		if (bByterFirstRun) Console.Write(sMsg);
			iLength=iFileSize;
			iMaxLength=iLength;
			if (iFileSize>0) {
			//ifstream ifData(carrFile, ios::out | ios::in | ios::binary);//OFSTRUCT file_data; //the file data information
			    sMsg="create file structure";
				std::ifstream ifData(carrFile, std::ios::in | std::ios::binary);
			    sMsg="checking if file is open";
				if (ifData.is_open()) {// ((file_handle=OpenFile("by3dAlphaLookup.raw",&file_data,OF_READ))==-1) //open the file if it exists
					//std::istringstream issData;

					//issData << ifData.rdbuf();
					//std::string sData = issData.str();
					//iFileSize=sData.length();
					//const char* carrData=sData.c_str();
					sMsg="creating data array";
					byarrData=(byte*)malloc(iFileSize);
					sMsg="creating file structure";
					ifstream ifData(carrFile, ios::out | ios::in | ios::binary);//OFSTRUCT file_data; //the file data information
					sMsg="reading to file structure";
					ifData.read((char*)byarrData, iFileSize); //_lread(file_handle, by3dAlphaLookup,256*256*256);
					sMsg="closing file structure";
					ifData.close();//_lclose(file_handle);


					//byte* byarrSrc=(byte*)carrData;
					//for (int iNow; iNow<iFileSize; iNow++) {
					//	byarrData[iNow]=byarrSrc[iNow];
					//}
					//ifData.read((char*)by3dAlphaLookup, 256*256*256); //_lread(file_handle, by3dAlphaLookup,256*256*256);
					//ifData.close();//_lclose(file_handle);
					//byarrSrc=NULL;
					//free(carrData);
					sMsg="done...";
       				if (bByterFirstRun) Console.Write(sMsg);
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
		catch (char* carrExn) {
			bGood=false;
			ShowAndDeleteException(carrExn,"OpenRead",sMsg);
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("OpenRead");
		}
		if (!bGood) {
			if (ShowError()) {
				if (carrFile!=NULL) {
					cerr<<"Exception error in Byter::OpenRead(\""<<carrFile<<"\"): "<<sMsg<<endl;
				}
				else {
					cerr<<"Exception error in Byter::OpenRead(NULL char*) (failed to read the exception): "<<sMsg<<endl;
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
		return iLength;
	}
	bool Byter::EndOfFile() {
		return iPlace>=iLength;
	}
	void Byter::Read(void* Dest, Uint32 dwBytesToRead) {
		UInt32 dwTest;
		Read(NULL,Dest,dwBytesToRead,dwTest,NULL);
	}
	bool Byter::Read(void* hDummyA, void* Dest, Uint32 dwBytesToRead, Uint32 &dwNumBytesRead, void* hDummyB) {
		string sMsg="init";
		bool bGood=true;
		dwNumBytesRead=0;
		try {
			sMsg="setting pointers";
			byte* byDest=(byte*)Dest;
			byte* bySrc=(byte*)&byarrData[iPlace];
			sMsg="reading data";
			for (int iNow=0; iNow<dwBytesToRead; iNow++) {
				if (iPlace<iLength) {
					*byDest=*bySrc;
					dwNumBytesRead++;
					iPlace++;
					if (iNow<dwBytesToRead) {
						byDest++;
						bySrc++;
					}
				}
				else {
					sMsg="tried to read at ";
					sMsg+=ToString(iPlace);
					sMsg+=" from ";
					sMsg+=ToString(iLength);
					sMsg+=" byte file.";
					bGood=false;
					break;
				}
			}
		}
		catch (char* sExn) {
			ShowAndDeleteException(sExn,"Byter::Read byte array");
		}
		catch (...) {
			ShowUnknownException("Byter::Read byte array");
		}
		if (!bGood) {
			if (ShowErr()) {
				cerr<<"Byter::Read("<<dwBytesToRead<<" bytes from \""<<sFile<<"\") Error: "<<sMsg<<endl;
			}
		}
		return (bGood)?(dwNumBytesRead==dwBytesToRead):false;
	}//end Read to void*
	void Byter::Read(byte& val) {
		val=0;
		if (iPlace>=0 && iPlace<iLength) {
			try {
				val=byarrData[iPlace];
				iPlace+=1;
			}
			catch (char* sExn) {
				ShowAndDeleteException(sExn,"Byter::Read byte");
			}
			catch (...) {
				ShowUnknownException("Byter::Read byte");
			}
		}
		if (iPlace>iLength) iPlace=iLength;
	}//end Byter::Read byte
	void Byter::Read(ushort& val) { //does force little endian storage
		val=0;
		if (iPlace>=0 && iPlace<iLength) {
			try {
				val=byarrData[iPlace];
				iPlace+=1;
				if (iPlace<iLength) {
					val+=((ushort)byarrData[iPlace])*256;
					iPlace+=1;
				}
			}
			catch (char* sExn) {
				ShowAndDeleteException(sExn,"Byter::Read ushort");
			}
			catch (...) {
				ShowUnknownException("Byter::Read ushort");
			}
		}
		if (iPlace>iLength) iPlace=iLength;
	}//end Byter::Read ushort
	bool Byter::ReadAscii(string &sReturn, Uint32 dwBytesToRead, Uint32 &dwNumBytesRead) {
		string sMsg="init";
		bool bGood=true;
		dwNumBytesRead=0;
		sReturn="";
		try {
			sMsg="setting pointers";
			int iDest=0;//byte* byDest=(byte*)Dest;
			int iSrc=iPlace;//byte* bySrc=(byte*)&byarrData[iPlace];
			sMsg="reading data";
			char carrNow[2];
			carrNow[0]='\0';
			carrNow[1]='\0';
			string sNow="";
			for (int iNow=0; iNow<dwBytesToRead; iNow++) {
				if (iPlace<iLength) {
					carrNow[0]=byarrData[iSrc];
					sNow.assign(carrNow);
					sReturn+=sNow;//*byDest=*bySrc;
					dwNumBytesRead++;
					iPlace++;
					if (iNow<dwBytesToRead) {
						iDest++;//byDest++;
						iSrc++;//bySrc++;
					}
				}
				else {
					sMsg="tried to read at ";
					sMsg+=ToString(iPlace);
					sMsg+=" from ";
					sMsg+=ToString(iLength);
					sMsg+=" byte file.";
					bGood=false;
					break;
				}
			}
		}
		catch (char* sExn) {
			try {
				bGood=false;
				sMsg.assign(sExn);
				free(sExn);
			}
			catch(...) {
				bGood=false;
				sMsg="Exception";
			}
		}
		if (!bGood) {
			if (ShowErr()) {
				cerr<<"Byter::Read("<<dwBytesToRead<<" bytes from \""<<sFile<<"\") Error: "<<sMsg<<endl;
			}
		}
		return (bGood)?(dwNumBytesRead==dwBytesToRead):false;
	}//end ReadAscii
	void Byter::Write(byte* lpbySrc, Uint32 dwBytesToWrite) {
		UInt32 dwTest;
		Write(lpbySrc,dwBytesToWrite,dwTest);
	}
	bool Byter::Write(byte* lpbySrc, Uint32 dwBytesToWrite, Uint32 &dwBytesWritten) {
		string sMsg="Initialization";
		bool bGood=true;
		try {
	        dwBytesWritten=0;
			for (Uint32 iNow=0; iNow<dwBytesToWrite; iNow++) {
				if (iPlace>=iMaxLength) {
					bGood=SetMax(IROUNDF((long double)iMaxLength*(long double)1.25));
					if (!bGood) {
						sMsg="Couldn't expand.";
						break;
					}
				}
				if (iPlace<iMaxLength) {
					if (iPlace>=iLength) iLength++;
					byarrData[iPlace]=lpbySrc[iNow];
					dwBytesWritten++;
					iPlace++;
				}
				else {
					bGood=false;
					sMsg="Tried to write beyond array size.";
					break;
				}
			}
		}
		catch (char* sExn) {
			try {
				bGood=false;
				sMsg.assign(sExn);
				free(sExn);
			}
			catch(...) {
				bGood=false;
				sMsg="Exception";
			}
		}
		if (!bGood) {
			if (ShowErr()) cerr<<"Error in Byter::Write: "<<sMsg<<endl;
		}
		return bGood;
	}//end write byte array
	bool Byter::Write(byte& val) {
		bool bGood=false;
		if (iPlace>=0 && iPlace<iLength) {
			try {
				byarrData[iPlace]=val;
				bGood=true;
				iPlace+=1;
				if (iPlace>iLength) {
					iPlace=iLength;
				}
				else bGood=true;
			}
			catch (char* sExn) {
				ShowAndDeleteException(sExn,"Byter::Write byte");
			}
			catch (...) {
				ShowUnknownException("Byter::Write byte");
			}
		}
		return bGood;
	}//end Byter::Write byte
	void Byter::Write(ushort& val) { //does force little endian storage
		if (iPlace>=0 && iPlace<iLength) {
			try {
				byarrData[iPlace]=val;
				iPlace+=1;
				if (iPlace<iLength) {
					byarrData[iPlace]=(byte)(val/256); //(>>8)
					iPlace+=1;
				}
			}
			catch (char* sExn) {
				ShowAndDeleteException(sExn,"Byter::Write ushort");
			}
			catch (...) {
				ShowUnknownException("Byter::Write ushort");
			}
		}
		if (iPlace>iLength) iPlace=iLength;
	}//end Byter::Write ushort
	bool Byter::WriteAscii(string val, Uint32 &dwBytesWritten) {
		string sMsg="Initialization";
		bool bGood=true;
		try {
			Uint32 dwBytesToWrite=val.length();
	        dwBytesWritten=0;

	        char* carrSrc=NULL;
	        if (val.length()>0) {
				carrSrc=(char*)malloc(val.length()*sizeof(char));
				for (Uint32 iNow=0; iNow<dwBytesToWrite; iNow++) {
					if (iPlace>=iMaxLength) {
						bGood=SetMax(IROUNDF((long double)iMaxLength*(long double)1.25));
						if (!bGood) {
							sMsg="Couldn't expand.";
							break;
						}
					}
					if (iPlace<iMaxLength) {
						if (iPlace>=iLength) iLength++;
						byarrData[iPlace]=(byte)carrSrc[iNow];
						dwBytesWritten++;
						iPlace++;
					}
					else {
						bGood=false;
						sMsg="Tried to write beyond array size.";
						break;
					}
				}//end for character iNow
				SafeFree(carrSrc);
			}//end if length()>0
		}
		catch (char* sExn) {
			try {
				bGood=false;
				sMsg.assign(sExn);
				free(sExn);
			}
			catch(...) {
				bGood=false;
				sMsg="Exception";
			}
		}
		if (!bGood) {
			if (ShowErr()) cerr<<"Error in Byter::Write: "<<sMsg<<endl;
		}
		return bGood;
	}//end WriteAscii

	bool Byter::Save() {
		bool bGood=true;
		string sMsg="initialization";
		try {
			//TODO: finish this
			sMsg="Not yet implemented";bGood=false;
			//string sFileNow="";
			//sFileNow.assign();
			//const char* carrFile=sFileNow.c_str();
			//bGood=OpenRead(carrFile);
		}
		catch (char* sExn) {
			try {
				bGood=false;
				sMsg.assign(sExn);
				free(sExn);
			}
			catch(...) {
				bGood=false;
				sMsg="Exception";
			}
		}
		if (!bGood) {
			if (ShowErr()) cerr<<"Error in Byter::Save() \""<<sFile<<"\": "<<sMsg<<endl;
		}
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
				iLength=iSizeTo;
			}
		}
		else iLength=iSizeTo;
		return bGood;
	}//
	bool Byter::SetMax(int iSizeTo) {
		bool bGood=true;
		string sMsg="initialization";
		try {
			if (iSizeTo>iMaxLength) {
				byte* byarrDataNew=(byte*)malloc(iSizeTo);
				for (int iNow=0; iNow<iLength; iNow++) {
					byarrDataNew[iNow]=byarrData[iNow];
				}
				byte* byarrDataOld=byarrData;
				byarrData=byarrDataNew;
				free(byarrDataOld);//done last to prevent crashes from affecting result.
			}
			//string sFileNow="";
			//sFileNow.assign();
			//const char* carrFile=sFileNow.c_str();
			//bGood=OpenRead(carrFile);
		}
		catch (char* sExn) {
			try {
				bGood=false;
				sMsg.assign(sExn);
				free(sExn);
			}
			catch(...) {
				bGood=false;
				sMsg="Exception";
			}
		}
		if (!bGood) {
			if (ShowErr()) cerr<<"Error in Byter::ResizeTo(): "<<sMsg<<endl;
		}
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
