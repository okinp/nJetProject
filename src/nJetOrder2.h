/*
 *  nJetOrder2.h
 *  kinectInvariants
 *
 *  Created by Nikolas Psaroudakis on 3/4/11.
 *  Copyright 2011 Addictive Labs. All rights reserved.
 *
 */

#pragma once 
#include "cv.h"
#include "gaussianDerivatives.h"
#include "ofImage.h"


class nJetOrder2 {
public:
	nJetOrder2();
	void setParams(int ksize, double sigma, int ktype, int width, int height);
	void calculate(const cv::Mat &input);
	void draw(int x, int y, int deriv);
	cv::Mat Gs,Gsx,Gsy,Gsxx,Gsxy,Gsyy;
private:
	int kernelSize, kernelType;
	double gaussSigma;
	gaussianDerivatives gDerivative;
	cv::Mat K0,K1, K2;
	cv::Ptr<cv::FilterEngine> Fs,Fsx,Fsy, Fsxx, Fsxy, Fsyy;
	ofImage* output;
	cv::Mat outputMat;
};