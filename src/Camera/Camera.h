/*
Camera class bodged together from the following code:
ofxCamera and ofxEasyCam, http://code.google.com/p/ofx3dutils/
Arcball, http://www.julapy.com/blog/2009/07/20/of_iphone_earth/ 
Quaternion, http://aaron-meyers.com/osg-ofx.zip


Add the ofxQuat and ofxMatrix4x4 files to ofxVectorMath 
and add the following includes to ofxVectorMath.h
#include "ofxQuat.h"
#include "ofxMatrix4x4.h"

Add a member variable to testApp.h:
Camera camera;

In draw() put:
camera.place();
To render from the camera's perspective.

Press left mouse button to tumble, middle to track and right to dolly.
Double click to reset the camera to it's initial position.

-------------------------------------
Tobias Jewson, tobiasjewson@gmail.com
-------------------------------------
*/

#ifndef _CAMERA
#define _CAMERA

#include "ofMain.h"
#include "ofxVectorMath.h"

class Camera
{

public:

	Camera();

	void place();

	ofxVec3f getEye();
	ofxVec3f getCenter();

	// converts coordinates to the cameras coordinate system. 
	// positive X -> right, positive Y -> down, positiv Z -> towards center.
	ofxVec3f convertToLocal(ofxVec3f _v);
	ofxVec3f convertToLocal(float _x, float _y, float _z);

private:

	float fieldOfView;
	int	w;
	int	h;
	float aspectRatio;
	float zNear, zFar;

	void reset();
	void setPerspective();
	void setClippingPlane();

	ofxVec3f eye;
	ofxVec3f center;
	ofxVec3f up;

	void tumble(float _px, float _py, float _x, float _y);
	void track(float _px, float _py, float _x, float _y);
	void dolly(float _px, float _py, float _x, float _y);

	ofxVec3f compArcBallVector(float _x, float _y);

	float prevMouseX, prevMouseY;
	bool mouseClicked;
	int millis;

	void mousePressed(ofMouseEventArgs& event);
	void mouseDragged(ofMouseEventArgs& event);
	void mouseReleased(ofMouseEventArgs& event);
	void windowResized(ofResizeEventArgs& event);

};

#endif