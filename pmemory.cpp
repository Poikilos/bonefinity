#ifndef PMEMORY_CPP
#define PMEMORY_CPP

#include "pmemory.h"
#include <cstdlib> //malloc, free
//#include <cstring>

using namespace std;

namespace ExpertMultimediaBase {
	void PMemory::SafeFree(byte* &reftopointer_WillBeSetToNull) {
		try {
			if (reftopointer_WillBeSetToNull!=NULL) free(reftopointer_WillBeSetToNull);
			reftopointer_WillBeSetToNull=NULL;
		}
		catch (exception& exn) {//!!!!!!!!!!TODO: finish this: copy this to ALL catch statements EVERYWHERE!!!!!!!!!!!!!!!!!!!!!!!!!!!
			PReporting::ShowExn(exn, "SafeFree(byte*)");
		}
		catch (...) {
			PReporting::ShowUnknownExn("","SafeFree(byte*)");
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
			if ((dest!=NULL)&&dest_OldLength>0) {
				byte* arrayNew=(byte*)malloc(dest_NewLength);
				for (unsigned int iNow=0; iNow<dest_NewLength; iNow++) {
					if (iNow<dest_OldLength) arrayNew[iNow]=dest[iNow];
					else break;//arrayNew[iNow]=0;
				}
				free(dest);
				dest=arrayNew;
				arrayNew=NULL;
			}
			else dest=(byte*)malloc(dest_NewLength);
		}
	}
}//end namespace


#endif
