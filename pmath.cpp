#ifndef PMATH_CPP
#define PMATH_CPP

#include "pmath.h"
#include <cmath>//<math.h>
#include <cstdlib> //malloc, free
#include <cstring> //memcpy

using namespace std;

namespace ExpertMultimediaBase {
	float** PMath::pfDistCache=NULL;
	int PMath::pfDistCache_w=0;
	int PMath::pfDistCache_h=0;

	POINTINFO* PMath::ppiCache=NULL;//std::vector<POINTINFO> PMath::ppiCache;
	unsigned int PMath::ppiCache_Max=0;
	unsigned int PMath::ppiCache_Used=0;

	PIXELINFO* PMath::ppxiCache=NULL;
	bool PMath::ppxiCache_bHorizontalMovement=true;
	bool PMath::ppxiCache_bVerticalMovement=true;
	unsigned int PMath::ppxiCache_Max=0;
	unsigned int PMath::ppxiCache_Used=0;

	float PMath::f180_DIV_PI=57.2957795130823208767981548141052f; // 180/PI
	double PMath::d180_DIV_PI=57.2957795130823208767981548141052; // 180/PI
	float PMath::FPi=3.1415926535897932384626433832795f;
	double PMath::DPi=3.1415926535897932384626433832795;
	const byte PMath::by0=0; //constant to avoid type conversions
	const byte PMath::by255=255; //constant to avoid type conversions
	//long double PMath::LD180_DIV_PI=57.2957795130823208767981548141052L; // 180/PI

	PMathStatic pmathstatic;//NOTE: DOING THIS HERE prevents VirtualDub from finding plugin! (runs static constructors)

	void PMath::staticconstructorPMath() {
		//NOTE: normally this should be:
		//PMath::ResizeDistanceCache(720,480);
		//PMath::ResizePointInfoCache(720,true,true);//TODO: debug performance--change this to true, true for omnidirectional background search (calling program should set that too, but difference in params will recreate whole array
		//ResizePixelInfoCache(720*480,false);
		PMath::ResizeDistanceCache(72,48);
		PMath::ResizePointInfoCache(72,true,true);//TODO: debug performance--change this to true, true for omnidirectional background search (calling program should set that too, but difference in params will recreate whole array
		ResizePixelInfoCache(72*48,false);
	}
	void PMath::staticdeconstructorPMath() {
		if (ppxiCache!=NULL) {
			free(ppxiCache);
			ppxiCache=NULL;
		}
		if (ppiCache!=NULL) {
			free(ppiCache);
			ppiCache=NULL;
		}
		if (pfDistCache!=NULL) {
			for (int x=0; x<pfDistCache_w; x++) {
				if (pfDistCache[x]!=NULL) {
					free(pfDistCache[x]);
					pfDistCache[x]=NULL;
				}
			}
			free(pfDistCache);
			pfDistCache=NULL;
		}
	}//end staticdeconstructorPMath

	void PMath::ResizeDistanceCache(int w, int h) {
		int x;
		int y;
		if (PMath::pfDistCache_w<w||PMath::pfDistCache_h<h||PMath::pfDistCache==NULL) {
			//TODO: debug performance--preserve any existing data to avoid recalculation
			Console::Error.Write((string)"Creating PMath distance cache, size "+Convert::ToString(w)+(string)"x"+Convert::ToString(h)+(string)"...");
			Console::Out.Flush();
			if (PMath::pfDistCache!=NULL) {
				PReporting::setParticiple("freeing old pfDistCache inner arrays");
				for (x=0; x<PMath::pfDistCache_w; x++) {
					if (PMath::pfDistCache[x]!=NULL) {
						free(PMath::pfDistCache[x]);
						PMath::pfDistCache[x]=NULL;
					}
				}
				PReporting::setParticiple("freeing old pfDistCache");
				free(PMath::pfDistCache);
				PMath::pfDistCache=NULL;
			}
			PReporting::setParticiple("creating pfDistCache");
			PMath::pfDistCache=(float**)malloc(sizeof(float*)*w);
			PMath::pfDistCache_w=w;
			float fX=0.0f;
			float fY;
			for (x=0; x<w; x++) {
				PReporting::setParticiple("creating pfDistCache inner arrays");
				PMath::pfDistCache[x]=(float*)malloc(sizeof(float)*h);
				fY=0.0f;
				for (y=0; y<h; y++) {
					PMath::pfDistCache[x][y]=0.0f;
				}
				PReporting::setParticiple("setting pfDistCache inner arrays");
				for (y=0; y<h; y++) {
					PMath::pfDistCache[x][y]=PMath::ROfXY(fX,fY);
					fY+=1.0f;
				}
				fX+=1.0f;
			}
			PMath::pfDistCache_h=h;
			Console::Error.WriteLine("done. (PMath::pfDistCache["+Convert::ToString(pfDistCache_w)+"]["+Convert::ToString(pfDistCache_h)+"])");
			PReporting::setParticiple("finished creating pfDistCache");
		}//end if PMath::pfDistCache needs to be recreated
	}//end ResizeDistanceCache
	void PMath::ResizePointInfoCache(unsigned int iRadius, bool bHorizontalMovement, bool bVerticalMovement) {
		if (!bHorizontalMovement && !bVerticalMovement) {
			bHorizontalMovement=true;
			bVerticalMovement=true;
		}
		unsigned int x;
		unsigned int y;
		unsigned int iDesiredSize=iRadius*iRadius;
		PReporting::setParticiple("checking ppiCache size");
		if ( PMath::ppiCache_Max<iDesiredSize
				|| PMath::ppiCache==NULL
				|| PMath::ppxiCache_bHorizontalMovement!=bHorizontalMovement
				|| PMath::ppxiCache_bVerticalMovement!=bVerticalMovement
			) {
			try {
				//TODO: debug performance--preserve any existing data to avoid recalculation
				//POINTINFO* ppiTemp=(POINTINFO*)malloc(sizeof(POINTINFO)*iDesiredSize);
				//int ppiTemp_Used=0;
				//int ppiTemp_Max=iDesiredSize;
				Console::Error.Write("Creating PMath POINTINFO cache, size "+Convert::ToString(iRadius)+"...");
				Console::Out.Flush();
				if (PMath::ppiCache!=NULL) {
					PReporting::setParticiple("deleting old ppiCache");
					free(PMath::ppiCache);
					PMath::ppiCache=NULL;
					PMath::ppiCache_Max=0;
				}
				PReporting::setParticiple("creating new ppiCache");
				PMath::ppiCache=(POINTINFO*)malloc(sizeof(POINTINFO)*iDesiredSize);//PMath::ppiCache.resize(iDesiredSize);
				PMath::ppiCache_Max=iDesiredSize;
				PReporting::setParticiple("creating 2d flag array");
				bool** b2dDone=(bool**)malloc(sizeof(bool*)*iRadius);
				PReporting::setParticiple("initializing 2d flag array");
				for (x=0; x<iRadius; x++) {
					b2dDone[x]=(bool*)malloc(sizeof(bool)*iRadius);
					for (y=0; y<iRadius; y++) {
						b2dDone[x][y]=false;
					}
				}
				PMath::ppiCache_Used=0;
				//PReporting::setParticiple("accessing ppiCache");
				PReporting::setParticiple("accessing ppiCache index "+Convert::ToString(PMath::ppiCache_Used)+" in "+Convert::ToString(ppiCache_Max)+"-sized vector");//PMath::ppiCache.size()
				PMath::ppiCache[PMath::ppiCache_Used].iDist=0;
				PMath::ppiCache[PMath::ppiCache_Used].fDist=0.0f;
				PMath::ppiCache[PMath::ppiCache_Used].dDist=0.0;
				PMath::ppiCache[PMath::ppiCache_Used].x=0;
				PMath::ppiCache[PMath::ppiCache_Used].y=0;
				PMath::ppiCache[PMath::ppiCache_Used].fX=0.0f;
				PMath::ppiCache[PMath::ppiCache_Used].fY=0.0f;
				PMath::ppiCache[PMath::ppiCache_Used].dX=0.0;
				PMath::ppiCache[PMath::ppiCache_Used].dY=0.0;
				//PReporting::setParticiple("finished accessing ppiCache");
				PMath::ppiCache_Used++;
				PReporting::setParticiple("accessing 2d flag array");
				b2dDone[0][0]=true;
				PReporting::setParticiple("finished accessing 2d flag array");
				//NOTE: only positive values need to be cached since distance would be the same for negative
				unsigned int iFar;
				unsigned int iNear;
				double rFar;
				double rNear;
				int iFoundThisRound;
				double rRadNow=0.0;
				//int iFarStartNow=0;
				//int iNearStartNow=0;
				//int iStartNow=0;
				for (unsigned int iRadNow=0; iRadNow<iRadius; iRadNow++) {
					PReporting::setParticiple("running radius "+Convert::ToString(iRadNow));
					do {//while iFoundThisRound>0
						iFoundThisRound=0;
						rFar=0.0;//(double)iStartNow;
						iFar=(unsigned int)(rFar);
						while ( (bHorizontalMovement && bVerticalMovement) ? (iFar<=iRadNow) : (iFar==0) ) {
								//Keep retrying the smaller squares to fill the circle
								// (doing both the x loop and the y loop forms a square;
								// technically a quarter square since only positive is cached)
								rNear=0.0;
								iNear=0;
								while (iNear<=iRadNow) {//for (iNear=iStartNow; iNear<=iRadNow; iNear++) {
									double rDist=PMath::ROfXY(rFar,rNear);
									if (rDist<=rRadNow) {
										if (!b2dDone[iNear][iFar]) {
											if (bHorizontalMovement) {
												PReporting::setParticiple("adding at index "+Convert::ToString(PMath::ppiCache_Used)+" of "+Convert::ToString(PMath::ppiCache_Max));
												PMath::ppiCache[PMath::ppiCache_Used].x=iNear;
												PMath::ppiCache[PMath::ppiCache_Used].y=iFar;
												PMath::ppiCache[PMath::ppiCache_Used].fX=(float)iNear;
												PMath::ppiCache[PMath::ppiCache_Used].fY=(float)iFar;
												PMath::ppiCache[PMath::ppiCache_Used].dX=(double)iNear;
												PMath::ppiCache[PMath::ppiCache_Used].dY=(double)iFar;
												PMath::ppiCache[PMath::ppiCache_Used].iDist=(int)(rDist+.5);//+.5 for rounding
												PMath::ppiCache[PMath::ppiCache_Used].fDist=(float)rDist;
												PMath::ppiCache[PMath::ppiCache_Used].dDist=rDist;
												PMath::ppiCache_Used++;
												b2dDone[iNear][iFar]=true;
											}
											if (bVerticalMovement) {
												PReporting::setParticiple("adding at index "+Convert::ToString(PMath::ppiCache_Used)+" of "+Convert::ToString(PMath::ppiCache_Max));
												PMath::ppiCache[PMath::ppiCache_Used].x=iFar;
												PMath::ppiCache[PMath::ppiCache_Used].y=iNear;
												PMath::ppiCache[PMath::ppiCache_Used].fX=(float)iFar;
												PMath::ppiCache[PMath::ppiCache_Used].fY=(float)iNear;
												PMath::ppiCache[PMath::ppiCache_Used].dX=(double)iFar;
												PMath::ppiCache[PMath::ppiCache_Used].dY=(double)iNear;
												PMath::ppiCache[PMath::ppiCache_Used].iDist=(int)(rDist+.5);//+.5 for rounding
												PMath::ppiCache[PMath::ppiCache_Used].fDist=(float)rDist;
												PMath::ppiCache[PMath::ppiCache_Used].dDist=rDist;
												PMath::ppiCache_Used++;
												b2dDone[iFar][iNear]=true;
											}
											//if (iFar==iNear && iNear>=iStartNow ) //&& don't check since iFar==iNear: iFar>=iStartNow
											//	iStartNow=(iNear<iFar)?(iNear+1):(iFar+1); //ONLY OK since CORNER (iFar==iNear) being done denotes that square is done
											if ( b2dDone[iNear][iFar] || b2dDone[iFar][iNear]) iFoundThisRound++;
											//"finished at radius "+Convert::ToString(iRadNow)+"
											if (PReporting::getIsMegaDebug()) PReporting::setParticiple("finished adding points that are permutations of ("+Convert::ToString(iNear)+","+Convert::ToString(iFar)+")");
										}//if not already done this point
									}//if rDist<=rRadNow
									else {
										if (PReporting::getIsMegaDebug()) PReporting::setParticiple("finished finding nothing at points that are permutations of ("+Convert::ToString(iNear)+","+Convert::ToString(iFar)+")");
									}
									rNear+=1.0;
									iNear++;
								}//end while iNear<=iRadNow
							rFar+=1.0;
							iFar++;
						}//end while iFar<=iRadNow
					} while (iFoundThisRound>0);
					rRadNow+=1.0;
				}//end for iRadNow
				PReporting::setParticiple("freeing 2d flags inner arrays");
				for (unsigned int i=0; i<iRadius; i++) {
					free(b2dDone[i]);
					b2dDone[i]=NULL;
				}
				PReporting::setParticiple("freeing 2d flags array");
				free(b2dDone);
				//free(ppiTemp);
				//ppiTemp=NULL;
				Console::Error.WriteLine("done. (PMath::ppiCache["+Convert::ToString(ppiCache_Max)+"])");//ppiCache.size()
				PMath::ppxiCache_bHorizontalMovement=bHorizontalMovement;
				PMath::ppxiCache_bVerticalMovement=bVerticalMovement;
			}
			catch (exception &exn) {
				PReporting::ShowExn(exn);
			}
			catch (...) {
				PReporting::ShowUnknownExn("ResizePointInfoCache");
			}
		}//end if PMath::ppiCache needs to be recreated
	}//end ResizePointInfoCache
	///<summary>
	///Resizes the array ppxiCache
	///</summary>
	void PMath::ResizePixelInfoCache(unsigned int iDesiredSize, bool bPreservePreviousData) {
		if (ppxiCache_Max<iDesiredSize||ppxiCache==NULL) {
			Console::Error.Write("Resizing pixel cache to "+Convert::ToString(iDesiredSize)+"...");
			PIXELINFO* ppxiCacheNew=(PIXELINFO*)malloc(sizeof(PIXELINFO)*iDesiredSize);
			if (ppxiCache!=NULL) {
				if (bPreservePreviousData) {
					if (ppxiCache_Used<iDesiredSize) {
						Console::Error.Write("WARNING: Truncating used pixel buffer data (had "+Convert::ToString(ppxiCache_Used)+" entries, truncated to max size "+Convert::ToString(iDesiredSize)+"...");
						ppxiCache_Used=iDesiredSize;
					}
					for (unsigned int i=0; i<ppxiCache_Used; i++) {
						memcpy(&ppxiCacheNew[i],&ppxiCache[i],sizeof(PIXELINFO));
					}
				//NOTE: ppxiCache_Used stays the same
				}
				else {
					ppxiCache_Used=0;
					//ppxiCache_Max=iDesiredSize;//done below in outer scope
				}//else don't preserve data
				free(ppxiCache);
				ppxiCache=ppxiCacheNew;
			}
			else {//was previously NULL
				ppxiCache_Used=0;
				ppxiCache=NULL;//ok since set to ppxiCacheNew below
			}
			ppxiCache=ppxiCacheNew;
			ppxiCache_Max=iDesiredSize;
			Console::Error.WriteLine("done. (pixel cache size:"+Convert::ToString(ppxiCache_Max)+")");
		}//end if ppxiCache needs to be recreated
	}//end ResizePixelInfoCache

	int PMath::Abs(int val) {
		return (val>=0) ? val : ((val>INT_MIN)?(0-val):INT_MAX) ;
	}
	//approach formula (src+=(dest-src)*rMultiplier) examples:
	//0 toward 4 by .5 = 0+(4-0)*.5 = 2
	//-1 toward 3 by .5 = -1+(3--1)*.5 = 1
	//3 toward -1 by .5 = 3+(-1-3)*.5 = 1
	void PMath::Approach(float& x, float& y, float xDest, float yDest, float rMultiplier) {
		x+=(xDest-x)*rMultiplier;
		y+=(yDest-y)*rMultiplier;
	}
	void PMath::Approach(float& valToMove, float valDest, float rMultiplier) {
		valToMove+=(valDest-valToMove)*rMultiplier;
	}
	void PMath::Approach(double& x, double& y, double xDest, double yDest, double rMultiplier) {
		x+=(xDest-x)*rMultiplier;
		y+=(yDest-y)*rMultiplier;
	}
	void PMath::Approach(double& valToMove, double valDest, double rMultiplier) {
		valToMove+=(valDest-valToMove)*rMultiplier;
	}
	bool PMath::HavePoint(unsigned int* haystack_X, unsigned int* haystack_Y, unsigned int haystack_Count, unsigned int xNeedle, unsigned int yNeedle) {
		bool bFound=false;
		for (unsigned int iAt=0; iAt<haystack_Count; iAt++) {
			if ( (haystack_X[iAt]==xNeedle) && (haystack_Y[iAt]==yNeedle) ) {
				//iFound=iAt;
				bFound=true;
				break;
			}
		}
		return bFound;
	}//end HavePoint
	bool PMath::HavePoint(double* haystack_X, double* haystack_Y, unsigned int haystack_Count, double xNeedle, double yNeedle) {
		bool bFound=false;
		for (unsigned int iAt=0; iAt<haystack_Count; iAt++) {
			if ( (haystack_X[iAt]==xNeedle) && (haystack_Y[iAt]==yNeedle) ) {
				//iFound=iAt;
				bFound=true;
				break;
			}
		}
		return bFound;
	}//end HavePoint
	bool PMath::HavePointRounded(double* haystack_X, double* haystack_Y, unsigned int haystack_Count, unsigned int xNeedle, unsigned int yNeedle) {
		bool bFound=false;
		for (unsigned int iAt=0; iAt<haystack_Count; iAt++) {
			if ( ((unsigned int)(haystack_X[iAt]+.5)==xNeedle) && ((unsigned int)(haystack_Y[iAt]+.5)==yNeedle) ) {
				//iFound=iAt;
				bFound=true;
				break;
			}
		}
		return bFound;
	}//end HavePoint
	float PMath::ROfXY(float X, float Y) {
		return ( sqrt( (X) * (X) + (Y) * (Y) ) );
	}
	double PMath::ROfXY(double X, double Y) {
		return ( sqrt( (X) * (X) + (Y) * (Y) ) );
	}
	float PMath::ThetaDegOfXY(float X, float Y) {
		return ( ((Y)!=0 || (X)!=0) ? (atan2((Y),(X))*PMath::f180_DIV_PI) : 0 );
	}
	double PMath::ThetaDegOfXY(double X, double Y) {
		return ( ((Y)!=0 || (X)!=0) ? (atan2((Y),(X))*PMath::d180_DIV_PI) : 0 );
	}
	float PMath::XOfRThetaDeg(float r, float theta) {
		return ((r)*cos((theta)/PMath::f180_DIV_PI)); //divide since cos takes radians
	}
	float PMath::YOfRThetaDeg(float r, float theta) {
		return ((r)*sin((theta)/PMath::f180_DIV_PI)); //divide since cos takes radians
	}
	double PMath::XOfRThetaDeg(double r, double theta) {
		return ((r)*cos((theta)/PMath::d180_DIV_PI)); //divide since cos takes radians
	}
	double PMath::YOfRThetaDeg(double r, double theta) {
		return ((r)*sin((theta)/PMath::d180_DIV_PI)); //divide since cos takes radians
	}
	void PMath::Travel2d(float& xToMove, float& yToMove, float rDirectionDeg, float rDistance) {
		while (rDirectionDeg>=360.0) rDirectionDeg-=360.0;
		while (rDirectionDeg<0.0) rDirectionDeg+=360.0;
		float xRel;
		float yRel;
		xRel=PMath::XOfRThetaDeg(rDistance,rDirectionDeg);
		yRel=PMath::YOfRThetaDeg(rDistance,rDirectionDeg);
		xToMove+=xRel;
		yToMove+=yRel;
	}//end Travel2d
	void PMath::Travel2d(double& xToMove, double& yToMove, double rDirectionDeg, double rDistance) {
		while (rDirectionDeg>=360.0) rDirectionDeg-=360.0;
		while (rDirectionDeg<0.0) rDirectionDeg+=360.0;

		double xRel;//=rDistance;
		double yRel;//=0;
		//NOTE: another way is to set (xRel,yRel) to (rDistance,0) then rotate the point
		xRel=PMath::XOfRThetaDeg(rDistance,rDirectionDeg);
		yRel=PMath::YOfRThetaDeg(rDistance,rDirectionDeg);

		//if (rDirectionDeg==0) xToMove+=rDistance;
		//else if (rDirectionDeg==90) yToMove+=rDistance;
		//else if (rDirectionDeg==180) xToMove-=rDistance;
		//else if (rDirectionDeg=270) yToMove-=rDistance;
		//else {}
		xToMove+=xRel;
		yToMove+=yRel;
	}//end Travel2d
	float PMath::Dist(float x1, float y1, float x2, float y2) {
		register float xSquaring=(x1-x2);
		register float ySquaring=(y1-y2);
		register float valSumOfSquares=xSquaring*xSquaring+ySquaring*ySquaring;
		return ((valSumOfSquares>0)?sqrt(valSumOfSquares):0);
	}
	double PMath::Dist(double x1, double y1, double x2, double y2) {
		register double xSquaring=(x1-x2);
		register double ySquaring=(y1-y2);
		register double valSumOfSquares=xSquaring*xSquaring+ySquaring*ySquaring;
		return ((valSumOfSquares>0)?sqrt(valSumOfSquares):0);
	}
	///<summary>
	///The closeness ratio of val toward end
	///e.g. Nextness(10,0,1) would be .9
	///since (1-10)/(0-10) == -9/-10 == .9
	///(does prevent divide by zero and in that case sets nextness to 1 [since 100% close to dest {assumes between prev and next}]
	///</summary>
	double PMath::Nextness(double prev, double next, double val) {
		//example:
		//  from 10 to 0, val 2 should result in nextness .8
		//  spread==-10
		//  return .8 //i.e. return -8/-10 //i.e. return (2-10)/(-10)
		double spread=next-prev;
		return (spread!=0.0) ? ( (val-prev)/(spread) ) : 1; //!=0 (as opposed to >0) takes into account next<prev
	}
	void PMath::Rotate(double& xToMove, double& yToMove, double rotate_deg) {
		double rTemp=PMath::ROfXY(xToMove,yToMove);
		double thetaTemp=PMath::ThetaDegOfXY(xToMove,yToMove);
		thetaTemp+=rotate_deg;
		xToMove=XOfRThetaDeg(rTemp,thetaTemp);
		yToMove=YOfRThetaDeg(rTemp,thetaTemp);
	}
	void PMath::Rotate(double& xToMove, double& yToMove, double xCenter, double yCenter, double rotate_deg) {
		xToMove-=xCenter;
		yToMove-=yCenter;
		double rTemp=PMath::ROfXY(xToMove,yToMove);
		double thetaTemp=PMath::ThetaDegOfXY(xToMove,yToMove);
		thetaTemp+=rotate_deg;
		xToMove=PMath::XOfRThetaDeg(rTemp,thetaTemp);
		yToMove=PMath::YOfRThetaDeg(rTemp,thetaTemp);
		xToMove+=xCenter;
		yToMove+=yCenter;
	}
	double PMath::RadiansToDegrees(double val) {
		return val * PMath::d180_DIV_PI;
	}
	//double PMath::RadiansToDegrees(double val) {
	//	return val * PMath::DPI_DIV_180;
	//}
	//double PMath::ArcLengthOfCircleSliceStraightSideLength(double val) {
	//	asdf
	//}
	double PMath::DegreesPerPixelAt(double radius_pixels) {
		//NOTES:
		//NO LONGER returns 90 when radius_pixels<=1 which
		// would assume center is in intersection of 4-pixel block
		//How many radians are in the arc (rRA) is how many radius_pixels pixels there are in the arc
		//  * since segment is 1 degree:
		//    * rad = 1 * PMath::DPi/180.0
		//    * simplify:
		//    * PIXELS_per_degree = PMath::DPi/180.0 * radius_pixels
		//  * USE INVERSE since we want DEGREES per pixel:
		if (radius_pixels>0.0) return 180.0/(radius_pixels*PMath::DPi);//simplified equation (see above)
		else return 360.0;
	}//end DegreesPerPixelAt
	byte PMath::ByRound(float val) {
		return ( (val<.5f) ? PMath::by0 : ((val>=254.5f)?PMath::by255:((byte)(val+.5f))) );
	}
	byte PMath::ByRound(double val) {
		return ( (val<.5) ? PMath::by0 : ((val>=254.5)?PMath::by255:((byte)(val+.5))) );
	}
	int PMath::IRoundCropped(float val, int start, int endbefore) {
		int iRound=(int)(val+.5f);
		return ( (iRound<start) ? start : ((iRound>=endbefore)?endbefore:(iRound)) );
	}
	int PMath::IRoundCropped(double val, int start, int endbefore) {
		int iRound=(int)(val+.5);
		return ( (iRound<start) ? start : ((iRound>=endbefore)?endbefore:(iRound)) );
	}
	float PMath::Abs(float val) {
		return (val>=0.0f) ? val : ((val==FLT_MIN)?FLT_MAX:(0.0f-val));//prevent overflow since abs(MIN) is greater than MAX!
	}
	double PMath::Abs(double val) {
		return (val>=0.0) ? val : ((val==DBL_MIN)?DBL_MAX:(0.0-val));//prevent overflow since abs(MIN) is greater than MAX!
	}

	PMathStatic::PMathStatic() {
		PMath::staticconstructorPMath();
	}
	PMathStatic::~PMathStatic() {
		PMath::staticdeconstructorPMath();
	}


}//end namespace

#endif
