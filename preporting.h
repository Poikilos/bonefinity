#ifndef PREPORTING_H
#define PREPORTING_H

#include <cstdio>
#include <string>
#include <iostream>


namespace ExpertMultimediaBase {
	class PReporting {
	private:
		static std::string sParticiple;
		static int iDebugLevel;
	public:
		static void setParticiple(std::string participle);
		static void setIsMegaDebug(bool val);
		static bool getIsMegaDebug();
		static void setIsUltraDebug(bool val);
		static bool getIsUltraDebug();
		static int getDebugLevel();
		static void ShowExn(std::exception& exn);
		static void ShowExn(std::exception& exn, std::string sParticiple);
		static void ShowExn(std::exception& exn, std::string sParticiple, std::string sWhere);
		static void Debug(std::exception& exn, std::string sParticiple, std::string sWhere);
		static void ShowUnknownExn(std::string sWhere);
		static void ShowUnknownExn(std::string sParticiple, std::string sWhere);//do NOT allow 1-std::string version, because some early code (e.g. old version of targa.cpp) uses sFunction as 1st param!
		static void ShowErr(std::string sErr, std::string sParticiple, std::string sWhere); //do NOT allow 2-std::string version, because some early code (e.g. old version of targa.cpp) uses sFunction as 2nd param!
	};
} //end namespace

#endif
