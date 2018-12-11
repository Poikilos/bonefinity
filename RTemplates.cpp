#include "RTemplates.h"
#include <iostream>
using namespace std;//NULL etc

//compilers don't like dividing class templates:

namespace exm {
	/*
	template <typename T> nondumbset<T>::nondumbset()
	{
		InitNull();
		set_Maximum(nondumbset_DefaultMax);
	}
	template <typename T> nondumbset<T>::nondumbset(int maximum)
	{
		InitNull();
		set_Maximum(maximum);
	}
	template <typename T> nondumbset<T>::~nondumbset() {
		ForceDestruct();
	}
	template <typename T> void nondumbset<T>::ForceDestruct() {
		//TODO(?): use
		//lplpval = new T*[size]
		//and
		//delete [] lplpval
		//?
		if (lplpval!=NULL) {
			for (int i=0; i<iUsed; i++) {
				if (lplpval[i]!=NULL) {
					delete (lplpval[i]);
					lplpval[i]=NULL;
				}
			}
		}
	}
	template <typename T> void nondumbset<T>::InitNull() {
		lplpval=NULL;
		lplpval_Length=-1;
		iUsed=0;
	}
	template <typename T> void nondumbset<T>::set_Maximum(int maximum) {
		if (maximum>0) {
			if (maximum<iUsed) {
				cerr<<"warning: nondumbset can't set max to "<<maximum<<" since below "<<iUsed<<" used--setting to "<<iUsed<<" instead."<<endl;
				maximum=iUsed;//TODO: debug unpredictable memory usage
			}
			if (lplpval!=NULL) {
				T** lplpvalOld=lplpval;
				lplpval=NULL;
				set_Maximum(maximum);//recursion is safe since this case won't run since just set lplpval=NULL
				int set_Maximum_result=get_Maximum();
				for (int i=0; i<set_Maximum_result; i++) {
					lplpval[i]=(i<iUsed)?(lplpvalOld[i]):(NULL);
				}
			}
			else {
				lplpval=new T *[maximum];//lplpval=(T**)malloc(sizeof(T*)*maximum);
				memset(lplpval,'\0',maximum*sizeof(T*));
				//for (int i=0; i<maximum; i++) {
				//	lplpval[i]=NULL;
				//}
				lplpval_Length=maximum;
			}
		}
		else {
			ForceDestruct();
		}
	}//end set_Maximum
	template <typename T> int nondumbset<T> ::get_Maximum() {
		return (lplpval!=NULL)?lplpval_Length:-1;
	}
	///<summary>
	///usage:
	///MyClass* lpmc=myclassnondumbset[3];
	///int iWantThatValueAt3=-1;
	///if (lpmc!=NULL) iWantThatValueAt3 = lpmc->iValue;
	///</summary>
	template <typename T> T* nondumbset<T> ::operator[](int index) {
		return (   ( lplpval!=NULL )  ?  ( (index>=0) ? ((index<iUsed)?(lplpval[index]):(NULL)) : NULL )  :  NULL   );
	}
	*/
}//end namespace
