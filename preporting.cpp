#ifndef PREPORTING_CPP
#define PREPORTING_CPP

#include "preporting.h"

namespace ExpertMultimediaBase {
	string PReporting::sParticiple="(unknown)";
	int PReporting::iDebugLevel=0;
	void PReporting::setParticiple(string participle) {
		sParticiple=participle;
		if (getIsMegaDebug()) {
			cerr<<participle<<"..."<<flush;
		}
	}
	//void setDebugLevel(int iModifyBy) {
	//	iDebugLevel+=iModifyBy;
	//}
	bool PReporting::getIsMegaDebug() {
		return iDebugLevel>=2;
	}
	void PReporting::setIsMegaDebug(bool val) {
		//always use +/- 2 to accont for recursion
		if (val) { if (iDebugLevel<2) iDebugLevel+=2; }
		else {
			while (iDebugLevel>=2) iDebugLevel-=2;
		}
	}
	bool PReporting::getIsUltraDebug() {
		return iDebugLevel>=3;
	}
	int PReporting::getDebugLevel() {
		return iDebugLevel;
	}
	void PReporting::setIsUltraDebug(bool val) {
		//always use +/- 2 to accont for recursion
		if (val) { if (iDebugLevel<3) iDebugLevel+=3; }
		else {
			while (iDebugLevel>=3) iDebugLevel-=3;
		}
	}

	void PReporting::ShowExn(exception& exn) {
		PReporting::ShowExn(exn,"","");
	}
	void PReporting::ShowExn(exception& exn, string sParticiple) {
		PReporting::ShowExn(exn,sParticiple,"");
	}
	void PReporting::ShowExn(exception& exn, string sParticiple, string sWhere) {
		cerr<<"Could not finish";
		if (sWhere.length()>0) cerr<<" in "<<sWhere;
		if (sParticiple.length()>0) cerr<<" while "<<sParticiple;
		cerr<<":";
		cerr<<exn.what();
		cerr<<endl;
	}
	void PReporting::Debug(exception& exn, string sParticiple, string sWhere) {
		cerr<<"Debug: skipped operation since could not finish";
		if (sWhere.length()>0) cerr<<" in "<<sWhere;
		if (sParticiple.length()>0) cerr<<" while "<<sParticiple;
		cerr<<":";
		cerr<<exn.what();
		cerr<<endl;
	}
	///<summary>
	///Make sure you have used PReporting::setParticiple if you use this overload,
	///as it will use that value in the message!
	///</summary>
	void PReporting::ShowUnknownExn(string sWhere) {
		ShowUnknownExn(PReporting::sParticiple,sWhere);
	}
	void PReporting::ShowUnknownExn(string sParticiple, string sWhere) {
		cerr<<"Could not finish";
		if (sWhere.length()>0) cerr<<" in "<<sWhere;
		if (sParticiple.length()>0) cerr<<" while "<<sParticiple;
		cerr<<" (unknown exception type).";
		cerr<<endl;
	}
	void PReporting::ShowErr(string sErr, string sParticiple, string sWhere) {
		if (sErr.length()>0) cerr<<"Error";
		else cerr<<"Unrecorded error";
		if (sWhere.length()>0) cerr<<" in "<<sWhere;
		if (sParticiple.length()>0) cerr<<" while "<<sParticiple;
		if (sErr.length()>0) cerr<<":"<<sErr;
		else cerr<<".";
		cerr<<endl;
	}
} //end namespace

#endif
