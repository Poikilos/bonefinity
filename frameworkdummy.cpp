#ifndef FRAMEWORKDUMMY_CPP
#define FRAMEWORKDUMMY_CPP

#include <frameworkdummy.h>

using namespace std;

namespace FileMode {
	string OpenRead;//must be declared BEFORE staticconstructors
	string OpenWrite;//must be declared BEFORE staticconstructors
}

namespace System {
	DateTime DateTime_Now(true);
	DateTime::DateTime() {
		this->alwaysReturnNow=false;
	}
	DateTime::DateTime(bool AlwaysReturnNow) {
		this->alwaysReturnNow=AlwaysReturnNow;
	}
	///<summary>
	///Converts to string. Implements replacing the following so far: yyyy, MMM, MM, dd, HH, mm, ss, ff [always becomes 00], t [only becomes P or A]
	///--NOT YET IMPLEMENTED: escape sequences for literals
	///--NOT YET IMPLEMENTED: single-character styles
	///</summary>
	string DateTime::ToString(string format) {
		string returnString=format;
		if (alwaysReturnNow) time(&rawtime);
		string stdTimeString(ctime(&rawtime));
		int monthNumber = DateTime::monthNameWithUppercaseFirstCharacterToDigit(stdTimeString.substr(4,3));
		string monthNumberStringMin1Char = Convert::ToString(monthNumber);
		string monthNumberStringMin2Chars = Convert::ToString(monthNumber);
		if (monthNumberStringMin2Chars.length()<2) monthNumberStringMin2Chars = "0"+monthNumberStringMin2Chars;
		string hourNumberMilitaryStringMin2Chars = stdTimeString.substr(11,2);
		string hourNumberMilitaryStringMin1Char = hourNumberMilitaryStringMin2Chars;
		if (hourNumberMilitaryStringMin1Char.substr(0,1)=="0") hourNumberMilitaryStringMin1Char=hourNumberMilitaryStringMin1Char.substr(1,1);
		int hourNumberMilitary=Convert::ToInt32(hourNumberMilitaryStringMin1Char);
		bool isPostMeridian = hourNumberMilitary>=12;
		int hourNonMilitary=hourNumberMilitary;
		if (hourNonMilitary==0) hourNonMilitary=12;
		else if (hourNonMilitary>=13) hourNonMilitary-=12;
		string meridianFlagCharacterAsString="A";
		if (isPostMeridian) meridianFlagCharacterAsString="P";
		// C++ standard library's ctime function ALWAYS converts time to string as:
		// Www Mmm dd hh:mm:ss yyyy
		// 012345678901234567890123
		// 0		10		20
		returnString = ExpertMultimediaBase::String_Replace(returnString, "yyyy", stdTimeString.substr(20,4));
		returnString = ExpertMultimediaBase::String_Replace(returnString, "MMM", stdTimeString.substr(4,3));
		returnString = ExpertMultimediaBase::String_Replace(returnString, "MM", monthNumberStringMin2Chars);
		returnString = ExpertMultimediaBase::String_Replace(returnString, "dd", stdTimeString.substr(8,2));
		returnString = ExpertMultimediaBase::String_Replace(returnString, "h", hourNumberMilitaryStringMin1Char);
		returnString = ExpertMultimediaBase::String_Replace(returnString, "HH", stdTimeString.substr(11,2));
		returnString = ExpertMultimediaBase::String_Replace(returnString, "mm", stdTimeString.substr(14,2));
		returnString = ExpertMultimediaBase::String_Replace(returnString, "ss", stdTimeString.substr(17,2));
		returnString = ExpertMultimediaBase::String_Replace(returnString, "ff", "00");
		returnString = ExpertMultimediaBase::String_Replace(returnString, "t", meridianFlagCharacterAsString);
		return returnString;
	}//end ToString, DateTime
	int DateTime::monthNameWithUppercaseFirstCharacterToDigit(string mustBeThreeLettersFirstUppercase) {
	  int monthNum=-1;
	  if (mustBeThreeLettersFirstUppercase=="Jan") monthNum=1;
	  else if (mustBeThreeLettersFirstUppercase=="Feb") monthNum=2;
	  else if (mustBeThreeLettersFirstUppercase=="Mar") monthNum=3;
	  else if (mustBeThreeLettersFirstUppercase=="Apr") monthNum=4;
	  else if (mustBeThreeLettersFirstUppercase=="May") monthNum=5;
	  else if (mustBeThreeLettersFirstUppercase=="Jun") monthNum=6;
	  else if (mustBeThreeLettersFirstUppercase=="Jul") monthNum=7;
	  else if (mustBeThreeLettersFirstUppercase=="Aug") monthNum=8;
	  else if (mustBeThreeLettersFirstUppercase=="Sep") monthNum=9;
	  else if (mustBeThreeLettersFirstUppercase=="Oct") monthNum=10;
	  else if (mustBeThreeLettersFirstUppercase=="Nov") monthNum=11;
	  else if (mustBeThreeLettersFirstUppercase=="Dec") monthNum=12;
	  else {
			ExpertMultimediaBase::Console::Error.WriteLine("ERROR: unrecognized month name \""+mustBeThreeLettersFirstUppercase+"\"");
	  }
	  return monthNum;
	}//end monthNameWithUppercaseFirstCharacterToDigit

	/////////////////////////////  Convert  /////////////////////////////

	byte Convert::ToByte(int val) {
		return (val<=0) ? (0) : ((val>=255)?255:(byte)(val));
	}
	byte Convert::ToByte(long val) {
		return (val<=0) ? (0) : ((val>=255)?255:(byte)(val));
	}
	#ifdef TYPE__int64
	byte Convert::ToByte(__int64 val) {
		return (val<=0) ? (0) : ((val>=255)?255:(byte)(val));
	}
	#endif // TYPE__int64
	byte Convert::ToByte(size_t val) {
		return ((val>=255)?255:(byte)(val)); //(val<=0) ? (0) :
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
	int Convert::ToInt32(string val) {
		//return std::stoi(val); FAILS on some versions of MinGW if not up to date with standard GNU compiler [sometimes compiling with the following command fixes it: g++ -std=c++0x]
		//NOTE: 9223372036854775807 is max 64-bit integer (18 characters)
		//	  0123456789012345678
		//	  0		10
		static string digits="0123456789";
		int runningVal=0;
		int negative1Or1=1;
		if (val.substr(0,1)=="-") {
			negative1Or1=-1;
			val=val.substr(1,val.length()-1);
		}
		int charIndex=val.length()-1;
		int multiplier=1;
		while (charIndex>=0) {
			size_t thisDigit=digits.find(val.substr(charIndex,1));
			if (thisDigit != string::npos) {
				runningVal+=(int)thisDigit*multiplier;
			}
			charIndex--;
			multiplier*=10;
		}
		return runningVal*negative1Or1;
	}//end ToInt32


	decimal Convert::ToDecimal(byte val) {
		return (decimal)val;
	}
	decimal Convert::ToDecimal(int val) {
		return (decimal)val;
	}
	decimal Convert::ToDecimal(float val) {
		return (decimal)val;
	}
	decimal Convert::ToDecimal(double val) {
		return (decimal)val;
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
	string Convert::ToString(decimal val) {
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
	string Convert::ToString(size_t val) {
		stringstream ssReturn;
		string sReturn;
		ssReturn << val;
		ssReturn >> sReturn;
		return sReturn;
	}
	#ifdef TYPE__int64
	string Convert::ToString(__int64 val) {
		stringstream ssReturn;
		string sReturn;
		ssReturn << val;
		ssReturn >> sReturn;
		return sReturn;
	}
	#endif // TYPE__int64
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
	#ifdef TYPE__int64
	string Convert::ToString(unsigned __int64 val) {
		stringstream ssReturn;
		string sReturn;
		ssReturn << val;
		ssReturn >> sReturn;
		return sReturn;
	}
	#endif // TYPE__int64



	}//end System

	namespace ExpertMultimediaBase {
	bool bReverseEndian=false;
	//string FileMode_OpenRead;//must be declared BEFORE staticconstructors
	//string FileMode_OpenWrite;//must be declared BEFORE staticconstructors
	int iConsoleLinesMax=3000;
	int iConsoleLinesWritten=0;
	const decimal M_NEG1=(decimal)-1.0;
	const decimal M_0=(decimal)0.0;
	const decimal M_0_1=(decimal)0.1;
	const decimal M_0_5=(decimal)0.5;
	const decimal M_1=(decimal)1.0;
	const decimal M_2=(decimal)2.0;
	StaticConstructors staticconstructors;

	StaticConstructors::StaticConstructors() {
		cerr<<"loading static constructors..."<<flush;
		FileMode::OpenWrite="wb";//FileMode_OpenWrite="wb";//FileMode::OpenWrite="wb";
		FileMode::OpenRead="rb";//FileMode_OpenRead="rb";//FileMode::OpenRead="rb";
		//FileMode::staticconstructorFileMode();
		cerr<<"iConsoleLinesWritten:"<<iConsoleLinesWritten<<"..."<<flush;
		cerr<<"iConsoleLinesMax:"<<iConsoleLinesMax<<"..."<<flush;
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
	float System_Math_Round(float val) {
		return (float)(round(val));//(float)( (val)>=0 ? (long double)((val)+.5) : (long double)((val)-.5) );
	}
	double System_Math_Round(double val) {
		return round(val);//(double)( (val)>=0 ? (long double)((val)+.5) : (long double)((val)-.5) );
	}
	decimal System_Math_Round(decimal val) {
		return (decimal)round((long double)val);//(decimal)( (val)>=0 ? (long double)((val)+.5) : (long double)((val)-.5) );
	}
	float System_Math_Floor(float val) {
		return ( (float)((int)(val)) );
	}
	double System_Math_Floor(double val) {
		return ( (double)((long)(val)) );
	}
	decimal System_Math_Floor(decimal val) {
		return (decimal)( (long long)(val) );
	}

	float System_Math_Ceiling(float val) {
		return (  (float)(  ( (int)((
			(((val)>(1.0f)) ? ( (val) - (float)(int((val)/(1.0f)))*(1.0f)) : 0.0f )//floating modulus of 1
			)>0.0f) ) ? ((int)(val))+1 : (int)(val) )  );
	}
	double System_Math_Ceiling(double val) {
		return (  (double)(  ( (long)((
			(((val)>(1.0d)) ? ( (val) - (double)(long((val)/(1.0d)))*(1.0d)) : 0.0d )//floating modulus of 1
			)>0.0d) ) ? ((long)(val))+1 : (long)(val) )  );
	}
	decimal System_Math_Ceiling(decimal val) {
		return (  (decimal)(  ( (long long)((
			(((val)>(M_1)) ? ( (val) - (decimal)((long long)((val)/(M_1)))*(M_1)) : M_0 )//floating modulus of 1
			)>M_0) ) ? ((long long)(val))+1 : (long long)(val) )  );
	}
	int System_Math_Abs(int val) {
		return abs(val);
	}
	float System_Math_Abs(float val) {
		return abs(val);
	}
	double System_Math_Abs(double val) {
		return abs(val);
	}
	decimal System_Math_Abs(decimal val) {
		return abs((long double)val);//TODO: fix truncation by checking how decimal is defined & calling appropriate method if exists
	}

	float System_Math_Atan2(float y, float x) {
		return atan2(y,x);
	}
	double System_Math_Atan2(double y, double x) {
		return atan2(y,x);
	}
	decimal System_Math_Atan2(decimal y, decimal x) {
		return (decimal)atan2((double)y,(double)x);//TODO: fix truncation by checking how decimal is defined & calling appropriate method if exists
	}

	///////////////////////////// String members as functions /////////////
	string String_Replace(string haystack, string oldValue, string newValue) {
	size_t found = haystack.find(oldValue);
	if (oldValue!=newValue) {
		while (found != string::npos) {
			haystack.replace(found, oldValue.length(), newValue);
			found = haystack.find(oldValue);
		}
	}
	return haystack;
	}

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
		pfile=nullptr;
		Position=0;
		sModePrev="";
	}
	Stream::~Stream() {
		Close();
	}
	void Stream::Close() {
		if (pfile!=nullptr) {
			fclose(pfile);
			pfile=nullptr;
		}
		sModePrev="";
	}
	size_t Stream::fread(void* _DstBuf, size_t _ElementSize, size_t _Count) {
		size_t iReturn=0;
		if (pfile!=nullptr) {
			iReturn=::fread(_DstBuf,_ElementSize,_Count,this->pfile);
			if (iReturn>0) Position+=_ElementSize*iReturn;
		}
		return iReturn;
	}
	size_t Stream::fwrite(const void* _Str, size_t _ElementSize, size_t _Count) {
		size_t iReturn=0;
		if (pfile!=nullptr) {
			iReturn=::fwrite(_Str,_ElementSize,_Count,this->pfile);
			if (iReturn>0) Position+=_ElementSize*iReturn;
		}
		return iReturn;
	}
	void Stream::Open(const char* _Filename, const char* _Mode) {
		sModePrev=Convert::ToString(_Mode);
		if (pfile!=nullptr) {
			fclose(pfile);
			pfile=nullptr;
		}
		if (_Mode!=nullptr) {
			if (_Mode[0]!='\0') pfile=::fopen(_Filename, _Mode);
			else cerr<<"Stream::Open Error: zero-length Mode {FileMode::OpenWrite:"<<FileMode::OpenWrite<<"; FileMode::OpenRead:"<<FileMode::OpenRead<<"}"<<endl;
		}
		else cerr<<"Stream::Open Error: null Mode char*"<<endl;
	}
	bool Stream::CanRead() {
		return (pfile!=nullptr)&&(sModePrev.find_first_of('r',0)!=string::npos);
	}
	bool Stream::CanWrite() {
		return pfile!=nullptr&&(sModePrev.find_first_of('w',0)!=string::npos);
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
			if (bFirstRun) cerr<<"done File::Exists...";
		}
		catch (exception& exn) {
			bReturn=false;
			cerr<<"File::Exists check:"<<exn.what();//avoids base.cpp -- ShowExn(exn,"File.Exists",sMsg);
		}
		catch (...) {
			bReturn=false;
			cerr<<"File::Exists check could not finish (Unknown exception).";//avoids base.cpp -- ShowUnknownExn("File.Exists",sMsg);
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
		filestream=nullptr;
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
			if (filestream!=nullptr) {
				filestream->Close();
				delete filestream;
				filestream=nullptr;
			}
		}
	}
	void BinaryReader::Close() {
		if (filestream!=nullptr) {
			filestream->Close();
			delete filestream;
			filestream=nullptr;
		}
	}
	long long BinaryReader::BaseStream_Position() {
		return (filestream!=nullptr)?filestream->BaseStream.getPosition():-1;
	}
	bool BinaryReader::BaseStream_CanRead() {
		return ((filestream!=nullptr)&&filestream->BaseStream.CanRead());
	}
	int BinaryReader::Read_octets(unsigned char* buffer, int indexInBuffer, int count) {
		int iFound=0;
		size_t ElementSize=1;
		if (filestream!=nullptr) {
			iFound=ElementSize*this->filestream->BaseStream.fread(&buffer[indexInBuffer],ElementSize,count);
		}
		return iFound;
	}
	unsigned char BinaryReader::ReadByte(){
		unsigned char valReturn=(unsigned char)0;
		int iFound=0;
		size_t ElementSize=1;
		if (filestream!=nullptr) {
			iFound=ElementSize*this->filestream->BaseStream.fread(&valReturn,ElementSize,1);
		}
		return valReturn;
	}
	char BinaryReader::ReadChar_octect(){
		char valReturn=(char)0;
		int iFound=0;
		size_t ElementSize=1;
		if (filestream!=nullptr) {
			iFound=ElementSize*this->filestream->BaseStream.fread(&valReturn,ElementSize,1);
		}
		return valReturn;
	}
	string BinaryReader::ReadChars_octects(int count){
		char* carr=(char*)malloc(count+1);
		int iFound=0;
		size_t ElementSize=1;
		if (filestream!=nullptr) {
			iFound=ElementSize*this->filestream->BaseStream.fread(carr,ElementSize,count);
		}
		carr[count]='\0';//ok since size is count+1
		string sReturn=Convert::ToString(carr);
		free(carr);
		carr=nullptr;
		return sReturn;
	}
	bool bShowElementSizeErr=true;
	unsigned short BinaryReader::ReadUInt16(){
		unsigned short valReturn=(unsigned short)0;
		size_t iFound=0;
		size_t ElementSize=2;
		if (filestream!=nullptr) {
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
		size_t iFound=0;
		size_t ElementSize=4;
		if (filestream!=nullptr) {
			iFound=ElementSize*this->filestream->BaseStream.fread(&valReturn,ElementSize,1);
		}
		return valReturn;
	}

	/////////////////////////////  BinaryWriter  /////////////////////////////


	BinaryWriter::BinaryWriter() {
		filestream=nullptr;
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
			if (filestream!=nullptr) {
				filestream->Close();
				delete filestream;
				filestream=nullptr;
			}
		}
	}
	void BinaryWriter::Close() {
		if (filestream!=nullptr) {
			filestream->Close();
			delete filestream;
			filestream=nullptr;
		}
	}
	long long BinaryWriter::BaseStream_Position() {
		return (filestream!=nullptr)?filestream->BaseStream.getPosition():-1;
	}
	void BinaryWriter::Write(unsigned char val) {
		long long iGot=0;
		if (filestream!=nullptr) {
			iGot=(long long)this->filestream->BaseStream.fwrite(&val,1,1);
		}
	}
	void BinaryWriter::Write(string val) {
		long long iGot=0;
		if (filestream!=nullptr) {
			const char* carr=val.c_str();
			for (unsigned int iChar=0; iChar<val.length(); iChar++) {
				iGot=(long long)this->filestream->BaseStream.fwrite(&carr[iChar],1,1);
			}
		}
	}
	void BinaryWriter::Write(unsigned short val) {
		long long iGot=0;
		if (filestream!=nullptr) {
			if (bReverseEndian) {
				iGot=(long long)this->filestream->BaseStream.fwrite((&val)+1,1,1);
				iGot+=(long long)this->filestream->BaseStream.fwrite((&val),1,1);
			}
			else iGot=(long long)this->filestream->BaseStream.fwrite(&val,2,1);
		}
	}
	void BinaryWriter::Write(unsigned int val) {
		long long iGot=0;
		if (filestream!=nullptr) {
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
		if (filestream!=nullptr) {
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
