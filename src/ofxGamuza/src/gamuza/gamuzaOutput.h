#ifndef _GAMUZA_OUTPUT
#define _GAMUZA_OUTPUT

// constants
#include "gamuzaConstants.h"
// includes
#include "gamuzaIncludes.h"

class gamuzaOutput : public ofxFensterListener{
    public:

        void draw(){

            ofBackground(0,0,0);

            ofEnableAlphaBlending();
            ofFill();
            glColor4f(1.0,0.0,0.0,0.9);
            ofCircle(0,0,100);
            ofDisableAlphaBlending();


        }

};

#endif
