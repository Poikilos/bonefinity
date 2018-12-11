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
			lplpval=NULL;
			lplpval_Length=-1;
			iUsed=0;
		}
		void ForceDestruct() {
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
				delete[] lplpval;
				lplpval=NULL;
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
					//memset(lplpval,'\0',maximum*sizeof(T*));
					for (int i=0; i<maximum; i++) {
						lplpval[i]=NULL;
					}
					lplpval_Length=maximum;
				}
			}
			else {
				ForceDestruct();
			}
		}//end set_Maximum
		int get_Maximum() {
			return (lplpval!=NULL)?lplpval_Length:-1;
		}
		T* operator[](int index) {
			return (   ( lplpval!=NULL )  ?  ( (index>=0) ? ((index<iUsed)?(lplpval[index]):(NULL)) : NULL )  :  NULL   );
		}
	};

}//end namespace


#endif // RTEMPLATES_H_INCLUDED
