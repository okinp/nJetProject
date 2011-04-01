//
//  nJetProjectApp.h
//  nJetProject
//
//  Created by Nikolas Psaroudakis on 3/4/11.
//  Copyright Addictive Labs 2011. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "nJetOrder2.h"
#include "cv.h"
#include "ofxVec3f.h"
#include "Camera.h"

//========================================================================
class nJetProjectApp : public ofBaseApp {
	
public:
    void setup();
	void update();
    void draw();
	
    void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void displayNormals();
	ofImage inputImage;
	ofImage outputImage;
	nJetOrder2 imageJet;
	unsigned char * pixels;
	cv::Mat inputMat, processMat;
	ofxVec3f v0, v1;
	Camera cam;
};
