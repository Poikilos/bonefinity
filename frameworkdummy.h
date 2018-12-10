#ifndef DOTNETFAKE_H
#define DOTNETFAKE_H

#include <SDL/SDL_types.h> //gives us Uint32 etc
#include <cstdio>
#include <string>
#include <float.h> //maximums i.e. LONG_MAX
#include <limits.h> //maximums i.e. INT_MAX
using namespace std;

namespace ExpertMultimediaBase {
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
	class FILE_FAKESTATIC_CLASS {
	public:
		bool Exists(string sFile);
	};
	class ref_ERROR_TEXTWRITER {
	public:
        void Write(string val);
        void WriteLine();
        void WriteLine(string val);
        void Flush();
	};
	class ERROR_TEXTWRITER {
	public:
        void Write(string val);
        void WriteLine();
        void WriteLine(string val);
        void Flush();
        ref_ERROR_TEXTWRITER Out;
	};
	class CONSOLE_TEXTWRITER {
	public:
        void Flush();
	};
	class CONSOLE_FAKESTATIC_CLASS {
	public:
		void Write(string val);
		void WriteLine();
		void WriteLine(string val);
		ERROR_TEXTWRITER Error;
		CONSOLE_TEXTWRITER Out;
	};
	extern CONSOLE_FAKESTATIC_CLASS Console;
	extern FILE_FAKESTATIC_CLASS File;
}//end namespace
#endif
