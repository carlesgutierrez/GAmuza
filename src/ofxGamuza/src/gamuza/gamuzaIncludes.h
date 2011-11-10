#ifdef TARGET_LINUX
	//-------------------------

	//-------------------------
#endif

#ifdef TARGET_OSX
	//-------------------------
	#include <mach/mach_time.h>
	//-------------------------
#endif

//////////////////////////////////////
// main core of OpenFrameworks
#include "ofMain.h"

// official addons
#include "ofxOpenCv.h"				// computer vision library
#include "ofxOsc.h"					// Open Sound Control communication library
#include "ofxXmlSettings.h"			// read/write on/to xml file library

//////////////////////////////////////
// experimental addons

// GUI
#include "ofxControlPanel.h"		// graphical user interface library [from Theo Watson]

// Multiple Windows handling
//#include "ofxFensterManager.h"

// OpenNI SensorKinect
#include "ofxOpenNI.h"				// Simple wrapper for OpenNI and SensorKinect

// live coding "fluxus" style
#include "ofxKeyMap.h"				// alt,ctrl,shift key detection
#include "ofxGLEditor.h"			// openGL text editor "fluxus" style <http://www.pawfal.org/fluxus/>

// live coding language
#include "ofxLua.h"					// Lua embedded scripting interpreter

// Audio output classes
#include "ofxAudioSample.h"			// reading&writing audio sample from hard drive with libsndfile

// Midi input/output
#include "ofxMidi.h"

// SYSTEM
#include "ofxMissing.h"				// Date/Time, INI Reader, Recursive file listing. Roxlu missing utils
#include "ofxFileDialog.h"			// open/save file dialog
#include "ofxStringEncoders.h"		// Html string encoders (UTF8, unquote)

// PHYSICS
#include "ofxBullet.h"				// OpenFrameworks add-on for Bullet Physics

// Pixels manipulators
#include "ofxHalftoner.h"

//////////////////////////////////////
// unofficial code

// webcam input classes
#include "sourceTracking.h"			// video input source class [from Emanuele Mazza]
#include "openniTracking.h"			// sensor kinect input source class [from Emanuele Mazza]
#include "matrixAreas.h"			// multiple points/areas mouse control, xml loading and saving [from Emanuele Mazza with great help from code fragment of Chris Sugrue]

// audio input [microphones, instruments, etc. routed via soundcard] class
#include "audioInputChannel.h"		// audio input source class [from Emanuele Mazza]

// audio Synthesis
#include "gaDsp.h"
#include "gaAmplifier.h"
#include "gaOscillator.h"
#include "audioModule.h"
