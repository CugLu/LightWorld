#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "common/Mat4.h"
#include "common/Vec3.h"

class Camera  
{
public:
	Camera();
	virtual ~Camera();
	void Init() {}
	void Setup3DCamera(int width, int height);
	void Setup2DCamera(float width, float height);
	
	mat4* GetProj();
	mat4* GetView();
	mat4* GetViewProj();
	Vec3 GetPosition();
	
	void Forward(float displacement);
	void Yaw(float angle);
	void Right(float displacement);
	void Rise(float displacement);
	void SetPosition(float x, float y, float z);
	void RotateByAxis(Vec3 axis, float angle);
	void LookAt(float x, float y, float z);
	void RotateByY(float angle);
	void Pitch(float angle);

	void UpdateViewProj();
private:
	mat4 _matView;
	mat4 _matProj;
	Vec3 _at;					
	Vec3 _pos;								
	mat4 _matViewProj;
	mat4 _matBillboard;
};

#endif // !defined(AFX_CAMERA_H__48B7BC27_B223_47B1_ABD4_C17EFD0A7401__INCLUDED_)
