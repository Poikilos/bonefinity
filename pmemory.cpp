#ifndef PMEMORY_CPP
#define PMEMORY_CPP

#include <pmemory.h>

using namespace std;

namespace ExpertMultimediaBase {
	void PMemory::SafeFree(byte* &reftopointer_WillBeSetToNull, string DebugNote) {
		try {
			if (reftopointer_WillBeSetToNull!=nullptr) free(reftopointer_WillBeSetToNull);
			reftopointer_WillBeSetToNull=nullptr;
		}
		catch (exception& exn) {//!!!!!!!!!!TODO: finish this: copy this to ALL catch statements EVERYWHERE!!!!!!!!!!!!!!!!!!!!!!!!!!!
			PReporting::ShowExn(exn, "SafeFree(byte*) ("+DebugNote+")");
		}
		catch (...) {
			PReporting::ShowUnknownExn("","SafeFree(byte*) ("+DebugNote+")");
		}
	}
	void PMemory::Push(byte* &dest, unsigned int& dest_Length, unsigned int& dest_BytesUsed, byte val) {
		if (dest_BytesUsed>=dest_Length) {
			unsigned int uSizeNew=dest_BytesUsed*3/2+2;
			//unsigned int oldmin=(dest_BytesUsed<dest_Length)?dest_BytesUsed:dest_Length;
			PMemory::Redim(dest,dest_Length,uSizeNew);
			dest_Length=uSizeNew;
		}
		dest[dest_BytesUsed]=val;
		dest_BytesUsed++;
	}//end PMemory::Push
	void PMemory::Redim(byte* &dest, unsigned int dest_OldLength, unsigned int dest_NewLength) {
		if (dest_OldLength!=dest_NewLength) {
			if ((dest!=nullptr)&&dest_OldLength>0) {
				byte* arrayNew=(byte*)malloc(dest_NewLength);
				for (unsigned int iNow=0; iNow<dest_NewLength; iNow++) {
					if (iNow<dest_OldLength) arrayNew[iNow]=dest[iNow];
					else break;//arrayNew[iNow]=0;
				}
				free(dest);
				dest=arrayNew;
				arrayNew=nullptr;
			}
			else dest=(byte*)malloc(dest_NewLength);
		}
	}
}//end namespace


#endif
