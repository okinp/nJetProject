#include "Camera.h"

Camera::Camera()
{
	reset();

	mouseClicked = false;

	//Setup listeners
	ofAddListener(ofEvents.mousePressed, this, &Camera::mousePressed);
	ofAddListener(ofEvents.mouseDragged, this, &Camera::mouseDragged);
	ofAddListener(ofEvents.mouseReleased, this, &Camera::mouseReleased);
	ofAddListener(ofEvents.windowResized, this, &Camera::windowResized);
}

void Camera::reset()
{
	setPerspective();

	eye.x = (float)w / 2.0f;
	eye.y = (float)h / 2.0f;
	float halfFov = PI * fieldOfView / 360.0f;
	float theTan = tanf(halfFov);
	eye.z = -eye.y / theTan;

	center.x = (float)w / 2.0f;
	center.y = (float)h / 2.0f;
	center.z = 0;

	up.x = 0;
	up.y = -1;
	up.z = 0;

	setClippingPlane();
}

void Camera::setPerspective()
{
	fieldOfView = 65.0f;
	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);
	aspectRatio = (float)w/(float)h;
}

void Camera::setClippingPlane()
{
	zNear = 1.0f;
	ofxVec3f dist = center - eye;
	zFar = dist.length() * 2;
}

void Camera::place()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fieldOfView, aspectRatio, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
}

ofxVec3f Camera::getEye()
{
	return eye;
}

ofxVec3f Camera::getCenter()
{
	return center;
}

ofxVec3f Camera::convertToLocal(ofxVec3f _v)
{
	return convertToLocal(_v.x, _v.y, _v.z);
}

ofxVec3f Camera::convertToLocal(float _x, float _y, float _z)
{
	ofxVec3f localZ = center - eye;
	localZ.normalize();
	ofxVec3f localX = localZ.getCrossed(up).normalize();
	ofxVec3f localY = localZ.getCrossed(localX).normalize();

	ofxVec3f v = (localX * _x) + (localY * _y) + (localZ * _z);
	return v;
}

void Camera::tumble(float _px, float _py, float _x, float _y)
{
	float tolerance = 0.01;

	if (fabs(_x - _px) < tolerance && fabs(_y - _py) < tolerance)
		return; // Not enough change to have an action.

	/* 
	The arcball works by pretending that a ball encloses the 3D view.  
	You roll this pretend ball with the mouse.  For example, if you click on 
	the center of the ball and move the mouse straight to the right, you 
	roll the ball around its Y-axis.  This produces a Y-axis rotation.  
	You can click on the "edge" of the ball and roll it around in a circle 
	to get a Z-axis rotation.

	The math behind the trackball is simple: start with a vector from the first
	mouse-click on the ball to the center of the 3D view.  At the same time, set 
	the radius of the ball to be the smaller dimension of the 3D view.  
	As you drag the mouse around in the 3D view, a second vector is computed from 
	the surface of the ball to the center.  The axis of rotation is the cross 
	product of these two vectors, and the angle of rotation is the angle between 
	the two vectors.
	*/

	ofxVec3f arcBallStart = compArcBallVector(_px, _py);
	ofxVec3f arcBallEnd = compArcBallVector(_x, _y);

	// Convert from screen position to world position
	arcBallStart = convertToLocal(arcBallStart);
	arcBallEnd = convertToLocal(arcBallEnd);

	//-------------------------------------------
	// Figure the axis and angle of rotation between the start and end point.

	float cosAng, sinAng, angle;
	float ls, le;

	// Take the cross product of the two vectors. r = s X e
	ofxVec3f axis = arcBallStart.getCrossed(arcBallEnd);

	// Use atan for a better angle.  If you use only cos or sin, you only get
	// half the possible angles, and you can end up with rotations that flip 
	// around near the poles.

	// cos(a) = (s . e) / (||s|| ||e||)
	cosAng = arcBallStart.dot(arcBallEnd); // (s . e)
	ls = arcBallStart.length();
	ls = 1.0 / ls; // 1 / ||s||
	le = arcBallEnd.length();
	le = 1.0 / le; // 1 / ||e||
	cosAng = cosAng * ls * le;

	// sin(a) = ||(s X e)|| / (||s|| ||e||)
	sinAng = axis.length(); // ||(s X e)||
	sinAng = sinAng * ls * le;

	angle = atan2f(sinAng, cosAng);

	// Normalize the rotation axis.
	axis.normalize();

	//-------------------------------------------
	// Apply the rotation to the eye.

	ofxQuaternion rotation, point, rotated;

	ofxVec3f p = eye - center; // eye moved to rotate around 0, 0, 0.
	point.set(p.x, p.y, p.z, 0);

	rotation.makeRotate(angle, axis);

	rotated = (rotation*point)*rotation.conj();

	eye = center + rotated.asVec3();


	// Apply the rotation to the up vector.

	point.set(up.x, up.y, up.z, 0);

	rotated = (rotation*point)*rotation.conj();

	up = rotated.asVec3();

}

ofxVec3f Camera::compArcBallVector(float _x, float _y)
{
	ofxVec3f v;

	float arcBallRadi;
	float arcBallX, arcBallY;
	float xxyy;

	// Make the radius of the arcball slightly smaller than the smalest dimension of the screen.
	if (w > h)
		arcBallRadi = (h * .95) * 0.5;
	else
		arcBallRadi = (w * .95) * 0.5;

	// Figure the center of the view.
	arcBallX = w * 0.5;
	arcBallY = h * 0.5;

	// Compute the vector from the surface of the ball to its center.
	v.x = arcBallX - _x;
	v.y = arcBallY - _y;
	xxyy = v.x * v.x + v.y * v.y;
	if (xxyy > arcBallRadi * arcBallRadi) {
		// Outside the sphere.
		v.z = 0.0;
	} else
		v.z = sqrt(arcBallRadi * arcBallRadi - xxyy);

	return v;
}

void Camera::track(float _px, float _py, float _x, float _y)
{
	float dx = _px - _x;
	float dy = _py - _y;

	ofxVec3f offset = convertToLocal(dx, dy, 0);
	eye += offset;
	center += offset;
}

void Camera::dolly(float _px, float _py, float _x, float _y)
{
	float dx = _px - _x;
	float dy = _py - _y;

	ofxVec3f offset = convertToLocal(0, 0, -dx - dy);

	eye += offset;
}

void Camera::mousePressed(ofMouseEventArgs& event) 
{
	if(mouseClicked && event.button == 0)
	{
		if(ofGetElapsedTimeMillis() - millis < 300)
			reset();

		mouseClicked = false;;
	}
	else if(event.button == 0)
	{
		mouseClicked = true;
		millis = ofGetElapsedTimeMillis();
	}

	prevMouseX = event.x;
	prevMouseY = event.y;
}

void Camera::mouseDragged(ofMouseEventArgs& event) 
{
	if(event.button == 0)
	{
		tumble(prevMouseX, prevMouseY, event.x, event.y);
	}
	else if(event.button == 1)
	{
		track(prevMouseX, prevMouseY, event.x, event.y);
	}
	else if(event.button == 2)
	{
		dolly(prevMouseX, prevMouseY, event.x, event.y);
		setClippingPlane();
	}

	prevMouseX = event.x;
	prevMouseY = event.y;

	mouseClicked = false;;

}

void Camera::mouseReleased(ofMouseEventArgs& event) 
{

}

void Camera::windowResized(ofResizeEventArgs& event)
{
	setPerspective();
}