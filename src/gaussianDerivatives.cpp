/*
 *  gaussianDerivatives.cpp
 *  kinectInvariants
 *
 *  Created by Nikolas Psaroudakis on 3/4/11.
 *  Copyright 2011 Addictive Labs. All rights reserved.
 *
 */
#include <iostream.h>
#include "gaussianDerivatives.h"
gaussianDerivatives::gaussianDerivatives(){
	float sigma  = 1.0;
	kernelSize = 5;
	kernelType = CV_32F;
	gaussianKernel = cv::getGaussianKernel(kernelSize, 1, kernelType);
	sigmaX = sigma > 0 ? sigma : ((kernelSize-1)*0.5 - 1)*0.3 + 0.8;
	scale2X = -0.5/(sigmaX*sigmaX);
}
gaussianDerivatives::gaussianDerivatives(int ksize, double sigma, int ktype){
	kernelSize = ksize;
	kernelType = ktype;
	gaussianKernel = cv::getGaussianKernel(kernelSize, sigma, kernelType);
	sigmaX = sigma > 0 ? sigma : ((kernelSize-1)*0.5 - 1)*0.3 + 0.8;
	scale2X = -0.5/(sigmaX*sigmaX);
}
cv::Mat gaussianDerivatives::getDerivativeKernel(int order){
	cv::Mat kernel = gaussianKernel.clone();
	float* cf = (float*)kernel.data;
    double* cd = (double*)kernel.data;
	if (order==1) {
		//	cout << "started 1" << endl;
		for (int i=0; i<kernelSize; i++) {
			double x = i - (kernelSize-1)*0.5;
			if( kernelType == CV_32F ){
				cf[i]*= (float)(-x/(sigmaX*sigmaX));	
			}
			else {
				cd[i]*= (-x/(sigmaX*sigmaX));
				//			cout << cd[i] << endl;
			}
		}
	} else if (order==2) {
		//	cout << "started 2" << endl;
		for (int i=0; i<kernelSize; i++) {
			
			double x = i - (kernelSize-1)*0.5;
			if( kernelType == CV_32F ){
				cf[i]*= (float)(x*x-sigmaX*sigmaX)/(sigmaX*sigmaX*sigmaX*sigmaX);	
				
			}
			else {
				cd[i]*= (x*x-sigmaX*sigmaX)/(sigmaX*sigmaX*sigmaX*sigmaX);
				//			cout << cd[i] << endl;
			}
		}
	} else if (order==0){
		//
		cout << "asked for gaussian" << endl;
	}

	return kernel;
}