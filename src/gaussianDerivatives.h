/*
 *  gaussianDerivatives.h
 *  kinectInvariants
 *
 *  Created by Nikolas Psaroudakis on 3/4/11.
 *  Copyright 2011 Addictive Labs. All rights reserved.
 *
 */



#pragma once
#include "cv.h"

class gaussianDerivatives {
public:
	gaussianDerivatives();
	gaussianDerivatives(int ksize, double sigma, int ktype);
	cv::Mat getDerivativeKernel(int order);
	cv::Mat gaussianKernel;
private:
	double sigmaX;
	double scale2X;
	int kernelSize;
	int kernelType;
};