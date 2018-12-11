#ifndef CAMERA_H
#define CAMERA_H

#include <base.h>

using namespace std;

namespace ExpertMultimediaBase {
	class Camera {
	private:
		float fScreenW;
		float fScreenH;
		float fScreenRatio;
		float fApertureW;
		float fApertureH;
		float fAngleApertureTop;
		float fAngleApertureLeft;
		float rCameraDist;
///#region temp vars
		double xCameraTarget;
		double yCameraTarget;
		double zCameraTarget;
		float fRatioFromLeft;
		float fRatioFromTop;
		float fYaw;
		float fPitch;
///#endregion temp vars
	public:
		//static double DBL_HITDETECTION2D_CONSTANT_Z;//=1.0f; //meters for new 2011 non-changing height
		Mass3d m3dCam;
		Camera();
		void InitNull();
		void SetCamera(float UseScreenW, float UseScreenH, float x1, float y1, float z1, float xRot1, float yRot1, float zRot1, float fAperture);
		bool Point2dMultipliersFrom3d(float &xReturn2d, float &yReturn2d, float x3d, float y3d, float z3d);
		float DestPixelsPerMeterAt(float fDistanceFromCamera);
		bool Point2dFrom3d(FPoint &pointReturn, Mass3d &point3d);
		bool PointIsInView(float x3d, float y3d, float z3d);
		bool Point2dFrom3dWithScreenOffset(FPoint &pointReturn, Mass3d &point3d, float xOffset, float yOffset);
		bool Point3dMoveAndRotateBy2d(Mass3d& pointToChange,float xRelativeToScreenPoint,float yRelativeToScreenPoint,float zDistanceFromCameraInVirtualPixels, float xScreenPoint, float yScreenPoint);
		bool Point3dFrom2d(Mass3d &pointReturn,float x2D,float y2D,float fMetersFromCamera);
		bool Point3dFrom2dAssumingHeight(Mass3d& pointReturn,int& iIntersectionType, float x2D,float y2D,float z3D, float fMaximumPitch);
		bool Mass2dFrom3d(Mass2d &m2dWithPixPerMeter_ToModify, Mass2d &m2dShadow, Mass3d &m3dEnt);
		void GetTarget(Mass3d& m3dReturn);
		string ToString(bool bMultiLine);
	};
}//end namespace

#endif
