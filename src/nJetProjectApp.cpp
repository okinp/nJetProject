//
//  nJetProjectApp.cpp
//  nJetProject
//
//  Created by Nikolas Psaroudakis on 3/4/11.
//  Copyright Addictive Labs 2011. All rights reserved.
//

#include "nJetProjectApp.h"

//--------------------------------------------------------------
void nJetProjectApp::setup() {
	inputImage.loadImage("Lenna.jpg");
	inputImage.setImageType(OF_IMAGE_GRAYSCALE);
	inputImage.update();
	pixels = inputImage.getPixels();
	inputMat = cv::Mat(inputImage.width, inputImage.height, CV_8UC1, pixels);
	processMat = cv::Mat(inputImage.width, inputImage.height, CV_32FC1);
	
	inputMat.convertTo(processMat, CV_32F, 1/255.,0);
	imageJet.setParams(10, 0.1, CV_32F, inputImage.width, inputImage.height);
}

//--------------------------------------------------------------
void nJetProjectApp::update() {
	
	imageJet.calculate(processMat);
}

//--------------------------------------------------------------

void nJetProjectApp::displayNormals(){
	glColor3f(0., 0., 1.);
	for (int i=150; i<250; i++) {
		for (int j=150; j<250; j++) {
			v0 = ofxVec3f(i,j,0);
			float f1 = imageJet.Gsx.at<float>(j,i);
			float f2 = imageJet.Gsy.at<float>(j,i);
			v1 = ofxVec3f(f1, f2, -1);
			v1.normalize();
			v1*=5;
			v1 = v0+v1;
			glBegin(GL_LINES);
			glVertex3f(v0.x, v0.y, 0);
			glVertex3f(v1.x, v1.y, v1.z);
			glEnd();
		}
	}
	glColor3f(1., 1., 1.);
}
void nJetProjectApp::draw() {
	cam.place();
	//inputImage.draw(0,0);
	imageJet.draw(0, 0, 0);
	imageJet.draw(inputImage.width, 0, 3);
	// Print framerate on screen
	char fpsStr[255]; // an array of char
	sprintf(fpsStr, "frame rate: %f", ofGetFrameRate());
	//ofSetColor(0, 0, 0);
	ofDrawBitmapString(fpsStr, 40,450);
	displayNormals();

}

//--------------------------------------------------------------
void nJetProjectApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void nJetProjectApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void nJetProjectApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void nJetProjectApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void nJetProjectApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void nJetProjectApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void nJetProjectApp::windowResized(int w, int h) {

}

