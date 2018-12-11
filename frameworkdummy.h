#ifndef FRAMEWORKDUMMY_H
#define FRAMEWORKDUMMY_H

#include <cstdio>
#include <string>
#include <sstream>
#include <cfloat> //<float.h> //maximums i.e. LONG_MAX
#include <climits> //<limits.h> //maximums i.e. INT_MAX
using namespace std;

namespace FileMode {
	extern string OpenRead;
	extern string OpenWrite;
}

namespace System {
	#ifndef null
	#define null NULL
	#endif
	#ifndef byte
	typedef unsigned char byte;
	#endif
	#ifndef UInt32
	typedef unsigned int UInt32;
	#endif
	#ifndef uint
	typedef unsigned int uint;
	#endif
	#ifndef UInt64
	typedef unsigned long UInt64;
	#endif
	#ifndef ulong
	typedef unsigned long ulong;
	#endif
	#ifndef ushort
	typedef unsigned short ushort;
	#endif

	#ifndef ref_string
	#define ref_string string&
	#endif
	#ifndef out_string
	#define out_string string&
	#endif
	#ifndef ref_byte
	#define ref_byte byte&
	#endif
	#ifndef out_byte
	#define out_byte byte&
	#endif
	#ifndef ref_ushort
	#define ref_ushort ushort&
	#endif
	#ifndef out_ushort
	#define out_ushort ushort&
	#endif
	#ifndef ref_int
	#define ref_int int&
	#endif
	#ifndef out_int
	#define out_int int&
	#endif
	#ifndef ref_uint
	#define ref_uint uint&
	#endif
	#ifndef out_uint
	#define out_uint uint&
	#endif
	#ifndef ref_long
	#define ref_long long&
	#endif
	#ifndef out_long
	#define out_long long&
	#endif
	#ifndef ref_ulong
	#define ref_ulong ulong&
	#endif
	#ifndef out_ulong
	#define out_ulong ulong&
	#endif
	#ifndef ref_float
	#define ref_float float&
	#endif
	#ifndef out_float
	#define out_float float&
	#endif
	#ifndef ref_double
	#define ref_double double&
	#endif
	#ifndef out_double
	#define out_double double&
	#endif
	#ifndef ref_Rectangle
	#define ref_Rectangle Rectangle&
	#endif
	#ifndef out_Rectangle
	#define out_Rectangle Rectangle&
	#endif
	#ifndef ref_RectangleF
	#define ref_RectangleF RectangleF&
	#endif
	#ifndef out_RectangleF
	#define out_RectangleF RectangleF&
	#endif

	#ifndef decimal_MinValue
	#define decimal_MinValue LDBL_MIN
	#endif
	#ifndef decimal_MaxValue
	#define decimal_MaxValue LDBL_MAX
	#endif

	#ifndef System_Math_Asin
	#define System_Math_Asin(x) asin(x)
	#endif

	#ifndef float_IsPositiveInfinity
	#define float_IsPositiveInfinity(val)  ((isinf(val)&&(val)>0))
	#endif
	#ifndef float_IsNegativeInfinity
	#define float_IsNegativeInfinity(val)  (isinf(val)&&((val)<0))
	#endif
	#ifndef float_IsNaN
	#define float_IsNaN(val)  ((isnan(val)))
	#endif
	///NOTE: if fpclassify returns FP_SUBNORMAL, then number is so close zero as to be negligible!

	#ifndef double_IsPositiveInfinity
	#define double_IsPositiveInfinity(val) ((isinf(val)&&(val)>0))
	#endif
	#ifndef double_IsNegativeInfinity
	#define double_IsNegativeInfinity(val) (isinf(val)&&((val)<0))
	#endif
	#ifndef double_IsNaN
	#define double_IsNaN(val) ((isnan(val)))
	#endif


	#ifdef __float128
	typedef __float128 decimal;
	#elseif _Quad
	typedef _Quad decimal;
	#else
	typedef long double decimal;
	#endif


	#ifndef ref_decimal
	#define ref_decimal decimal&
	#endif

	#ifndef float_MaxValue
	#define float_MaxValue FLT_MAX
	#endif
	#ifndef double_MaxValue
	#define double_MaxValue DBL_MAX
	#endif
	#ifndef short_MinValue
	#define short_MinValue SHRT_MIN
	#endif
	#ifndef short_MaxValue
	#define short_MaxValue SHRT_MAX
	#endif
	#ifndef int_MinValue
	#define int_MinValue INT_MIN  //2147483647;
	#endif
	#ifndef int_MaxValue
	#define int_MaxValue INT_MAX  //2147483647;
	#endif
	#ifndef long_MinValue
	#define long_MinValue LONG_MIN  //9223372036854775807
	#endif
	#ifndef long_MaxValue
	#define long_MaxValue LONG_MAX  //9223372036854775807
	#endif
	#ifndef byte_MaxValue
	#define byte_MaxValue 255
	#endif
	#ifndef ushort_MaxValue
	#define ushort_MaxValue USHRT_MAX //65535
	#endif
	#ifndef uint_MaxValue
	#define uint_MaxValue UINT_MAX  //2147483647;
	#endif
	#ifndef ulong_MaxValue
	#define ulong_MaxValue ULONG_MAX  //9223372036854775807
	#endif
	#ifndef float_MinValue
	#define float_MinValue FLT_MIN
	#endif
	#ifndef double_MinValue
	#define double_MinValue DBL_MIN
	#endif

	#ifndef System_Math_Sqrt
	#define System_Math_Sqrt(x) sqrt(x)
	#endif
	#ifndef System_Math_Min
	#define System_Math_Min(val1,val2) min(val1,val2)
	#endif

	#ifndef string_Remove
	#define string_Remove(s, start, len) ( ((s).substr(0,(start))+(s).substr((start)+(len),string::npos)) )
	#endif

}

using namespace System;

namespace ExpertMultimediaBase {
	extern int iConsoleLinesMax;
	extern int iConsoleLinesWritten;
	extern bool bReverseEndian;

	extern const decimal M_NEG1;//=(decimal)-1.0;
	extern const decimal M_0;//=(decimal)0.0;
	extern const decimal M_0_1;//=(decimal)0.1;
	extern const decimal M_0_5;//=(decimal)0.5;
	extern const decimal M_1;//=(decimal)1.0;
	extern const decimal M_2;//=(decimal)2.0;

	float System_Math_Round(float val);
	double System_Math_Round(double val);
	decimal System_Math_Round(decimal val);
	float System_Math_Floor(float val);
	double System_Math_Floor(double val);
	decimal System_Math_Floor(decimal val);
	float System_Math_Ceiling(float val);
	double System_Math_Ceiling(double val);
	decimal System_Math_Ceiling(decimal val);
	int System_Math_Abs(int val);
	float System_Math_Abs(float val);
	double System_Math_Abs(double val);
	decimal System_Math_Abs(decimal val);
	float System_Math_Atan2(float,float);
	double System_Math_Atan2(double,double);
	decimal System_Math_Atan2(decimal,decimal);



	class RectangleF {
	public:
		float X;
		float Y;
		float Width;
		float Height;
		void Set(float x, float y, float width, float height);
	};

	class SizeF {
	public:
		float Width;
		float Height;
	};

	class Rectangle {
	public:
		int X;
		int Y;
		int Width;
		int Height;
		void Set(int x, int y, int width, int height);
	};

	class Convert {
	public:
		static byte ToByte(int val);
		static byte ToByte(long val);
		static byte ToByte(__int64 val);
		static byte ToByte(size_t val);
		static byte ToByte(float val);
		static byte ToByte(double val);
		static byte ToByte(char val);
		static char ToChar8(float val); //only "__wchar_t ToChar()" and "unsigned char ToByte()" exist in actual framework (has no 8-bit ToChar method)
		static char ToChar8(double val);

		static string ToString(const char* val);
		static string ToString(bool val);
		static string ToString(unsigned char val);
		//static string ToString(__wchar_t val);
		//static string ToString(DateTime val);
		//static string ToString(Decimal val);
		static string ToString(float val);
		static string ToString(double val);
		static string ToString(decimal val);
		static string ToString(short val);
		static string ToString(int val);
		static string ToString(__int64 val);
		//static string ToString(Object* val);
		static string ToString(char val);
		//static string ToString(String* val);
		static string ToString(unsigned short val);
		static string ToString(unsigned int val);
		static string ToString(unsigned __int64 val);

		//static string ToString(bool val, IFormatProvider* pifmtp); //and so on with IFormatProvider* for each type
	};


	//extern string FileMode_OpenRead;
	//extern string FileMode_OpenWrite;

	//class FileMode {
	//public:
	//	//static string OpenRead;
	//	//static string OpenWrite;
	//	////static string Open;
	//	static void staticconstructorFileMode();
	//};

	class StaticConstructors {
	public:
		StaticConstructors();//runs equivalent of static constructors
		~StaticConstructors();//runs equivalent of static deconstructors
	};

	class Stream {
	private:
		FILE* pfile;
		long long Position;
		string sModePrev;
	public:
		long long getPosition();
		Stream();
		~Stream();
		void Close();
		void Open(const char* _Filename, const char* _Mode);
		size_t fread(void* _DstBuf, size_t _ElementSize, size_t _Count);
		size_t fwrite(const void* _Str, size_t _ElementSize, size_t _Count);
		bool CanRead();
		bool CanWrite();
	};

	class FileStream {
	public:
		Stream BaseStream;
		FileStream();
		void Close();
	};

	class File {
	public:
		static bool Exists(string sFile);
		static FileStream* Open(string sFile, string filemode);
	};

	//class ref_BaseStream {
	//	long long Position;
	//};

	//class ref_Stream {
	//public:
	//	long long Position;
	//};

//TODO: allow binReader->BaseStream->Seek(0, SeekOrigin::Begin);

	class BinaryReader {
	private:
		bool bDeleteFileStream;
		FileStream* filestream; //aka InStream
	public:
		BinaryReader();
		~BinaryReader();
		BinaryReader(FileStream* filestreamNew, bool AllowThisToDeleteFileStream);
		//Stream* BaseStream;
		void Dispose(bool bDeleteFileStreamIfNonNull);
		void Close();
		long long BaseStream_Position();
		bool BaseStream_CanRead();
		//void Close();
		//void Dispose();
		//bool Equals(Object^ obj);
		//void FillBuffer(int numBytes);
		//int PeekChar(); //this is really int in real framework
		//int Read();//one byte //this is really int in the real framework
		int Read_octets(unsigned char* buffer, int indexInBuffer, int count);
		//int Read(array<unsigned char>^ buffer, int indexInBuffer, int count);
		//int Read(array<wchar_t>^ buffer, int index, int count);
		//int Read7BitEncodedInt();
		//bool ReadBoolean(); //one byte
		unsigned char ReadByte();
		//array<unsigned char> ReadBytes(int count);//array<unsigned char>^ ReadBytes(int count);

		char ReadChar_octect();
		string ReadChars_octects(int count);
		//__wchar_t ReadChar();
		//array<wchar_t> ReadChars();//array<wchar_t>^ ReadChars();
		//Decimal ReadDecimal();
		//double ReadDouble();
		//short ReadInt16();
		//int ReadInt32();
		//long long ReadInt64();
		//signed char ReadSByte();
		//float ReadSingle();
		//String^ ReadString(); //The string is prefixed with the length, encoded as an integer seven bits at a time. //framework version really doesn't have any params
		unsigned short ReadUInt16();
		unsigned int ReadUInt32();
		//unsigned long long ReadUInt64();
	};

	class BinaryWriter {
	private:
		bool bDeleteFileStream;
		FileStream* filestream; //aka OutStream
	public:
		BinaryWriter();
		~BinaryWriter();
		BinaryWriter(FileStream* filestreamNew, bool AllowThisToDeleteFileStream);
		//Stream* BaseStream;
		void Dispose(bool bDeleteFileStreamIfNonNull);
		void Close();
		long long BaseStream_Position();
		void Write(unsigned char val);
		//void Write(array<unsigned char>^ arr);
		//void Write(wchar_t val);
		//void Write(Decimal val);
		//void Write(double val);
		//void Write(short val);
		//void Write(int val);
		//void Write(long long val);
		//void Write(signed char val);
		//void Write(float val);
		void Write(string val);//originally String^
		void Write(unsigned short val);
		void Write(unsigned int val);
		//void Write(unsigned long long val);
		void Write(unsigned char* arr, int index, int count);
		//void Write(array<unsigned char>^ arr, int index, int count);
		//void Write(array<wchar_t>^ arr, int index, int count);
	};

	class ref_ERROR_TEXTWRITER {
	public:
		static void Write(string val);
		static void WriteLine();
		static void WriteLine(string val);
		static void Flush();
	};

	class ERROR_TEXTWRITER {
	public:
		static void Write(string val);
		static void WriteLine();
		static void WriteLine(string val);
		static void Flush();
		static ref_ERROR_TEXTWRITER Out;
	};

	class ref_CONSOLE_TEXTWRITER {
	public:
		void Flush();
	};

	class Console {
	public:
		static void Write(string val);
		static void WriteLine();
		static void WriteLine(string val);
		static ERROR_TEXTWRITER Error;
		static ref_CONSOLE_TEXTWRITER Out;
	};
}//end namespace
#endif
