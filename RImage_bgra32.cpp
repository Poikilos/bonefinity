#ifndef GBUFFER_CPP
#define GBUFFER_CPP

#include <RImage_bgra32.h>


using namespace std;

namespace ExpertMultimediaBase {
	byte by2d_ByteAsByte_times_ByteAsFloatLookup[256][256];
	int GBuffer_debug_index=0;
	bool bInit_by2d_ByteAsByte_times_ByteAsFloatLookup=false;
	double dDiagonalUnit = 1.4142135623730950488016887242097;//the sqrt. of 2, dist of diagonal pixel (e.g. one to next diagonally, or corner to corner diagonally)
	/////////////////////////// class GBuffer methods //////////////////////////////
	//public methods:
	byte* GBuffer::BytePtrStart() {
		return arrbyData;
	}
	void GBuffer::InitNull() {
		arrbyData=nullptr;
		if (!bInit_by2d_ByteAsByte_times_ByteAsFloatLookup) {
			bInit_by2d_ByteAsByte_times_ByteAsFloatLookup=true;
			Console::Error.Write("GBuffer: generating byte-as-byte times byte-as-float lookup table...");
			Console::Error.Flush();
			for (int iAsByte=0; iAsByte<256; iAsByte++) {
				for (int iAsFloat=0; iAsFloat<256; iAsFloat++) {
					by2d_ByteAsByte_times_ByteAsFloatLookup[iAsByte][iAsFloat]=SafeByte( (float)iAsByte*((float)iAsFloat/255.0f) ); //this started as a c++ auto array not a pointer
				}
			}
			Console::Error.WriteLine("done.");
		}
		sErrBy="GBuffer";
		sFile=".unnamed-gbuffer "+RString_ToString(GBuffer_debug_index)+".raw";
		GBuffer_debug_index++;
		if (bMegaDebug) {
			Console::Error.WriteLine("Initialized a GBuffer as: "+sFile);
		}
		iWidth=0;
		iHeight=0;
		iBytesPP=0;
		iStride=0;
		iBytesTotal=0;
		bBufferAsPointerNotCopy=false;

		Text_ColumnCount=1;
		Text_ColumnIndex=0;
		Text_marginTopRatio=0.01f;
		Text_marginBottomRatio=0.01f;
		Text_marginLeftRatio=0.01f;
		Text_marginColumnsRatio=0.01f;
		Text_marginRightRatio=0.01f;
		Text_X=-1;
		Text_Y=-1;
		Text_SetColumn(0);
	}  // end GBuffer::InitNull()
	void GBuffer::ShadeAlpha(Gradient* gradient_ptr, Uint32 u32DestBufferLoc, byte bySrcValue, int iDrawMode) {  // bool Gradient::Shade(byte* arrbyDest, Uint32 u32DestLoc, byte bySrcValue) {
		ShadeAlpha(gradient_ptr, u32DestBufferLoc, (Uint32)bySrcValue, iDrawMode);//return Shade(arrbyDest, u32DestLoc, (Uint32)bySrcValue);
	}
	void GBuffer::ShadeAlpha(Gradient* gradient_ptr, Uint32 u32DestBufferLoc, Uint32 u32SrcValue, int iDrawMode) {  // bool Gradient::Shade(byte* arrbyDest, Uint32 u32DestLoc, Uint32 u32SrcValue) {
		//bool bGood=false;
		try {
			if (gradient_ptr!=nullptr) {
				Uint32 src_u32BytesPP=gradient_ptr->get_u32BytesPP();
				Uint32 u32SrcLoc=u32SrcValue*src_u32BytesPP;
				Uint32 u32DestBytesPP=(Uint32)this->iBytesPP;
				byte* gradient_band=gradient_ptr->get_lpbyShade();
				//if (lpbyShade!=null) {
					//if (this!=null) {
					if (u32SrcLoc<gradient_ptr->get_u32BytesTotal()) { //if (u32SrcValue<gradient_ptr->u32Levels) {
						/*
						if (this->iBytesPP==gradient_ptr->u32BytesPP) {
							if (gradient_ptr->u32BytesPP==4) {
							}//end if 32-bit to 32-bit
							else if (gradient_ptr->u32BytesPP=1) {
								byte* lpbySrcNow=&gradient_band[u32SrcLoc];// *gradient_ptr->u32BytesPP];
								byte* lpbyDestNow=&this->arrbyData[u32DestBufferLoc];
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
								memcpy(&this->arrbyData[u32DestBufferLoc],&gradient_band[u32SrcLoc],u32BytesPP);
							}//end else 24-bit to 24-bit (or something other than 8-bit or 32-bit)
						}//end if same bitdepths
						///if ( (u32DestBytesPP==1) && (u32BytesPP>u32DestBytesPP) ) return;
						else { //different bitdepths

							*/
							if (gradient_ptr->get_u32BytesPP()==4) {
								if (u32DestBytesPP==3) { //32-bit to 24-bit
									if (iDrawMode==DrawModeCopyAlpha) {
										memcpy(&this->arrbyData[u32DestBufferLoc],&gradient_band[u32SrcLoc],3);
									}
									else { //since dest is 24-bit (see slightly above), DrawModeKeepGreaterAlpha, DrawModeBlendAlpha, and DrawModeKeepDestAlpha all behave the same (as DrawModeKeepDestAlpha)
										byte* lpbyDestNow=&this->arrbyData[u32DestBufferLoc];
										byte* lpbySrcNow=&gradient_band[u32SrcLoc];
										byte alpha=lpbySrcNow[3];
										if (alpha==255) {
											memcpy(lpbyDestNow,&gradient_band[u32SrcLoc],3);
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
									byte* lpbySrcNow=&gradient_band[u32SrcLoc];
									byte alpha=lpbySrcNow[3];
									byte* lpbyDestNow=&this->arrbyData[u32DestBufferLoc];
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
									byte* lpbySrcNow=&gradient_band[u32SrcLoc+3];//+3 for alpha
									//byte alpha=lpbySrcNow[3];
									byte* lpbyDestNow=&this->arrbyData[u32DestBufferLoc];
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
									Console::Error.Write("<gradient skipping "+RString_ToString(gradient_ptr->get_u32BytesPP())+"- to "+RString_ToString(u32DestBytesPP)+"-BytesPP>");
									Console::Error.Flush();
								}
							}//end if gradient bitdepth is 4
							else if (gradient_ptr->get_u32BytesPP()==1) {//1-channel gradient
								byte* lpbyDestNow=&this->arrbyData[u32DestBufferLoc];
								byte* lpbySrcNow=&gradient_band[u32SrcLoc];
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
							else if (gradient_ptr->get_u32BytesPP()==3) { //24-bit gradient
								Uint32 u32SrcChanRel=0;
								int src_u32BytesTotal = gradient_ptr->get_u32BytesTotal();
								for (int iChan=0; (iChan<this->iBytesPP)&&(u32SrcLoc<src_u32BytesTotal); iChan++) {
									this->arrbyData[u32DestBufferLoc]=gradient_band[u32SrcLoc];
									u32DestBufferLoc++;
									if (u32SrcChanRel<src_u32BytesPP) {
										u32SrcLoc++;
										u32SrcChanRel++;
									}
									//arrbyDest[u32DestLoc]=lpbyShade[u32SrcValue];//TODO: finish this, write and entire pixel to this
								}
							}
							else {
								Console::Error.Write("<gradient skipping "+RString_ToString(src_u32BytesPP)+"- to "+RString_ToString(u32DestBytesPP)+"-BytesPP>");
								Console::Error.Flush();
							}
						//}//end else dissimilar bitdepths
					}//end if value is within range
					//}//end this!=null
				//}//end lpbyShade!=null
			}
			else Console::Error.WriteLine("ShadeAlpha Error: gradient_ptr is nullptr");
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
	}  // end ShadeAlpha
	void GBuffer::Text_SetColumn(int set_ColumnIndex) {
		//ColumnCount=1;
		Text_ColumnIndex=set_ColumnIndex;
		//if (ColumnIndex>ColumnCount) ColumnIndex=0;
		//marginTopRatio=0.1f;
		//marginBottomRatio=0.1f;
		//marginLeftRatio=0.1f;
		//marginColumnsRatio=0.1f;
		//marginRightRatio=0.1f;
		Text_X=-1;
		Text_Y=-1;
		double ColumnCount_DBL=(double)Text_ColumnCount;
		double ColumnIndex_DBL=(double)Text_ColumnIndex;
		double totalColumnSpacingSlackRatio=0.0;
		if (Text_ColumnCount>1) {
			totalColumnSpacingSlackRatio=(ColumnCount_DBL-1.0)*Text_marginColumnsRatio;
		}
		double xRemainingSpaceRatio=1.0-Text_marginLeftRatio-Text_marginRightRatio;
		double yRemainingSpaceRatio=1.0-Text_marginTopRatio-Text_marginTopRatio;
		Text_columnWidthRatio=(xRemainingSpaceRatio-totalColumnSpacingSlackRatio)/ColumnCount_DBL;
		if (Text_columnWidthRatio<.01) {
			Console::Error.WriteLine("WARNING: Columns are "+RString_ToString(Text_columnWidthRatio));
		}
		//int marginLeft=(int)((double)iWidth*marginLeftRatio+.5); //+.5 for rounding
		//int marginTop=(int)((double)iHeight*marginTopRatio+.5); //+.5 for rounding
		double FirstColumn_minLeft=(double)iWidth*Text_marginLeftRatio;
		Text_minTop=(double)iHeight*Text_marginTopRatio;
		double marginRight=(double)iWidth-(double)iWidth*Text_marginRightRatio;
		double marginBottom=(double)iHeight-(double)iHeight*Text_marginBottomRatio;
		double LastColumn_maxRight=((double)(iWidth-1))-marginRight;
		Text_maxBottom=((double)(iHeight-1))-marginBottom;
		Text_Column_minLeft = FirstColumn_minLeft  +  (double)iWidth  *  ( ColumnIndex_DBL * (Text_columnWidthRatio+Text_marginColumnsRatio) );
		Text_Column_maxRight = LastColumn_maxRight  -  (double)iWidth  *  ( (ColumnCount_DBL-1.0-ColumnIndex_DBL) * (Text_columnWidthRatio+Text_marginColumnsRatio) );
		Text_X=Text_Column_minLeft;
		Text_Y=Text_minTop;
	}  // end GBuffer::Text_SetColumn(int set_ColumnIndex)
	void GBuffer::Text_SetMargins(double set_marginLeftRatio, double set_marginTopRatio, double set_marginRightRatio, double set_marginBottomRatio,
											double set_columnWidthRatio, double set_marginColumnsRatio) {
		Text_marginLeftRatio=set_marginLeftRatio;
		Text_marginTopRatio=set_marginBottomRatio;
		Text_marginRightRatio=set_marginRightRatio;
		Text_marginBottomRatio=set_marginBottomRatio;
		Text_columnWidthRatio=set_columnWidthRatio;
		Text_marginColumnsRatio=set_marginColumnsRatio;
	}  // end GBuffer::Text_SetMargins(double set_marginLeftRatio, double set_marginTopRatio, double set_marginRightRatio, double set_marginBottomRatio, double set_columnWidthRatio, double set_marginColumnsRatio)
	double GBuffer::Text_getTop() {
		return Text_minTop;
	}
	double GBuffer::Text_getBottom() {
		return Text_maxBottom;
	}
	double GBuffer::Text_getColumnLeft() {
		return Text_Column_minLeft;
	}
	double GBuffer::Text_getColumnRight() {
		return Text_Column_maxRight;
	}

	GBuffer::GBuffer() {
		InitNull();
	}
	GBuffer::GBuffer(string sFileImage) {
		InitNull();
		if (!Load(sFileImage)) iBytesTotal=0;
	}
	bool GBuffer::Load(string sFileNow) {
		bool bGood=true;
		static bool bFirstRun=true;
		if (bFirstRun) Console::Error.Write("GBuffer::Load...");
		//BitmapData bmpdata;
		//GraphicsUnit gunit;
		RectangleF rectNowF;
		Rectangle rectNow;
		sFile=sFileNow;
		try {
			if (bFirstRun) Console::Error.Write("calling targa...");
			bGood=targaLoaded.Load(sFile);//bmpLoaded=new Bitmap(sFile);
			if (bFirstRun) Console::Error.Write("get pointer...");
			arrbyData=targaLoaded.GetBufferPointer();
			bBufferAsPointerNotCopy=true;
			//gunit = GraphicsUnit.Pixel;
			targaLoaded.ToRect(rectNowF);//rectNowF = bmpLoaded.GetBounds(gunit);
			rectNow.Set((int)rectNowF.X, (int)rectNowF.Y,
								(int)rectNowF.Width, (int)rectNowF.Height);
			//bmpdata = bmpLoaded.LockBits(rectNow, ImageLockMode.ReadOnly, PixelFormatNow());
			if (bFirstRun) Console::Error.Write("set vars...");
			iStride=targaLoaded.Stride(); //iStride=bmpdata.Stride;
			iWidth=rectNow.Width;
			iHeight=rectNow.Height;
			iBytesPP=targaLoaded.BytesPP();//assumes 32-bit //iBytesPP=iStride/iWidth;
			iBytesTotal=targaLoaded.BytesAsUncompressed();//BytesBuffer()
			if (bFirstRun) Console::Error.Write("dimensions:"+RString_ToString(iWidth)
										+"x"+RString_ToString(iHeight)
										+"x"+RString_ToString(iBytesPP*8)+"...");
			//SafeFree(arrbyData);
			//arrbyData=(byte*)malloc(iBytesTotal);
			//byte* lpbyNow = (byte*) bmpdata.Scan0.ToPointer();
			//for (int iBy=0; iBy<iBytesTotal; iBy++) {
			//	arrbyData[iBy]=*lpbyNow;
			//	lpbyNow++;
			//}
			//bmpLoaded.UnlockBits(bmpdata);
		}
		catch (exception& exn) {
			bGood=false;
			if (&sFile==nullptr) sFile="null";
			ShowExn(exn, "gbuffer32bgra.Load(\""+sFile+"\")");
		}
		catch (...) {
			bGood=false;
			if (&sFile==nullptr) sFile="null";
			ShowUnknownExn("gbuffer32bgra.Load(\""+sFile+"\")");
		}
		bFirstRun=false;
		return bGood;
	}  // end Load
	bool GBuffer::Save(string sFileNow) {
		bool bGood=true;
		sFile=sFileNow;
		try {
			//TODO: finish this--other formats, and check extension/header
			if (!targaLoaded.IsLoaded()) {  // if (!targaLoaded.IsLike(iWidth,iHeight,iBytesPP))
				targaLoaded.From(iWidth,iHeight,iBytesPP,arrbyData,false);
			}
			//targaLoaded.ChangeType(TypeAs);
			targaLoaded.Save(sFileNow);
			//if (!bBufferAsPointerNotCopy)
			//	targaLoaded.Close();
		}
		catch (exception& exn) {
			bGood=false;
			if (&sFile==nullptr) sFile="null";
			ShowExn(exn, "gbuffer32bgra.Save(\""+sFileNow+"\")");
		}
		catch (...) {
			bGood=false;
			if (&sFile==nullptr) sFile="null";
			ShowUnknownExn("gbuffer32bgra.Save(\""+sFileNow+"\")");
		}
		return bGood;
	}  // end Save
	bool GBuffer::SaveJec(string sFileNow, int iSetFrames, int iColorTolerance, int iLightnessTolerance) {
		bool bGood=true;
		sFile=sFileNow;
		try {
			//TODO: finish this (create from targa [to allow drawing directly in future apps] then save)
			//if (!jecLoaded.IsLoaded()) //if (!jecLoaded.IsLike(iWidth,iHeight,iBytesPP))
			//	jecLoaded.From(iWidth,iHeight,iBytesPP,arrbyData,true);
			//jecLoaded.Save(sFileNow);
			ShowErr("NOT YET IMPLEMENTED","SaveJec("+sFileNow+","+RString_ToString(iSetFrames)+","+RString_ToString(iColorTolerance)+","+RString_ToString(iLightnessTolerance)+")");
			//if (!bBufferAsPointerNotCopy)
			//	targaLoaded.Close();
		}
		catch (exception& exn) {
			bGood=false;
			if (&sFile==nullptr) sFile="null";
			ShowExn(exn, "gbuffer32bgra.Save(\""+sFileNow+"\")");
		}
		catch (...) {
			bGood=false;
			if (&sFile==nullptr) sFile="null";
			ShowUnknownExn("gbuffer32bgra.Save(\""+sFileNow+"\")");
		}
		return bGood;
	}  // end Save
	bool GBuffer::SaveRaw(string sFileNow) {
		bool bGood=true;
		try {
			Byter byterTemp;
			byterTemp.SetLength(iBytesTotal);
			Uint32 u32Test=0;
			if (false==byterTemp.Write(arrbyData, (Uint32)iBytesTotal, u32Test)) {
				bGood=false;
				ShowError("Failed to write raw data to buffer","SaveRaw("+sFileNow+")");
			}
			if (false==byterTemp.Save(sFileNow)) {
				bGood=false;
				ShowError("Failed to save raw data to file","SaveRaw("+sFileNow+")");
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn, "gbuffer32bgra.SaveRaw("+sFileNow+")");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("gbuffer32bgra.SaveRaw("+sFileNow+")");
		}
		return bGood;
	}  // end SaveRaw
	GBuffer::GBuffer(int iSetWidth, int iHeightNow, int iSetBytesPP) {
		InitNull();
		Init(iSetWidth, iHeightNow, iSetBytesPP, true);
	}
	GBuffer::GBuffer(int iSetWidth, int iHeightNow, int iSetBytesPP, bool bInitializeBuffer) {
		InitNull();
		Init(iSetWidth, iHeightNow, iSetBytesPP, bInitializeBuffer);
	}
	GBuffer::~GBuffer() {
		Dispose();
	}
	void GBuffer::Dispose() {
		if (!bBufferAsPointerNotCopy) {
			SafeFree(arrbyData, "GBuffer::Dispose {sFile:"+sFile+"}");
		}
	}
	void GBuffer::Init(int iSetWidth, int iHeightNow, int iSetBytesPP) {
		Init(iSetWidth,iHeightNow,iSetBytesPP,true);
	}
	void GBuffer::Init(int iSetWidth, int iSetHeight, int iSetBytesPP, bool bInitializeBuffer) {
		static bool bFirstRun=true;
		iWidth=iSetWidth;
		iHeight=iSetHeight;
		iBytesPP=iSetBytesPP;
		iStride=iWidth*iBytesPP;
		iBytesTotal=iStride*iHeight;
		if (bFirstRun) Console::Error.Write("GBuffer::Init");
		if (bFirstRun) Console::Error.Write("("+RString_ToString(iWidth)+","+RString_ToString(iHeight)+","+RString_ToString(iBytesPP)+","+RString_ToString(bInitializeBuffer)+")...");
		if (bInitializeBuffer) {
			try {
				if (bFirstRun) Console::Error.Write("free...");
				SafeFree(arrbyData, "arrbyData in GBuffer::Init");
				if (bFirstRun) Console::Error.Write("allocate...");
				arrbyData=(byte*)malloc(iBytesTotal);
				if (bFirstRun) Console::Error.Write("done gbuffer32bgra.Init...");
			}
			catch (...) {
				ShowError("Couldn't allocate memory",".Init("+RString_ToString(iSetWidth)+","+RString_ToString(iSetHeight)+","+RString_ToString(iSetBytesPP)+","+RString_ToString(bInitializeBuffer)+")");
			}
		}
		bFirstRun=false;
	}  // end init
	GBuffer* GBuffer::Copy() {
		bool bGood=true;
		GBuffer* gbNew=nullptr;
		gbNew=new GBuffer(iWidth,iHeight,iBytesPP,false);
		gbNew->arrbyData=nullptr;  // otherwise exception will occur when SafeFree is called later (when CopyTo calls Init(w,h,bytesPP), because false above says to not touch this member
		bGood=CopyTo(gbNew);
		return gbNew;
		//TODO: make sure delete not free is called if Copy is deleted!!!!!!!
	}
	string GBuffer::Description() {
		return Description(false);
	}
	string GBuffer::Description(bool bVerbose) {
		if (bVerbose) return RString_ToString((arrbyData==nullptr)?"null":"")+RString_ToString(iWidth)+"x"+RString_ToString(iHeight)+"x"+RString_ToString(iBytesPP*8)+" "+RString_ToString(iBytesTotal)+"-length buffer";
		else return RString_ToString((arrbyData==nullptr)?"null":"")+RString_ToString(iWidth)+"x"+RString_ToString(iHeight)+"x"+RString_ToString(iBytesPP*8);
	}
	bool GBuffer::CopyTo(GBuffer* gbBlankNonNullObjectToSet) {
		bool bGood=true;
		try {
			gbBlankNonNullObjectToSet->Init(iWidth,iHeight,iBytesPP);
			memcpy(gbBlankNonNullObjectToSet->arrbyData,arrbyData,iBytesTotal);
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn, "gbuffer32bgra.Copy()");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("gbuffer32bgra.Copy()");
		}
		return bGood;
	}  // end CopyTo(*)
	bool GBuffer::CopyTo(GBuffer &gbBlankNonNullObjectToSet) {
		return CopyTo(&gbBlankNonNullObjectToSet);
	}  // end CopyTo(&)
	bool GBuffer::SetBrushColor(byte r, byte g, byte b, byte a) {
		string sFuncNow="SetBrushColor(r,g,b,a)";
		bool bGood=true;
		try {
			arrbyBrush[0]=b;
			arrbyBrush[1]=g;
			arrbyBrush[2]=r;
			arrbyBrush[3]=a;
			arrbyBrush32Copied64[0]=arrbyBrush[0];
			arrbyBrush32Copied64[1]=arrbyBrush[1];
			arrbyBrush32Copied64[2]=arrbyBrush[2];
			arrbyBrush32Copied64[3]=arrbyBrush[3];
			arrbyBrush32Copied64[4]=arrbyBrush[0];
			arrbyBrush32Copied64[5]=arrbyBrush[1];
			arrbyBrush32Copied64[6]=arrbyBrush[2];
			arrbyBrush32Copied64[7]=arrbyBrush[3];
		}
		catch (exception& exn) {
			sFuncNow="set brush color rgba";
			ShowExn(exn,sFuncNow);
			bGood=false;
		}
		catch (...) {
			sFuncNow="set brush color rgba";
			ShowUnknownExn(sFuncNow);
			bGood=false;
		}
		return bGood;
	}  // end set brush color rgba
	bool GBuffer::SetBrushColor(byte r, byte g, byte b) {
		return SetBrushColor(r,g,b,255);
	}  // end set brush color rgb
	bool GBuffer::SetBrushColor(string sHexCode) {
		bool bGood=true;
		try {

			if (StartsWith(sHexCode,"#")) sHexCode=sHexCode.substr(1);
			if (sHexCode.length()<6) {
				ShowError("This hex color code in the file is not complete","SetBrushColor("+sHexCode+")");
				bGood=false;
			}
			else {
				sHexCode=ToUpper(sHexCode);
				if (false==SetBrushColor(HexCharsToByte(sHexCode.substr(0,2)),
							   HexCharsToByte(sHexCode.substr(2,2)),
							   HexCharsToByte(sHexCode.substr(4,2)), 255)) {
					bGood=false;
				}
			}
		}
		catch (exception& exn) {
			ShowExn(exn,"SetBrushColor("+sHexCode+")","can't interpret specified hex color code");
			bGood=false;
		}
		catch (...) {
			ShowUnknownExn("SetBrushColor("+sHexCode+")","can't interpret specified hex color code");
			bGood=false;
		}
		return bGood;
	}  // end SetBrushColor from hex code

	//#region Draw methods
	void GBuffer::DrawExclusiveRect(int left, int top, int right, int bottom, Uint32 u32Pixel, bool bFilled) {
		if (left<0||right>this->iWidth||top<0||bottom>this->iHeight)//error checking
			return;
		bool bGood=true;
		Uint32* lpu32Dest=(UInt32*)this->arrbyData; //TODO: avoid low-level operations
		int iScreenW=this->iWidth;
		//int iScreenH=this->iHeight;
		if (bottom<=top) {//if bad, draw vert line
			bottom=top+6;
			right=left+1;
			DrawExclusiveRect(left,top,right,bottom,u32Pixel,false);
			if (right<=left) {//if bad, draw horz line too
				top+=2;
				bottom=top+1;
				left-=2;
				right=left+6;
				DrawExclusiveRect(left,top,right,bottom,u32Pixel,false);
			}
			bGood=false;
		}
		else if (right<=left) {//if bad, draw horz line
			bottom=top+1;
			right=left+6;
			DrawExclusiveRect(left,top,right,bottom,u32Pixel,false);
			bGood=false;
		}
		int iStart=top*iScreenW+left;//no stride since using UInt32* to buffer
		lpu32Dest+=iStart;
		int iSkipEdge=iScreenW-(right-left);
		if (bGood) {
			if (bFilled) {
				for (int yNow=top; yNow<bottom; yNow++) {
					for (int xNow=left; xNow<right; xNow++) {
						*lpu32Dest=u32Pixel;
						lpu32Dest++;
					}
					lpu32Dest+=iSkipEdge;
				}
			}//end if bFilled
			else {//only draw outline
				for (int yNow=top; yNow<bottom; yNow++) {
					for (int xNow=left; xNow<right; xNow++) {
						if (yNow==top||yNow==bottom-1||xNow==left||xNow==right-1)
							*lpu32Dest=u32Pixel;
						lpu32Dest++;
					}
					lpu32Dest+=iSkipEdge;
				}
			}
		}//end if bGood
	}  // end DrawExclusiveRect

	bool GBuffer::DrawRect(int xDest, int yDest, int iRectWidth, int iRectHeight) {
		bool bGood=false;
		DrawHorzLine(xDest,yDest,iRectWidth);
		iHeight--;
		DrawHorzLine(xDest,yDest+iRectHeight,iRectWidth);
		yDest++;
		iRectHeight--;//skip the verticle lines' other end too
		if (iRectHeight>0) {
			DrawVertLine(xDest,yDest,iRectHeight);
			iRectWidth--;
			bGood=DrawVertLine(xDest+iRectWidth,yDest,iRectHeight);
		}
		return bGood;
	}
	bool GBuffer::DrawRect(IRect &irectExclusive) {
		return DrawRect(irectExclusive.left, irectExclusive.top,
					 irectExclusive.right-irectExclusive.left,
					 irectExclusive.bottom-irectExclusive.top);
	}
	bool GBuffer::DrawRect(Rectangle &rectNow) {
		return DrawRect(rectNow.X, rectNow.X, rectNow.Width, rectNow.Height);
	}
	bool GBuffer::DrawRectFilled(Rectangle &rectNow) {
		return DrawRectFilled(rectNow.X, rectNow.Y, rectNow.Width, rectNow.Height);
	}
	/// <summary>
	/// DrawRectBorder horizontally and vertically symmetrical
	/// </summary>
	/// <param name="rectDest"></param>
	/// <param name="rectHole"></param>
	/// <returns></returns>
	bool GBuffer::DrawRectBorderSym(Rectangle &rectDest, Rectangle &rectHole) {
		bool bGood=true;
		int xNow;
		int yNow;
		int iSetWidth;
		int iSetHeight;
		try {
			xNow=rectDest.X;
			yNow=rectDest.Y;
			iSetWidth=rectDest.Width;
			iSetHeight=rectHole.Y-rectDest.Y;
			bool bTest=DrawRectFilled(xNow, yNow, iSetWidth, iSetHeight);//top full width
			if (bTest==false) bGood=false;
			yNow+=rectHole.Height+iSetHeight;
			//would need to change iSetHeight here if asymmetrical
			bTest=DrawRectFilled(xNow, yNow, iSetWidth, iSetHeight);//bottom full width
			if (bTest==false) bGood=false;
			yNow-=rectHole.Height;
			iSetWidth=rectHole.X-rectDest.X;
			iSetHeight=rectHole.Height;
			bTest=DrawRectFilled(xNow, yNow, iSetWidth, iSetHeight);//left remaining height
			if (bTest==false) bGood=false;
			xNow+=rectHole.Width+iSetWidth;
			//would need to change iSetWidth here if asymmetrical
			bTest=DrawRectFilled(xNow, yNow, iSetWidth, iSetHeight);//right remaining height
			if (bTest==false) bGood=false;
		}
		catch (exception& exn) {
			ShowExn(exn,"DrawRectBorderSym");
			bGood=false;
		}
		catch (...) {
			ShowUnknownExn("DrawRectBorderSym");
			bGood=false;
		}
		return bGood;
	}  // DrawRectBorderSym
	bool GBuffer::DrawRectBorder(int xDest, int yDest, int iRectWidth, int iRectHeight, int iThick) {
		Rectangle rectOuter;
		Rectangle rectInner;
		rectOuter.X=xDest;
		rectOuter.Y=yDest;
		rectOuter.Width=iRectWidth;
		rectOuter.Height=iRectHeight;
		rectInner.X=xDest+iThick;
		rectInner.Y=yDest+iThick;
		rectInner.Width=iRectWidth-(iThick*2);
		rectInner.Height=iRectHeight-(iThick*2);
		if ((rectInner.Width<1) || (rectInner.Height<1)) {
			return DrawRectFilled(rectOuter);
		}
		else return DrawRectBorderSym(rectOuter, rectInner);
	}  // DrawRectBorder
	bool GBuffer::DrawRectFilled(int xDest, int yDest, int iRectWidth, int iRectHeight) {
		if ((iRectWidth<1)||(iRectHeight<1)) return false;
		bool bGood=true;
		try {
			int iDest=yDest*iStride+xDest*iBytesPP;

			byte* lpDest=&arrbyData[iDest];
			byte* lpSrc=arrbyBrush32Copied64;
			byte* lpDestNow;
			byte* lpDestStartNow=lpDest;

			for (int yNow=0; yNow<iRectHeight; yNow++) {
				lpDestNow=lpDestStartNow;
				for (int i=iRectWidth/2; i!=0; i--) { //since coping 8 bytes (2 copies of brush pixel) at a time for speed
					memcpy(lpDestNow,lpSrc,8);
					//*((Uint64*)lpDestNow) = *((Uint64*)lpSrc);
					lpDestNow+=8;
				}
				if ((iRectWidth%2)!=0) {
					memcpy(lpDestNow,lpSrc,4);
					//*((Uint32*)lpDestNow) = *((Uint32*)lpSrc);
				}

				lpDestStartNow+=iStride;
			}

		}
		catch (exception& exn) {
			ShowExn(exn,"DrawRectFilled");
			bGood=false;
		}
		catch (...) {
			ShowUnknownExn("DrawRectFilled");
			bGood=false;
		}
		return bGood;
	}  // DrawRectFilled
	bool GBuffer::DrawVertLine(int xDest, int yDest, int iPixelCopies) {
		if (iPixelCopies<1) return false;
		bool bGood=true;
		try {
			int iDest=yDest*iStride+xDest*iBytesPP;
			byte* lpDest=&arrbyData[iDest];
			byte* lpSrc=arrbyBrush;
			byte* lpDestNow=lpDest;
			//TODO: use memcpy
			for (int i=iPixelCopies; i!=0; i--) {
				*((Uint32*)lpDestNow) = *((Uint32*)lpSrc);
				lpDestNow+=iStride;
			}
		}
		catch (exception& exn) {
			ShowExn(exn,"DrawVertLine");
			bGood=false;
		}
		catch (...) {
			ShowUnknownExn("DrawVertLine");
			bGood=false;
		}
		return bGood;
	}  // DrawVertLine
	bool GBuffer::DrawHorzLine(int xDest, int yDest, int iPixelCopies) {
		if (iPixelCopies<1) return false;
		bool bGood=true;
		try {
			int iDest=yDest*iStride+xDest*iBytesPP;
			byte* lpDest=&arrbyData[iDest];
			byte* lpSrc=arrbyBrush32Copied64;
			byte* lpDestNow=lpDest;
			for (int i=iPixelCopies/2; i!=0; i--) { //copy 2 pixels at a time for speed
				memcpy(lpDestNow,lpSrc,8);//*((UInt64*)lpDestNow) = *((UInt64*)lpSrc);
				lpDestNow+=8;
			}
			if ((iPixelCopies%2)!=0) {
				*((UInt32*)lpDestNow) = *((UInt32*)lpSrc);
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"DrawHorzLine");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("DrawHorzLine");
		}
		return bGood;
	}  // end DrawHorzLine
	//#endregion Draw methods

		//additions in cpp only (not in c#):
	void GBuffer::DrawAlphaPix(int xPix, int yPix, byte r, byte g, byte b, byte a) {
		try {
			int iChannel=yPix*iStride+xPix*iBytesPP;

			// There only needs to be TWO ++ operations for 3 channels:
			if ((iChannel+2>=0) && (iChannel+2<iStride*iHeight)) {
				if (((iChannel+3)/4)<(iWidth*iBytesPP*iHeight)) {
					float fPremultA=(float)a/255.0f;
					arrbyData[iChannel]=(APPROACH(arrbyData[iChannel],b,fPremultA)); //commented for debug only: by3dAlphaLookup[b][arrbyData[iChannel]][a];
					iChannel++;
					arrbyData[iChannel]=APPROACH(arrbyData[iChannel],g,fPremultA); //commented for debug only: by3dAlphaLookup[g][arrbyData[iChannel]][a];
					iChannel++;
					arrbyData[iChannel]=APPROACH(arrbyData[iChannel],r,fPremultA); //commented for debug only: by3dAlphaLookup[r][arrbyData[iChannel]][a];
				}
			}
		}
		catch (...) {
			ShowUnknownExn("DrawAlphaPix using RGB bytes");
		}
	}
	void GBuffer::DrawSubpixelDot(float xDot, float yDot, Pixel &pixelColor) {
		//bool bGood=true;
		try {
			//TODO: finish this (finish subpixel accuracy)
			// Begin header fields in order of writing //
			//Targa struct reference:
			//bySizeofID byMapType byTgaType wMapOrigin wMapLength byMapBitDepth
			//xImageOrigin yImageOrigin width height byBitDepth bitsDescriptor sTag
			//*arrbyColorMap *arrbyData footer;
			//LPIPOINT *lpipointarrNow=malloc(4*sizeof(LPIIPOINT));

			int xMin=IFLOOR(xDot);
			int xMax=ICEILING(xDot);
			int yMin=IFLOOR(yDot);
			int yMax=ICEILING(yDot);
			float xfMin=(float)xMin;
			float xfMax=(float)xMax;
			float yfMin=(float)yMin;
			float yfMax=(float)yMax;
			//int iBytesPP=byBitDepth/8;
			//int iStride=iWidth*iBytesPP;
			//int iStart=yMin*iStride+xMin*iBytesPP;
			float xEccentric,yEccentric,xNormal,yNormal;
			xNormal=1.0f-fabs(xDot-xfMin);
			xEccentric=1.0f-fabs(xDot-xfMax);
			yNormal=1.0f-fabs(yDot-yfMin);
			yEccentric=1.0f-fabs(yDot-yfMax);
			DrawAlphaPix(xMin,yMin,pixelColor.r,pixelColor.g,pixelColor.b,SafeByte(pixelColor.a*xNormal*yNormal));
			DrawAlphaPix(xMax,yMin,pixelColor.r,pixelColor.g,pixelColor.b,SafeByte(pixelColor.a*xEccentric*yNormal));
			DrawAlphaPix(xMin,yMax,pixelColor.r,pixelColor.g,pixelColor.b,SafeByte(pixelColor.a*xNormal*yEccentric));
			DrawAlphaPix(xMax,yMax,pixelColor.r,pixelColor.g,pixelColor.b,SafeByte(pixelColor.a*xEccentric*yEccentric));
		}
		catch (exception& exn) {
			ShowExn(exn,"DrawSubpixelDot by rgb pixel");
		}
		catch (...) {
			ShowUnknownExn("DrawSubpixelDot by rgb pixel");
		}
	}  // end DrawSubpixelDot
	void GBuffer::DrawSubpixelDot(float xDot, float yDot, byte* lpbySrcPixel) {
		try {
			bool bMake=false;
			if (lpbySrcPixel==nullptr) {
				lpbySrcPixel=(byte*)malloc(4);
				lpbySrcPixel[0]=0;
				lpbySrcPixel[1]=0;
				lpbySrcPixel[2]=0;
				lpbySrcPixel[3]=0;
				bMake=true;
			}
			static Pixel pixelNow;
			pixelNow.Set(lpbySrcPixel[2],lpbySrcPixel[1],lpbySrcPixel[0],lpbySrcPixel[3]);
			DrawSubpixelDot(xDot,yDot,pixelNow);
			if (bMake) {
				free(lpbySrcPixel);
				lpbySrcPixel=nullptr;
			}
		}
		catch (exception& exn) {
			ShowExn(exn,"DrawSubpixelDot from pixel pointer");
		}
		catch (...) {
			ShowUnknownExn("DrawSubpixelDot from pixel pointer");
		}
	}  // end DrawSubpixelDot(float xDot, float yDot, byte* lpbySrcPixel)
	void GBuffer::DrawSubpixelLine(float xStart, float yStart, float xEnd, float yEnd,
			Pixel &pixelStart, Pixel* pixelEndOrNull, float fPrecisionIncrement) {
		int iLoops=0;
		static int iMaxLoops=1000000; //debug hard-coded limitation
		static float xNow, yNow, xRelMax, yRelMax, rRelMax, thetaRadians, rRel;
		xNow=xStart;
		yNow=yStart;
		xRelMax=xEnd-xStart;
		yRelMax=yEnd-yStart;
		rRelMax=ROFXY(xRelMax,yRelMax);
		thetaRadians=RMath::ThetaOfXY_Rad(xRelMax,yRelMax);
		rRel=0;
		rRel-=fPrecisionIncrement; //the "0th" value
		static Pixel pixelColor;
		pixelColor.r=pixelStart.r;
		pixelColor.g=pixelStart.g;
		pixelColor.b=pixelStart.b;
		pixelColor.a=pixelStart.a;
		while (rRel<rRelMax && iLoops<iMaxLoops) {
			rRel+=fPrecisionIncrement;
			if (pixelEndOrNull!=nullptr) {
				pixelColor.r=SafeByte(APPROACH(pixelStart.r,pixelEndOrNull->r,rRel/rRelMax));
				pixelColor.g=SafeByte(APPROACH(pixelStart.g,pixelEndOrNull->g,rRel/rRelMax));
				pixelColor.b=SafeByte(APPROACH(pixelStart.b,pixelEndOrNull->b,rRel/rRelMax));
				pixelColor.a=SafeByte(APPROACH(pixelStart.a,pixelEndOrNull->a,rRel/rRelMax));
			}
			xNow=(RMath::XOfRTheta_Rad(rRel,thetaRadians))+xStart;
			yNow=(RMath::YOfRTheta_Rad(rRel,thetaRadians))+yStart;
			if (xNow>0&&yNow>0 && xNow<(iWidth-1) && yNow<(iHeight-1))
				DrawSubpixelDot(xNow, yNow, pixelColor);
			iLoops++;
			if (iLoops>=iMaxLoops) break;
		}//end while drawing line
		if (iLoops>=iMaxLoops && iErrors<iMaxErrors) {
			Console::Error.WriteLine("In DrawSubpixelLine: loop overflow!");
			iErrors++;
		}
	}//end DrawSubpixelLine

	void GBuffer::DrawSubpixelLine(FPoint &pointStart, FPoint &pointEnd,
			Pixel &pixelStart, Pixel* pixelEndOrNull, float fPrecisionIncrement) {
		DrawSubpixelLine(pointStart.X, pointStart.Y, pointEnd.X, pointEnd.Y,
			pixelStart, pixelEndOrNull, fPrecisionIncrement);
	}//DrawSubpixelLine
	void GBuffer::DrawSubpixelArc(float xCenter, float yCenter,
			float fRadius, float fWidthMultiplier, float fRotate,
			float fDegStart, float fDegEnd,
			Pixel &pixelColor,
			float fPrecisionIncrement,float fPushSpiralPixPerRotation) {
		static float fTemp,xNow,yNow;
		//TODO: make the fPrecisionIncrement a pixel increment to match other subpixel draw functions
		if (fDegStart>fDegEnd) {
			fTemp=fDegStart;
			fDegStart=fDegEnd;
			fDegEnd=fTemp;
		}
		int iLoops=0;
		static int iMaxLoops=1000000;
		for (float fNow=fDegStart; fNow<fDegEnd; fNow+=fPrecisionIncrement) {
			xNow=(RMath::XOfRTheta_Deg(fRadius,fNow));
			yNow=-(RMath::YOfRTheta_Deg(fRadius,fNow));//negative to flip to non-cartesian monitor loc
			xNow*=fWidthMultiplier;
			RMath::Rotate(xNow,yNow,fRotate);
			xNow+=xCenter;
			yNow+=yCenter;
			if (xNow>0&&yNow>0&&xNow<iWidth&&yNow<iHeight)
				DrawSubpixelDot(xNow, yNow, pixelColor);
			iLoops++;
			if (iLoops>=iMaxLoops) break;
			fRadius+=fPushSpiralPixPerRotation/360.0f;
		}
		if (iLoops>=iMaxLoops && iErrors<iMaxErrors) {
			Console::Error.WriteLine("In DrawSubpixelArc: loop overflow!");
			iErrors++;
		}
	}//DrawSubpixelArc
	bool GBuffer::FillAlpha(byte val) {
		bool bGood=false;
		try {
			if (iBytesPP==4) {
				byte* alphaPtr=&arrbyData[3];
				int iPixels=iWidth*iHeight;
				for (int iPixel=0; iPixel<iPixels; iPixel++) {
					*alphaPtr=val;
					alphaPtr+=iBytesPP;
				}
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"Fill");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Fill");
		}
		return bGood;
	}
	bool GBuffer::Fill(byte byGrayVal) {
		bool bGood=false;
		//int iNow=0;
		try {
			if (arrbyData!=nullptr) {
				memset(arrbyData,byGrayVal,iBytesTotal);
				//while (iNow<iBytesTotal) {
				//	iNow+=
				//}
				bGood=true;
			}
			else {
				bGood=false;
				ShowError("null buffer! {"+RString_ToString(iWidth)+"x"+RString_ToString(iHeight)+"x"+RString_ToString(iBytesPP*8)+"}","GBuffer::Fill");
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"Fill");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Fill");
		}
		return bGood;
	}  // end Fill
	bool GBuffer::IsLoaded() {
		return arrbyData!=nullptr;
	}
	bool GBuffer::IsLike(GBuffer* gbDest) {
		return gbDest!=nullptr
			&& gbDest->iBytesPP==iBytesPP
			&& gbDest->iStride==iStride
			&& gbDest->iHeight==iHeight
			&& gbDest->iBytesTotal==iBytesTotal;
	}
	bool GBuffer::NeedsToCrop(GBuffer* gbDest, int xDest, int yDest) {
		static bool bFirstRun=true;
		bool bReturn=true;
		if (bFirstRun) Console::Error.Write("check NeedsToCrop...");
		if (iBytesTotal==0) {
			ShowError("Getting status on zero-length buffer!","NeedsToCrop");
		}
		try {
			if (xDest>=0 && yDest>=0
				&& xDest+iWidth<=gbDest->iWidth
				&& yDest+iHeight<=gbDest->iHeight) bReturn=false;
		}
		catch (...) {bReturn=true;}
		if (bFirstRun) Console::Error.Write(((bReturn)?"yes...":"no..."));
		bFirstRun=false;
		return bReturn;
	}
	//bool GBuffer::DrawToSmallerWithoutVoidAreasElseCancel(GBuffer gbDest, int xSrc, int ySrc, int iDrawMode) {
	//}
	bool GBuffer::DrawToWithClipping(GBuffer* gbDest, int xDest, int yDest, float fOpacityMultiplier) {
		bool bGood=false;
		static int iDestW;
		static int iDestH;
		static int iSrcW;
		static int iSrcH;
		static int iSrcBytesPP;
		static int iDestBytesPP;
		static int iSrcStride;
		static int iDestStride;
		if (bMegaDebug) {
			Console::Error.Write("TargaToTarga32_Alpha_Clipped...");
			Console::Error.Flush();
		}
		try {
			if (&gbDest!=nullptr) {
				iDestW=gbDest->iWidth;
				iDestH=gbDest->iHeight;
				iSrcW=this->iWidth;
				iSrcH=this->iHeight;
				iSrcBytesPP=this->iBytesPP;
				iDestBytesPP=(int)gbDest->iBytesPP;
				iSrcStride=this->iStride;
				iDestStride=gbDest->iStride;
				int xSrcStart=0;
				int ySrcStart=0;
				int xDestStart=xDest;
				if (xDestStart<0) {
					xSrcStart=-1*xDestStart;
					xDestStart=0;
				}
				int yDestStart=yDest;
				if (yDestStart<0) {
					ySrcStart=-1*yDestStart;
					yDestStart=0;
				}
				if ( xSrcStart<iSrcW && ySrcStart<iSrcH && xDestStart<iDestW && yDestStart<iDestH ) {
				//IF there are any pixels to draw (i.e. within bounds)
					//register unsigned __int32* lpu32Dest=(unsigned __int32*)gbDest->arrbyData;
					//register unsigned __int32* lpdwSrc=(unsigned __int32*)gbDest->arrbyData;
					int iDestStride=gbDest->iStride;

					//register byte *lpbyDestNow=(byte*)&lpu32Dest[xDestStart + (yDestStart*iDestStride>>2)]; //dest pointer
					register byte* lpbyDestNow=&gbDest->arrbyData[ (yDestStart*iDestStride) + (xDestStart*iDestBytesPP) ];
					register byte* lpbyDestLineNow=lpbyDestNow;
					register byte* lpbySrcNow=&this->arrbyData[ (ySrcStart*iSrcStride) + (xSrcStart*iSrcBytesPP) ];
					register byte* lpbySrcLineNow=lpbySrcNow;

					register byte alpha;//,blue,green,red;
					//register unsigned __int32 pixel;
					//int iSrcBytesPP=this->iBytesPP;
					//int iDestBytesPP=gbDest->iBytesPP;
					//int iSrcLineOffset=iSrcStride-this->iWidth*iSrcBytesPP;
					//int iDestLineOffset=iDestStride-gbDest->iWidth*iDestBytesPP;
					if (bMegaDebug) {
						Console::Error.WriteLine("drawing...");//if (bMegaDebug) { Console::Error.WriteLine("source lines..."); Console::Error.Flush(); }
						Console::Error.Flush();
					}
					int xSrcEndEx=iSrcW;//exclusive ender
					if (xDestStart+xSrcEndEx>iDestW) xSrcEndEx=iDestW-xDestStart;
					int ySrcEndEx=iSrcH;//exclusive ender
					if (yDestStart+ySrcEndEx>iDestH) ySrcEndEx=iDestH-yDestStart;
					byte byOpacityMultiplier_ByteAsDecimal=SafeByte(fOpacityMultiplier*255.0f);
					for (register int ySrc=ySrcStart; ySrc < ySrcEndEx; ySrc++) {
						//if (bMegaDebug) Console::Error.WriteLine(RString_ToString(ySrc)+RString_ToString(" "));
						lpbySrcNow=lpbySrcLineNow;
						lpbyDestNow=lpbyDestLineNow;
						for (register int xSrc=xSrcStart; xSrc < xSrcEndEx; xSrc++) {

							//Get BGR values and alpha
							//byte blue	= (this->arrbyData[ySrc*this->width*4 + xSrc*4 + 0]),
							//	green=(this->arrbyData[ySrc*this->width*4 + xSrc*4 + 1]),
							//	red	=(this->arrbyData[ySrc*this->width*4 + xSrc*4 + 2]),
							//	alpha=(this->arrbyData[ySrc*this->width*4 + xSrc*4 + 3]);

							alpha=*(lpbySrcNow+3);

							//blue=*lpbySrcNow++;
							//green=*lpbySrcNow++;
							//red=*lpbySrcNow++;
							//alpha=*lpbySrcNow++;//must increment sinc next iteration of X needs to start at beginning of next pixel
							alpha=by2d_ByteAsByte_times_ByteAsFloatLookup[alpha][byOpacityMultiplier_ByteAsDecimal];//alpha=(byte)((float)alpha*fOpacityMultiplier);
							//if (alpha>opacityCap) alpha=opacityCap; //"Cap" opacity off at opacityCap to create transparency if not 255

							if (alpha==0) {
								lpbyDestNow+=4;//assumes 32-bit
								lpbySrcNow+=4;//assumes 32-bit
							}
							else if (alpha==127) {
								//Do quick average
								*lpbyDestNow=(*lpbyDestNow + *lpbySrcNow++)/2;
								lpbyDestNow++;
								*lpbyDestNow=(*lpbyDestNow + *lpbySrcNow++)/2;
								lpbyDestNow++;
								*lpbyDestNow=(*lpbyDestNow + *lpbySrcNow)/2;
								lpbyDestNow+=2; //increment past alpha
								lpbySrcNow+=2; //increment past alpha
								//*(unsigned __int32*)(lpbyDestNow-4)=_RGB32BIT(255,red,green,blue);//pixel=_RGB32BIT(255,red,green,blue);
								//no need to clip since clipping is done by loop:
								//if (xSrc+xDest<iDestW)
								//	if (xSrc+xDest>=0)
								//		if (ySrc+yDest>=0)
								//			if (ySrc+yDest<iDestH)
								//				lpu32Dest[xDest + xSrc + ((ySrc+yDest)*iDestStride >> 2)]=pixel;

							}
							else if (alpha==255) {
								//lpbyDestNow+=4;
								*(Uint32*)(lpbyDestNow) = *(Uint32*)(lpbySrcNow);  // _RGB32BIT(255,red,green,blue);//pixel=_RGB32BIT(255,red,green,blue);
								//no need to clip since clipping is done by loop:
								//if (xSrc+xDest<SCREEN_WIDTH)
								//	if (xSrc+xDest>=0)
								//		if (ySrc+yDest>=0)
								//			if (ySrc+yDest<SCREEN_HEIGHT)
								//				lpu32Dest[xDest + xSrc + ((ySrc+yDest)*iDestStride >> 2)]=pixel;
								lpbyDestNow+=4;
								lpbySrcNow+=4;
							}
							else {
								//Do alpha, FORMULA: ((Source-Dest)*alpha/255+Dest)
								register float cookedAlpha=(float)alpha/255.0f;
								*lpbyDestNow=SafeByte(  ( (*lpbySrcNow-*lpbyDestNow) * cookedAlpha )  +  *lpbyDestNow  );
								lpbyDestNow++;
								lpbySrcNow++;
								*lpbyDestNow=SafeByte(  ( (*lpbySrcNow-*lpbyDestNow) * cookedAlpha )  +  *lpbyDestNow  );
								lpbyDestNow++;
								lpbySrcNow++;
								*lpbyDestNow=SafeByte(  ( (*lpbySrcNow-*lpbyDestNow) * cookedAlpha )  +  *lpbyDestNow  );
								lpbyDestNow+=2; //increment past alpha
								lpbySrcNow+=2; //increment past alpha
								//*(unsigned __int32*)(lpbyDestNow-4)=_RGB32BIT(255,red,green,blue);//pixel=_RGB32BIT(255,red,green,blue);
								//unsigned __int32 alphaPix=_RGB32BIT(1,alpha/255,alpha/255,alpha/255);
								//Manual clip and set pixel:
								//if (xSrc+xDest<SCREEN_WIDTH)
								//	if (xSrc+xDest>=0)
								//		if (ySrc+yDest>=0)
								//			if (ySrc+yDest<SCREEN_HEIGHT)
								//				lpu32Dest[xDest + xSrc + ((ySrc+yDest)*iDestStride >> 2)]=pixel;
							}
						} //end for xSrc
						lpbySrcLineNow+=iSrcStride;//lpbyDestNow+=iDestLineOffset;
						lpbyDestLineNow+=iDestStride;
					} // end for ySrc
				}//end if any pixels are within either image
				bGood=true;
			}//end if both images are non-null
		}
		catch (exception &exn) {
			ShowExn(exn, "running TargaToScreen");
			bGood=false;
		}
		catch (...) {
			ShowUnknownExn("running TargaToScreen");
			bGood=false;
		}
		if (bMegaDebug) Console::Error.WriteLine("Finished");
		return(bGood);
	}  // end DrawToWithClipping
	bool GBuffer::DrawToLargerWithoutCropElseCancel(GBuffer* gbDest, int xDest, int yDest, int iDrawMode) {
		bool bGood=true;
		static bool bFirstRun=true;
		if (arrbyData==nullptr) {
			ShowError("Tried to draw null buffer!","DrawToLargerWithoutCropElseCancel");
			return false;
		}
		try {
			if (bFirstRun) Console::Error.Write("DrawToLargerWithoutCropElseCancel...");
			if (NeedsToCrop(gbDest,xDest,yDest)) {
				if (bFirstRun) {
					Console::Error.Write("failed since not in bounds("+RString_ToString(iWidth)+"x"+RString_ToString(iHeight)+" to "+RString_ToString(gbDest->iWidth)+"x"+RString_ToString(gbDest->iHeight)+" at ("+RString_ToString(xDest)+","+RString_ToString(yDest)+") )...");
				}
				bGood=false;
			}
			if (bGood) {
				if (bFirstRun) Console::Error.Write("offset...");
				byte* lpDestLine=&gbDest->arrbyData[yDest*gbDest->iStride+xDest*gbDest->iBytesPP];
				byte* lpDestPix;
				byte* lpSrcLine=arrbyData;
				byte* lpSrcPix;
				int iStrideMin=(iStride<gbDest->iStride)?iStride:gbDest->iStride;
				int dest_MoreBytes=gbDest->iBytesPP-2;
				int src_MoreBytes=iBytesPP-2;
				int iMinBytesPP = (iBytesPP<gbDest->iBytesPP) ? iBytesPP : gbDest->iBytesPP;
				if (gbDest->iBytesPP>=2 && iBytesPP>=2) {
					switch (iDrawMode) {
					case DrawModeCopyAlpha:
						if (bFirstRun) Console::Error.Write("DrawModeCopyAlpha...");
						if (IsLike(gbDest) && xDest==0 && yDest==0) {
							memcpy(gbDest->arrbyData,arrbyData,iBytesTotal);
						}
						else if (gbDest->iBytesPP==iBytesPP) {
							for (int Y=0; Y<iHeight; Y++) {
								memcpy(lpDestLine,lpSrcLine,iStrideMin);
								lpDestLine+=gbDest->iStride;
								lpSrcLine+=iStride;
							}
						}
						else {
							register int X,Y;
							for (Y=0; Y<iHeight; Y++) {
								//if (bFirstRun) Console::Error.Write(RString_ToString(Y));
								lpDestPix=lpDestLine;
								lpSrcPix=lpSrcLine;
								for (X=0; X<iWidth; X++) {
									memcpy(lpDestPix, lpSrcPix, iMinBytesPP);
									lpSrcPix+=iBytesPP; lpDestPix+=gbDest->iBytesPP;
								}
								lpDestLine+=gbDest->iStride;
								lpSrcLine+=iStride;
							}
						}
						break;
					case DrawModeBlendAlpha:
						if (bFirstRun) Console::Error.Write("DrawModeBlendAlpha(...)");
						// if (bFirstRun) Console::Error.Write("("+RString_ToString(iWidth)+"x"+RString_ToString(iHeight)+"x"+RString_ToString(iBytesPP)+" to "+RString_ToString(gbDest->iWidth)+"x"+RString_ToString(gbDest->iHeight)+")...");
						// alpha result: ((Source-Dest)*alpha/255+Dest)
						float fCookedAlpha;
						// if (bFirstRun) Console::Error.WriteLine();
						register int X,Y;
						for (Y=0; Y<iHeight; Y++) {
							//if (bFirstRun) Console::Error.Write(RString_ToString(Y));
							lpDestPix=lpDestLine;
							lpSrcPix=lpSrcLine;
							for (X=0; X<iWidth; X++) {
								//if (bFirstRun) Console::Error.Write(".");
								if (lpSrcPix[3]==0) {
									lpSrcPix+=iBytesPP; lpDestPix+=gbDest->iBytesPP;
								}
								else if (lpSrcPix[3]==255) {
									memcpy(lpDestPix, lpSrcPix, iMinBytesPP);
									lpSrcPix+=iBytesPP; lpDestPix+=gbDest->iBytesPP;
								}
								else {
									fCookedAlpha=(float)lpSrcPix[3]/255.0f;
									*lpDestPix=BYROUNDF(((float)(*lpSrcPix-*lpDestPix))*fCookedAlpha+*lpDestPix); //B
									lpSrcPix++; lpDestPix++;
									*lpDestPix=BYROUNDF(((float)(*lpSrcPix-*lpDestPix))*fCookedAlpha+*lpDestPix); //G
									lpSrcPix++; lpDestPix++;
									*lpDestPix=BYROUNDF(((float)(*lpSrcPix-*lpDestPix))*fCookedAlpha+*lpDestPix); //R
									lpSrcPix+=src_MoreBytes; lpDestPix+=dest_MoreBytes;
								}
							}
							lpDestLine+=gbDest->iStride;
							lpSrcLine+=iStride;
							//if (bFirstRun) Console::Error.WriteLine();
						}
						break;
					case DrawModeKeepGreaterAlpha:
						if (bFirstRun) Console::Error.Write("DrawModeKeepGreaterAlpha...");
						//alpha result: ((Source-Dest)*alpha/255+Dest)
						for (int Y=0; Y<iHeight; Y++) {
							lpDestPix=lpDestLine;
							lpSrcPix=lpSrcLine;
							for (int X=0; X<iWidth; X++) {
								*lpDestPix=(*lpSrcPix>*lpDestPix)?*lpSrcPix:*lpDestPix; //B
								lpSrcPix++; lpDestPix++;
								*lpDestPix=(*lpSrcPix>*lpDestPix)?*lpSrcPix:*lpDestPix; //G
								lpSrcPix++; lpDestPix++;
								*lpDestPix=(*lpSrcPix>*lpDestPix)?*lpSrcPix:*lpDestPix; //R
								lpSrcPix+=src_MoreBytes; lpDestPix+=dest_MoreBytes;
							}
							lpDestLine+=gbDest->iStride;
							lpSrcLine+=iStride;
						}
						break;
					case DrawModeKeepDestAlpha:
						if (bFirstRun) Console::Error.Write("DrawModeKeepDestAlpha...");
						for (int Y=0; Y<iHeight; Y++) {
							lpDestPix=lpDestLine;
							lpSrcPix=lpSrcLine;
							for (int X=0; X<iWidth; X++) {
								memcpy(lpDestPix,lpSrcPix,iMinBytesPP);
								lpDestPix+=gbDest->iBytesPP; lpSrcPix+=iBytesPP;
							}
							lpDestLine+=gbDest->iStride;
							lpSrcLine+=iStride;
						}
						break;
					}//end switch
				}
				else {
					string msg="Can't draw fast unless both GBuffers are 32-bit BGRA {";
					msg+="this->iBytesPP:"+RString_ToString(this->iBytesPP)+"; "
						+"gbDest->iBytesPP:"+RString_ToString(gbDest->iBytesPP)+"; "
						+"}"
						;
					ShowError(msg,"GBuffer::Draw");
				}
			}//end if does not need to crop
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"GBuffer.DrawToLargerWithoutCropElseCancel");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("GBuffer.DrawToLargerWithoutCropElseCancel");
		}
		if (bFirstRun) Console::Error.WriteLine(bGood?"DrawToLargerWithoutCropElseCancel Success...":"DrawToLargerWithoutCropElseCancel failed...");
		bFirstRun=false;
		return bGood;
	}  // end DrawToLargerWithoutCropElseCancel
	//#endregion class GBuffer methods

	//#region GBuffer functions
	/// <summary>
	/// gbDest must be true color 24- or 32-bit for the raw source
	/// to be represented correctly.
	/// </summary>
	/// <param name="arrbySrc"></param>
	/// <param name="gbDest"></param>
	/// <param name="iSrcWidth"></param>
	/// <param name="iSrcHeight"></param>
	/// <param name="iSrcBytesPP"></param>
	/// <returns></returns>
	bool RawOverlayNoClipToBig(GBuffer* gbDest, IPoint &ipAt, byte* arrbySrc, int iSrcWidth, int iSrcHeight, int iSrcBytesPP) {
		int iSrcByte;
		int iDestByte;
		bool bGood=true;
		int iDestAdder;
		try {
			if (gbDest!=nullptr) {
				if (iSrcBytesPP==16) {
					ShowError("16-bit source isn't implemented in this function","RawOverlayNoClipToBig");
					bGood=false;
				}
				if (bGood) {
					iDestByte=ipAt.Y*gbDest->iStride+ipAt.X*gbDest->iBytesPP;
					int gbSrc_iStride=iSrcWidth*iSrcBytesPP;
					iDestAdder=gbDest->iStride - iSrcWidth*gbDest->iBytesPP;//intentionally gbDest->iBytesPP
					iSrcByte=0;
					int iSlack=(iSrcBytesPP>gbDest->iBytesPP)?(iSrcBytesPP-gbDest->iBytesPP):1;
							//offset of next source pixel after loop
					for (int ySrc=0; ySrc<iSrcHeight; ySrc++) {
						for (int xSrc=0; xSrc<iSrcWidth; xSrc++) {
							for (int iChannel=0; iChannel<gbDest->iBytesPP; iChannel++) {
								gbDest->arrbyData[iDestByte]=arrbySrc[iSrcByte];
								if ((iChannel+1)<iSrcBytesPP) iSrcByte++;//don't advance to next pixel
								iDestByte++;
							}
							iSrcByte+=iSlack;
						}
						iDestByte+=iDestAdder;
					}
					if (bGood==false) {
						ShowError("Error copying graphics buffer data","OverlayNoClipToBigCopyAlpha(...)");
					}
				}//end if bGood
			}
			else Console::Error.WriteLine("RawOverlayNoClipToBig Error: nullptr gbDest");
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"RawOverlayNoClipToBig");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("RawOverlayNoClipToBig");
		}
		return bGood;
	}  // end RawOverlayNoClipToBig

	/// <summary>
	/// Gradient version of Alpha overlay
	/// </summary>
	/// <param name="gbDest"></param>
	/// <param name="gbSrc"></param>
	/// <param name="ipDest"></param>
	/// <param name="gradNow"></param>
	/// <param name="iSrcChannel"></param>
	/// <returns></returns>
	bool OverlayNoClipToBig(GBuffer* gbDest, GBuffer* gbSrc, IPoint &ipDest, Gradient &gradNow, int iSrcChannel) {
		int iSrcByte;
		int iDestByte;
		int iDestAdder;
		static bool IsFirst_OverlayNoClipToBig_Error=true;
		bool bGood=true;
		if (gbDest!=nullptr) {
			if (gbDest->iBytesPP>0) {
				try {
					iDestByte=ipDest.Y*gbSrc->iStride+ipDest.X*gbSrc->iBytesPP;
					iSrcByte=(iSrcChannel<gbSrc->iBytesPP)?iSrcChannel:gbSrc->iBytesPP-1;
					iDestAdder=gbDest->iStride - gbDest->iBytesPP*gbSrc->iWidth;//intentionally the dest BytesPP
					for (int ySrc=0; ySrc<gbSrc->iHeight; ySrc++) {
						for (int xSrc=0; xSrc<gbSrc->iWidth; xSrc++) {
							//if (!
							gbDest->ShadeAlpha(&gradNow, iDestByte, gbSrc->arrbyData[iSrcByte], DrawModeBlendAlpha);//) {
								//TODO: change above to ShadeAlpha
							//	bGood=false;
							//}
							iSrcByte+=gbSrc->iBytesPP;
							iDestByte+=gbDest->iBytesPP;
						}
						iDestByte+=iDestAdder;
					}
					if (bGood==false) {
						ShowError("Error shading","OverlayNoClipToBig gradient to "+ipDest.ToString());
					}
				}
				catch (exception& exn) {
					bGood=false;
					if (&ipDest==nullptr) ShowExn(exn,"OverlayNoClipToBig gradient to nullptr point");
					else ShowExn(exn,"OverlayNoClipToBig gradient to "+ipDest.ToString());
				}
				catch (...) {
					bGood=false;
					if (&ipDest==nullptr) ShowUnknownExn("OverlayNoClipToBig gradient to nullptr point");
					else ShowUnknownExn("OverlayNoClipToBig gradient to "+ipDest.ToString());
				}
			}
			else {
				bGood=false;
				if (IsFirst_OverlayNoClipToBig_Error) {
					cout << "<OverlayNoClipToBig ERROR: "<<gbDest->iBytesPP<<"-BytesPP destination>"<< flush;
					IsFirst_OverlayNoClipToBig_Error=false;
				}
			}
		}
		else {
			if (IsFirst_OverlayNoClipToBig_Error) {
				cout << "<OverlayNoClipToBig ERROR: nullptr destination>"<< flush;
				IsFirst_OverlayNoClipToBig_Error=false;
			}
		}
		return bGood;
	}  // end OverlayNoClipToBig

	/// <summary>
	/// Gradient version of CopyAlpha (no blending) overlay
	/// </summary>
	/// <param name="gbDest"></param>
	/// <param name="ipAt"></param>
	/// <param name="gbSrc"></param>
	/// <param name="gradNow"></param>
	/// <param name="iSrcChannel"></param>
	/// <returns></returns>
	bool OverlayNoClipToBigCopyAlpha(GBuffer* gbDest, IPoint &ipAt, GBuffer* gbSrc, Gradient &gradNow, int iSrcChannel) {
		int iSrcByte;
		int iDestByte;
		int iDestLineLocNow;//int iDestAdder;
		bool bGood=true;
		static bool IsFirst_OverlayNoClipToBigCopyAlpha_gradient_version_Error=true;
		if (gbSrc!=nullptr) {
			if (gbDest!=nullptr) {
				if (gbDest->iBytesPP>0) {
					try {
						iSrcByte=((iSrcChannel<gbSrc->iBytesPP)&&(iSrcChannel>=0))?iSrcChannel:gbSrc->iBytesPP-1; //default is iBytesPP-1 since that would be the alpha channel, OR value channel if 8-bit
						iDestLineLocNow=ipAt.Y*gbDest->iStride+ipAt.X*gbDest->iBytesPP;//iDestByte;
						//iDestByte=ipAt.Y*gbDest->iStride+ipAt.X*gbDest->iBytesPP;
						//iDestAdder=gbDest->iStride - gbSrc->iWidth*gbDest->iBytesPP;//was intentionally the dest BytesPP, because width of source is traversed on dest for each line
						for (int ySrc=0; ySrc<gbSrc->iHeight; ySrc++) {
							iDestByte=iDestLineLocNow;
							for (int xSrc=0; xSrc<gbSrc->iWidth; xSrc++) {
								//if (!
								gbDest->ShadeAlpha(&gradNow, iDestByte, gbSrc->arrbyData[iSrcByte], DrawModeKeepDestAlpha);//) {
								//	bGood=false;
								//}
								iSrcByte+=gbSrc->iBytesPP;
								iDestByte+=gbDest->iBytesPP;
							}
							//iDestByte+=iDestAdder;
							iDestLineLocNow+=gbDest->iStride;
						}
						if (!bGood) {
							ShowError("Error copying graphics buffer data","OverlayNoClipToBigCopyAlpha(GBuffer,IPoint,GBuffer,Gradient,int)");
						}
					}
					catch (exception& exn) {
						bGood=false;
						ShowExn(exn,"OverlayNoClipToBigCopyAlpha gradient");
					}
					catch (...) {
						bGood=false;
						ShowUnknownExn("OverlayNoClipToBigCopyAlpha gradient");
					}
				}
				else {
					bGood=false;
					if (IsFirst_OverlayNoClipToBigCopyAlpha_gradient_version_Error) {
						cout << "<OverlayNoClipToBigCopyAlpha(. . . Gradient) ERROR: "<<gbDest->iBytesPP<<"-BytesPP destination>"<< flush;
						IsFirst_OverlayNoClipToBigCopyAlpha_gradient_version_Error=false;
					}
				}
			}
			else Console::Error.WriteLine("OverlayNoClipToBigCopyAlpha gradient Error: gbDest is nullptr");
		}
		else Console::Error.WriteLine("OverlayNoClipToBigCopyAlpha gradient Error: gbSrc is nullptr");
		return bGood;
	}  // end OverlayNoClipToBigCopyAlpha gradient

	/// <summary>
	/// CopyAlpha overlay function.
	/// "ToBig" functions must overlay small
	/// image to big image without cropping else unexpected results occur.
	/// </summary>
	/// <param name="gbDest"></param>
	/// <param name="ipAt"></param>
	/// <param name="gbSrc"></param>
	/// <returns></returns>
	bool OverlayNoClipToBigCopyAlpha(GBuffer* gbDest, IPoint &ipAt, GBuffer* gbSrc) {
		int iSrcByte;
		int iDestByte;
		bool bGood=true;
		try {
			iDestByte=ipAt.Y*gbDest->iStride+ipAt.X*gbDest->iBytesPP;
			iSrcByte=0;
			for (int ySrc=0; ySrc<gbSrc->iHeight; ySrc++) {
				if (false==CopySafe(gbDest->arrbyData, gbSrc->arrbyData, iDestByte, iSrcByte, gbSrc->iStride)) {
					bGood=false;
				}
				iSrcByte+=gbSrc->iStride;
				iDestByte+=gbDest->iStride;
			}
			if (bGood==false) {
				ShowError("Error copying graphics buffer data","OverlayNoClipToBigCopyAlpha(...)");
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"OverlayNoClipToBigCopyAlpha(...)");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("OverlayNoClipToBigCopyAlpha(...)");
		}
		return bGood;
	}  // end OverlayNoClipToBigCopyAlpha
	//bool MaskFromChannelByRef(GBuffer* gbDest, GBuffer* gbSrc, int iByteInPixel) {
	//	MaskFromChannel(&gbDest, &gbSrc, iByteInPixel);
	//}
	bool MaskFromChannel(GBuffer* gbDest, GBuffer* gbSrc, int iByteInPixel) {
		int iDestByte=0;
		if (iByteInPixel<0) iByteInPixel=gbSrc->iBytesPP-1;
		else if (iByteInPixel>=gbSrc->iBytesPP) {
			Console::Error.Write("Error in MaskFromChannel while copying channel {iChannelOffset:"+RString_ToString(iByteInPixel)+"; gbSrc-lastchannel:"+RString_ToString(gbSrc->iBytesPP-1)+"; gbSrc->iBytesPP:"+RString_ToString(gbSrc->iBytesPP)+"}: got bad channel so defaulting to last channel for value!");
			Console::Error.Flush();
			iByteInPixel=gbSrc->iBytesPP-1;
		}
		int iSrcByte=iByteInPixel;
		int iBytesCopy;
		int iBytesPPOffset;
		bool bGood=true;
		try {
			if (gbDest!=nullptr) {
				if (gbSrc!=nullptr) {
					if (gbDest->arrbyData==nullptr) {
						gbDest->Init((int)gbSrc->iWidth, (int)gbSrc->iHeight, 1);
					}
					else if ((gbDest->iWidth!=gbSrc->iWidth)||(gbDest->iHeight!=gbSrc->iHeight)||(gbDest->iBytesPP!=1)) {
						gbDest->Dispose();
						gbDest->Init((int)gbSrc->iWidth, (int)gbSrc->iHeight, 1);
					}
					iBytesCopy=gbDest->iBytesTotal;
					iBytesPPOffset=gbSrc->iBytesPP;
					for (iDestByte=0; iDestByte<iBytesCopy; iDestByte++) {
						gbDest->arrbyData[iDestByte]=gbSrc->arrbyData[iSrcByte];
						iDestByte++;
						iSrcByte+=iBytesPPOffset;
					}
				}
				else {
					bGood=false;
					Console::Error.WriteLine("MaskFromChannel Error: nullptr gbSrc");
				}
			}
			else {
				bGood=false;
				Console::Error.WriteLine("MaskFromChannel Error: nullptr gbDest");
			}
		}
		catch (exception& exn) {
			string sMsg="{\n";
			sMsg+="  iByteInPixel:"; sMsg+=RString_ToString(iByteInPixel);
				//+"; iDestCharPitch:"+iDestCharPitch.ToString()
				//+"; iChar1:"+iChar1.ToString()
				//+"; iCharNow:"+iCharNow.ToString()
				//+"; yNow:"+yNow.ToString()
				//+"; xNow:"+xNow.ToString()
			sMsg+="; iSrcByte:"; sMsg+=RString_ToString(iSrcByte);
			sMsg+="; iDestByte:"+RString_ToString(iDestByte) +"}";
			ShowExn(exn,"MaskFromChannel()",sMsg);
			bGood=false;
		}
		catch (...) {
			string sMsg="{\n  iByteInPixel:"+RString_ToString(iByteInPixel)
				//+"; iDestCharPitch:"+iDestCharPitch.ToString()
				//+"; iChar1:"+iChar1.ToString()
				//+"; iCharNow:"+iCharNow.ToString()
				//+"; yNow:"+yNow.ToString()
				//+"; xNow:"+xNow.ToString()
				+"; iSrcByte:"+RString_ToString(iSrcByte)
				+"; iDestByte:"+RString_ToString(iDestByte) +"}";
			ShowUnknownExn("MaskFromChannel()",sMsg);
			bGood=false;
		}
		return bGood;
	}  // end MaskFromChannel

	///<summary>
	///Uses average of 3 channels to get value
	///</summary>
	//bool MaskFromValueByRef(GBuffer* gbDest, GBuffer* gbSrc) {
	//	MaskFromValue(&gbDest, &gbSrc);
	//}
	bool MaskFromValue(GBuffer* gbDest, GBuffer* gbSrc) {
		int iDestByte=0;
		int iSrcByte=0;
		int iPixels;
		//int iBytesPPOffset;
		bool bGood=true;
		if (gbDest!=nullptr) gbDest->sFile="<MaskFromValue about to check size of >"+gbDest->sFile;
		try {
			if (gbDest!=nullptr) {
				if (gbSrc!=nullptr) {
					if (gbDest->arrbyData==nullptr) {
						gbDest->Init((int)gbSrc->iWidth, (int)gbSrc->iHeight, 1);
					}
					else if ((gbDest->iWidth!=gbSrc->iWidth)||(gbDest->iHeight!=gbSrc->iHeight)||(gbDest->iBytesPP!=1)) {
						gbDest->Dispose();
						gbDest->Init((int)gbSrc->iWidth, (int)gbSrc->iHeight, 1);
					}
					gbDest->sFile="Mask of "+gbSrc->sFile;
					iPixels=gbSrc->iWidth*gbSrc->iHeight;
					// iBytesPPOffset=gbSrc->iBytesPP;
					if (gbSrc->iBytesPP>=3) {
						for (iDestByte=0; iDestByte<iPixels; iDestByte++) {
							gbDest->arrbyData[iDestByte]=(byte)(((float)gbSrc->arrbyData[iSrcByte]
									+(float)gbSrc->arrbyData[iSrcByte+1]
									+(float)gbSrc->arrbyData[iSrcByte+2])/3.0f);
							iSrcByte+=gbSrc->iBytesPP;
						}
					}
					else if (gbSrc->iBytesPP==1) {
						for (iDestByte=0; iDestByte<iPixels; iDestByte++) {
							gbDest->arrbyData[iDestByte]=gbSrc->arrbyData[iSrcByte];
							iSrcByte+=gbSrc->iBytesPP;
						}
					}
					else {
						ShowErr("MaskFromValue(GBuffer,GBuffer) cannot process source buffer bit depth {gbSrc->iBytesPP:"+RString_ToString(gbSrc->iBytesPP)+"}");
					}
				}
				else {
					bGood=false;
					Console::Error.WriteLine("MaskFromValue Error: nullptr gbSrc");
				}
			}
			else {
				bGood=false;
				Console::Error.WriteLine("MaskFromValue Error: nullptr gbDest");
			}
		}
		catch (exception& exn) {
			string sLastErr="make sure source bitmap is 24-bit or 32-bit {\n";
				//+"  "+"iByteInPixel:"+iByteInPixel.ToString()
				//+"; iDestCharPitch:"+iDestCharPitch.ToString()
				//+"; iChar1:"+iChar1.ToString()
				//+"; iCharNow:"+iCharNow.ToString()
				//+"; yNow:"+yNow.ToString()
				//+"; xNow:"+xNow.ToString()
			sLastErr+="; iSrcByte:"+RString_ToString(iSrcByte);
			sLastErr+="; iDestByte:"+RString_ToString(iDestByte) +"}";
			ShowExn(exn,"MaskFromValue()"+sLastErr);
			bGood=false;
		}
		catch (...) {
			string sLastErr="make sure source bitmap is 24-bit or 32-bit {\n";
				//+"  "+"iByteInPixel:"+iByteInPixel.ToString()
				//+"; iDestCharPitch:"+iDestCharPitch.ToString()
				//+"; iChar1:"+iChar1.ToString()
				//+"; iCharNow:"+iCharNow.ToString()
				//+"; yNow:"+yNow.ToString()
				//+"; xNow:"+xNow.ToString()
			sLastErr+="; iSrcByte:"+RString_ToString(iSrcByte)
				+"; iDestByte:"+RString_ToString(iDestByte) +"}";
			ShowUnknownExn("MaskFromValue()",sLastErr);
			bGood=false;
		}
		return bGood;
	}  // end MaskFromValue
	bool InterpolatePixel(GBuffer* gbDest, GBuffer* gbSrc, int iDest, DPoint &dpSrc) {
		bool bGood=false;
		bool bOnX;
		bool bOnY;
		double dWeightNow;
		double dWeightTotal;
		double dHeavyChannel;
		DPoint dparrQuad[4]; //rounded
		int iSrcRoundX;
		int iSrcRoundY;
		double dSrcRoundX;
		double dSrcRoundY;
		int iSampleQuadIndex;
		double dMaxX;
		double dMaxY;
		int iQuad;
		int iChan;
		int iDestNow;
		int iTotal=0;
		int iarrLocOfQuad[4];
		try {
			// iarrLocOfQuad=(int*)malloc(4*sizeof(int));
			dMaxX=(double)gbSrc->iWidth-1.0;
			dMaxY=(double)gbSrc->iHeight-1.0;
			// iDest=gbDest->iStride*ipDest.Y+gbDest->iBytesPP*ipDest.X;
			dWeightNow=0;
			dWeightTotal=0;
			// dparrQuad=new DPoint[4];
			iSrcRoundX=(int)(dpSrc.X+.5);
			iSrcRoundY=(int)(dpSrc.Y+.5);
			dSrcRoundX=(double)iSrcRoundX;
			dSrcRoundY=(double)iSrcRoundY;
			if (dSrcRoundX<dpSrc.X) {
				if (dSrcRoundY<dpSrc.Y) {
					iSampleQuadIndex=0;
					dparrQuad[0].X=dSrcRoundX;		dparrQuad[0].Y=dSrcRoundY;
					dparrQuad[1].X=dSrcRoundX+1.0;	dparrQuad[1].Y=dSrcRoundY;
					dparrQuad[2].X=dSrcRoundX;		dparrQuad[2].Y=dSrcRoundY+1.0;
					dparrQuad[3].X=dSrcRoundX+1.0;	dparrQuad[3].Y=dSrcRoundY+1.0;
				}
				else {
					iSampleQuadIndex=2;
					dparrQuad[0].X=dSrcRoundX;		dparrQuad[0].Y=dSrcRoundY-1.0;
					dparrQuad[1].X=dSrcRoundX+1.0;	dparrQuad[1].Y=dSrcRoundY-1.0;
					dparrQuad[2].X=dSrcRoundX;		dparrQuad[2].Y=dSrcRoundY;
					dparrQuad[3].X=dSrcRoundX+1.0;	dparrQuad[3].Y=dSrcRoundY;
				}
			}
			else {
				if (dSrcRoundY<dpSrc.Y) {
					iSampleQuadIndex=1;
					dparrQuad[0].X=dSrcRoundX-1.0;	dparrQuad[0].Y=dSrcRoundY;
					dparrQuad[1].X=dSrcRoundX;		dparrQuad[1].Y=dSrcRoundY;
					dparrQuad[2].X=dSrcRoundX-1.0;	dparrQuad[2].Y=dSrcRoundY+1.0;
					dparrQuad[3].X=dSrcRoundX;		dparrQuad[3].Y=dSrcRoundY+1.0;
				}
				else {
					iSampleQuadIndex=3;
					dparrQuad[0].X=dSrcRoundX-1.0;	dparrQuad[0].Y=dSrcRoundY-1.0;
					dparrQuad[1].X=dSrcRoundX;		dparrQuad[1].Y=dSrcRoundY-1.0;
					dparrQuad[2].X=dSrcRoundX-1.0;	dparrQuad[2].Y=dSrcRoundY;
					dparrQuad[3].X=dSrcRoundX;		dparrQuad[3].Y=dSrcRoundY;
				}
			}
			if (dpSrc.X<0) {
				for (iQuad=0; iQuad<4; iQuad++) {
					if (dparrQuad[iQuad].X<0) dparrQuad[iQuad].X=0;
				}
			}
			else if (dpSrc.X>dMaxX) {
				for (iQuad=0; iQuad<4; iQuad++) {
					if (dparrQuad[iQuad].X>dMaxX) dparrQuad[iQuad].X=dMaxX;
				}
			}
			if (dpSrc.Y<0) {
				for (iQuad=0; iQuad<4; iQuad++) {
					if (dparrQuad[iQuad].Y<0) dparrQuad[iQuad].Y=0;
				}
			}
			else if (dpSrc.Y>dMaxY) {
				for (iQuad=0; iQuad<4; iQuad++) {
					if (dparrQuad[iQuad].Y>dMaxY) dparrQuad[iQuad].Y=dMaxY;
				}
			}
			if (dpSrc.X==(double)iSrcRoundX) bOnX=true;
			else bOnX=false;
			if (dpSrc.Y==(double)iSrcRoundY) bOnY=true;
			else bOnY=false;

			if (bOnY&&bOnX) {
				CopySafe(gbDest->arrbyData, gbSrc->arrbyData, iDest, iSrcRoundY*gbSrc->iStride+iSrcRoundX*gbSrc->iBytesPP, gbDest->iBytesPP);
			}
			else {
				iDestNow=iDest;
				for (iQuad=0; iQuad<4; iQuad++) {
					iarrLocOfQuad[iQuad]=gbSrc->iStride*(int)dparrQuad[iQuad].Y + gbSrc->iBytesPP*(int)dparrQuad[iQuad].X;
				}
				for (iChan=0; iChan<gbSrc->iBytesPP; iChan++, iTotal++) {
					dHeavyChannel=0;
					dWeightTotal=0;
					for (iQuad=0; iQuad<4; iQuad++) {
						dWeightNow=dDiagonalUnit-RMath::Dist(dpSrc, dparrQuad[iQuad]);
						dWeightTotal+=dWeightNow; //debug performance, this number is always the same theoretically
						dHeavyChannel+=(double)gbSrc->arrbyData[iarrLocOfQuad[iQuad]+iChan]*dWeightNow;
					}
					gbDest->arrbyData[iDestNow]=(byte)(dHeavyChannel/dWeightTotal);
					iDestNow++;
				}
			}
			bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"InterpolatePixel");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("InterpolatePixel");
		}
		return bGood;
	}  // end InterpolatePixel
	/// <summary>
	/// Fakes motion blur.
	///   Using a byDecayTotal of 255 makes the blur trail fade to transparent.
	/// </summary>
	bool EffectMoBlurSimModWidth(GBuffer* gbDest, GBuffer* gbSrc, int xOffsetTotal, byte byDecayTotal) {
		bool bGood=true;
		int xDirection;
		int xLength;
		int iDestByteStart;
		if (xOffsetTotal<0) {
			xDirection=-1;
			xLength=xOffsetTotal*-1;
		}
		else {
			xDirection=1;
			xLength=xOffsetTotal;
		}
		try {
			gbDest->iWidth=gbSrc->iWidth+xLength;
			gbDest->iBytesPP=gbSrc->iBytesPP;
			gbDest->iStride=gbSrc->iStride;
			gbDest->iHeight=gbSrc->iHeight;
			gbDest->iBytesTotal=gbDest->iStride*gbDest->iHeight;
			if (gbDest->arrbyData==nullptr || (gbSrc->iBytesTotal!=gbDest->iBytesTotal))
				gbDest->Init((int)gbSrc->iWidth,(int)gbSrc->iHeight,(int)gbSrc->iBytesPP);
			//int iHeight2=gbDest->iHeight;
			//int iWidth2=gbDest->iWidth;
			int iHeight1=gbSrc->iHeight;
			//int iWidth1=gbSrc->iWidth;
			int iStride=gbSrc->iStride;
			int iStride2=gbDest->iStride;
			int iSrcByte=0;
			iDestByteStart=0;
			if (xDirection<0) {
				iDestByteStart=xLength;
			}
			int iDestByte=iDestByteStart;
			bool bTest=true;
			int yNow;
			for (yNow=0; yNow<iHeight1; yNow++) {
				bTest=CopySafe(gbDest->arrbyData,
								gbSrc->arrbyData,
								iDestByte, iSrcByte, iStride);
				if (bTest==false) {
					ShowError("Error precopying blur data.","EffectMoBlurSimModWidth(...)");
					break;
				}
				iSrcByte+=iStride;
				iDestByte+=iStride2;
			}
			int iOffsetEnder=xLength;
			if (xDirection<0) {
				iOffsetEnder=-1;
			}
			//debug float precision error on super-high res?
			float fMultiplier=1.0f;
			float fPixNow=0;
			float fMaxPix=(float)(xLength-1);
			float fDecayTotal=(float)byDecayTotal;
			//bTest=true;
			for (int iOffsetNow=iDestByteStart; fPixNow<=fMaxPix; iOffsetNow+=xDirection) {
				if (bTest==false) break;
				iSrcByte=0;
				iDestByte=iOffsetNow;
				for (yNow=0; yNow<iHeight1; yNow++) {
					bTest=EffectLightenOnly(gbDest->arrbyData,
						gbSrc->arrbyData,iDestByte, iSrcByte, iStride, fMultiplier);
					if (bTest==false) {
						ShowError("Error overlaying blur data.","EffectMoBlurSimModWidth(...)");
						break;
					}
					iSrcByte+=iStride;
					iDestByte+=iStride2;
				}
				fPixNow++;
				fMultiplier=(fDecayTotal/255.0f)*(fPixNow/fMaxPix);
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"EffectMoBlurSimModWidth");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("EffectMoBlurSimModWidth");
		}
		return bGood;
	}  // EffectMoBlurSimModWidth
	bool EffectSkewModWidth(GBuffer* gbDest, GBuffer* gbSrc, int xOffsetBottom) {
		bool bGood=true;
		int iDestLine;
		double xDirection;
		double dHeight;
		double yNow;
		double dWidthDest;
		//double dWidthSrc;
		double xNow;
		double xAdd;
		double dMaxY;
		int iDestIndex;
		DPoint dpSrc;
		int iDestByte;
		int iSrcByte;
		if (xOffsetBottom<0) {
			xDirection=-1;
			xAdd=(double)(xOffsetBottom*-1);
		}
		else {
			xDirection=1;
			xAdd=(double)xOffsetBottom;
		}
		try {
			gbDest->iWidth=gbSrc->iWidth+((xOffsetBottom<0)?xOffsetBottom*-1:xOffsetBottom);
			gbDest->iBytesPP=gbSrc->iBytesPP;
			gbDest->iStride=gbSrc->iStride;
			gbDest->iHeight=gbSrc->iHeight;
			gbDest->iBytesTotal=gbDest->iStride*gbDest->iHeight;
			if (gbDest->arrbyData==nullptr || (gbSrc->iBytesTotal!=gbDest->iBytesTotal))
				gbDest->Init((int)gbSrc->iWidth,(int)gbSrc->iHeight,(int)gbSrc->iBytesPP);
			iSrcByte=0;
			iDestByte=0;//iDestByteStart;//TODO: Uncomment, and separate the blur code here and make alpha overlay version
			bool bTest=true;
			iDestLine=0;
			//dpSrc=new DPoint();
			dpSrc.Y=0;
			dHeight=(double)gbDest->iHeight;
			dWidthDest=(double)gbDest->iWidth;
			//dWidthSrc=(double)gbSrc->iWidth;
			dMaxY=dHeight-1.0;
			iDestIndex=0;
			for (yNow=0; yNow<dHeight; yNow+=1.0) {
				dpSrc.X=(yNow/dMaxY)*xAdd;
				if (xOffsetBottom<0) dpSrc.X=(xAdd-dpSrc.X);
				for (xNow=0; xNow<dWidthDest; xNow+=1.0) {
					if (dpSrc.X>-1.0) {
						if (dpSrc.X<dWidthDest)
							bTest=InterpolatePixel(gbDest, gbSrc, iDestIndex, dpSrc);
					}
					if (bTest==false) {
						bGood=false;
						break;
					}
					iDestIndex+=gbSrc->iBytesPP;
				}
				if (bGood==false) break;
				//iDestLine+=gbDest->iStride;
				dpSrc.Y+=1.0;
			}
			if (bGood==false) {
				ShowError("Error calculating skew data.","EffectSkewModWidth(...)");
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"EffectSkewModWidth");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("EffectSkewModWidth");
		}
		return bGood;
	}  // end EffectSkewModWidth
	bool EffectLightenOnly(byte* arrbyDest, byte* arrbySrc, int iDestByte, int iSrcByte, int iBytes) {
		bool bGood=true;
		try {
			for (int iByteNow=0; iByteNow<iBytes; iByteNow++) {
				if (arrbySrc[iSrcByte]>arrbyDest[iDestByte]) arrbyDest[iDestByte]=arrbySrc[iSrcByte];
				iDestByte++;
				iSrcByte++;
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"EffectLightenOnly regular");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("EffectLightenOnly regular");
		}
		return bGood;
	}  // end EffectLightenOnly
	bool EffectLightenOnly(byte* arrbyDest, byte* arrbySrc, int iDestByte, int iSrcByte, int iBytes, float fMultiplySrc) {
		bool bGood=true;
		if (fMultiplySrc>1.0f) fMultiplySrc=1.0f;
		byte bySrc;
		float fVal;
		try {
			for (int iByteNow=0; iByteNow<iBytes; iByteNow++) {
				fVal=((float)arrbySrc[iSrcByte]*fMultiplySrc);
				if (fVal>255.0) fVal=255;
				bySrc=(byte)fVal;
				if (bySrc>arrbyDest[iDestByte]) arrbyDest[iDestByte]=bySrc;
				iDestByte++;
				iSrcByte++;
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"EffectLightenOnly multiplied source");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("EffectLightenOnly multiplied source");
		}
		return bGood;
	}  // end EffectLightenOnly

	//#endregion GBuffer functions

}//end namespace
#endif
