#ifndef OFX_KEY_MAP
#define OFX_KEY_MAP

/************************************************************

OpenFrameworks Library

File: 			ofxKeyMap.h
Description: 	Keeps track of pressed keys
Notes:

Last Modified: 2009.09.17
Creator: Rick Companje

************************************************************/

#include "ofMain.h"
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#define NUM_KEYS 512

class ofxKeyMap {
public:

    ofxKeyMap();

    bool isKeyDown(int key);
    bool isAltDown();
    bool isShiftDown();
    bool isControlDown();

    inline bool & operator [] (int i) { return keys[i]; }

	void _keyPressed(ofKeyEventArgs &e);
	void _keyReleased(ofKeyEventArgs &e);

private:
    bool keys[NUM_KEYS];
};

#endif
