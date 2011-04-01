/*
 *  nJetOrder2.cpp
 *  kinectInvariants
 *
 *  Created by Nikolas Psaroudakis on 3/4/11.
 *  Copyright 2011 Addictive Labs. All rights reserved.
 *
 */

#include "nJetOrder2.h"

nJetOrder2::nJetOrder2(){
	output = NULL;
	
}
void nJetOrder2::setParams(int ksize, double sigma, int ktype, int width, int height){
	kernelSize = ksize;
	kernelType = ktype;
	gaussSigma = sigma;
	gDerivative = gaussianDerivatives(kernelSize, gaussSigma, kernelType);
	K0 = gDerivative.getDerivativeKernel(0);
	K1 = gDerivative.getDerivativeKernel(1);
	K2 = gDerivative.getDerivativeKernel(2);
	Fs = createSeparableLinearFilter( ktype, ktype, K0, K0);
	Fsx = createSeparableLinearFilter( ktype, ktype, K1, K0);
	Fsy = createSeparableLinearFilter( ktype, ktype, K0, K1);
	Fsxx = createSeparableLinearFilter( ktype, ktype, K2, K0);
	Fsxy = createSeparableLinearFilter( ktype, ktype, K1, K1);
	Fsyy = createSeparableLinearFilter( ktype, ktype, K0, K2);
	//
	outputMat = cv::Mat(height, width, CV_8UC1);
	Gs = cv::Mat(height, width, CV_32FC1);
	Gsx = cv::Mat(height, width, CV_32FC1);
	Gsy = cv::Mat(height, width, CV_32FC1);
	Gsxx = cv::Mat(height, width, CV_32FC1);
	Gsxy = cv::Mat(height, width, CV_32FC1);
	Gsyy = cv::Mat(height, width, CV_32FC1);
	
}
void nJetOrder2::calculate(const cv::Mat &input){
	if (output == NULL) {
		output = new ofImage();
		cout << "Cols are: " << input.cols << " and rows are: " << input.rows << endl;
		output->allocate(input.cols, input.rows, OF_IMAGE_GRAYSCALE);
	}
	Fs->apply(input, Gs);
	Fsx->apply(input, Gsx);
	Fsy->apply(input, Gsy);
	Fsxx->apply(input, Gsxx);
	Fsxy->apply(input, Gsxy);
	Fsyy->apply(input, Gsyy);
	
}
void nJetOrder2::draw(int x, int y, int deriv){
	double minVal, maxVal;
	if(deriv==1) {
		minMaxLoc(Gsx, &minVal, &maxVal);
		Gsx.convertTo(outputMat, CV_8U, 255./(maxVal-minVal), -minVal*255./(maxVal-minVal));
	} else if(deriv==2) {
		minMaxLoc(Gsy, &minVal, &maxVal);
		Gsy.convertTo(outputMat, CV_8U, 255./(maxVal-minVal), -minVal*255./(maxVal-minVal));
	} else if(deriv==3) {
		minMaxLoc(Gsxx, &minVal, &maxVal);
		Gsxx.convertTo(outputMat, CV_8U, 255./(maxVal-minVal), -minVal*255./(maxVal-minVal));
	} else if(deriv==4) {
		minMaxLoc(Gsxy, &minVal, &maxVal);
		Gsxy.convertTo(outputMat, CV_8U, 255./(maxVal-minVal), -minVal*255./(maxVal-minVal));
	} else if(deriv==5) {
		minMaxLoc(Gsxy, &minVal, &maxVal);
		Gsyy.convertTo(outputMat, CV_8U, 255./(maxVal-minVal), -minVal*255./(maxVal-minVal));
	} else {
		//Gausian blurring
		minMaxLoc(Gs, &minVal, &maxVal);
		Gs.convertTo(outputMat, CV_8U, 255./(maxVal-minVal), -minVal*255./(maxVal-minVal));
	}
	output->setFromPixels((unsigned char*) outputMat.data, output->width, output->height, OF_IMAGE_GRAYSCALE);
	output->update();
	output->draw(x,y);
}
