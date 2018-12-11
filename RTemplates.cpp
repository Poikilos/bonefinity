#ifndef RTEMPLATES_CPP
#define RTEMPLATES_CPP

#include <RTemplates.h>


using namespace std;  // nullptr etc

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
		if (lplpval!=nullptr) {
			for (int i=0; i<iUsed; i++) {
				if (lplpval[i]!=nullptr) {
					delete (lplpval[i]);
					lplpval[i]=nullptr;
				}
			}
		}
	}
	template <typename T> void nondumbset<T>::InitNull() {
		lplpval=nullptr;
		lplpval_Length=-1;
		iUsed=0;
	}
	template <typename T> void nondumbset<T>::set_Maximum(int maximum) {
		if (maximum>0) {
			if (maximum<iUsed) {
				cerr<<"warning: nondumbset can't set max to "<<maximum<<" since below "<<iUsed<<" used--setting to "<<iUsed<<" instead."<<endl;
				maximum=iUsed;//TODO: debug unpredictable memory usage
			}
			if (lplpval!=nullptr) {
				T** lplpvalOld=lplpval;
				lplpval=nullptr;
				set_Maximum(maximum);  // recursion is safe since this case won't run since just set lplpval=nullptr
				int set_Maximum_result=get_Maximum();
				for (int i=0; i<set_Maximum_result; i++) {
					lplpval[i]=(i<iUsed)?(lplpvalOld[i]):(nullptr);
				}
			}
			else {
				lplpval=new T *[maximum];  // lplpval=(T**)malloc(sizeof(T*)*maximum);
				memset(lplpval,'\0',maximum*sizeof(T*));
				//for (int i=0; i<maximum; i++) {
				//	lplpval[i]=nullptr;
				//}
				lplpval_Length=maximum;
			}
		}
		else {
			ForceDestruct();
		}
	}//end set_Maximum
	template <typename T> int nondumbset<T> ::get_Maximum() {
		return (lplpval!=nullptr)?lplpval_Length:-1;
	}
	///<summary>
	///usage:
	///MyClass* lpmc=myclassnondumbset[3];
	///int iWantThatValueAt3=-1;
	///if (lpmc!=nullptr) iWantThatValueAt3 = lpmc->iValue;
	///</summary>
	template <typename T> T* nondumbset<T> ::operator[](int index) {
		return (   ( lplpval!=nullptr )  ?  ( (index>=0) ? ((index<iUsed)?(lplpval[index]):(nullptr)) : nullptr )  :  nullptr   );
	}
	*/
}//end namespace

#endif RTEMPLATES_CPP
