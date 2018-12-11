#ifndef CAMERA_H
#define CAMERA_H

#include <base.h>
#include <RImage_bgra32.h>
// for cpp:
#include <base.h>
#include <RMath.h>
// #define _USE_MATH_DEFINES  // needed by math.h in order for it to define M_PI on some older platforms
#include <math.h>  // M_PI etc

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
		//float fRatioFromLeft;
		//float fRatioFromTop;
		//float fYaw;
		//float fPitch;
///#endregion temp vars
	public:
///#region public temp vars
		int LastSideLineRelationshipTypeNumber=0;
		int LastTopLineRelationshipTypeNumber=0;
///#endregion public temp vars
		//static double HITDETECTION2D_CONSTANT_Z_DBL;//=1.0f; //meters for new 2011 non-changing height
		Mass3d m3dCam;
		bool make_orientation_Z_UP;
		bool make_orientation_X_FORWARD;
		Camera();
		Camera(float UseScreenW, float UseScreenH, float x1, float y1, float z1, float xRot1, float yRot1, float zRot1, float fAperture, bool set_make_orientation_Z_UP, bool set_make_orientation_X_FORWARD);
		void InitNull();
		void SetCamera(float UseScreenW, float UseScreenH, float x1, float y1, float z1, float xRot1, float yRot1, float zRot1, float fAperture, bool set_make_orientation_Z_UP, bool set_make_orientation_X_FORWARD);
		float DestPixelsPerMeterAt(float fDistanceFromCamera);
		bool Point2dMultipliersFrom3d(float &xReturn2d, float &yReturn2d, float x3d, float y3d, float z3d);
		bool Point2dFrom3d(FPoint &pointReturn, Mass3d &point3d);
		bool PointIsInView(float x3d, float y3d, float z3d);
		bool Point2dFrom3d(float& xReturn, float& yReturn, float x, float y, float z);
		bool Point2dFrom3dWithScreenOffset(FPoint &pointReturn, Mass3d &point3d, float xOffset, float yOffset);
		bool Point2dFrom3dWithScreenOffset(float& xReturn, float& yReturn, float x, float y, float z, float xOffset, float yOffset);
		bool Point3dMoveAndRotateBy2d(Mass3d& pointToChange,float xRelativeToScreenPoint,float yRelativeToScreenPoint,float zDistanceFromCameraInVirtualPixels, float xScreenPoint, float yScreenPoint);
		bool Point3dFrom2d(Mass3d &pointReturn,float x2D,float y2D,float fMetersFromCamera);
		bool Point3dFrom2dAssumingHeight(Mass3d& pointReturn,int& iIntersectionType, float x2D,float y2D,float z3D, float maximumPitch);
		bool Mass2dFrom3d(Mass2d &m2dWithPixPerMeter_ToModify, Mass2d &m2dShadow, Mass3d &m3dEnt);

		///#region 3d drawing
		void DrawAxes(GBuffer* thisBuffer, Mass3d& m3dAxisCenter);
		void Draw3DLine(GBuffer* thisBuffer, float startVector3[], float endVector3[], byte r, byte g, byte b, byte a, byte a_FadeFarthestPoint_To);
		void Draw3DLine(GBuffer* thisBuffer, float x1, float y1, float z1, float x2, float y2, float z2, byte r, byte g, byte b, byte a, byte a_FadeFarthestPoint_To);
		void Draw3DLine(GBuffer* thisBuffer, Mass3d& m3dStart, Mass3d& m3dEnd, byte r, byte g, byte b, byte a, byte a_FadeFarthestPoint_To);
		void Draw3dRegularPolygon(GBuffer* thisBuffer, Mass3d& m3dCenter, char Axis, float StartAngle_Deg, float Radius, int Sides, Pixel color);
		void DrawBox(GBuffer* thisBuffer, Mass3d &m3dNow, Pixel &pixelNear, Pixel &pixelFar);
		///#endregion 3d drawing
		void DrawDebug(GBuffer* thisBuffer);
		void GetTarget(Mass3d& m3dReturn);
		string ToString(bool bMultiLine);
		float get_fScreenW();
		float get_fScreenH();
		float get_fScreenRatio();
		float get_fApertureW();
		float get_fApertureH();
		float get_fAngleApertureTop();
		float get_fAngleApertureLeft();
		float get_rCameraDist();
	};
}//end namespace

#endif
