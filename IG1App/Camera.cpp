#include "Shader.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

using namespace glm;

Camera::Camera(Viewport* vp)
	: mViewMat(1.0)
	, mProjMat(1.0)
	, xRight(vp->width() / 2.0)
	, xLeft(-xRight)
	, yTop(vp->height() / 2.0)
	, yBot(-yTop)
	, mViewPort(vp)
	, mRadio(1000)
	, mAng(-45)
{
	setPM();
}

void
Camera::uploadVM() const
{
	Shader::setUniform4All("modelView", mViewMat);
	glm::mat4 matrix = mViewMat * glm::dmat4({ -1, 0, 0, 0 }, { 0, -1, 0, 0 }, { 0, 0, -1, 0 }, { 0, 0, 0, 1 });
	Shader::setUniform4All("LightDir", matrix);
}

void
Camera::setVM()
{
	mViewMat = lookAt(mEye, mLook, mUp); // glm::lookAt defines the view matrix
	setAxes();
}

void
Camera::set2D()
{
	mEye = {0, 0, 500};
	mLook = {0, 0, 0};
	mUp = {0, 1, 0};
	mRadio = 1000;
	mAng = -45;
	setVM();
}

void
Camera::set3D()
{
	mEye = {500, 500, 500};
	mLook = {0, 10, 0};
	mUp = {0, 1, 0};
	mRadio = 1000;
	mAng = -45;
	setVM();
}

void
Camera::pitch(GLdouble a)
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::dvec3(1.0, 0, 0));
	// glm::rotate returns mViewMat * rotationMatrix
	setAxes();
}

void
Camera::yaw(GLdouble a)
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::dvec3(0, 1.0, 0));
	// glm::rotate returns mViewMat * rotationMatrix
	setAxes();
}

void
Camera::roll(GLdouble a)
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::dvec3(0, 0, 1.0));
	// glm::rotate returns mViewMat * rotationMatrix
	setAxes();
}

void
Camera::setSize(GLdouble xw, GLdouble yh)
{
	xRight = xw / 2.0;
	xLeft = -xRight;
	yTop = yh / 2.0;
	yBot = -yTop;
	setPM();
}

void
Camera::setScale(GLdouble s)
{
	mScaleFact -= s;
	if (mScaleFact < 0)
		mScaleFact = 0.01;
	setPM();
}

void
Camera::setPM()
{
	if (bOrto) { //  if orthogonal projection
		mProjMat = ortho(xLeft * mScaleFact,
		                 xRight * mScaleFact,
		                 yBot * mScaleFact,
		                 yTop * mScaleFact,
		                 mNearVal,
		                 mFarVal);
		// glm::ortho defines the orthogonal projection matrix
	}
	else {
		mProjMat = frustum(xLeft * mScaleFact,
						   xRight * mScaleFact,
						   yBot * mScaleFact,
						   yTop * mScaleFact,
						   mNearVal+250,
						   mFarVal);
	}
}

void
Camera::uploadPM() const
{
	Shader::setUniform4All("projection", mProjMat);
}

void
Camera::upload() const
{
	mViewPort->upload();
	uploadVM();
	uploadPM();
}

void 
Camera::setAxes() {
	mRight = row(mViewMat, 0);
	mUpward = row(mViewMat, 1);
	mFront = -row(mViewMat, 2);
}

void 
Camera::moveLR(GLdouble cs) {
	mEye += mRight * cs;
	mLook += mRight * cs;
	setVM();
}

void
Camera::moveUD(GLdouble cs) {
	mEye += mUpward * cs;
	mLook += mUpward * cs;
	setVM();
}

void
Camera::moveFB(GLdouble cs) {
	mEye += mFront * cs;
	mLook += mFront * cs;
	setVM();
}

void 
Camera::pitchReal(GLdouble cs) {
	mLook = mEye + rotate(mLook - mEye, glm::radians(cs), mRight);
	mUp = rotate(mUp, radians(cs), mRight);
	setVM();
}

void 
Camera::yawReal(GLdouble cs) {
	mLook += mRight * cs * 10.0;
	setVM();
}

void 
Camera::rollReal(GLdouble cs) {
	mLook += mFront * cs;
	mUp += mRight * cs;
	setVM();
}

void 
Camera::orbit(GLdouble incAng, GLdouble incY) {
	mAng += incAng;
	mEye.x = mLook.x + cos(radians(mAng)) * mRadio;
	mEye.z = mLook.z - sin(radians(mAng)) * mRadio;
	mEye.y += incY;
	setVM();
}

void
Camera::setCenital() {
	mEye = { 0, 500, 0 };
	mLook = { 0, 0, 0 };
	mUp = { 0, 0, 1 };
	mRadio = 1000;
	mAng = -45;
	setVM();
}