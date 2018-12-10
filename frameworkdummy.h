#ifndef DOTNETFAKE_H
#define DOTNETFAKE_H

#include <SDL/SDL_types.h> //gives us Uint32 etc
#include <float.h>
#include <cstdio>
using namespace std;

namespace ExpertMultimediaBase {
	int iConsoleLinesMax=3000;
	int iConsoleLinesWritten=0;
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
	const float float_MaxValue=FLT_MAX;
	const float double_MaxValue=DBL_MAX;
	const int int_MaxValue=INT_MAX;//2147483647;
	const long long_MaxValue=LONG_MAX;//9223372036854775807;
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
	class FILE_FAKENAMESPACE_CLASS {
	public:
		bool Exists(string sFile);
	};
	class ERROR_FAKENAMESPACE_CLASS {
	public:
        void Write(string val);
        void WriteLine();
        void WriteLine(string val);
	};
	class CONSOLE_FAKENAMESPACE_CLASS {
	public:
		void Write(string val);
		void WriteLine();
		void WriteLine(string val);
        ERROR_FAKENAMESPACE_CLASS Error;
	};
	FILE_FAKENAMESPACE_CLASS File;
	CONSOLE_FAKENAMESPACE_CLASS Console;
}//end namespace
#endif
