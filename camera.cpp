#ifndef CAMERA_CPP
#define CAMERA_CPP

#include "camera.h"
//#include "dxman.h" //fScreenW etc
#include <base.h>
#include <RMath.h>

using namespace std;

namespace ExpertMultimediaBase {
	//double Camera::DBL_HITDETECTION2D_CONSTANT_Z=1.0d;
	Camera::Camera() {
		InitNull();
		SetCamera(640,480,0.0f,-14.836f,5.376f, 0.0f,-11.77,90.0f, 39.2025f);
	//	Console::Error.WriteLine("CODING ERROR: CAMERA DEFAULT CONSTRUCTOR MUST NOT BE USED--needs screen dimensions");
	}
	//Camera::Camera(float UseScreenW, float UseScreenH) {
	//	fScreenRatio=UseScreenW/UseScreenW; //i.e. 4/3 ratio
	//	SetCamera(UseScreenW,UseScreenH,0.0f,-14.836f,5.376f, 0.0f,-11.77,90.0f, 39.2025f);
		//i.e.  52.27f screen aperture width if 4:3 and 39.2025f height
	//}
	void Camera::InitNull() {
		rCameraDist=200.0f;
		xCameraTarget=0.0d;
		yCameraTarget=0.0d;
		zCameraTarget=0.0d;
		fRatioFromLeft=0.0f;
		fRatioFromTop=0.0f;
		fYaw=0.0f;
		fPitch=0.0f;
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
			ShowExn(exn,"Point2dFrom3d");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Point2dFrom3d");
		}
		return bGood;
	}
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
			float fWidthScaled=(float)(IROUNDF((float)m2dWithPixPerMeter_ToModify.rectOriginal.right*m2dWithPixPerMeter_ToModify.fScale))+1;//debug only: +1 shouldn't be needed
			float fHeightScaled=(float)(IROUNDF((float)m2dWithPixPerMeter_ToModify.rectOriginal.bottom*m2dWithPixPerMeter_ToModify.fScale))+1;//debug only: +1 shouldn't be needed
			//float fHalfWidthScaled=FFLOOR(fWidthScaled/2.0f);
			//float fHalfHeightScaled=FFLOOR(fHeightScaled/2.0f);
			m2dWithPixPerMeter_ToModify.rectRender.left=IROUNDF(xTranslate-m2dWithPixPerMeter_ToModify.FCenterXRelScaled());
			m2dWithPixPerMeter_ToModify.rectRender.top=IROUNDF(yTranslate-m2dWithPixPerMeter_ToModify.FCenterYRelScaled());
			m2dWithPixPerMeter_ToModify.rectRender.right=m2dWithPixPerMeter_ToModify.rectRender.left+IROUNDF(fWidthScaled);
			m2dWithPixPerMeter_ToModify.rectRender.bottom=m2dWithPixPerMeter_ToModify.rectRender.top+IROUNDF(fHeightScaled);

			Point2dMultipliersFrom3d(xRightness,yDownness,m3dEnt.X,m3dEnt.Y,0);
			m2dReturnShadow.rectRender.left=m2dWithPixPerMeter_ToModify.rectRender.left;//assumes top-down lighting
			m2dReturnShadow.rectRender.right=m2dReturnShadow.rectRender.left+(int)(m2dReturnShadow.fScale*(float)m2dReturnShadow.rectOriginal.right+.5f);//m2dWithPixPerMeter_ToModify.rectRender.right;
			m2dReturnShadow.rectRender.top=IROUNDF(yDownness*fScreenH-(float)m2dReturnShadow.rectOriginal.top/2.0f*m2dReturnShadow.fScale);
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
		float fReturn=fScreenW / FPDIST( FXOFRTHETA_DEG(fDistanceFromCamera,0),
										FYOFRTHETA_DEG(fDistanceFromCamera,0),
										FXOFRTHETA_DEG(fDistanceFromCamera,fApertureW),
										FYOFRTHETA_DEG(fDistanceFromCamera,fApertureW) );
		if (fReturn<0.0f) fReturn=0.0f;
		return fReturn;
	}
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
			fAzimuth=RMath::SafeAngle360( FTHETAOFXY_DEG(pointRel.X,pointRel.Y) );
			pointTempForAltitude.X=pointRel.X;
			pointTempForAltitude.Y=pointRel.Y;
			pointTempForAltitude.Z=pointRel.Z;
			RMath::Rotate(pointTempForAltitude.X,pointTempForAltitude.Y,-fAzimuth);
			fAltitude=RMath::SafeAngle360( FTHETAOFXY_DEG(pointTempForAltitude.X,pointTempForAltitude.Z) );
		//xRelativeToScreenPoint, yRelativeToScreenPoint, zDistanceFromCameraInVirtualPixels, xScreenPoint, yScreenPoint

			//CONVERTS TO METERS:
			fDistance/=fDestPixPerUnitHere;
			pointToChange.yRot=fAltitude;
			pointToChange.zRot=fAzimuth;
			Travel3d(pointToChange, pointToChange.yRot, pointToChange.zRot, fDistance);

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
			//Travel3d(pointToChange, fTestAlt, fTestAz, fDistance);

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
	bool Camera::Point3dFrom2d(Mass3d &pointReturn, float x2D, float y2D, float fMetersFromCamera) {
		bool bGood=true;
		static float fRatioFromLeft,
				fRatioFromTop,
				fYaw,
				fPitch;
		try {
			fRatioFromLeft=x2D/fScreenW;
			fRatioFromTop=y2D/fScreenH;
			fYaw=fAngleApertureLeft-fRatioFromLeft*fApertureW;
			fPitch=fAngleApertureTop-fRatioFromTop*fApertureH;
			pointReturn.X=m3dCam.X;
			pointReturn.Y=m3dCam.Y;
			pointReturn.Z=m3dCam.Z;
			Travel3d(pointReturn,fPitch,fYaw,fMetersFromCamera);
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
	string Camera::ToString(bool bMultiLine) {
		string sReturn="{";
		try {
			//sReturn+=" fScreenW:"+RString_ToString(fScreenW)+"; ";
			//if (bMultiLine) sReturn+="\n";
			//sReturn+=" fScreenH:"+RString_ToString(fScreenH)+"; ";
			//if (bMultiLine) sReturn+="\n";
			//sReturn+=" fScreenRatio:"+RString_ToString(fScreenRatio)+"; ";
			//if (bMultiLine) sReturn+="\n";
			//sReturn+=" fApertureW:"+RString_ToString(fApertureW)+"; ";
			//if (bMultiLine) sReturn+="\n";
			//sReturn+=" fApertureH:"+RString_ToString(fApertureH)+"; ";
			//if (bMultiLine) sReturn+="\n";
			//sReturn+=" fAngleApertureTop:"+RString_ToString(fAngleApertureTop)+"; ";
			//if (bMultiLine) sReturn+="\n";
			//sReturn+=" fAngleApertureLeft:"+RString_ToString(fAngleApertureLeft)+"; ";
			//if (bMultiLine) sReturn+="\n";
			sReturn+=" xCameraTarget:"+RString_ToString(zCameraTarget)+"; ";//temp var
			if (bMultiLine) sReturn+="\n";
			sReturn+=" yCameraTarget:"+RString_ToString(yCameraTarget)+"; ";//temp var
			if (bMultiLine) sReturn+="\n";
			sReturn+=" zCameraTarget:"+RString_ToString(zCameraTarget)+"; ";//temp var
			if (bMultiLine) sReturn+="\n";
			sReturn+=" fRatioFromLeft:"+RString_ToString(fRatioFromLeft)+"; ";//temp var
			if (bMultiLine) sReturn+="\n";
			sReturn+=" fRatioFromTop:"+RString_ToString(fRatioFromTop)+"; ";//temp var
			if (bMultiLine) sReturn+="\n";
			sReturn+=" fYaw:"+RString_ToString(fYaw)+"; ";//temp var
			if (bMultiLine) sReturn+="\n";
			sReturn+=" fPitch:"+RString_ToString(fPitch)+"; ";//temp var
			sReturn+="}";
		}
		catch (exception& exn) {
			ShowExn(exn,"camera.ToString()","appending to string");
		}
		return sReturn;
	}
	///<summary>
	///Assumes height to get a location on a plane using a 2D point on the screen.
	///ALSO assumes plane x,y rotation is 0,0.
	///fMaximumPitch should be about -5.0 (degrees) so that the point isn't super far away (0.0 or above will never intersect plane & would be impossible to calculate)
	///returns false if couldn't find a 3D point (e.g. cursor doesn't intersect the plane from this camera's point of view)
	///</summary>
	bool Camera::Point3dFrom2dAssumingHeight(Mass3d& pointReturn,int& iLineRelationshipType, float x2D,float y2D,float z3D, float fMaximumPitch) {
		bool bGood=true;
		try {
			if (fMaximumPitch>0.0) fMaximumPitch=0.0;
			fRatioFromLeft=x2D/fScreenW;
			fRatioFromTop=y2D/fScreenH;
			fYaw=fAngleApertureLeft-fRatioFromLeft*fApertureW;
			fPitch=fAngleApertureTop-fRatioFromTop*fApertureH;
			if (fPitch>fMaximumPitch) {
				//ANSWER: where does pitch intersect a plane whose z==z3D
				double xInter=0.0d, yInter=0.0d,zInter=0.0d;//the intersection is on the yz plane, and is ALWAYS z==height of plane since plane is flat
				//iLastLineRelationshipType=RMath::IntersectionAndRelationship(yInter, zInter, Line1_y1, Line1_z1, Line1_y2, Line1_z2, Line2_y1, Line2_z1, Line2_y2, Line2_z2);
				xCameraTarget=m3dCam.X;
				yCameraTarget=m3dCam.Y;
				zCameraTarget=m3dCam.Z;
				Travel3d(xCameraTarget,yCameraTarget,zCameraTarget,(double)fPitch,(double)fYaw,(double)rCameraDist);
				iLineRelationshipType=RMath::IntersectionAndRelationship(yInter, zInter, -100.0d, z3D, 100.0d, z3D, yCameraTarget, zCameraTarget, (double)m3dCam.Y, (double)m3dCam.Z);
				if ( (iLineRelationshipType==RMath::LineRelationshipIntersectionNotTouchingEndsOfLineB) || (iLineRelationshipType>=RMath::LineRelationshipThisOrHigherNotCrossingInRange) ) {
					//double fHypotenuse=yInter
					//xInter=fRatioFromLeft*ThirdAngle //I know the distance and angle but need hypoteneus
					xCameraTarget=m3dCam.X;
					yCameraTarget=m3dCam.Y;
					zCameraTarget=z3D;
					Travel3d(xCameraTarget,yCameraTarget,zCameraTarget,(double)m3dCam.yRot,(double)m3dCam.zRot,(double)rCameraDist);
					double xInterUsingYaw=xInter, yInterUsingYaw=yInter;
					int iIntersection2Type=RMath::IntersectionAndRelationship(xInterUsingYaw, yInterUsingYaw, -100.0d, yInter, 100.0d, yInter, (double)m3dCam.X, (double)m3dCam.Y, xCameraTarget, yCameraTarget);
					pointReturn.X=xInterUsingYaw;
					pointReturn.Y=yInter;
					pointReturn.Z=z3D;
				}
				//float fMetersFromCameraOnXYPlane;
				//Travel3d(pointReturn,fPitch,fYaw,fMetersFromCameraOnXYPlane);
			}
			else {
				bGood=false;//out of range (does not intersect plane)
			}
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
	}
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
			fAngleFromTop=(fAngleApertureTop-(FTHETAOFXY_DEG(yRel,zRel)));
			fAngleFromLeft=(fAngleApertureLeft-(FTHETAOFXY_DEG(xRel,yRel)));
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
			fAngleFromTop=(fAngleApertureTop-(FTHETAOFXY_DEG(yRel,zRel)));
			if (fAngleFromTop<0.0f) return false;
			else if (fAngleFromTop>=fApertureH) return false;
			fAngleFromLeft=(fAngleApertureLeft-(FTHETAOFXY_DEG(xRel,yRel)));
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
	void Camera::SetCamera(float UseScreenW, float UseScreenH, float xTo, float yTo, float zTo, float xRotTo, float yRotTo, float zRotTo, float fApertureAngleHeight) {
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
	}
	void Camera::GetTarget(Mass3d& m3dReturn) {
		m3dCam.CopyTo(m3dReturn);
		m3dReturn.HardLocation(xCameraTarget,yCameraTarget,zCameraTarget);
	}
}//end namespace

#endif
