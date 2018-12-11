#ifndef RTEMPLATES_H_INCLUDED
#define RTEMPLATES_H_INCLUDED

#include <iostream>


using namespace std;

namespace exm {
	// forward declaration of class templates used
	//template <typename T> class nondumbset;
	template <class T> class nondumbset;
	#define nondumbset_DefaultMax 1024
	//
	template <class T>
	class nondumbset {
		int lplpval_Length;
		void InitNull() {
			lplpval=nullptr;
			lplpval_Length=-1;
			iUsed=0;
		}
		void ForceDestruct() {
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
				delete[] lplpval;
				lplpval=nullptr;
			}
		}
	public:
		T** lplpval;
		int iUsed;
		nondumbset() {
			InitNull();
			set_Maximum(nondumbset_DefaultMax);
		}
		nondumbset(int maximum) {
			InitNull();
			set_Maximum(maximum);
		}
		~nondumbset() {
			ForceDestruct();
		}
		void set_Maximum(int maximum) {
			if (maximum>0) {
				if (maximum<iUsed) {
					cerr<<"warning: nondumbset can't set max to "<<maximum<<" since below "<<iUsed<<" used--setting to "<<iUsed<<" instead."<<endl;
					maximum=iUsed;//TODO: debug unpredictable memory usage
				}
				if (lplpval!=nullptr) {
					T** lplpvalOld=lplpval;
					lplpval=nullptr;
					set_Maximum(maximum);//recursion is safe since this case won't run since just set lplpval=nullptr
					int set_Maximum_result=get_Maximum();
					for (int i=0; i<set_Maximum_result; i++) {
						lplpval[i]=(i<iUsed)?(lplpvalOld[i]):(nullptr);
					}
				}
				else {
					lplpval=new T *[maximum];//lplpval=(T**)malloc(sizeof(T*)*maximum);
					//memset(lplpval,'\0',maximum*sizeof(T*));
					for (int i=0; i<maximum; i++) {
						lplpval[i]=nullptr;
					}
					lplpval_Length=maximum;
				}
			}
			else {
				ForceDestruct();
			}
		}//end set_Maximum
		int get_Maximum() {
			return (lplpval!=nullptr)?lplpval_Length:-1;
		}
		T* operator[](int index) {
			return (   ( lplpval!=nullptr )  ?  ( (index>=0) ? ((index<iUsed)?(lplpval[index]):(nullptr)) : nullptr )  :  nullptr   );
		}
	};

}//end namespace


#endif // RTEMPLATES_H_INCLUDED
