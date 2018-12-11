#ifndef PMEMORY_H
#define PMEMORY_H

#include <string>
#include "preporting.h"
#include "frameworkdummy.h"

using namespace std;

namespace ExpertMultimediaBase {
	class PMemory {
	public:
		static void SafeFree(byte* &reftopointer_WillBeSetToNull);
		static void Push(byte* &dest, unsigned int& dest_Length, unsigned int& dest_BytesUsed, byte byNow);
		static void Redim(byte* &dest, unsigned int dest_OldLength, unsigned int dest_NewLength);
	};
}//end namespace

#endif
