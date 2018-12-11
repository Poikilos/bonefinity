#ifndef PMEMORY_H
#define PMEMORY_H

#include <string>
#include <preporting.h>
#include <frameworkdummy.h>
// for cpp:
#include <cstdlib> //malloc, free
// #include <cstring>

using namespace std;

namespace ExpertMultimediaBase {
	class PMemory {
	public:
		static void SafeFree(byte* &reftopointer_WillBeSetToNull, string DebugNote);
		static void Push(byte* &dest, unsigned int& dest_Length, unsigned int& dest_BytesUsed, byte byNow);
		static void Redim(byte* &dest, unsigned int dest_OldLength, unsigned int dest_NewLength);
	};
}//end namespace

#endif
