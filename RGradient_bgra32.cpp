#ifndef GRADIENT_CPP
#define GRADIENT_CPP
#include <gradient32bgra.h>
#include <gbuffer32bgra.h>

using namespace std;

namespace ExpertMultimediaBase {
	void Gradient::ShadeAlpha(ExpertMultimediaBase::GBuffer &gbDest, Uint32 u32DestBufferLoc, byte bySrcValue, int iDrawMode) {//bool Gradient::Shade(byte* arrbyDest, Uint32 u32DestLoc, byte bySrcValue) {
		ShadeAlpha(gbDest, u32DestBufferLoc, (Uint32)bySrcValue, iDrawMode);//return Shade(arrbyDest, u32DestLoc, (Uint32)bySrcValue);
	}
	void Gradient::ShadeAlpha(ExpertMultimediaBase::GBuffer &gbDest, Uint32 u32DestBufferLoc, Uint32 u32SrcValue, int iDrawMode) {//bool Gradient::Shade(byte* arrbyDest, Uint32 u32DestLoc, Uint32 u32SrcValue) {
		//bool bGood=false;
		try {
			Uint32 u32SrcLoc=u32SrcValue*u32BytesPP;
			Uint32 u32DestBytesPP=(Uint32)gbDest.iBytesPP;
			//if (lpbyShade!=null) {
				//if (gbDest!=null) {
				if (u32SrcLoc<this->u32BytesTotal) { //if (u32SrcValue<this->u32Levels) {
					/*
					if (gbDest.iBytesPP==this->u32BytesPP) {
						if (this->u32BytesPP==4) {
						}//end if 32-bit to 32-bit
						else if (this->u32BytesPP=1) {
							byte* lpbySrcNow=&lpbyShade[u32SrcLoc];//*this->u32BytesPP];
							byte* lpbyDestNow=&gbDest.arrbyData[u32DestBufferLoc];
							if (*lpbySrcNow==255) {//copy the value, multiple times if dest is greater bitdepth
								*lpbyDestNow=*lpbySrcNow;
								if (u32DestBytesPP>1) {
									lpbyDestNow++;
									*lpbyDestNow=*lpbySrcNow;
									if (u32DestBytesPP>2) {
										lpbyDestNow++;
										*lpbyDestNow=*lpbySrcNow;
										if (u32DestBytesPP>3) {
											if ( (iDrawMode==DrawModeCopyAlpha) || (iDrawMode==DrawModeKeepGreaterAlpha) ) { //these draw modes behave the same since alpha==255
												lpbyDestNow++;
												*lpbyDestNow=*lpbySrcNow;
											}
											else if ( iDrawMode==DrawModeBlendAlpha ) {
												lpbyDestNow++;
												*lpbyDestNow=ExpertMultimediaBase::by3dAlphaLookup[*lpbySrcNow][*lpbyDestNow][*lpbySrcNow];
											}
											//else DrawModeKeepDestAlpha so do nothing
										}
									}
								}
							}
							else if (*lpbySrcNow>0) { //else if blend alpha from 1-channel gradient to any# of channels on dest
								///modes: DrawModeCopyAlpha, DrawModeBlendAlpha, DrawModeKeepGreaterAlpha, DrawModeKeepDestAlpha
								*lpbyDestNow=ExpertMultimediaBase::by3dAlphaLookup[*lpbySrcNow][*lpbyDestNow][*lpbySrcNow];//intentionally using *lpbySrcNow as source and alpha since grayscale
								if (u32DestBytesPP>1) {
									lpbyDestNow++;
									*lpbyDestNow=ExpertMultimediaBase::by3dAlphaLookup[*lpbySrcNow][*lpbyDestNow][*lpbySrcNow];//intentionally using *lpbySrcNow as source and alpha since grayscale
									if (u32DestBytesPP>2) {
										lpbyDestNow++;
										*lpbyDestNow=ExpertMultimediaBase::by3dAlphaLookup[*lpbySrcNow][*lpbyDestNow][*lpbySrcNow];//intentionally using *lpbySrcNow as source and alpha since grayscale
										if (u32DestBytesPP>3) {
											if (iDrawMode=DrawModeBlendAlpha) {
												lpbyDestNow++;
												*lpbyDestNow=ExpertMultimediaBase::by3dAlphaLookup[*lpbySrcNow][*lpbyDestNow][*lpbySrcNow];//intentionally using *lpbySrcNow as source and alpha since grayscale
											}
											else if (iDrawMode==DrawModeKeepGreaterAlpha) {
												lpbyDestNow++;
												*lpbyDestNow=(*lpbySrcNow>*lpbyDestNow)?*lpbySrcNow:*lpbyDestNow;//intentionally using *lpbySrcNow as source and alpha since grayscale
											}
											else if (iDrawMode==DrawModeCopyAlpha) {
												lpbyDestNow++;
												*lpbyDestNow=*lpbySrcNow;//intentionally using *lpbySrcNow as source and alpha since grayscale
											}
											//else DrawModeKeepDestAlpha so do nothing
										}
									}
								}
							}
							//else do nothing since alpha==0
						}//end if gradient is 1-channel
						else {
							memcpy(&gbDest.arrbyData[u32DestBufferLoc],&lpbyShade[u32SrcLoc],u32BytesPP);
						}//end else 24-bit to 24-bit (or something other than 8-bit or 32-bit)
					}//end if same bitdepths
					///if ( (u32DestBytesPP==1) && (u32BytesPP>u32DestBytesPP) ) return;
					else { //different bitdepths

						*/
						if (u32BytesPP==4) {
							if (u32DestBytesPP==3) { //32-bit to 24-bit
								if (iDrawMode==DrawModeCopyAlpha) {
									memcpy(&gbDest.arrbyData[u32DestBufferLoc],&lpbyShade[u32SrcLoc],3);
								}
								else { //since dest is 24-bit (see slightly above), DrawModeKeepGreaterAlpha, DrawModeBlendAlpha, and DrawModeKeepDestAlpha all behave the same (as DrawModeKeepDestAlpha)
									byte* lpbyDestNow=&gbDest.arrbyData[u32DestBufferLoc];
									byte* lpbySrcNow=&lpbyShade[u32SrcLoc];
									byte alpha=lpbySrcNow[3];
									if (alpha==255) {
										memcpy(lpbyDestNow,&lpbyShade[u32SrcLoc],3);
									}
									else if (alpha>0) {
										*lpbyDestNow=ExpertMultimediaBase::by3dAlphaLookup[*lpbySrcNow][*lpbyDestNow][alpha];
										lpbySrcNow++;
										lpbyDestNow++;
										*lpbyDestNow=ExpertMultimediaBase::by3dAlphaLookup[*lpbySrcNow][*lpbyDestNow][alpha];
										lpbySrcNow++;
										lpbyDestNow++;
										*lpbyDestNow=ExpertMultimediaBase::by3dAlphaLookup[*lpbySrcNow][*lpbyDestNow][alpha];
									}
									//else alpha==0 so do nothing
								}
							}
							else if (u32DestBytesPP==4) { //32-bit to 32-bit
								byte* lpbySrcNow=&lpbyShade[u32SrcLoc];
								byte alpha=lpbySrcNow[3];
								byte* lpbyDestNow=&gbDest.arrbyData[u32DestBufferLoc];
								if (iDrawMode==DrawModeCopyAlpha) {//already handled above
									memcpy(lpbyDestNow,lpbySrcNow,4);
								}
								else if (alpha==255) {
									memcpy(lpbyDestNow,lpbySrcNow,3);
									if ( (iDrawMode==DrawModeKeepGreaterAlpha) || (iDrawMode==DrawModeBlendAlpha) ) { //these DrawModes behave the same since source is 255
										lpbyDestNow+=3;
										*lpbyDestNow=255;//(*lpbySrcNow>*lpbyDestNow)?*lpbySrcNow:*lpbyDestNow;
									}
								}
								else if (alpha>0) {//else not 255, and either DrawModeBlendAlpha, DrawModeKeepGreaterAlpha, or DrawModeKeepDestAlpha
									*lpbyDestNow=ExpertMultimediaBase::by3dAlphaLookup[*lpbySrcNow][*lpbyDestNow][alpha];
									lpbyDestNow++;
									lpbySrcNow++;
									*lpbyDestNow=ExpertMultimediaBase::by3dAlphaLookup[*lpbySrcNow][*lpbyDestNow][alpha];
									lpbyDestNow++;
									lpbySrcNow++;
									*lpbyDestNow=ExpertMultimediaBase::by3dAlphaLookup[*lpbySrcNow][*lpbyDestNow][alpha];
									if (iDrawMode==DrawModeKeepGreaterAlpha) {
										lpbyDestNow++;
										lpbySrcNow++;
										*lpbyDestNow=(*lpbySrcNow>*lpbyDestNow)?*lpbySrcNow:*lpbyDestNow;
									}
									else if (iDrawMode==DrawModeBlendAlpha) {
										lpbyDestNow++;
										lpbySrcNow++;
										*lpbyDestNow=ExpertMultimediaBase::by3dAlphaLookup[*lpbySrcNow][*lpbyDestNow][alpha];
									}
									//else DrawModeKeepDestAlpha so do nothing
								}
								//else do nothing since alpha==0
							}//32-bit to 32-bit (dest is 32-bit)
							else if (u32DestBytesPP==1) {//32-bit to 1-channel (dest is 1-channel)
								byte* lpbySrcNow=&lpbyShade[u32SrcLoc+3];//+3 for alpha
								//byte alpha=lpbySrcNow[3];
								byte* lpbyDestNow=&gbDest.arrbyData[u32DestBufferLoc];
								if (iDrawMode==DrawModeCopyAlpha) {//already handled above
									*lpbyDestNow=*lpbySrcNow;
								}
								else if (*lpbySrcNow==255) {
									if ( (iDrawMode==DrawModeKeepGreaterAlpha) || (iDrawMode==DrawModeBlendAlpha) ) { //these DrawModes behave the same since source is 255
										*lpbyDestNow=255;//(*lpbySrcNow>*lpbyDestNow)?*lpbySrcNow:*lpbyDestNow;
									}
								}
								else if (*lpbySrcNow>0) {
									if (iDrawMode==DrawModeKeepGreaterAlpha) {
										*lpbyDestNow=(*lpbySrcNow>*lpbyDestNow)?*lpbySrcNow:*lpbyDestNow;
									}
									else if (iDrawMode==DrawModeBlendAlpha) {
										*lpbyDestNow=ExpertMultimediaBase::by3dAlphaLookup[*lpbySrcNow][*lpbyDestNow][*lpbySrcNow];
									}
								}
							}
							else {
								Console.Error.Write("<gradient skipping "+RString_ToString(u32BytesPP)+"- to "+RString_ToString(u32DestBytesPP)+"-bit>");
								Console.Error.Flush();
							}
						}//end if gradient bitdepth is 4
						else if (u32BytesPP==1) {//1-channel gradient
							byte* lpbyDestNow=&gbDest.arrbyData[u32DestBufferLoc];
							byte* lpbySrcNow=&lpbyShade[u32SrcLoc];
							byte alpha=*lpbySrcNow;
							if (iDrawMode==DrawModeCopyAlpha) {
								*lpbyDestNow=*lpbySrcNow;
								lpbyDestNow++; //do NOT increment source, since 1-channel source
								if (u32DestBytesPP>1) {
									lpbyDestNow++;
									*lpbyDestNow=*lpbySrcNow;
									if (u32DestBytesPP>2) {
										lpbyDestNow++;
										*lpbyDestNow=*lpbySrcNow;
										if (u32DestBytesPP>3) {
											lpbyDestNow++;
											//no clauses since DrawModeCopyAlpha was already found
											*lpbyDestNow=*lpbySrcNow;
										}
									}
								}
							}//end if DrawModeCopyAlpha
							else { ///DrawModeBlendAlpha, DrawModeKeepDestAlpha, or DrawModeKeepGreaterAlpha
								*lpbyDestNow=ExpertMultimediaBase::by3dAlphaLookup[*lpbySrcNow][*lpbyDestNow][alpha];
								lpbyDestNow++; //do NOT increment source, since 1-channel source
								if (u32DestBytesPP>1) {
									lpbyDestNow++;
									*lpbyDestNow=ExpertMultimediaBase::by3dAlphaLookup[*lpbySrcNow][*lpbyDestNow][alpha];
									if (u32DestBytesPP>2) {
										lpbyDestNow++;
										*lpbyDestNow=ExpertMultimediaBase::by3dAlphaLookup[*lpbySrcNow][*lpbyDestNow][alpha];
										if (u32DestBytesPP>3) {
											if (iDrawMode==DrawModeBlendAlpha) {
												lpbyDestNow++;
												*lpbyDestNow=ExpertMultimediaBase::by3dAlphaLookup[*lpbySrcNow][*lpbyDestNow][alpha];
											}
											else if (iDrawMode==DrawModeKeepGreaterAlpha) {
												lpbyDestNow++;
												*lpbyDestNow=ExpertMultimediaBase::by3dAlphaLookup[*lpbySrcNow][*lpbyDestNow][alpha];
											}
											//else DrawModeKeepDestAlpha so do nothing
										}
									}
								}
							}//end DrawModeBlendAlpha, DrawModeKeepDestAlpha, or DrawModeKeepGreaterAlpha
						}//end else if 1-channel gradient
						else if (u32BytesPP==3) { //24-bit gradient
							Uint32 u32SrcChanRel=0;
							for (int iChan=0; (iChan<gbDest.iBytesPP)&&(u32SrcLoc<this->u32BytesTotal); iChan++) {
								gbDest.arrbyData[u32DestBufferLoc]=this->lpbyShade[u32SrcLoc];
								u32DestBufferLoc++;
								if (u32SrcChanRel<this->u32BytesPP) {
									u32SrcLoc++;
									u32SrcChanRel++;
								}
								//arrbyDest[u32DestLoc]=lpbyShade[u32SrcValue];//TODO: finish this, write and entire pixel to gbDest
							}
						}
						else {
							Console.Error.Write("<gradient skipping "+RString_ToString(u32BytesPP)+"- to "+RString_ToString(u32DestBytesPP)+"-bit>");
							Console.Error.Flush();
						}
					//}//end else dissimilar bitdepths
				}//end if value is within range
				//}//end gbDest!=null
			//}//end lpbyShade!=null
		}
		catch (exception& exn) {
			ShowExn(exn,"Shade");
			//return false;
		}
		catch(...) {
			ShowUnknownExn("Shade");
			//return false;
		}
		//return bGood;
	}//end Shade
	Gradient::Gradient() {
		InitNull();
		Init(256);
	}
	bool Gradient::InitNull() {
		u32Levels=0;
		u32BytesPP=0;
		u32BytesTotal=0;
		lpbyShade=NULL;
	}
	bool Gradient::Init(Uint32 u32SetLevels) {
		try {
			SafeFree(lpbyShade);
			u32BytesPP=4;
			u32BytesTotal=u32SetLevels*u32BytesPP;
			lpbyShade=(byte*)malloc(sizeof(byte)*u32BytesTotal);
			int iLoc=0;
			for (Uint32 iNow=0; iNow<u32SetLevels; iNow++) {
				lpbyShade[iLoc]=SafeByte( ((double)iNow/(double)(u32SetLevels-1)) * 255.0 );
				iLoc++;
				lpbyShade[iLoc]=SafeByte( ((double)iNow/(double)(u32SetLevels-1)) * 255.0 );
				iLoc++;
				lpbyShade[iLoc]=SafeByte( ((double)iNow/(double)(u32SetLevels-1)) * 255.0 );
				iLoc++;
				lpbyShade[iLoc]=SafeByte( ((double)iNow/(double)(u32SetLevels-1)) * 255.0 );
				iLoc++;
			}
			u32Levels=u32SetLevels;
		}
		catch (exception& exn) {
			ShowExn(exn,"Gradient Init");
		}
		catch (...) {
			ShowUnknownExn("Gradient Init");
		}
	}//end Init
}//end namespace
#endif
