//
//  main.cpp
//  nJetProject
//
//  Created by Nikolas Psaroudakis on 3/4/11.
//  Copyright Addictive Labs 2011. All rights reserved.
//

#include "ofMain.h"
#include "nJetProjectApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main() {

    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);

	ofRunApp(new nJetProjectApp());

}
