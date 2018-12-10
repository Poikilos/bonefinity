#ifndef TARGA_H
#ifndef GBUFFER_H
//not compatible with above

#ifndef TARGASTRUCT_C
#define TARGASTRUCT_C

LPTARGA TargaNew(int nWidth1, int nHeight1, int bpp, BYTE byType1) {
//See header for byType1 values (TGATYPE_...).
	iLastErr=0;
	LPTARGA lptarga=(LPTARGA)malloc(sizeof(TARGA));
	memset(lptarga, 0, sizeof(TARGA));
	int iByteDepth = bpp/8;//>>3;
	if (lptarga==NULL) {
		iLastErr=100;//general error allocating memory
		return(NULL);
	}
	if (byType1>=4 || byType1==0) {
		iLastErr=(108);//assume can't create compressed targa or nodata (0) targa.
		return(NULL);
	}
	if (bpp%8 || bpp>32 || bpp==0) {
		iLastErr=(109);//odd bit depth
		return(NULL);
	}

	if (nWidth1 * nHeight1 * iByteDepth > 0) {
		if (!(lptarga->byarrData = (BYTE*)malloc(iByteDepth*(int)nWidth1*(int)nHeight1))) {//if (!(lptarga->byarrData = (BYTE *)malloc(640*480*iByteDepth)))
			iLastErr=107;
			return(NULL); //return error
		}
		memset(lptarga->byarrData,0,int(iByteDepth*nWidth1*nHeight1));
	}
	else {
		iLastErr=108;//can't create zero-length image
		return(NULL);
	}

	//Colormap variables fixed later if 8-bit
	lptarga->bySizeofID=32;//Set the ID length for "Created by an Axle Media product" (non-terminated BYTE array)
	lptarga->byMapType=0;
	lptarga->byTgaType=TypeUncompressedTrueColor;//Uncompressed Truecolor, fixed later if 8-bit
	lptarga->wMapOrigin = 0;
	lptarga->wMapLength = 0;
	lptarga->byMapBitDepth=0;
	lptarga->xImageOrigin = 0;
	lptarga->yImageOrigin = 0;
	lptarga->width = nWidth1;
	lptarga->height = nHeight1;
	lptarga->byBitDepth = bpp;
	lptarga->bitsDescriptor=0; //bitsDescriptor&8 if alpha, and maybe other obscure options.

	//Set the ID to "Created by an Axle Media product"(assume 32-character non-terminated string)
	//BYTE byIDTemp[32]={'C','r','e','a','t','e','d',' ','b','y',' ','a','n',' ','A','x','l','e',' ','M','e','d','i','a',' ','p','r','o','d','u','c','t'};
	lptarga->sTag="Created by an Axle Media product";

	lptarga->byarrColorMap=NULL;

	if (bpp==32) {
		lptarga->bitsDescriptor = bitAlpha8;
	}
	else if (bpp==24) {
	}
	else { //assume(bpp==8)
		lptarga->byTgaType = byType1;
		if (byType1==TypeUncompressedColorMapped) { //Uncompressed ColorMapped
			lptarga->byMapType=MapType256;
			lptarga->wMapLength=256;//assume 256 palette, others are allowed too.
			lptarga->byMapBitDepth=24;//assume palette 24-bit, others are allowed too.
			lptarga->byarrColorMap=(BYTE*)malloc(lptarga->byMapBitDepth/8 * lptarga->wMapLength);
		}
		else {//assume TypeUncompressedGrayscale
			//colormap variables are already reset to zero.
		}
	}
	return(lptarga);
} // end TargaNew

///////////////////////////////////////////////////////////
TARGA** TargaNewSeq(int nWidth1, int nHeight1, int bpp, BYTE byType1, int iFrames) {
	iLastErr=0;
	TARGA **lplptarga;
	lplptarga=(TARGA **)malloc(iFrames * sizeof(LPTARGA));
	if (lplptarga==NULL) {
		iLastErr=100;
		return(NULL);
	}
	memset(lplptarga, 0, iFrames * sizeof(LPTARGA));

	for (int i=0; i<iFrames; i++) {
		lplptarga[i]=TargaNew(nWidth1, nHeight1, bpp, byType1);
		if (lplptarga[i]==NULL) {
			for (int ix=0; ix<i; ix++) {
				free(lplptarga[ix]);
				lplptarga[ix]==NULL;
			}
			free(lplptarga);
			iLastErr=100;
			return(NULL);
		}
	}
	return (lplptarga);
}
///////////////////////////////////////////////////////////
LPTARGA TargaCopy(LPTARGA lptarga1) {
	iLastErr=0;
	LPTARGA lptargaNew=NULL;
	lptargaNew=(LPTARGA)malloc(sizeof(TARGA));
	memset(lptargaNew,0,sizeof(TARGA));
	if (lptargaNew==NULL) {
		iLastErr=100;//couldn't allocate
		return(NULL);
	}
	if (lptarga1->byBitDepth%8 || lptarga1->byBitDepth>32 || lptarga1->byBitDepth==0) {
		iLastErr=109;//odd bit depth
		free(lptargaNew);
		lptargaNew=NULL;
		return(NULL);
	}
	if (lptarga1->width * lptarga1->width * lptarga1->byBitDepth <= 0) {
		iLastErr=(107);//couldn't allocate targa byarrData
		free(lptargaNew);
		lptargaNew=NULL;
		return(NULL);
	}
	int iByteDepth = lptarga1->byBitDepth/8;
	int iErrNow=0;
	int iErr=0;
	lptargaNew->bySizeofID	=lptarga1->bySizeofID;
	lptargaNew->byMapType	=lptarga1->byMapType;
	lptargaNew->byTgaType	=lptarga1->byTgaType;
	lptargaNew->wMapOrigin	=lptarga1->wMapOrigin;
	lptargaNew->wMapLength	=lptarga1->wMapLength;
	lptargaNew->byMapBitDepth=lptarga1->byMapBitDepth;
	lptargaNew->xImageOrigin	=lptarga1->xImageOrigin;
	lptargaNew->yImageOrigin	=lptarga1->yImageOrigin;
	lptargaNew->width		=lptarga1->width;
	lptargaNew->height		=lptarga1->height;
	lptargaNew->byBitDepth	=lptarga1->byBitDepth;
	lptargaNew->bitsDescriptor	=lptarga1->bitsDescriptor;

	lptargaNew->byarrData=NULL;
	lptargaNew->byarrData = (BYTE *)malloc(lptarga1->width * lptarga1->height * iByteDepth);
	if (lptargaNew->byarrData)
		memcpy(lptargaNew->byarrData, lptarga1->byarrData, lptarga1->width * lptarga1->height * iByteDepth);
	else
		iErrNow=110;//general targa memory error
	if (iErrNow) iErr=iErrNow;

	if (lptarga1->footer.dwSizeofDump>0 && lptarga1->footer.dump) {
		lptargaNew->footer.dwSizeofDump = lptarga1->footer.dwSizeofDump;
		lptargaNew->footer.dump = (BYTE *)malloc(lptarga1->footer.dwSizeofDump);
		if (lptargaNew->footer.dump)
			memcpy(lptargaNew->footer.dump, lptarga1->footer.dump, lptarga1->footer.dwSizeofDump);
		else
			iErrNow=110;
		if (iErrNow) iErr=iErrNow;
	}
	else {
		lptargaNew->footer.dump=NULL;
		lptargaNew->footer.dwSizeofDump=0;
	}
	lptargaNew=>sTag=lptarga1.sTag;

	if (lptarga1->wMapLength*lptarga1->byMapBitDepth && lptarga1->byarrColorMap && lptarga1->byMapBitDepth%8==0) {
		int iSizeofBuff=int(lptarga1->wMapLength)*int(lptarga1->byMapBitDepth/8);
		lptargaNew->byarrColorMap=NULL;
		lptargaNew->byarrColorMap=(BYTE*)malloc(iSizeofBuff);
		if (lptargaNew->byarrColorMap)
			memcpy(lptargaNew->byarrColorMap, lptarga1->byarrColorMap, iSizeofBuff);
		else
			iErrNow=110;
		if (iErrNow) iErr=iErrNow;
		//length variables already copied.
	}
	else {
		lptargaNew->wMapLength=0;
		lptargaNew->byMapBitDepth=0;
		lptargaNew->byarrColorMap=NULL;
	}
	iLastErr=iErr;
	return(lptargaNew);
} //end TargaCopy

///////////////////////////////////////////////////////////

void TargaFill(LPTARGA lptarga, BYTE *byPix, unsigned int uiPixBytes) {
	iLastErr=0;
	if (lptarga==NULL) {
		return;
	}
	//if (lptarga->byarrData==NULL)
	//{
	//	return;
	//}
	//if (byBuff==NULL)
	//{
	//	return;
	//}
	Uint32 dwByteDepth, dwPixels, dwPlace;
	BYTE *lpbyte;
	lpbyte=lptarga->byarrData;
	dwByteDepth=lptarga->byBitDepth/8;
	dwPixels=(Uint32)(lptarga->width)*(Uint32)(lptarga->height)*dwByteDepth;
	for (dwPlace=0; dwPlace<dwPixels; dwPlace++) {
		memcpy(lpbyte, byPix, uiPixBytes);
		lpbyte+=dwByteDepth;
	}
	return;
}

///////////////////////////////////////////////////////////

void TargaFlip(BYTE *image, int bytes_per_line, int height) {
	iLastErr=0;
	BYTE *byarrData; // used to perform the image processing
	if (!(byarrData = (BYTE *)malloc(bytes_per_line*height))) {
		iLastErr=1;
		return;
	}
	memcpy(byarrData,image,bytes_per_line*height);
	for (int index=0; index < height; index++)
	memcpy(&image[((height-1) - index)*bytes_per_line],
			 &byarrData[index*bytes_per_line], bytes_per_line);
	free(byarrData);
	return;
}//end TargaFlip

///////////////////////////////////////////////////////////
LPTARGA TargaLoad(char* sFile, bool bOriginAtBottomLeft) {
	iLastErr=0;
	//Before using this function, open the file and make sure that
	//it ends with the null terminated BYTE string "TRUEVISION-XFILE"
	LPTARGA lptarga=NULL;
	bool bGood=true;
	try {
		lptarga=(LPTARGA)malloc(sizeof(TARGA));
		if (lptarga==NULL) {
			iLastErr=100;//cannot load null targa pointer
			return(NULL);
  		}
		lptarga->byarrColorMap=NULL;
		lptarga->sTag="";
		HANDLE hFile=NULL;
		Byter byterNow;
		cout<<"TargaLoad opening \""<<sFile<<"\"..."<<flush;
		bGood=byterNow.OpenRead(sFile);
		cout<<"done loading "<<sFile<<" (OpenRead "<<((bGood)?"success":"failure")<<")..."<<flush;
		//hFile = CreateFile(sFile,//CreateFile will ask for CD-ROM or floppy disk if needed.
		//	GENERIC_READ,	//needed by ReadFile // GENERIC_READ | GENERIC_WRITE, //use bitwise for both
		//	FILE_SHARE_READ,//FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE //bitwise for multiple.	0 for total lockout until unloaded.	Delete lets other programs delete it.
		//	NULL,	//no pointer to SECURITY_ATTRIBUTES struct (which NT parses)
		//	OPEN_EXISTING,	//create/overwrite file //other kinds: OPEN_ALWAYS(i.e. create/append), CREATE_NEW, OPEN_EXISTING, TRUNCATE_EXISTING(erases data but fails if doesn't exist already)
		//	FILE_ATTRIBUTE_NORMAL //not normal: -_TEMP for cached, or -_HIDDEN, -_COMPRESSED, _READONLY etc.
		//	|FILE_FLAG_SEQUENTIAL_SCAN,//optimize caching for sequential read. //others: FILE_FLAG_WRITE_THROUGH forces immediate flush to disk, FILE_FLAG_DELETE_ON_CLOSE, etc.
		//	NULL);	//(NT only - other OS won't open with non-NULL parameter) handle to template file, for copying attributes from another file
		//if (hFile==NULL) {
		//	bGood=false;
		//}
		if (!bGood) {
			cerr<<"TargaLoad: Error opening file."<<endl;
			//CloseHandle(hFile);
			iLastErr=313;
			return(NULL);//file not found
		}
		bool bTest=false;
		Uint32 dwNumBytesRead=0;
		Uint32 dwTotalHeaderRead=0;

		lptarga->bySizeofID=0;
		int iPlacePrev=byterNow.Place();
		BYTE byNow=0;
		USHORT wNow=0;
		bTest=byterNow.Read(hFile,(void*)&byNow,1,dwNumBytesRead, NULL);//ReadFile(hFile,&lptarga->bySizeofID,1,dwNumBytesRead, NULL);
		lptarga->bySizeofID=byNow;
		if (dwNumBytesRead != 1) {// || lptarga->bySizeofID<0)
			//CloseHandle(hFile);
			iLastErr=320;
			cerr<<"TargaLoad: read "<<dwNumBytesRead<<" expected 1 at "<<iPlacePrev<<" of "<<(byterNow.Length()-1)<<endl;
			return(NULL);//"couldn't load source targa file"
		}
		dwTotalHeaderRead+=dwNumBytesRead; iPlacePrev=byterNow.Place();
		bTest=byterNow.Read(hFile,&byNow,1,dwNumBytesRead, NULL);
		lptarga->byMapType=byNow;
		if (!bTest) {iLastErr=320;cerr<<"TargaLoad: read "<<dwNumBytesRead<<" expected 1 at "<<iPlacePrev<<" of "<<(byterNow.Length()-1)<<endl;}
		dwTotalHeaderRead+=dwNumBytesRead; iPlacePrev=byterNow.Place();
		bTest=byterNow.Read(hFile,&byNow,1,dwNumBytesRead, NULL);
		lptarga->byTgaType=byNow;
		if (!bTest) {iLastErr=320;cerr<<"TargaLoad: read "<<dwNumBytesRead<<" expected 1 at "<<iPlacePrev<<" of "<<(byterNow.Length()-1)<<endl;}
		dwTotalHeaderRead+=dwNumBytesRead; iPlacePrev=byterNow.Place();
		bTest=byterNow.Read(hFile,&wNow,2,dwNumBytesRead, NULL);
        lptarga->wMapOrigin=wNow;
		if (!bTest) {iLastErr=320;cerr<<"TargaLoad: read "<<dwNumBytesRead<<" expected 2 at "<<iPlacePrev<<" of "<<(byterNow.Length()-1)<<endl;}
		dwTotalHeaderRead+=dwNumBytesRead; iPlacePrev=byterNow.Place();
		bTest=byterNow.Read(hFile,&wNow,2,dwNumBytesRead, NULL);
		lptarga->wMapLength=wNow;
		if (!bTest) {iLastErr=320;cerr<<"TargaLoad: read "<<dwNumBytesRead<<" expected 2 at "<<iPlacePrev<<" of "<<(byterNow.Length()-1)<<endl;}
		dwTotalHeaderRead+=dwNumBytesRead; iPlacePrev=byterNow.Place();
		bTest=byterNow.Read(hFile,&byNow,1,dwNumBytesRead, NULL);
		lptarga->byMapBitDepth=byNow;
		if (!bTest) {iLastErr=320;cerr<<"TargaLoad: read "<<dwNumBytesRead<<" expected 1 at "<<iPlacePrev<<" of "<<(byterNow.Length()-1)<<endl;}
		dwTotalHeaderRead+=dwNumBytesRead; iPlacePrev=byterNow.Place();
		bTest=byterNow.Read(hFile,&wNow,2,dwNumBytesRead, NULL);
		lptarga->xImageOrigin=wNow;
		if (!bTest) {iLastErr=320;cerr<<"TargaLoad: read "<<dwNumBytesRead<<" expected 2 at "<<iPlacePrev<<" of "<<(byterNow.Length()-1)<<endl;}
		dwTotalHeaderRead+=dwNumBytesRead; iPlacePrev=byterNow.Place();
		bTest=byterNow.Read(hFile,&wNow,2,dwNumBytesRead, NULL);
		lptarga->yImageOrigin=wNow;
		if (!bTest) {iLastErr=320;cerr<<"TargaLoad: read "<<dwNumBytesRead<<" expected 2 at "<<iPlacePrev<<" of "<<(byterNow.Length()-1)<<endl;}
		dwTotalHeaderRead+=dwNumBytesRead; iPlacePrev=byterNow.Place();
		bTest=byterNow.Read(hFile,&wNow,2,dwNumBytesRead, NULL);
		lptarga->width=wNow;
		if (!bTest) {iLastErr=320;cerr<<"TargaLoad: read "<<dwNumBytesRead<<" expected 2 at "<<iPlacePrev<<" of "<<(byterNow.Length()-1)<<endl;}
		dwTotalHeaderRead+=dwNumBytesRead; iPlacePrev=byterNow.Place();
		bTest=byterNow.Read(hFile,&wNow,2,dwNumBytesRead, NULL);
		lptarga->height=wNow;
		if (!bTest) {iLastErr=320;cerr<<"TargaLoad: read "<<dwNumBytesRead<<" expected 2 at "<<iPlacePrev<<" of "<<(byterNow.Length()-1)<<endl;}
		dwTotalHeaderRead+=dwNumBytesRead; iPlacePrev=byterNow.Place();
		bTest=byterNow.Read(hFile,&byNow,1,dwNumBytesRead, NULL);
		lptarga->byBitDepth=byNow;
		if (!bTest) {iLastErr=320;cerr<<"TargaLoad: read "<<dwNumBytesRead<<" expected 1 at "<<iPlacePrev<<" of "<<(byterNow.Length()-1)<<endl;}
		dwTotalHeaderRead+=dwNumBytesRead; iPlacePrev=byterNow.Place();
		bTest=byterNow.Read(hFile,&byNow,1,dwNumBytesRead, NULL);
		lptarga->bitsDescriptor=byNow;
		if (!bTest) {iLastErr=320;cerr<<"TargaLoad: read "<<dwNumBytesRead<<" expected 1 at "<<iPlacePrev<<" of "<<(byterNow.Length()-1)<<endl;}
		dwTotalHeaderRead+=dwNumBytesRead;
		if (lptarga->bySizeofID>0) {
			lptarga->sTag="";
            iPlacePrev=byterNow.Place();
			bTest=byterNow.ReadAscii(lptarga->sTag,byNow,dwNumBytesRead);
			lptarga->bySizeofID=byNow;
			if (!bTest) {iLastErr=320;cerr<<"TargaLoad: read "<<dwNumBytesRead<<" expected "<<lptarga->bySizeofID<<" at "<<iPlacePrev<<" of "<<(byterNow.Length()-1)<<endl;}
			dwTotalHeaderRead+=dwNumBytesRead;
		}
		int iSizeofMap=int(lptarga->wMapLength)*int(lptarga->byMapBitDepth/8);
		if (iSizeofMap && lptarga->byMapBitDepth%8==0) {
			lptarga->byarrColorMap=NULL;
			lptarga->byarrColorMap=(BYTE*)malloc(iSizeofMap);
			if (lptarga->byarrColorMap) {
                iPlacePrev=byterNow.Place();
				bTest=byterNow.Read(hFile,lptarga->byarrColorMap,iSizeofMap,dwNumBytesRead, NULL);
				if (!bTest) {iLastErr=320;cerr<<"TargaLoad: read "<<dwNumBytesRead<<" expected "<<lptarga->byarrColorMap<<" (colormap) at "<<iPlacePrev<<" of "<<(byterNow.Length()-1)<<endl;}
				dwTotalHeaderRead+=dwNumBytesRead;
			}
			else {
				//CloseHandle(hFile);
				iLastErr=110;
				return(NULL);
			}
		}
		else {	//if zero or less, reset:
			iSizeofMap=0;
			lptarga->wMapLength=0;
			lptarga->byMapBitDepth=0;
		}

		if (dwTotalHeaderRead != (Uint32)(18 + lptarga->bySizeofID + iSizeofMap)) {
			//CloseHandle(hFile);
			iLastErr=313;
			return(NULL);//"couldn't load source lptarga file"
		}
		int iByteDepth=lptarga->byBitDepth/8;
		if (!(lptarga->byarrData=(BYTE *)malloc((int)lptarga->width*(int)lptarga->height*iByteDepth))) {
			//CloseHandle(hFile);
			iLastErr=315;
			return(NULL);
		}
		//Read in the byarrData
		iPlacePrev=byterNow.Place();
		int iImageDataSize=((int)lptarga->width*(int)lptarga->height*iByteDepth);
		bTest=byterNow.Read(hFile,lptarga->byarrData,iImageDataSize,dwNumBytesRead, NULL);
		if (!bTest) {iLastErr=320;cerr<<"TargaLoad: read "<<dwNumBytesRead<<" expected "<<iImageDataSize<<" (image data) at "<<iPlacePrev<<" of "<<(byterNow.Length()-1)<<endl;}
		if (dwNumBytesRead != ((Uint32)lptarga->width*(Uint32)lptarga->height*(Uint32)iByteDepth)) {
			//CloseHandle(hFile);
			iLastErr=313;
			return(NULL);//"couldn't load source targa file"
		}

		//FOOTER:
		BYTE tempFooter[521]; //521 is the theoretical maximum footer size (this could be trouble if that is not correct)
		lptarga->footer.dwSizeofDump=0;
		lptarga->footer.dump=NULL;
		Uint32 dwToRead=byterNow.Length()-byterNow.Place();
		if (dwToRead>0) {
            iPlacePrev=byterNow.Place();
			bTest=byterNow.Read(hFile,&tempFooter[lptarga->footer.dwSizeofDump],dwToRead,dwNumBytesRead, NULL);//byte by byte
			if (!bTest) {iLastErr=320;cerr<<"TargaLoad: read "<<dwNumBytesRead<<" expected "<<dwToRead<<" (footer dump) at "<<iPlacePrev<<" of "<<(byterNow.Length()-1)<<endl;}
			lptarga->footer.dwSizeofDump+=(int)dwNumBytesRead;
		}
		if (lptarga->footer.dwSizeofDump>0) {
			lptarga->footer.dump=(BYTE *)malloc(lptarga->footer.dwSizeofDump);
			memcpy(lptarga->footer.dump, tempFooter, lptarga->footer.dwSizeofDump);
		}
		else {
			lptarga->footer.dump=NULL;
		}

		//bTest=CloseHandle(hFile);
		//if (!bTest) cerr<< "TargaLoad:	Error closing file."<<endl;
		if (bOriginAtBottomLeft)
			TargaFlip(lptarga->byarrData, lptarga->width*iByteDepth, lptarga->height);
	}
	catch (...) {
		iLastErr=1000;//Exception
		ShowError(1000, "Exception", "TargaLoad");
		lptarga=NULL;
	}
	return(lptarga);
}//end TargaLoad
LPTARGA TargaLoad(char* sFile) {
	return TargaLoad(sFile, true);
}
//int iTargaMaxFrames=10000;
///////////////////////////////////////////////////////////
TARGA** TargaLoadSeq(char* sFileFirstFrame, int &iReturnFrames) {//pass *0008.tga to start at frame 9, or pass * to add 0000.tga etc. automatically.
	iLastErr=0;
	bool bGood=true;
	TARGA **lplptargaTemp;
	TARGA **lplptargaReturn;
	lplptargaTemp=NULL;
	lplptargaReturn=NULL;
	int iMaxFramesNow=(iReturnFrames>0)?iReturnFrames:10000;
	iReturnFrames=0;
	try {
		lplptargaTemp=(TARGA**)malloc(iMaxFramesNow * sizeof(LPTARGA));
		if (lplptargaTemp==NULL) {
			iLastErr=100;
			bGood=false;
			return(NULL);
		}
		char sExtCaseAware[5]={'.','t','g','a','\0'};//overwritten below only if extension was sent
		memset(lplptargaTemp, 0, iMaxFramesNow * sizeof(LPTARGA));
		char sBaseName[512];
		strcpy(sBaseName, sFileFirstFrame);
		int iLen=strlen(sBaseName);
		int iStartFrame=0;
		if (IsLikeMask(sBaseName, "*.tga")) {
			strcpy(sExtCaseAware, &sBaseName[iLen-4]);
			trunc(sBaseName, iLen-4);//remove extension
			static char sNumber[128];
			strcpy(sNumber, &sBaseName[iLen-8]);//-8 to get numbers (8 since iLen is original length).
			trunc(sBaseName, iLen-8);//now remove the numbers
			iStartFrame=IntOfString(sNumber);
			if (iStartFrame==-1) {
				if (ShowErr()) cerr << "TargaSeq2Array: couldn't interpret starting frame from given name"<<endl;
				iStartFrame=0;
			}
			iLen-=8;
		}
		cout<<"\n   --Starting to load frames at #"<<iStartFrame<<"..."<<flush;
		char sNameNow[512];
		//static char sDigit[10][2]={"0","1","2","3","4","5","6","7","8","9"};
		int iErrNow=0;
		int iErr=0;
		//string sNameAnsiString="";
		for (int iFrameNow=iStartFrame;iReturnFrames<iMaxFramesNow && bGood; iFrameNow++) {
			int i=iFrameNow;
			strcpy(sNameNow, sBaseName);
			strcat(sNameNow, sDigit[i/1000]);
			if (i>=1000) i-=int(i/1000)*1000;
			strcat(sNameNow, sDigit[iFrameNow/100]);
			if (i>=100) i-=int(i/100)*100;
			strcat(sNameNow, sDigit[iFrameNow/10]);
			strcat(sNameNow, sDigit[iFrameNow%10]);
			strcat(sNameNow, sExtCaseAware);
			//sNameAnsiString.assign(sNameNow);
			//TODO: if (FileSize(sNameAnsiString)<=0) break;
			lplptargaTemp[iReturnFrames]=TargaLoad(sNameNow);
			if (lplptargaTemp[iReturnFrames]!=NULL) iReturnFrames++;
			else bGood=false;
			/*
			iErrNow=iLastErr;
			if (iErrNow) {
				if (iCountFiles>0) {
					iErr=0;
					iFrameNow=iMaxFramesNow;
					iReturnFrames=iCountFiles;
					break;
				}
				else {
					//NO ERROR!  This is just auto-detect //cerr << "TargaLoadSeq: could not load targa named \"" << sNameNow << "\" make sure the file is present or try saving the TGA file without compression." << endl;
					iErr=iErrNow;
					iFrameNow=iMaxFramesNow;
					iReturnFrames=0;
					break;
				}
			}
			*/
			//iCountFiles++;
		}//end for iFrameNow
		if (!bGood) iLastErr=iErr;
		if (lplptargaTemp) {
			if (iReturnFrames>0) {
				lplptargaReturn=(TARGA**)malloc(iReturnFrames * sizeof(LPTARGA));
				if (lplptargaReturn) {
					for (int iFrameX=0; iFrameX<iReturnFrames; iFrameX++) {
						lplptargaReturn[iFrameX]=lplptargaTemp[iFrameX];
					}
				}
				else {
					iLastErr=1000;//Exception
					ShowError(1000, "Couldn't create memory for lplptargaReturn", "TargaLoadSeq");
					lplptargaTemp=NULL;
				}
			}
			free(lplptargaTemp);
			lplptargaTemp=NULL;
		}
	}//end try
	catch (...) {
		iLastErr=1000;//Exception
		ShowError(1000, "Exception", "TargaLoadSeq");
		lplptargaTemp=NULL;
	}
	return(lplptargaReturn);
}

///////////////////////////////////////////////////////////////
/* //commented for debug only:
void TargaSave(LPTARGA targa, char* sFile) {
	iLastErr=0;
	int iByteDepth=targa->byBitDepth/8;
	if (iByteDepth<=0 || targa->byBitDepth%8) {
		iLastErr=109;
		return;//odd bit depth error
	}
	char newName[256];
	strcpy(newName, sFile);
	int iErrNow=0;
	int iErr=0; //start with a good return

	TargaFlip(targa->byarrData, targa->width*iByteDepth, targa->height);//Flip the targa upside-down for save
	BOOL bTest=FALSE;
	//Open the new or used file to overwrite
	HANDLE hFile=CreateFile(newName,GENERIC_WRITE,FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//Save the file header //old://bTest=WriteFile(hFile,&targa->targaFileHeader,sizeof(targaFileHeader),&dwNumBytesWritten, NULL);
	Uint32 dwNumBytesWritten=0;
	Uint32 dwTotalHeaderWritten=0;
	bTest=WriteFile(hFile,&targa->bySizeofID,1,&dwNumBytesWritten, NULL);
	if (dwNumBytesWritten != 1) {
		//CloseHandle(hFile);
		iLastErr=307;
		return;//error saving, corrupt or disk error
	}
	dwTotalHeaderWritten+=dwNumBytesWritten;
	bTest=WriteFile(hFile,&targa->byMapType,1,&dwNumBytesWritten, NULL);
	dwTotalHeaderWritten+=dwNumBytesWritten;
	bTest=WriteFile(hFile,&targa->byTgaType,1,&dwNumBytesWritten, NULL);
	dwTotalHeaderWritten+=dwNumBytesWritten;
	bTest=WriteFile(hFile,&targa->wMapOrigin,2,&dwNumBytesWritten, NULL);
	dwTotalHeaderWritten+=dwNumBytesWritten;
	bTest=WriteFile(hFile,&targa->wMapLength,2,&dwNumBytesWritten, NULL);
	dwTotalHeaderWritten+=dwNumBytesWritten;
	bTest=WriteFile(hFile,&targa->byMapBitDepth,1,&dwNumBytesWritten, NULL);
	dwTotalHeaderWritten+=dwNumBytesWritten;
	bTest=WriteFile(hFile,&targa->xImageOrigin,2,&dwNumBytesWritten, NULL);
	dwTotalHeaderWritten+=dwNumBytesWritten;
	bTest=WriteFile(hFile,&targa->yImageOrigin,2,&dwNumBytesWritten, NULL);
	dwTotalHeaderWritten+=dwNumBytesWritten;
	bTest=WriteFile(hFile,&targa->width,2,&dwNumBytesWritten, NULL);
	dwTotalHeaderWritten+=dwNumBytesWritten;
	bTest=WriteFile(hFile,&targa->height,2,&dwNumBytesWritten, NULL);
	dwTotalHeaderWritten+=dwNumBytesWritten;
	bTest=WriteFile(hFile,&targa->byBitDepth,1,&dwNumBytesWritten, NULL);
	dwTotalHeaderWritten+=dwNumBytesWritten;
	bTest=WriteFile(hFile,&targa->bitsDescriptor,1,&dwNumBytesWritten, NULL);
	dwTotalHeaderWritten+=dwNumBytesWritten;
	if (targa->sTag.length()>0) {
		bTest=byterNow.WriteAscii(targa->sTag,&dwNumBytesWritten);
		dwTotalHeaderWritten+=dwNumBytesWritten;
	}
	int iSizeofMap=int(targa->wMapLength)*int(targa->byMapBitDepth/8);
	if (iSizeofMap && targa->byMapBitDepth%8==0) {
		if (targa->byarrColorMap) {
			bTest=WriteFile(hFile,targa->byarrColorMap,iSizeofMap,&dwNumBytesWritten, NULL);
			dwTotalHeaderWritten+=dwNumBytesWritten;
		}
		else {
			//CloseHandle(hFile);
			iLastErr=307;
			return;//colormap size is corrupt
		}

	}
	else {// if zero or less, reset:
		iSizeofMap=0;
		targa->wMapLength=0;
		targa->byMapBitDepth=0;
	}
	if (dwTotalHeaderWritten != (Uint32)(18 + (int)targa->bySizeofID + iSizeofMap)) {
		//CloseHandle(hFile);
		iLastErr=307;
		return;//"couldn't load source targa file"
	}
	//end header data

	int iSizeofBuff=(int)targa->width * (int)targa->height * iByteDepth;
	bTest=WriteFile(hFile, targa->byarrData, iSizeofBuff, &dwNumBytesWritten, NULL);	//Save the image data
	if (dwNumBytesWritten!=(Uint32)(iSizeofBuff))
		iErrNow=307;
	if (iErrNow) iErr=iErrNow;
	if (targa->footer.dwSizeofDump && targa->footer.dump) bTest=WriteFile(hFile,targa->footer.dump, targa->footer.dwSizeofDump, &dwNumBytesWritten, NULL);//Save the file footer info if any
		//bTest= CloseHandle(hFile);//Close the file
	TargaFlip(targa->byarrData, targa->width*iByteDepth, targa->height);//Flip the targa back to normal
	iLastErr=iErr;
} // end TargaSave
*/
///////////////////////////////////////////////////////////
//void TargaSaveSeq(TARGA** lplptarga, char* sFile, int *lpiReturnFrames)
//{
//	TargaSave(
//}

///////////////////////////////////////////////////////////
void TargaUnload(LPTARGA &lptarga) {
	 int iErr=0;
	iLastErr=0;
	try {
		if (lptarga) {
			if (lptarga->byarrData) {
				free(lptarga->byarrData);
			}
			if (lptarga->footer.dump) {
				free(lptarga->footer.dump);
			}
			if (lptarga->byarrColorMap) {
				free(lptarga->byarrColorMap);
			}
			memset(lptarga,0,sizeof(TARGA)); //also sets above pointers to null
			free(lptarga);
			lptarga=NULL;
		}
		else iErr=9;
	}//end try
	catch (...) {
		iErr=1000;
		iLastErr=1000;//Exception
		ShowError(1000, "Exception", "TargaUnload");
		lptarga=NULL;
	}

	iLastErr=iErr;
	return;
}
void TargaUnloadSeq(TARGA ** &lplptarga, int iFrames) {
	iLastErr=0;
	try {
		if (lplptarga==NULL) {
			iLastErr=51;
			return;
		}
		int iErr=0;
		for (int i=0; i<iFrames; i++) {
			if (lplptarga[i]) {
				TargaUnload(lplptarga[i]);
				lplptarga[i]==NULL;
			}
			if (iLastErr) iErr=iLastErr;
		}
		free(lplptarga);
		lplptarga=NULL;
	}
	catch (...) {
		iLastErr=1000;//Exception
		ShowError(1000, "Exception", "TargaUnloadSeq");
		lplptarga=NULL;
	}
	return;
}



///////////////////////////////////////// FX ///////////////////////////////////




void DrawAlphaPix(LPTARGA lptargaDest, int xPix, int yPix, byte r, byte g, byte b, byte a) {
	try {
		int iByteDepth=lptargaDest->byBitDepth/8;
		int iStride=lptargaDest->width*iByteDepth;
		int iChannel=yPix*iStride+xPix*iByteDepth;

		//The ++ operators are right:
		if ((iChannel+2>=0) && (iChannel+2<iStride*lptargaDest->height))
		if (((iChannel+3)/4)<((int)lptargaDest->width*(int)(lptargaDest->byBitDepth/8)*(int)lptargaDest->height)) {
			lptargaDest->byarrData[iChannel]=by3dAlphaLookup[b][lptargaDest->byarrData[iChannel]][a];
			lptargaDest->byarrData[++iChannel]=by3dAlphaLookup[g][lptargaDest->byarrData[iChannel]][a];
			lptargaDest->byarrData[++iChannel]=by3dAlphaLookup[r][lptargaDest->byarrData[iChannel]][a];
		}
	}
	catch (...) {
		FakeException("Exception drawing subpixel by dwColor.");
	}
}//DrawAlphaPix
void DrawSubpixelDot(LPTARGA lptargaDest, float xDot, float yDot, Pixel &pixelColor) {
	try {
		//TODO: finish this (finish subpixel accuracy)
		// Begin header fields in order of writing //
		//Targa struct reference:
		//bySizeofID byMapType byTgaType wMapOrigin wMapLength byMapBitDepth
		//xImageOrigin yImageOrigin width height byBitDepth bitsDescriptor sTag
		//*byarrColorMap *byarrData footer;
		bool bGood=true;
		//LPIPOINT *lpipointarrNow=malloc(4*sizeof(LPIIPOINT));

		int xMin=IFLOOR(xDot);
		int xMax=ICEILING(xDot);
		int yMin=IFLOOR(yDot);
		int yMax=ICEILING(yDot);
		float xfMin=(float)xMin;
		float xfMax=(float)xMax;
		float yfMin=(float)yMin;
		float yfMax=(float)yMax;
		int iByteDepth=lptargaDest->byBitDepth/8;
		int iStride=lptargaDest->width*iByteDepth;
		//int iStart=yMin*iStride+xMin*iByteDepth;
		float xEccentric,yEccentric,xNormal,yNormal;
		xNormal=1.0f-fabs(xDot-xfMin);
		xEccentric=1.0f-fabs(xDot-xfMax);
		yNormal=1.0f-fabs(yDot-yfMin);
		yEccentric=1.0f-fabs(yDot-yfMax);
		DrawAlphaPix(lptargaDest,xMin,yMin,pixelColor.r,pixelColor.g,pixelColor.b,SafeByte(pixelColor.a*xNormal*yNormal));
		DrawAlphaPix(lptargaDest,xMax,yMin,pixelColor.r,pixelColor.g,pixelColor.b,SafeByte(pixelColor.a*xEccentric*yNormal));
		DrawAlphaPix(lptargaDest,xMin,yMax,pixelColor.r,pixelColor.g,pixelColor.b,SafeByte(pixelColor.a*xNormal*yEccentric));
		DrawAlphaPix(lptargaDest,xMax,yMax,pixelColor.r,pixelColor.g,pixelColor.b,SafeByte(pixelColor.a*xEccentric*yEccentric));
	}
	catch (string sExn) {
		if (ShowError()) cerr<<"Exception drawing subpixel by color:"<<sExn<<endl;
	}
}//end DrawSubpixelDot(LPTARGA lptargaDest, float xDot, float yDot, byte r, byte g, byte b, byte a) {
void DrawSubpixelDot(LPTARGA lptargaDest, float xDot, float yDot, byte* lpbySrcPixel) {
	try {
		bool bMake=false;
		if (lpbySrcPixel==NULL) {
			lpbySrcPixel=(byte*)malloc(4);
			lpbySrcPixel[0]=0;
			lpbySrcPixel[1]=0;
			lpbySrcPixel[2]=0;
			lpbySrcPixel[3]=0;
			bMake=true;
		}
		static Pixel pixelNow;
		pixelNow.Set(lpbySrcPixel[2],lpbySrcPixel[1],lpbySrcPixel[0],lpbySrcPixel[3]);
		DrawSubpixelDot(lptargaDest,xDot,yDot,pixelNow);
		if (bMake) {
			free(lpbySrcPixel);
			lpbySrcPixel=NULL;
		}
	}
	catch (char* sExn) {
		ShowAndDeleteException(sExn,"DrawSubpixelDot from source pixel");
	}
	catch (...) {
		ShowUnknownException("DrawSubpixelDot from source pixel");
	}
}//end DrawSubpixelDot(LPTARGA lptargaDest, float xDot, float yDot, byte* lpbySrcPixel)
void DrawSubpixelLine(LPTARGA lptargaDest, float xStart, float yStart, float xEnd, float yEnd,
		Pixel &pixelStart, Pixel* pixelEndOrNull, float fPrecisionIncrement) {
	int iLoops=0;
	static int iMaxLoops=1000000; //debug hard-coded limitation
	static float xNow, yNow, xRelMax, yRelMax, rRelMax, theta, rRel;
	xNow=xStart;
	yNow=yStart;
	xRelMax=xEnd-xStart;
	yRelMax=yEnd-yStart;
	rRelMax=ROFXY(xRelMax,yRelMax);
	theta=THETAOFXY(xRelMax,yRelMax);
	rRel=0;
	rRel-=fPrecisionIncrement; //the "0th" value
	static Pixel pixelColor;
	pixelColor.r=pixelStart.r;
	pixelColor.g=pixelStart.g;
	pixelColor.b=pixelStart.b;
	pixelColor.a=pixelStart.a;
	while (rRel<rRelMax && iLoops<iMaxLoops) {
		rRel+=fPrecisionIncrement;
		if (pixelEndOrNull!=NULL) {
			pixelColor.r=SafeByte(APPROACH(pixelStart.r,pixelEndOrNull->r,rRel/rRelMax));
			pixelColor.g=SafeByte(APPROACH(pixelStart.g,pixelEndOrNull->g,rRel/rRelMax));
			pixelColor.b=SafeByte(APPROACH(pixelStart.b,pixelEndOrNull->b,rRel/rRelMax));
			pixelColor.a=SafeByte(APPROACH(pixelStart.a,pixelEndOrNull->a,rRel/rRelMax));
		}
		xNow=(XOFRTHETA(rRel,theta))+xStart;
		yNow=(YOFRTHETA(rRel,theta))+yStart;
		if (xNow>0&&yNow>0&&xNow<lptargaDest->width&&yNow<lptargaDest->height)
			DrawSubpixelDot(lptargaDest, xNow, yNow, pixelColor);
		iLoops++;
		if (iLoops>=iMaxLoops) break;
	}//end while drawing line
	if (iLoops>=iMaxLoops && iErrors<iMaxErrors) {
		cerr<<"In DrawSubpixelLine: loop overflow!"<<endl;
		iErrors++;
	}
}//end DrawSubpixelLine

void DrawSubpixelLine(LPTARGA lptargaDest, FPOINT &pointStart, FPOINT &pointEnd,
		Pixel &pixelStart, Pixel* pixelEndOrNull, float fPrecisionIncrement) {
	DrawSubpixelLine(lptargaDest, pointStart.x, pointStart.y, pointEnd.x, pointEnd.y,
		pixelStart, pixelEndOrNull, fPrecisionIncrement);
}//DrawSubpixelLine
void DrawSubPixArc(LPTARGA lptargaDest, float xCenter, float yCenter,
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
		xNow=(XOFRTHETA(fRadius,fNow));
		yNow=-(YOFRTHETA(fRadius,fNow));//negative to flip to non-cartesian monitor loc
		xNow*=fWidthMultiplier;
		Rotate(xNow,yNow,fRotate);
		xNow+=xCenter;
		yNow+=yCenter;
        if (xNow>0&&yNow>0&&xNow<lptargaDest->width&&yNow<lptargaDest->height)
			DrawSubpixelDot(lptargaDest, xNow, yNow, pixelColor);
		iLoops++;
		if (iLoops>=iMaxLoops) break;
		fRadius+=fPushSpiralPixPerRotation/360.0f;
	}
	if (iLoops>=iMaxLoops && iErrors<iMaxErrors) {
		cerr<<"In DrawSubpixelArc: loop overflow!"<<endl;
		iErrors++;
	}
}//DrawSubPixArc

#endif
#endif
#endif
