#ifndef PREPORTING_H
#define PREPORTING_H

#include <cstdio>
#include <string>
#include <iostream>


using namespace std;

namespace ExpertMultimediaBase {
	class PReporting {
	private:
		static string sParticiple;
		static int iDebugLevel;
	public:
		static void setParticiple(string participle);
		static void setIsMegaDebug(bool val);
		static bool getIsMegaDebug();
		static void setIsUltraDebug(bool val);
		static bool getIsUltraDebug();
		static int getDebugLevel();
		static void ShowExn(exception& exn);
		static void ShowExn(exception& exn, string sParticiple);
		static void ShowExn(exception& exn, string sParticiple, string sWhere);
		static void Debug(exception& exn, string sParticiple, string sWhere);
		static void ShowUnknownExn(string sWhere);
		static void ShowUnknownExn(string sParticiple, string sWhere);//do NOT allow 1-string version, because some early code (e.g. old version of targa.cpp) uses sFunction as 1st param!
		static void ShowErr(string sErr, string sParticiple, string sWhere); //do NOT allow 2-string version, because some early code (e.g. old version of targa.cpp) uses sFunction as 2nd param!
	};
} //end namespace

#endif
