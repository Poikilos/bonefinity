#ifndef FRAMEWORKDUMMY_CPP
#define FRAMEWORKDUMMY_CPP

#include "frameworkdummy.h"
#include <fstream>
//#include <cstdio>
#include <iostream>
//#include <cfloat> //<float.h> //maximums i.e. LONG_MAX
//#include <climits> //<limits.h> //maximums i.e. INT_MAX
//#include "E:\Projects-cpp\Base\frameworkdummy.h"

using namespace std;

namespace FileMode {
	string OpenRead;//must be declared BEFORE staticconstructors
	string OpenWrite;//must be declared BEFORE staticconstructors
}

namespace ProtoArmor {
	bool bReverseEndian=false;
	//string FileMode_OpenRead;//must be declared BEFORE staticconstructors
	//string FileMode_OpenWrite;//must be declared BEFORE staticconstructors
	int iConsoleLinesMax=3000;
	int iConsoleLinesWritten=0;
	StaticConstructors staticconstructors;

	StaticConstructors::StaticConstructors() {
		cerr<<"loading static constructors..."<<flush;
		FileMode::OpenWrite="wb";//FileMode_OpenWrite="wb";//FileMode::OpenWrite="wb";
		FileMode::OpenRead="rb";//FileMode_OpenRead="rb";//FileMode::OpenRead="rb";
		//FileMode::staticconstructorFileMode();
		cerr<<"done (loading static constructors)"<<endl;
	}
	StaticConstructors::~StaticConstructors() {
		//FileMode::staticdeconstructorFileMode();
	}


	//const float float_MaxValue=FLT_MAX;
	//const float double_MaxValue=DBL_MAX;
	//const int int_MaxValue=INT_MAX;//2147483647;
	//const long long_MaxValue=LONG_MAX;//9223372036854775807
	//FILE_FAKESTATIC_CLASS File;
	//CONSOLE_FAKESTATIC_CLASS Console;

/////////////////////////////  Rectangle  /////////////////////////////

	void Rectangle::Set(int x, int y, int width, int height) {
		X=x;
		Y=y;
		Width=width;
		Height=height;
		if (Height<0) Height=0;
		if (Width<0) Width=0;
	}
	void RectangleF::Set(float x, float y, float width, float height) {
		X=x;
		Y=y;
		Width=width;
		Height=height;
		if (Height<0) Height=0;
		if (Width<0) Width=0;
	}

/////////////////////////////  Convert  /////////////////////////////

	byte Convert::ToByte(int val) {
		return (val<=0) ? (0) : ((val>=255)?255:(byte)(val));
	}
	byte Convert::ToByte(long val) {
		return (val<=0) ? (0) : ((val>=255)?255:(byte)(val));
	}
	byte Convert::ToByte(__int64 val) {
		return (val<=0) ? (0) : ((val>=255)?255:(byte)(val));
	}
	byte Convert::ToByte(size_t val) {
		return (val<=0) ? (0) : ((val>=255)?255:(byte)(val));
	}
	byte Convert::ToByte(float val) {
		return (val<.5f) ? (0) : ((val>=254.5f)?255:(byte)(val+.5f));
	}
	byte Convert::ToByte(double val) {
		return (val<.5) ? (0) : ((val>=254.5)?255:(byte)(val+.5));
	}
	byte Convert::ToByte(char val) {
		return (byte)val;//e.g. converts -128 to 255
	}
	char Convert::ToChar8(float val) {
		return (val<.5f) ? (0) : ((val>=254.5f)?255:(char)(val+.5f));
	}
	char Convert::ToChar8(double val) {
		return (val<.5) ? (0) : ((val>=254.5)?255:(char)(val+.5));
	}
	string Convert::ToString(const char* val) {
		//stringstream ssReturn;
		//string sReturn;
		//ssReturn << val;
		//ssReturn >> sReturn;
		return string(val);
	}
	string Convert::ToString(bool val) {
		return val?string("true"):string("false");
	}
	string Convert::ToString(unsigned char val) {
		stringstream ssReturn;
		string sReturn;
		ssReturn << (unsigned int)val;//cast ensures showing number instead of character
		ssReturn >> sReturn;
		return sReturn;
	}
	//string Convert::ToString(__wchar_t val) {}
	//string Convert::ToString(DateTime val) {}
	//string Convert::ToString(Decimal val) {
	//	stringstream ssReturn;
	//	string sReturn;
	//	ssReturn << val.ToString();
	//	ssReturn >> sReturn;
	//	return sReturn;
	//}
	string Convert::ToString(float val) {
		stringstream ssReturn;
		string sReturn;
		ssReturn << val;
		ssReturn >> sReturn;
		return sReturn;
	}
	string Convert::ToString(double val) {
		stringstream ssReturn;
		string sReturn;
		ssReturn << val;
		ssReturn >> sReturn;
		return sReturn;
	}
	string Convert::ToString(short val) {
		stringstream ssReturn;
		string sReturn;
		ssReturn << val;
		ssReturn >> sReturn;
		return sReturn;
	}
	string Convert::ToString(int val) {
		stringstream ssReturn;
		string sReturn;
		ssReturn << val;
		ssReturn >> sReturn;
		return sReturn;
	}
	string Convert::ToString(__int64 val) {
		stringstream ssReturn;
		string sReturn;
		ssReturn << val;
		ssReturn >> sReturn;
		return sReturn;
	}
	//string Convert::ToString(Object* val) {}
	string Convert::ToString(char val) {
		//char carr[2];
		//carr[0]=val;
		//carr[1]='\0';
		//string sReturn(carr);
		string sReturn(1,val);
		return sReturn;
	}
	//string Convert::ToString(String* val) {}
	string Convert::ToString(unsigned short val) {
		stringstream ssReturn;
		string sReturn;
		ssReturn << val;
		ssReturn >> sReturn;
		return sReturn;
	}
	string Convert::ToString(unsigned int val) {
		stringstream ssReturn;
		string sReturn;
		ssReturn << val;
		ssReturn >> sReturn;
		return sReturn;
	}
	string Convert::ToString(unsigned __int64 val) {
		stringstream ssReturn;
		string sReturn;
		ssReturn << val;
		ssReturn >> sReturn;
		return sReturn;
	}


/////////////////////////////  FileMode  /////////////////////////////

	//void FileMode::staticconstructorFileMode() {
	//	//Open="wb";
	//	FileMode_OpenWrite="wb";//FileMode::OpenWrite="wb";
	//	FileMode_OpenRead="rb";//FileMode::OpenRead="rb";
	//	//cerr<<"finished staticconstructorFileMode {FileMode_OpenWrite:"<<FileMode_OpenWrite<<"; FileMode_OpenRead:"<<FileMode_OpenRead<<"}..."<<endl;
	//}

/////////////////////////////  Stream  /////////////////////////////
	long long Stream::getPosition() {
		return Position;
	}
	Stream::Stream() {
		pfile=NULL;
		Position=0;
		sModePrev="";
	}
	Stream::~Stream() {
		Close();
	}
	void Stream::Close() {
		if (pfile!=NULL) {
			fclose(pfile);
			pfile=NULL;
		}
		sModePrev="";
	}
	size_t Stream::fread(void* _DstBuf, size_t _ElementSize, size_t _Count) {
		size_t iReturn=0;
		if (pfile!=NULL) {
			iReturn=::fread(_DstBuf,_ElementSize,_Count,this->pfile);
			if (iReturn>0) Position+=_ElementSize*iReturn;
		}
		return iReturn;
	}
	size_t Stream::fwrite(const void* _Str, size_t _ElementSize, size_t _Count) {
		size_t iReturn=0;
		if (pfile!=NULL) {
			iReturn=::fwrite(_Str,_ElementSize,_Count,this->pfile);
			if (iReturn>0) Position+=_ElementSize*iReturn;
		}
		return iReturn;
	}
	void Stream::Open(const char* _Filename, const char* _Mode) {
		sModePrev=Convert::ToString(_Mode);
		if (pfile!=NULL) {
			fclose(pfile);
			pfile=NULL;
		}
		if (_Mode!=NULL) {
			if (_Mode[0]!='\0') pfile=::fopen(_Filename, _Mode);
			else cerr<<"Stream::Open Error: zero-length Mode {FileMode::OpenWrite:"<<FileMode::OpenWrite<<"; FileMode::OpenRead:"<<FileMode::OpenRead<<"}"<<endl;
		}
		else cerr<<"Stream::Open Error: null Mode char*"<<endl;
	}
	bool Stream::CanRead() {
		return (pfile!=NULL)&&(sModePrev.find_first_of('r',0)!=string::npos);
	}
	bool Stream::CanWrite() {
		return pfile!=NULL&&(sModePrev.find_first_of('w',0)!=string::npos);
	}

/////////////////////////////  FileStream  /////////////////////////////

	FileStream::FileStream() {
	}
	void FileStream::Close() {
		BaseStream.Close();
	}

/////////////////////////////  File  /////////////////////////////

	bool File::Exists(string sFile) {
		bool bReturn=false;
		static bool bFirstRun=true;
		if (bFirstRun) cerr<<"checking File::Exists(\""<<sFile<<"\")...";
		string sMsg="opening";
		if (bFirstRun) cerr<<sMsg<<"...";
		try {
			std::fstream fsNow;
			fsNow.open(sFile.c_str(),std::ios::in);
			sMsg="check if open";
			if (bFirstRun) cerr<<sMsg<<"...";
			bReturn=fsNow.is_open();
			sMsg=((bReturn)?"(yes) ":"(no) ")+(string)("closing");
			if (bFirstRun) cerr<<sMsg<<"...";
			fsNow.close();
			if (bFirstRun) cerr<<"done File.Exists...";
		}
		catch (exception& exn) {
			bReturn=false;
			cerr<<"File.Exists check:"<<exn.what();//avoids base.cpp -- ShowExn(exn,"File.Exists",sMsg);
		}
		catch (...) {
			bReturn=false;
			cerr<<"File.Exists check could not finish (Unknown exception).";//avoids base.cpp -- ShowUnknownExn("File.Exists",sMsg);
		}
		bFirstRun=false;
		return bReturn;
	}//end File.Exists
	FileStream* File::Open(string sFile, string filemode) {
		FileStream* fsReturn=new FileStream();
		fsReturn->BaseStream.Open(sFile.c_str() , filemode.c_str());
		return fsReturn;
	}

/////////////////////////////  BinaryReader  /////////////////////////////

	BinaryReader::BinaryReader() {
		filestream=NULL;
		bDeleteFileStream=false;
	}
	BinaryReader::BinaryReader(FileStream* filestreamNew, bool AllowThisToDeleteFileStream) {
		filestream=filestreamNew;
		bDeleteFileStream=AllowThisToDeleteFileStream;
	}
	BinaryReader::~BinaryReader() {
		Dispose(bDeleteFileStream);
	}
	void BinaryReader::Dispose(bool bDeleteFileStreamIfNonNull) {
		if (bDeleteFileStreamIfNonNull) {
			if (filestream!=NULL) {
				filestream->Close();
				delete filestream;
				filestream=NULL;
			}
		}
	}
	void BinaryReader::Close() {
		if (filestream!=NULL) {
			filestream->Close();
			delete filestream;
			filestream=NULL;
		}
	}
	long long BinaryReader::BaseStream_Position() {
		return (filestream!=NULL)?filestream->BaseStream.getPosition():-1;
	}
	bool BinaryReader::BaseStream_CanRead() {
		return ((filestream!=NULL)&&filestream->BaseStream.CanRead());
	}
	int BinaryReader::Read_octets(unsigned char* buffer, int indexInBuffer, int count) {
		int iFound=0;
		size_t ElementSize=1;
		if (filestream!=NULL) {
			int iFound=ElementSize*this->filestream->BaseStream.fread(&buffer[indexInBuffer],ElementSize,count);
		}
		return iFound;
	}
	unsigned char BinaryReader::ReadByte(){
		unsigned char valReturn=(unsigned char)0;
		int iFound=0;
		size_t ElementSize=1;
		if (filestream!=NULL) {
			iFound=ElementSize*this->filestream->BaseStream.fread(&valReturn,ElementSize,1);
		}
		return valReturn;
	}
	char BinaryReader::ReadChar_octect(){
		char valReturn=(char)0;
		int iFound=0;
		size_t ElementSize=1;
		if (filestream!=NULL) {
			iFound=ElementSize*this->filestream->BaseStream.fread(&valReturn,ElementSize,1);
		}
		return valReturn;
	}
	string BinaryReader::ReadChars_octects(int count){
		char* carr=(char*)malloc(count+1);
		int iFound=0;
		size_t ElementSize=1;
		if (filestream!=NULL) {
			iFound=ElementSize*this->filestream->BaseStream.fread(carr,ElementSize,count);
		}
		carr[count]='\0';//ok since size is count+1
		string sReturn=Convert::ToString(carr);
		free(carr);
		carr=NULL;
		return sReturn;
	}
	bool bShowElementSizeErr=true;
	unsigned short BinaryReader::ReadUInt16(){
		unsigned short valReturn=(unsigned short)0;
		int iFound=0;
		size_t ElementSize=2;
		if (filestream!=NULL) {
			iFound=ElementSize*this->filestream->BaseStream.fread(&valReturn,ElementSize,1);
			if (iFound!=ElementSize) {
				if (bShowElementSizeErr) {
					cerr<<"frameworkdummy.cpp: BinaryReader::ReadUInt16 iFound does NOT match element size  {iFound:"<<iFound<<"; Elementsize:"<<ElementSize<<"}"<<endl;//debug only
					bShowElementSizeErr=false;
				}
			}
		}
		return valReturn;
	}
	unsigned int BinaryReader::ReadUInt32(){
		unsigned int valReturn=(unsigned int)0;
		int iFound=0;
		size_t ElementSize=4;
		if (filestream!=NULL) {
			iFound=ElementSize*this->filestream->BaseStream.fread(&valReturn,ElementSize,1);
		}
		return valReturn;
	}

/////////////////////////////  BinaryWriter  /////////////////////////////


	BinaryWriter::BinaryWriter() {
		filestream=NULL;
		bDeleteFileStream=false;
	}
	BinaryWriter::BinaryWriter(FileStream* filestreamNew, bool AllowThisToDeleteFileStream) {
		filestream=filestreamNew;
		bDeleteFileStream=AllowThisToDeleteFileStream;
	}
	BinaryWriter::~BinaryWriter() {
		Dispose(bDeleteFileStream);
	}
	void BinaryWriter::Dispose(bool bDeleteFileStreamIfNonNull) {
		if (bDeleteFileStreamIfNonNull) {
			if (filestream!=NULL) {
				filestream->Close();
				delete filestream;
				filestream=NULL;
			}
		}
	}
	void BinaryWriter::Close() {
		if (filestream!=NULL) {
			filestream->Close();
			delete filestream;
			filestream=NULL;
		}
	}
	long long BinaryWriter::BaseStream_Position() {
		return (filestream!=NULL)?filestream->BaseStream.getPosition():-1;
	}
	void BinaryWriter::Write(unsigned char val) {
		long long iGot=0;
		if (filestream!=NULL) {
			iGot=(long long)this->filestream->BaseStream.fwrite(&val,1,1);
		}
	}
	void BinaryWriter::Write(string val) {
		long long iGot=0;
		if (filestream!=NULL) {
			const char* carr=val.c_str();
			for (unsigned int iChar=0; iChar<val.length(); iChar++) {
				iGot=(long long)this->filestream->BaseStream.fwrite(&carr[iChar],1,1);
			}
		}
	}
	void BinaryWriter::Write(unsigned short val) {
		long long iGot=0;
		if (filestream!=NULL) {
			if (bReverseEndian) {
				iGot=(long long)this->filestream->BaseStream.fwrite((&val)+1,1,1);
				iGot+=(long long)this->filestream->BaseStream.fwrite((&val),1,1);
			}
			else iGot=(long long)this->filestream->BaseStream.fwrite(&val,2,1);
		}
	}
	void BinaryWriter::Write(unsigned int val) {
		long long iGot=0;
		if (filestream!=NULL) {
			if (bReverseEndian) {
				iGot=(long long)this->filestream->BaseStream.fwrite((&val)+3,1,1);
				iGot+=(long long)this->filestream->BaseStream.fwrite((&val)+2,1,1);
				iGot+=(long long)this->filestream->BaseStream.fwrite((&val)+1,1,1);
				iGot+=(long long)this->filestream->BaseStream.fwrite((&val),1,1);
			}
			else iGot=(long long)this->filestream->BaseStream.fwrite(&val,2,1);
		}
	}
	void BinaryWriter::Write(unsigned char* arr, int index, int count) {
		long long iGot=0;
		if (filestream!=NULL) {
			unsigned int iAbs=(unsigned int)((index<0)?0:index);
			for (int iRel=0; iRel<count; iRel++) {
				iGot=(long long)this->filestream->BaseStream.fwrite(&arr[iAbs],1,1);
				iAbs++;
			}
		}
	}

/////////////////////////////  Console::Error.Out  /////////////////////////////

	void ref_ERROR_TEXTWRITER::Write(string val) {
		if (iConsoleLinesWritten+1<=iConsoleLinesMax) { //note: these vars aren't really lines, but calls to Console::Error.Write
			cerr<<val;
		}
	}
	void ref_ERROR_TEXTWRITER::WriteLine(string val) {
		if (iConsoleLinesWritten+1<iConsoleLinesMax) { //note: these vars aren't really lines, but calls to Console::Error.Write
			cerr<<(val);
			cerr<<endl;
		}
		else if (iConsoleLinesWritten+1==iConsoleLinesMax) { //note: these vars aren't really lines, but calls to Console::Error.Write
			cerr<<"Too console messages.  This is the last error that will be shown:"<<endl;
			cerr<<(val);
			cerr<<endl;
		}
	}
	void ref_ERROR_TEXTWRITER::WriteLine() {
		if (iConsoleLinesWritten+1<iConsoleLinesMax) { //note: these vars aren't really lines, but calls to Console::Error.Write
			cerr<<endl;
			iConsoleLinesMax++;
		}
		else if (iConsoleLinesWritten+1==iConsoleLinesMax) {
			cerr<<endl;
			cerr<<"Too many errors--no more console errors will be shown."<<endl;//MUST BE cerr OR WILL BE INFINITE RECURSION!
		}
	}
	void ref_ERROR_TEXTWRITER::Flush() {
		cerr<<flush;
	}

/////////////////////////////  Console::Error  /////////////////////////////
	ERROR_TEXTWRITER Console::Error;

	void ERROR_TEXTWRITER::Write(string val) {
		if (iConsoleLinesWritten+1<=iConsoleLinesMax) { //note: these vars aren't really lines, but calls to Console::Error.Write
			cerr<<val;
		}
	}
	void ERROR_TEXTWRITER::WriteLine() {
		if (iConsoleLinesWritten+1<iConsoleLinesMax) { //note: these vars aren't really lines, but calls to Console::Error.Write
			cerr<<endl;
			iConsoleLinesMax++;
		}
		else if (iConsoleLinesWritten+1==iConsoleLinesMax) {
			cerr<<endl;//MUST BE cerr OR WILL BE INFINITE RECURSION!
			cerr<<"Too many errors--no more console errors will be shown."<<endl;//MUST BE cerr OR WILL BE INFINITE RECURSION!
		}
	}
	void ERROR_TEXTWRITER::WriteLine(string val) {
		if (iConsoleLinesWritten+1<iConsoleLinesMax) { //note: these vars aren't really lines, but calls to Console::Error.Write
			cerr<<(val); //MUST BE cerr OR WILL BE INFINITE RECURSION!
			cerr<<endl;//MUST BE cerr OR WILL BE INFINITE RECURSION!
		}
		else if (iConsoleLinesWritten+1==iConsoleLinesMax) { //note: these vars aren't really lines, but calls to Console::Error.Write
			cerr<<"Too console messages.  This is the last error that will be shown:"<<endl;
			cerr<<(val);//MUST BE cerr OR WILL BE INFINITE RECURSION!
			cerr<<endl;//MUST BE cerr OR WILL BE INFINITE RECURSION!
		}
	}
	void ERROR_TEXTWRITER::Flush() {
		cerr<<flush;
	}

/////////////////////////////  Console  /////////////////////////////
	ref_CONSOLE_TEXTWRITER Console::Out;
	void Console::Write(string val) {
		bool bGood=true;
		try {
			if (iConsoleLinesWritten==iConsoleLinesMax) {
				cout<<"Too many console messages ("<<iConsoleLinesWritten<<")"<<endl; //MUST BE COUT OR WILL BE INFINITE RECURSION!
				cout<<"--this is the last message that will be shown:"; //MUST BE COUT OR WILL BE INFINITE RECURSION!
			}
			if (iConsoleLinesWritten<=iConsoleLinesMax) {
				cout<<(val);
				cout<<flush;
			}
		}
		catch (exception& exn) {
			bGood=false;
			cerr<<"Console::Error.Write could not finish: "<<exn.what()<<endl; //MUST BE COUT OR WILL BE INFINITE RECURSION! //avoids base.cpp -- ShowExn(exn,"File.Exists",sMsg);
		}
		catch (...) {
			bGood=false;
			cerr<<"Console::Error.Write could not finish (Unknown exception)."<<endl; //MUST BE COUT OR WILL BE INFINITE RECURSION!//avoids base.cpp -- ShowUnknownExn("File.Exists",sMsg);
		}
		iConsoleLinesWritten++;
	}//end Console::Write
	void Console::WriteLine() {
		if (iConsoleLinesWritten+1<=iConsoleLinesMax) {
			cout<<endl;
			iConsoleLinesWritten++;
		}
	}//end Console::WriteLine
	void Console::WriteLine(string val) {
		if (iConsoleLinesWritten+1<=iConsoleLinesMax) { //note: these vars aren't really lines, but calls to Console::Write
			cout<<(val);
			cout<<endl;
			iConsoleLinesWritten++;
		}
	}//end Console::WriteLine
	void ref_CONSOLE_TEXTWRITER::Flush() {
		cout<<flush;
	}

}//end namespace
#endif
