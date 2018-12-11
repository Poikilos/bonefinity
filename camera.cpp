#ifndef CAMERA_CPP
#define CAMERA_CPP

#include <camera.h>

using namespace std;

namespace ExpertMultimediaBase {
	//double Camera::HITDETECTION2D_CONSTANT_Z_DBL=1.0d;
	Camera::Camera() {
		InitNull();
		//dxman: SetCamera(640,480,0.0f,-14.836f,5.376f, 0.0f,-11.77,90.0f, 39.2025f);
		Console::Error.WriteLine("CODING WARNING: CAMERA DEFAULT CONSTRUCTOR MUST NOT BE USED--needs screen dimensions--SetCamera must be called manually if default constructor is used, otherwise camera will not work.");
	}
	Camera::Camera(float UseScreenW, float UseScreenH, float x1, float y1, float z1, float xRot1, float yRot1, float zRot1, float fAperture, bool set_make_orientation_Z_UP, bool set_make_orientation_X_FORWARD) {
		InitNull();
		SetCamera(UseScreenW, UseScreenH, x1, y1, z1, xRot1, yRot1, zRot1, fAperture, set_make_orientation_Z_UP, set_make_orientation_X_FORWARD);
	}
	//Camera::Camera(float UseScreenW, float UseScreenH) {
	//	fScreenRatio=UseScreenW/UseScreenW; //i.e. 4/3 ratio
	//	SetCamera(UseScreenW,UseScreenH,0.0f,-14.836f,5.376f, 0.0f,-11.77,90.0f, 39.2025f);
		//i.e.  52.27f screen aperture width if 4:3 and 39.2025f height
	//}
	void Camera::InitNull() {
		m3dCam.name="Cam";
		make_orientation_Z_UP=false;
		make_orientation_X_FORWARD=false;
		rCameraDist=200.0f;
		//TEMP vars:
		xCameraTarget=0.0d;
		yCameraTarget=0.0d;
		zCameraTarget=0.0d;
		//fRatioFromLeft=0.0f;
		//fRatioFromTop=0.0f;
		//fYaw=0.0f;
		//fPitch=0.0f;
	}//end InitNull

	void Camera::SetCamera(float UseScreenW, float UseScreenH, float xTo, float yTo, float zTo, float xRotTo, float yRotTo, float zRotTo, float fApertureAngleHeight, bool set_make_orientation_Z_UP, bool set_make_orientation_X_FORWARD) {
		fScreenW=UseScreenW;
		fScreenH=UseScreenH;
		fScreenRatio=fScreenW/fScreenH;
		m3dCam.X=xTo;
		m3dCam.Y=yTo;
		m3dCam.Z=zTo;
		m3dCam.xRot=xRotTo;
		m3dCam.yRot=yRotTo;
		m3dCam.zRot=zRotTo;
		fApertureW=fApertureAngleHeight*fScreenRatio;
		fApertureH=fApertureAngleHeight;
		fAngleApertureTop=m3dCam.yRot+(fApertureH/2.0f);
		fAngleApertureLeft=m3dCam.zRot+(fApertureW/2.0f);
		make_orientation_Z_UP=set_make_orientation_Z_UP;
		make_orientation_X_FORWARD=set_make_orientation_X_FORWARD;
	}//end SetCamera

	///<summary>
	///Gets a rational point on the screen from a 3D point, not a pixel location. Return must be multiplied by screen dimensions to get a pixel location.
	///formerly Point2dRatiosFrom3d
	///</summary>
	bool Camera::Point2dMultipliersFrom3d(float &fReturnRightness, float &fReturnDownness, float x3d, float y3d, float z3d) {
		bool bGood=true;
		try {
			//debug assumes there is no m3dCam.Y rotation
			static float fAngleFromTop; //angle of point from top of aperture
			static float fAngleFromLeft; //angle of point from left of aperture
			static float xRel, yRel, zRel;//points relative to Camera;
			xRel=x3d-m3dCam.X;
			yRel=y3d-m3dCam.Y;
			zRel=z3d-m3dCam.Z;
			//the next line may assume camera is facing toward positive m3dCam.Y
			fAngleFromTop=(fAngleApertureTop-(RMath::ThetaOfXY_Deg(yRel,zRel)));
			fAngleFromLeft=(fAngleApertureLeft-(RMath::ThetaOfXY_Deg(xRel,yRel)));
			fReturnRightness=fAngleFromLeft/fApertureW;
			fReturnDownness=fAngleFromTop/fApertureH;
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"Point2dMultipliersFrom3d");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Point2dMultipliersFrom3d");
		}
		return bGood;//TODO: return false if not in camera bounds
	}//end Point2dMultipliersFrom3d

	bool Camera::Point2dFrom3d(float& xReturn, float& yReturn, float x, float y, float z) {
		bool bGood=true;
		try {
			static float xRightness, yDownness;
			Point2dMultipliersFrom3d(xRightness,yDownness,x,y,z);
			xReturn=xRightness*fScreenW;
			yReturn=yDownness*fScreenH;
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"Point2dFrom3d(float& xReturn, float& yReturn, float x, float y, float z)");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Point2dFrom3d(float& xReturn, float& yReturn, float x, float y, float z)");
		}
		return bGood;
	}
	bool Camera::Point2dFrom3d(FPoint &pointReturn, Mass3d &point3d) {
		bool bGood=true;
		try {
			static float xRightness, yDownness;
			Point2dMultipliersFrom3d(xRightness,yDownness,point3d.X,point3d.Y,point3d.Z);
			pointReturn.X=xRightness*fScreenW;
			pointReturn.Y=yDownness*fScreenH;
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"Point2dFrom3d(FPoint&,Mass3d&)");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Point2dFrom3d(FPoint&,Mass3d&)");
		}
		return bGood;
	}//end Point2dFrom3d


	bool Camera::Point2dFrom3dWithScreenOffset(float& xReturn, float& yReturn, float x, float y, float z, float xOffset, float yOffset) {
		bool bGood=true;
		try {
			Point2dFrom3d(xReturn, yReturn, x,y,z);
			xReturn+=xOffset;
			xReturn+=yOffset;
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn, "Point2dFrom3dWithScreenOffset(float& xReturn, float& yReturn, float x, float y, float z, float xOffset, float yOffset)");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Point2dFrom3dWithScreenOffset(float& xReturn, float& yReturn, float x, float y, float z, float xOffset, float yOffset)");
		}
		return bGood;
	}
	bool Camera::Point2dFrom3dWithScreenOffset(FPoint &pointReturn, Mass3d &point3d, float xOffset, float yOffset) {
		bool bGood=true;
		try {
			Point2dFrom3d(pointReturn, point3d);
			pointReturn.X+=xOffset;
			pointReturn.Y+=yOffset;
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn, "Point2dFrom3dWithScreenOffset(FPoint &pointReturn, Mass3d &point3d)");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Point2dFrom3dWithScreenOffset(FPoint &pointReturn, Mass3d &point3d)");
		}
		return bGood;
	}//end Point2dFrom3dWithScreenOffset



	bool Camera::Mass2dFrom3d(Mass2d &m2dWithPixPerMeter_ToModify, Mass2d &m2dReturnShadow, Mass3d &m3dEnt) {
		bool bGood=true;
		try {
			//debug yRot is the real Pitch NOT xrot!!!!!!!!
			static float xTranslate, yTranslate;//, xTestOffset, yTestOffset;
			static float xRightness, yDownness;
			Point2dMultipliersFrom3d(xRightness,yDownness,m3dEnt.X,m3dEnt.Y,m3dEnt.Z);
			//static float fScreenW=SCREEN_WIDTH, fScreenH=SCREEN_HEIGHT;
			xTranslate=xRightness*fScreenW;
			yTranslate=yDownness*fScreenH;
			//static float fSizer;//for getting m3dEnt.Z-bounds in order to calculate scale
			//fSizer=m3dEnt.Z+m3dEnt.zSize;
			//Point2dMultipliersFrom3d(xTestOffset,yTestOffset,m3dEnt.X,m3dEnt.Y,fSizer);
			//yTestOffset*=fScreenH;//makes it a pixel location (but xTestOffset is unused)
			float fDistFromCamera=DIST3D(m3dEnt,m3dCam);
			float fPixPerUnitHere=DestPixelsPerMeterAt(fDistFromCamera);
			m2dWithPixPerMeter_ToModify.fScale=fPixPerUnitHere/m2dWithPixPerMeter_ToModify.fPixelsPerMeter; //(yTranslate-yTestOffset)/(float)m2dWithPixPerMeter_ToModify.rectOriginal.bottom;//SET SCALE
			if (m2dWithPixPerMeter_ToModify.fScale<.05) m2dWithPixPerMeter_ToModify.fScale=.05;//prevents zero size after using scale
			m2dReturnShadow.fScale=m2dWithPixPerMeter_ToModify.fScale;
			//float fWidthScaled=(float)(IROUNDF((float)m2dWithPixPerMeter_ToModify.rectOriginal.right*m2dWithPixPerMeter_ToModify.fScale))+1;//debug only: +1 shouldn't be needed
			//float fHeightScaled=(float)(IROUNDF((float)m2dWithPixPerMeter_ToModify.rectOriginal.bottom*m2dWithPixPerMeter_ToModify.fScale))+1;//debug only: +1 shouldsn't be needed
			float fWidthScaled=(float)(IROUNDF((float)m2dWithPixPerMeter_ToModify.rectOriginal.right*m2dWithPixPerMeter_ToModify.fScale));
			float fHeightScaled=(float)(IROUNDF((float)m2dWithPixPerMeter_ToModify.rectOriginal.bottom*m2dWithPixPerMeter_ToModify.fScale));
			//float fHalfWidthScaled=FFLOOR(fWidthScaled/2.0f);
			//float fHalfHeightScaled=FFLOOR(fHeightScaled/2.0f);
			m2dWithPixPerMeter_ToModify.rectRender.left=IROUNDF(xTranslate-m2dWithPixPerMeter_ToModify.FCenterXRelScaled());
			m2dWithPixPerMeter_ToModify.rectRender.top=IROUNDF(yTranslate-m2dWithPixPerMeter_ToModify.FCenterYRelScaled());
			m2dWithPixPerMeter_ToModify.rectRender.right=m2dWithPixPerMeter_ToModify.rectRender.left+IROUNDF(fWidthScaled);
			m2dWithPixPerMeter_ToModify.rectRender.bottom=m2dWithPixPerMeter_ToModify.rectRender.top+IROUNDF(fHeightScaled);

			Point2dMultipliersFrom3d(xRightness,yDownness,m3dEnt.X,m3dEnt.Y,0);
			m2dReturnShadow.rectRender.left=m2dWithPixPerMeter_ToModify.rectRender.left;//assumes top-down lighting
			m2dReturnShadow.rectRender.right=m2dReturnShadow.rectRender.left+(int)(m2dReturnShadow.fScale*(float)m2dReturnShadow.rectOriginal.right+.5f);//m2dWithPixPerMeter_ToModify.rectRender.right;
			//m2dReturnShadow.rectRender.top=IROUNDF(yDownness*fScreenH-(float)m2dReturnShadow.rectOriginal.top/2.0f*m2dReturnShadow.fScale);
			yTranslate=yDownness*fScreenH;
			m2dReturnShadow.rectRender.top=IROUNDF(yTranslate-m2dReturnShadow.FCenterYRelScaled());
			m2dReturnShadow.rectRender.bottom=IROUNDF((float)m2dReturnShadow.rectRender.top+m2dReturnShadow.fScale*(float)m2dReturnShadow.rectOriginal.bottom);
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"Mass2dFrom3d");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Mass2dFrom3d");
		}
		return bGood;
	}//end Mass2dFrom3d

	float Camera::DestPixelsPerMeterAt(float fDistanceFromCamera) {
		float fReturn=fScreenW / FPDIST( RMath::XOfRTheta_Deg(fDistanceFromCamera,0),
										RMath::YOfRTheta_Deg(fDistanceFromCamera,0),
										RMath::XOfRTheta_Deg(fDistanceFromCamera,fApertureW),
										RMath::YOfRTheta_Deg(fDistanceFromCamera,fApertureW) );
		if (fReturn<0.0f) fReturn=0.0f;
		return fReturn;
	}//end DestPixelsPerMeterAt


	///<summary>
	///zDistanceFromCameraInVirtualPixels is aka zRelFakePixelsPositiveIsAwayFromCamera
	///<summary>
	bool Camera::Point3dMoveAndRotateBy2d(Mass3d& pointToChange,float xRelativeToScreenPoint,float yRelativeToScreenPoint,float zDistanceFromCameraInVirtualPixels, float xScreenPoint, float yScreenPoint) {
		/*//OLD: used to be used like:
			//Applicable things that used to be done before calling this method:
			xRelNear=xfCursor-p2dHeroEye.X;
			yRelNear=yfCursor-p2dHeroEye.Y;
			xRelNear=xCursorNearFlatAbs-p2dHeroEye.X;
			xRelNear*=xAimFlipper;
			yRelNear*=yAimFlipper;
			zFake*=zAimFlipper;
			p3dCursorAbs=p3dHeroEye.Copy();
			camera.Point3dMoveAndRotateBy2d(p3dCursorAbs,xRelNear,yRelNear,zFake,p2dHeroEye.X,p2dHeroEye.Y);
			//Then the resulting pointToChange from this method was used to calculate a relative point, for aiming:
			p3dCursorRel.X=p3dCursorAbs.X-p3dHeroEye.X;
			p3dCursorRel.Y=p3dCursorAbs.Y-p3dHeroEye.Y;
			p3dCursorRel.Z=p3dCursorAbs.Z-p3dHeroEye.Z;

		*/
		bool bGood=true;
		static float fMetersFromCamera,
				//fRatioFromLeft,
				//fRatioFromTop,
				fDestPixPerUnitHere,
				fAltitude,
				fAzimuth,
				fDistance;
		try {
			fMetersFromCamera=DIST3D(pointToChange,m3dCam);
			fDestPixPerUnitHere=DestPixelsPerMeterAt(fMetersFromCamera);

			static FPOINT3D pointRel, pointTempForAltitude;
			pointRel.X=zDistanceFromCameraInVirtualPixels; //xRelativeToScreenPoint
			pointRel.Y=-xRelativeToScreenPoint; //zDistanceFromCameraInVirtualPixels
			pointRel.Z=-yRelativeToScreenPoint; //-yRelativeToScreenPoint
			RMath::Rotate(pointRel.Y,pointRel.Z,m3dCam.xRot);
			RMath::Rotate(pointRel.X,pointRel.Z,m3dCam.yRot);
			RMath::Rotate(pointRel.X,pointRel.Y,m3dCam.zRot);
			static float fSquare;
			fSquare=FSQUARED(pointRel.X) + FSQUARED(pointRel.Y) + FSQUARED(pointRel.Z);
			fDistance=(fSquare>0)?sqrt(fSquare):0;
			fAzimuth=RMath::SafeAngle360(RMath::ThetaOfXY_Deg(pointRel.X,pointRel.Y) );
			pointTempForAltitude.X=pointRel.X;
			pointTempForAltitude.Y=pointRel.Y;
			pointTempForAltitude.Z=pointRel.Z;
			RMath::Rotate(pointTempForAltitude.X,pointTempForAltitude.Y,-fAzimuth);
			fAltitude=RMath::SafeAngle360(RMath::ThetaOfXY_Deg(pointTempForAltitude.X,pointTempForAltitude.Z) );
		//xRelativeToScreenPoint, yRelativeToScreenPoint, zDistanceFromCameraInVirtualPixels, xScreenPoint, yScreenPoint

			//CONVERTS TO METERS:
			fDistance/=fDestPixPerUnitHere;
			pointToChange.yRot=fAltitude;
			pointToChange.zRot=fAzimuth;
			Travel3d(pointToChange, pointToChange.yRot, pointToChange.zRot, fDistance, make_orientation_Z_UP, make_orientation_X_FORWARD);

			///TODO:? self-moving test dest vars:
			//static float fTestAlt=90.0f, fTestAz=0.0f;
			//MS:
			//static float fTickLastTest=GetTicks_Relative();
			//float fTicksPassed=(float)GetTicks_Relative()-fTickLastTest;
			//static float fAzDegPerTick=360.0f/1000.0f; //360deg in 1s
			//static float fAltDegPerTick=-180.0f/20000.0f; //-180deg in 20s
			//fTickLastTest=(float)GetTicks_Relative();
			//fTestAlt+=fAltDegPerTick*fTicksPassed;
			//fTestAz+=fAzDegPerTick*fTicksPassed;
			//S:
			//static float fSecLastTest=(float)Base_GetSeconds();
			//float fSecondsPassed=(float)Base_GetSeconds()-fSecLastTest;
			//static float fAzDegPerSec=360.0f;
			//static float fAltDegPerSec=-180.0f/20.0f; //-180deg in 20s
			//fTickLastTest=(float)Base_GetSeconds();
			//fTestAlt+=fAltDegPerSec*fSecondsPassed;
			//fTestAz+=fAzDegPerSec*fSecondsPassed;

			//while (fTestAlt<=-90.0f) fTestAlt+=180.0f;
			//while (fTestAz>=360.0f) fTestAz-=360.0f;
			//Travel3d(pointToChange, fTestAlt, fTestAz, fDistance,camera.make_orientation_Z_UP,camera.make_orientation_X_FORWARD);

			//static int iTest=0;
			//if (iTest<100) {
			//	Console::Error.WriteLine("{ fMetersFromCamera:"+fMetersFromCamera+"}");
			//	Console::Error.WriteLine("{ pointOfReferenceForDistanceOnly.X:"+pointOfReferenceForDistanceOnly.X+"; pointOfReferenceForDistanceOnly.Y:"+pointOfReferenceForDistanceOnly.Y+" (original value from Point3d by distance) }");
			//	Console::Error.WriteLine("{ pointReturn.X:"+pointReturn.X+"; pointReturn.Y:"+pointReturn.Y+" (original value from Point3d by distance) }");
			//}
			//Travel3d(pointReturn,m3dCam,-zFakePixelsPositiveIsAwayFromCamera/DestPixelsPerMeterAt(fMetersFromCamera));
			//if (iTest<100) {
			//	Console::Error.WriteLine("{ pointReturn.X:"+pointReturn.X+"; pointReturn.Y:"+pointReturn.Y+"; moved toward camera:"+(zFakePixelsPositiveIsAwayFromCamera/DestPixelsPerMeterAt(fMetersFromCamera))+"; DestPixelsPerMeterAt(fMetersFromCamera):"+DestPixelsPerMeterAt(fMetersFromCamera)+"; }");
			//	iTest++;
			//}
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"Point3dMoveAndRotateBy2d");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Point3dMoveAndRotateBy2d");
		}
		return bGood;
	}//end Point3dMoveAndRotateBy2d

	///<summary>
	///Assumes distance (uses Travel3d method). May be renamed to Point3Drom2DAssumingDistance
	///</summary>
	bool Camera::Point3dFrom2d(Mass3d &pointReturn, float x2D, float y2D, float fMetersFromCamera) {
		bool bGood=true;
		static float cursorRightness;
		static float cursorBottomness;
		static float cursorYaw;
		static float cursorPitch;
		try {
			cursorRightness=x2D/fScreenW;
			cursorBottomness=y2D/fScreenH;
			cursorYaw=fAngleApertureLeft-cursorRightness*fApertureW;
			cursorPitch=fAngleApertureTop-cursorBottomness*fApertureH;

			pointReturn.X=m3dCam.X;
			pointReturn.Y=m3dCam.Y;
			pointReturn.Z=m3dCam.Z;
			Travel3d(pointReturn,cursorPitch,cursorYaw,fMetersFromCamera,make_orientation_Z_UP,make_orientation_X_FORWARD);
			//static int iTest=0;
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"Point3dFrom2d");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Point3dFrom2d");
		}
		return bGood;
	}//end Point3dFrom2d


	///<summary>
	///Assumes height to get a location on a plane using a 2D point on the screen.
	///ALSO assumes plane x,y rotation is 0,0 (is a plane parallel to the ground)
	///maximumPitch should be about -5.0 (degrees) so that the point isn't super far away (0.0 could not be calculated. Above 0.0 will never intersect plane
	///& would be impossible to calculate, assuming camera is above the plane)
	///returns false if couldn't find a 3D point (e.g. cursor doesn't intersect the plane from this camera's point of view)
	///z3D is the assumed height (line of sight is intersected with plane which has z3D as its constant height).
	///</summary>
	bool Camera::Point3dFrom2dAssumingHeight(Mass3d& pointReturn, int& iLineRelationshipType, float x2D, float y2D, float z3D, float maximumPitch) {
		static float cursorRightness;
		static float cursorBottomness;
		static float cursorYaw;
		static float cursorPitch;
		static float xCursorTarget;
		static float yCursorTarget;
		static float zCursorTarget;
		LastSideLineRelationshipTypeNumber=0;
		LastTopLineRelationshipTypeNumber=0;
		bool bGood=true;
		try {
			if (maximumPitch>-5.0f) maximumPitch=-5.0f;
			cursorRightness=x2D/fScreenW;
			cursorBottomness=y2D/fScreenH;
			cursorYaw=fAngleApertureLeft-cursorRightness*fApertureW;
			cursorPitch=fAngleApertureTop-cursorBottomness*fApertureH;
			if (cursorPitch>maximumPitch) cursorPitch=maximumPitch;
			//ANSWER: where does pitch intersect a plane whose z==z3D
			double xInter=0.0d, yInter=0.0d,zInter=0.0d; //the intersection is on the yz plane, and is ALWAYS z==height of plane since plane is flat
			//iLastLineRelationshipType=RMath::IntersectionAndRelationship(yInter, zInter, Line1_y1, Line1_z1, Line1_y2, Line1_z2, Line2_y1, Line2_z1, Line2_y2, Line2_z2);
			xCursorTarget=m3dCam.X;
			yCursorTarget=m3dCam.Y;
			zCursorTarget=m3dCam.Z;
			Travel3d(xCursorTarget,yCursorTarget,zCursorTarget,(double)cursorPitch,(double)cursorYaw,(double)rCameraDist,this->make_orientation_Z_UP,this->make_orientation_X_FORWARD);

			///From side view, get intersection primarily to get yInter, aka depth (
			///Line1: Y-AXIS LINE, z3D units above ground (looking at Y-Z plane):
			///Line2: From camera to focal point (focal point xCursorTarget,yCursorTarget determined above using facing direction and distance)
			///				   RMath::IntersectionAndRelationship(x,	  y,		Line1_x1,		Line1_y1,	Line1_x2,Line1_y2,	Line2_x1,			Line2_y1,			Line2_x2,		Line2_y2);
			iLineRelationshipType=RMath::IntersectionAndRelationship(yInter, zInter,-100.0d,		z3D,		100.0d,	 z3D,		(double)m3dCam.Y,	(double)m3dCam.Z,	yCursorTarget,	zCursorTarget);
			LastSideLineRelationshipTypeNumber=iLineRelationshipType;
			//static double cameraLeftSideX;
			//static double cameraLeftSideY;
			//static double cameraRightSideX;
			//static double cameraRightSideY;
			//if ( (iLineRelationshipType==RMath::LineRelationshipIntersectionNotTouchingEndsOfLineB) || (iLineRelationshipType>=RMath::LineRelationships_ConstantsThisIntegerOrHigherAreNotCrossingOnLineSegment) ) {
			///From top view, intersect with a horizontal line along with constant depth (yInter above)
			if (
					(iLineRelationshipType==RMath::LineRelationshipIntersectionNotTouchingEndsOfLineB)
					 || (iLineRelationshipType==RMath::LineRelationshipIntersectionNotCrossingInRangeWouldIntersect)
					 || (iLineRelationshipType==RMath::LineRelationshipLineBPoint1IsOnLineA)
					 || (iLineRelationshipType==RMath::LineRelationshipLineBPoint2IsOnLineA)
				) {
				//double fHypotenuse=yInter
				//xInter=cursorRightness*ThirdAngle //I know the distance and angle but need hypotenuse
				//xCursorTarget=m3dCam.X;
				//yCursorTarget=m3dCam.Y;
				//zCursorTarget=m3dCam.Z;//z3D;
				//Travel3d(xCursorTarget,yCursorTarget,zCursorTarget,(double)m3dCam.yRot,(double)m3dCam.zRot,(double)rCameraDist);
				static double xInterUsingYaw;
				static double yInterUsingYaw;
				///NOTE: IntersectionType is DIFFERENT issue than LineRelationshipType
				///Constant depth is x axis line (see above if for description)
				///Line1: horizontal line along x
				///Line2: camera direction from top view
				///					RMath::IntersectionAndRelationship(x,	  			y,					Line1_x1,Line1_y1,Line1_x2,Line1_y2,Line2_x1,			Line2_y1,			Line2_x2,		Line2_y2
				int iIntersection2Type=RMath::IntersectionAndRelationship(xInterUsingYaw,	yInterUsingYaw,-100.0d, yInter, 	100.0d,	yInter,	(double)m3dCam.X, (double)m3dCam.Y, xCursorTarget, yCursorTarget);
				LastTopLineRelationshipTypeNumber=iIntersection2Type;
				pointReturn.X=xInterUsingYaw;
				pointReturn.Y=yInter;
				pointReturn.Z=z3D;
			}
			else {
				LastTopLineRelationshipTypeNumber=RMath::LineRelationshipNone;
	///asdf FINISH THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				pointReturn.X=0;
				pointReturn.Y=0;
				pointReturn.Z=z3D;
			}
			//float fMetersFromCameraOnXYPlane;
			//Travel3d(pointReturn,cursorPitch,cursorYaw,fMetersFromCameraOnXYPlane);
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"Point3dFrom2dAssumingHeight");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Point3dFrom2dAssumingHeight");
		}
		return bGood;
	}//end Point3dFrom2dAssumingHeight





	bool Camera::PointIsInView(float x3d, float y3d, float z3d) {
		bool bGood=false;
		//bool bReturn=false; //whether 3d point is on screen
		try {
			//debug assumes there is no m3dCam.Y rotation
			static float fAngleFromTop; //angle of point from top of aperture
			static float fAngleFromLeft; //angle of point from left of aperture
			static float xRel, yRel, zRel;//points relative to Camera;
			xRel=x3d-m3dCam.X;
			yRel=y3d-m3dCam.Y;
			zRel=z3d-m3dCam.Z;
			//the next line may assume camera is facing toward positive m3dCam.Y
			fAngleFromTop=(fAngleApertureTop-(RMath::ThetaOfXY_Deg(yRel,zRel)));
			if (fAngleFromTop<0.0f) return false;
			else if (fAngleFromTop>=fApertureH) return false;
			fAngleFromLeft=(fAngleApertureLeft-(RMath::ThetaOfXY_Deg(xRel,yRel)));
			if (fAngleFromLeft<0.0f) return false;
			else if (fAngleFromLeft>=fApertureW) return false;
			bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"PointIsInView");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("PointIsInView");
		}
		return bGood;
	}//end PointIsInView

	///#region 3D drawing
	void Camera::Draw3DLine(GBuffer* thisBuffer, float startVector3[], float endVector3[], byte r, byte g, byte b, byte a, byte a_FadeFarthestPoint_To) {
		Draw3DLine(thisBuffer,startVector3[0],startVector3[1],startVector3[2],endVector3[0],endVector3[1],endVector3[2],r,g,b,a,a_FadeFarthestPoint_To);
	}
	void Camera::Draw3DLine(GBuffer* thisBuffer, float x1, float y1, float z1, float x2, float y2, float z2, byte r, byte g, byte b, byte a, byte a_FadeFarthestPoint_To) {
		if (thisBuffer!=nullptr) {
			byte byFirstAlpha=a;
			byte byLastAlpha=a_FadeFarthestPoint_To;

			if (DIST3D(this->m3dCam.X, this->m3dCam.Y, this->m3dCam.Z, x2, y2, z2)<DIST3D(this->m3dCam.X, this->m3dCam.Y, this->m3dCam.Z, x1, y1, z1)) {
				byte bySwap=byFirstAlpha;
				byFirstAlpha=byLastAlpha;
				byLastAlpha=bySwap;
			}
			static Pixel pixelStart,pixelEnd;
			pixelStart.Set(r,g,b,byFirstAlpha);
			pixelEnd.Set(r,g,b,byLastAlpha);
			static FPoint pointSrc, pointDest;
			this->Point2dFrom3dWithScreenOffset(pointSrc.X, pointSrc.Y, x1, y1, z1, 0, 0);
			this->Point2dFrom3dWithScreenOffset(pointDest.X, pointDest.Y, x2, y2, z2, 0, 0);
			thisBuffer->DrawSubpixelLine( pointSrc, pointDest, pixelStart, &pixelEnd, 1);
		}
		else Console::Error.WriteLine("Camera::Draw3DLine Error: thisBuffer is nullptr");
	}
	void Camera::Draw3DLine(GBuffer* thisBuffer, Mass3d& m3dStart, Mass3d& m3dEnd, byte r, byte g, byte b, byte a, byte a_FadeFarthestPoint_To) {
		Draw3DLine(thisBuffer,m3dStart.X,m3dStart.Y,m3dStart.Z,m3dEnd.X,m3dEnd.Y,m3dEnd.Z,r,g,b,a,a_FadeFarthestPoint_To);
		/*
		float fOpacityOfEnd=ANGLEDIFFPOSITIVE(m3dStart.yRot,0);//always near RELATIVE to m3d_, not necessarily to camera (see below for swap when not)
		if (fOpacityOfEnd>180) {
			Console::Error.WriteLine("Math usage error: ANGLEDIFFPOSITIVE returned greater than 180 {ANGLEDIFFPOSITIVE("+RString_ToString(m3dStart.yRot)+",0):"+RString_ToString(fOpacityOfEnd)+"}");
			//fOpacityOfEnd=fOpacityOfEnd-180;
		}
		if (fOpacityOfEnd>90) {
			fOpacityOfEnd=180-fOpacityOfEnd;
		}
		//if (fOpacityOfEnd<0) fOpacityOfEnd=180+fOpacityOfEnd;
		fOpacityOfEnd=(fOpacityOfEnd)/90.0f;//the closer it is to zero (angled toward horizon) the less opaque End pixel will be (unless flipped when point End from object is closer to camera)
		byte byFirstAlpha=a;//=0;
		byte byLastAlpha=APPROACH(a,a_FadeFarthestPoint_To,1.0f-fOpacityOfEnd);//SafeByRoundF((fOpacityOfEnd)*255.0f);
		if (DIST3D(this->m3dCam,m3dEnd)<DIST3D(this->m3dCam,m3dStart)) {
			byte bySwap=byFirstAlpha;
			byFirstAlpha=byLastAlpha;
			byLastAlpha=bySwap;
		}
		static Pixel pixelStart,pixelEnd;
		pixelStart.Set(r,g,b,byFirstAlpha);
		pixelEnd.Set(r,g,b,byLastAlpha);
		static FPoint pointSrc, pointDest;
		camera.Point2dFrom3dWithScreenOffset(pointSrc,m3dStart,0,0);
		camera.Point2dFrom3dWithScreenOffset(pointDest,m3dEnd,0,0);
		screen_ptr->DrawSubpixelLine( pointSrc, pointDest, pixelStart, &pixelEnd, 1);
		*/
	}//end Draw3DLine
	///<summary>
	///Draw each axis (X:Red; Y:Green; Z:Blue)
	///</summary>
	void Camera::DrawAxes(GBuffer* thisBuffer, Mass3d& m3dAxisCenter) {
		//Blender Euler rotation:
		//if zRot=0 then +y points toward +y, +x points toward +x
		//if zRot=90 then +y points toward -x, +x points toward +y
		//as yRot becomes more negative, +x points more and more upward (which makes sense since that is left roll and rolling left raises +x [right] wing)
		//as xRot becomes more positive, +y points more and more upward (which makes sense since that is upward pitch)

		Mass3d m3dAxisFarX;
		m3dAxisFarX.name="m3dAxisFarX";
		m3dAxisFarX.X+=1.0;
		Mass3d m3dAxisFarY;
		m3dAxisFarY.name="m3dAxisFarY";
		m3dAxisFarY.Y+=1.0;
		Mass3d m3dAxisFarZ;
		m3dAxisFarZ.name="m3dAxisFarZ";
		m3dAxisFarZ.Z+=1.0;
		//start rotation
		RMath::Rotate(m3dAxisFarX.X,m3dAxisFarX.Y,m3dAxisCenter.zRot);
		RMath::Rotate(m3dAxisFarX.X,m3dAxisFarX.Z,m3dAxisCenter.yRot);
		RMath::Rotate(m3dAxisFarX.Y,m3dAxisFarX.Z,m3dAxisCenter.xRot);
		RMath::Rotate(m3dAxisFarY.X,m3dAxisFarY.Y,m3dAxisCenter.zRot);
		RMath::Rotate(m3dAxisFarY.X,m3dAxisFarY.Z,m3dAxisCenter.yRot);
		RMath::Rotate(m3dAxisFarY.Y,m3dAxisFarY.Z,m3dAxisCenter.xRot);
		RMath::Rotate(m3dAxisFarZ.X,m3dAxisFarZ.Y,m3dAxisCenter.zRot);
		RMath::Rotate(m3dAxisFarZ.X,m3dAxisFarZ.Z,m3dAxisCenter.yRot);
		RMath::Rotate(m3dAxisFarZ.Y,m3dAxisFarZ.Z,m3dAxisCenter.xRot);
		//end rotation
		m3dAxisCenter.OffsetSomethingByMyLocation(m3dAxisFarX);
		m3dAxisCenter.OffsetSomethingByMyLocation(m3dAxisFarY);
		m3dAxisCenter.OffsetSomethingByMyLocation(m3dAxisFarZ);
		Draw3DLine(thisBuffer,m3dAxisCenter,m3dAxisFarX,255,0,0,255,0);//Red
		Draw3DLine(thisBuffer,m3dAxisCenter,m3dAxisFarY,0,255,0,255,0);//Green
		Draw3DLine(thisBuffer,m3dAxisCenter,m3dAxisFarZ,0,0,255,255,0);//Blue
		//FPoint p2dOrig,p2dFarNow;
		//camera.Point2dFrom3dWithScreenOffset(p2dOrig,m3dAxisCenter,0,0);
		//Pixel pixelNow;
		//pixelNow.Set(255,0,0,255);
		//camera.Point2dFrom3dWithScreenOffset(p2dFarNow,m3dAxisFarX,0,0);
		//screen_ptr->DrawSubpixelLine(p2dOrig, p2dFarNow, pixelNow, null, 1);
		//pixelNow.Set(0,255,0,255);
		//camera.Point2dFrom3dWithScreenOffset(p2dFarNow,m3dAxisFarY,0,0);
		//screen_ptr->DrawSubpixelLine(p2dOrig, p2dFarNow, pixelNow, null, 1);
		//pixelNow.Set(0,0,255,255);
		//camera.Point2dFrom3dWithScreenOffset(p2dFarNow,m3dAxisFarZ,0,0);
		//screen_ptr->DrawSubpixelLine(p2dOrig, p2dFarNow, pixelNow, null, 1);
	}//end DrawAxes


	///<summary>
	///Draw equal-sided polygon around a 3d axis
	///</summary>
	void Camera::Draw3dRegularPolygon(GBuffer* thisBuffer, Mass3d& m3dCenter, char Axis, float StartAngle_Deg, float Radius, int Sides, Pixel color) {
		if (thisBuffer!=nullptr) {
			if (Sides>0) {
				if (Sides>32) Sides=32;//debug hard-coded limit
				static Mass3d m3dVert[32];//debug hard-coded limit //Mass3d[]
				static string this_func="";
				if (this_func!="Draw3dRegularPolygon") {
					this_func="Draw3dRegularPolygon";
					for (int i=0; i<32; i++) {
						m3dVert[i].name="Vert["+RString_ToString(i)+"]";
					}
				}

				//0 to 3 are clockwise starting from top left
				//float AngleOffset_Deg = 360.0f/(float)Sides;
				static float TAU = M_PI*2.0f;
				float thisAngle_Rad = StartAngle_Deg*(M_PI/180);
				float AngleOffsetPerSide_Rad = TAU /(float)Sides;

				m3dVert[0].X=m3dCenter.X;
				m3dVert[0].Y=m3dCenter.Y;
				m3dVert[0].Z=m3dCenter.Z;
				if (Axis=='Z') {
					Travel2d_Polar_Rad(m3dVert[0].X,m3dVert[0].Y,Radius,thisAngle_Rad,false);
				}
				else if (Axis=='Y') {
					Travel2d_Polar_Rad(m3dVert[0].X,m3dVert[0].Z,Radius,thisAngle_Rad,false);
				}
				else if (Axis=='X') {
					Travel2d_Polar_Rad(m3dVert[0].Y,m3dVert[0].Z,Radius,thisAngle_Rad,false);
				}
				int LastSideToDo=Sides-1;
				int Side=0;
				int NextSide=1;
				float nextAngle_Rad=thisAngle_Rad;
				static FPoint thisPoint2d, nextPoint2d;
				for (NextSide=1; NextSide<Sides; NextSide++) {
					nextAngle_Rad+=AngleOffsetPerSide_Rad;
					m3dVert[NextSide].X=m3dCenter.X;
					m3dVert[NextSide].Y=m3dCenter.Y;
					m3dVert[NextSide].Z=m3dCenter.Z;
					if (Axis=='Z') {
						Travel2d_Polar_Rad(m3dVert[NextSide].X,m3dVert[NextSide].Y,Radius,nextAngle_Rad,false);
					}
					else if (Axis=='Y') {
						Travel2d_Polar_Rad(m3dVert[NextSide].X,m3dVert[NextSide].Z,Radius,nextAngle_Rad,false);
					}
					else if (Axis=='X') {
						Travel2d_Polar_Rad(m3dVert[NextSide].Y,m3dVert[NextSide].Z,Radius,nextAngle_Rad,false);
					}
					Point2dFrom3d(thisPoint2d,m3dVert[Side]);
					Point2dFrom3d(nextPoint2d,m3dVert[NextSide]);
					thisBuffer->DrawSubpixelLine(thisPoint2d,nextPoint2d,color,nullptr,0.5f);
					Side++;
				}
				Point2dFrom3d(thisPoint2d,m3dVert[0]);
				Point2dFrom3d(nextPoint2d,m3dVert[Sides-1]);
				thisBuffer->DrawSubpixelLine(thisPoint2d,nextPoint2d,color,nullptr,0.5f);

				/*
				m3d0.HardLocation(m3dCenter.X,m3dCenter.Y,m3dCenter.Z);//m3dCenter.CopyTo(m3d0);
				m3d1.HardLocation(m3dCenter.X,m3dCenter.Y,m3dCenter.Z);//m3dCenter.CopyTo(m3d1);
				m3d2.HardLocation(m3dCenter.X,m3dCenter.Y,m3dCenter.Z);//m3dCenter.CopyTo(m3d2);
				m3d3.HardLocation(m3dCenter.X,m3dCenter.Y,m3dCenter.Z);//m3dCenter.CopyTo(m3d3);
				m3d0.X-=m3dNow.xSize/2.0f;
				m3d1.X+=m3dNow.xSize/2.0f;
				m3d2.X+=m3dNow.xSize/2.0f;
				m3d3.X-=m3dNow.xSize/2.0f;
				m3d0.Y-=m3dNow.ySize/2.0f;
				m3d1.Y-=m3dNow.ySize/2.0f;
				m3d2.Y+=m3dNow.ySize/2.0f;
				m3d3.Y+=m3dNow.ySize/2.0f;
				static FPoint point0;
				static FPoint point1;
				static FPoint point2;
				static FPoint point3;
				camera.Point2dFrom3d(point0,m3d0);
				camera.Point2dFrom3d(point1,m3d1);
				camera.Point2dFrom3d(point2,m3d2);
				camera.Point2dFrom3d(point3,m3d3);
				screen_ptr->DrawSubpixelLine(point0,point1,pixelGreen,null,1.0f);
				screen_ptr->DrawSubpixelLine(point1,point2,pixelGreen,null,1.0f);
				screen_ptr->DrawSubpixelLine(point2,point3,pixelGreen,null,1.0f);
				screen_ptr->DrawSubpixelLine(point3,point0,pixelGreen,null,1.0f);
				*/
			}//end if Sides>0
		}
		else Console::Error.WriteLine("Camera::Draw3dRegularPolygon Error: thisBuffer is nullptr");
	}//end Camera::Draw3dRegularPolygon(GBuffer* thisBuffer, Mass3d& m3dNow, char Axis, int Sides, Pixel color)

	///
	///Draws a 3d box using a Mass3d (formerly DrawCube, formerly DrawMass, aka Draw3dBox)
	///
	void Camera::DrawBox(GBuffer* thisBuffer, Mass3d &m3dNow, Pixel &pixelNear, Pixel &pixelFar) {
		FPoint fpStart, fpEnd;
		static Mass3d m3darrBox[8]; //(see drawing 1.box-indeces.png) //Mass3d[]
		float
			xSizeHalf=m3dNow.xSize/2.0f,
			ySizeHalf=m3dNow.ySize/2.0f,
			zSizeHalf=m3dNow.zSize/2.0f;
		if (thisBuffer!=nullptr) {
			try {
				m3darrBox[0].X=m3dNow.X-xSizeHalf;
				m3darrBox[0].Y=m3dNow.Y+ySizeHalf;
				m3darrBox[0].Z=m3dNow.Z+zSizeHalf;
				m3darrBox[1].X=m3dNow.X+xSizeHalf;
				m3darrBox[1].Y=m3dNow.Y+ySizeHalf;
				m3darrBox[1].Z=m3dNow.Z+zSizeHalf;
				m3darrBox[2].X=m3dNow.X+xSizeHalf;
				m3darrBox[2].Y=m3dNow.Y-ySizeHalf;
				m3darrBox[2].Z=m3dNow.Z+zSizeHalf;
				m3darrBox[3].X=m3dNow.X-xSizeHalf;
				m3darrBox[3].Y=m3dNow.Y-ySizeHalf;
				m3darrBox[3].Z=m3dNow.Z+zSizeHalf;

				m3darrBox[4].X=m3dNow.X-xSizeHalf;
				m3darrBox[4].Y=m3dNow.Y+ySizeHalf;
				m3darrBox[4].Z=m3dNow.Z-zSizeHalf;
				m3darrBox[5].X=m3dNow.X+xSizeHalf;
				m3darrBox[5].Y=m3dNow.Y+ySizeHalf;
				m3darrBox[5].Z=m3dNow.Z-zSizeHalf;
				m3darrBox[6].X=m3dNow.X+xSizeHalf;
				m3darrBox[6].Y=m3dNow.Y-ySizeHalf;
				m3darrBox[6].Z=m3dNow.Z-zSizeHalf;
				m3darrBox[7].X=m3dNow.X-xSizeHalf;
				m3darrBox[7].Y=m3dNow.Y-ySizeHalf;
				m3darrBox[7].Z=m3dNow.Z-zSizeHalf;
				float fSubPixAccuracy=3.0f;

				this->Point2dFrom3d(fpStart,m3darrBox[4]);
				this->Point2dFrom3d(fpEnd,m3darrBox[5]);
				thisBuffer->DrawSubpixelLine(fpStart.X, fpStart.Y, fpEnd.X, fpEnd.Y,
						pixelFar,&pixelFar,fSubPixAccuracy);
				this->Point2dFrom3d(fpStart,m3darrBox[4]);
				this->Point2dFrom3d(fpEnd,m3darrBox[7]);
				thisBuffer->DrawSubpixelLine(fpStart.X, fpStart.Y, fpEnd.X, fpEnd.Y,
						pixelFar,&pixelNear,fSubPixAccuracy);
				this->Point2dFrom3d(fpStart,m3darrBox[5]);
				this->Point2dFrom3d(fpEnd,m3darrBox[6]);
				thisBuffer->DrawSubpixelLine(fpStart.X, fpStart.Y, fpEnd.X, fpEnd.Y,
						pixelFar,&pixelNear,fSubPixAccuracy);
				this->Point2dFrom3d(fpStart,m3darrBox[4]);
				this->Point2dFrom3d(fpEnd,m3darrBox[0]);
				thisBuffer->DrawSubpixelLine(fpStart.X, fpStart.Y, fpEnd.X, fpEnd.Y,
						pixelFar,&pixelFar,fSubPixAccuracy);
				this->Point2dFrom3d(fpStart,m3darrBox[5]);
				this->Point2dFrom3d(fpEnd,m3darrBox[1]);
				thisBuffer->DrawSubpixelLine(fpStart.X, fpStart.Y, fpEnd.X, fpEnd.Y,
						pixelFar,&pixelFar,fSubPixAccuracy);
				this->Point2dFrom3d(fpStart,m3darrBox[0]);
				this->Point2dFrom3d(fpEnd,m3darrBox[1]);
				thisBuffer->DrawSubpixelLine(fpStart.X, fpStart.Y, fpEnd.X, fpEnd.Y,
						pixelFar,&pixelFar,fSubPixAccuracy);
				this->Point2dFrom3d(fpStart,m3darrBox[0]);
				this->Point2dFrom3d(fpEnd,m3darrBox[3]);
				thisBuffer->DrawSubpixelLine(fpStart.X, fpStart.Y, fpEnd.X, fpEnd.Y,
						pixelFar,&pixelNear,fSubPixAccuracy);
				this->Point2dFrom3d(fpStart,m3darrBox[1]);
				this->Point2dFrom3d(fpEnd,m3darrBox[2]);
				thisBuffer->DrawSubpixelLine(fpStart.X, fpStart.Y, fpEnd.X, fpEnd.Y,
						pixelFar,&pixelNear,fSubPixAccuracy);
				this->Point2dFrom3d(fpStart,m3darrBox[7]);
				this->Point2dFrom3d(fpEnd,m3darrBox[6]);
				thisBuffer->DrawSubpixelLine(fpStart.X, fpStart.Y, fpEnd.X, fpEnd.Y,
						pixelNear,&pixelNear,fSubPixAccuracy);
				this->Point2dFrom3d(fpStart,m3darrBox[7]);
				this->Point2dFrom3d(fpEnd,m3darrBox[3]);
				thisBuffer->DrawSubpixelLine(fpStart.X, fpStart.Y, fpEnd.X, fpEnd.Y,
						pixelNear,&pixelNear,fSubPixAccuracy);
				this->Point2dFrom3d(fpStart,m3darrBox[6]);
				this->Point2dFrom3d(fpEnd,m3darrBox[2]);
				thisBuffer->DrawSubpixelLine(fpStart.X, fpStart.Y, fpEnd.X, fpEnd.Y,
						pixelNear,&pixelNear,fSubPixAccuracy);
				this->Point2dFrom3d(fpStart,m3darrBox[3]);
				this->Point2dFrom3d(fpEnd,m3darrBox[2]);
				thisBuffer->DrawSubpixelLine(fpStart.X, fpStart.Y, fpEnd.X, fpEnd.Y,
						pixelNear,&pixelNear,fSubPixAccuracy);
			}
			catch (exception& exn) {
				//bGood=false;
				Console::Error.Write("Could not finish DrawBox: ");
				Console::Error.Flush();
				Console::Error.WriteLine(exn.what());
			}
			catch (...) {
				//bGood=false;
				Console::Error.WriteLine("Could not finish DrawBox for unknown reason.");
			}
		}
		else Console::Error.WriteLine("Camera::DrawBox Error: thisBuffer is nullptr");
	}//end DrawBox
	///#endregion 3D drawing

	void Camera::DrawDebug(GBuffer* thisBuffer) {
		static Mass3d m3dTarget;
		if (m3dTarget.name!="Target") m3dTarget.name="Target";
		static Pixel pixelGray(128,128,128,255);
		static Pixel pixelRed(255,0,0,255);
		m3dTarget.HardLocation(this->xCameraTarget, this->yCameraTarget, this->zCameraTarget);
		DrawBox(thisBuffer, m3dTarget, pixelRed, pixelGray);
	}
	void Camera::GetTarget(Mass3d& m3dReturn) {
		m3dCam.CopyTo(m3dReturn);
		m3dReturn.HardLocation(xCameraTarget,yCameraTarget,zCameraTarget);
	}//end GetTarget

	string Camera::ToString(bool bMultiLine) {
		string sReturn="{";
		try {
			sReturn+=" fScreenW:"+RString_ToString(fScreenW)+"; ";
			if (bMultiLine) sReturn+="\n";
			sReturn+=" fScreenH:"+RString_ToString(fScreenH)+"; ";
			if (bMultiLine) sReturn+="\n";
			sReturn+=" fScreenRatio:"+RString_ToString(fScreenRatio)+"; ";
			if (bMultiLine) sReturn+="\n";
			sReturn+=" fApertureW:"+RString_ToString(fApertureW)+"; ";
			if (bMultiLine) sReturn+="\n";
			sReturn+=" fApertureH:"+RString_ToString(fApertureH)+"; ";
			if (bMultiLine) sReturn+="\n";
			sReturn+=" fAngleApertureTop:"+RString_ToString(fAngleApertureTop)+"; ";
			if (bMultiLine) sReturn+="\n";
			sReturn+=" fAngleApertureLeft:"+RString_ToString(fAngleApertureLeft)+"; ";
			if (bMultiLine) sReturn+="\n";
			sReturn+=" [temp]xCameraTarget:"+RString_ToString(zCameraTarget)+"; ";//temp var
			if (bMultiLine) sReturn+="\n";
			sReturn+=" [temp]yCameraTarget:"+RString_ToString(yCameraTarget)+"; ";//temp var
			if (bMultiLine) sReturn+="\n";
			sReturn+=" [temp]zCameraTarget:"+RString_ToString(zCameraTarget)+"; ";//temp var
			if (bMultiLine) sReturn+="\n";
			//sReturn+=" [temp]cursorRightness:"+RString_ToString(cursorRightness)+"; ";//temp var
			//if (bMultiLine) sReturn+="\n";
			//sReturn+=" [temp]cursorBottomness:"+RString_ToString(cursorBottomness)+"; ";//temp var
			//if (bMultiLine) sReturn+="\n";
			//sReturn+=" [temp]fYaw:"+RString_ToString(fYaw)+"; ";//temp var
			//if (bMultiLine) sReturn+="\n";
			//sReturn+=" [temp]fPitch:"+RString_ToString(fPitch)+"; ";//temp var
			sReturn+="}";
		}
		catch (exception& exn) {
			ShowExn(exn,"camera.ToString()","appending to string");
		}
		return sReturn;
	}//end ToString
	float Camera::get_fScreenW() {
		return fScreenW;
	}
	float Camera::get_fScreenH() {
		return fScreenH;
	}
	float Camera::get_fScreenRatio() {
		return fScreenRatio;
	}
	float Camera::get_fApertureW() {
		return fApertureW;
	}
	float Camera::get_fApertureH() {
		return fApertureH;
	}
	float Camera::get_fAngleApertureTop() {
		return fAngleApertureTop;
	}
	float Camera::get_fAngleApertureLeft() {
		return fAngleApertureLeft;
	}
	float Camera::get_rCameraDist() {
		return rCameraDist;
	}
}//end namespace

#endif
