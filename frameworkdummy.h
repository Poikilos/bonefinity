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

namespace ProtoArmor {
	extern int iConsoleLinesMax;
	extern int iConsoleLinesWritten;
	extern bool bReverseEndian;
	//#ifndef float_MaxValue
	//#define float_MaxValue FLT_MAX
	//#endif
	//#ifndef double_MaxValue
	//#define double_MaxValue DBL_MAX
	//#endif
	//#ifndef int_MaxValue
	//#define int_MaxValue INT_MAX  //2147483647;
	//#endif
	//#ifndef long_MaxValue
	//#define long_MaxValue LONG_MAX  //9223372036854775807
	//#endif

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
	#ifndef ushort
	typedef unsigned short ushort;
	#endif
	#ifndef ref_byte
	#define ref_byte byte&
	#endif
	#ifndef ref_ushort
	#define ref_ushort ushort&
	#endif
	#ifndef ref_int
	#define ref_int int&
	#endif
	#ifndef ref_uint
	#define ref_uint uint&
	#endif
	#ifndef ref_Rectangle
	#define ref_Rectangle Rectangle&
	#endif
	#ifndef ref_RectangleF
	#define ref_RectangleF RectangleF&
	#endif

	class RectangleF {
	public:
		float X;
		float Y;
		float Width;
		float Height;
		void Set(float x, float y, float width, float height);
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
