#ifndef _GAMUZAGUI
#define _GAMUZAGUI

#include "gamuzaConstants.h"
#include "gamuzaFBO.h"

/* PALETA
 * 
 * glColor4f(0.941,0.941,0.941,1.0);		// 240, 240, 240
 * glColor4f(0.863,0.863,0.863,1.0);		// 220, 220, 220
 * glColor4f(0.353,0.353,0.353,1.0);		// 90, 90, 90
 * glColor4f(0.0784,0.0784,0.0784,1.0);		// 20, 20, 20
 * glColor4f(1.0,0.906,0.463,1.0);			// 255, 231, 118
 * glColor4f(0.847,0.25,0.25,1.0);			// 213, 64, 64
 *
 */

//////////////////////////////////////////////
// GUI vars
ofTrueTypeFont				fontSmall;
ofVideoGrabber				fake;

vector<string>				bgSubTechniques;
vector<string>				erodeDilateUse;
vector<string>				sourceFlipUse;
vector<string>				inputColorCorrection;
vector<string>				cfDetailUse;
vector<string>				calibScreenUse;
vector<string>				ardDigitalPinModes;
vector<string>				ardDigitalPinModesPWM;
vector<string>				ardAnalogPinModes;

ofImage						_empty;
ofImage						gamuzaLogo;
ofImage						splashImage;
ofImage						arduinoGraphics;
simpleColor					gamuzaMainColor;
simpleColor					gamuzaWhiteColor;
simpleColor					gamuzaMarkColor;
bool						splashFinish;
int							splashWait;
//////////////////////////////////////////////

//--------------------------------------------------------------
void gamuzaMain::setupGui(){
	
	char xml_name[256];
	char temp[128];
	
	///////////////////////////////////////////////
	// load fonts
	fontSmall.loadFont(GAMUZA_FONT, 6);
	///////////////////////////////////////////////
	
	///////////////////////////////////////////////
	// activate fake video grabber to obtain devices info
	if(trackingActivated){
		fake.setVerbose(true);
		fake.listDevices();
		fake.initGrabber(320, 240);
		
		// obtain cam devices available number
		numCamInputs = fake.getAvailableDevicesNum();
		
		camDevices	= new int[numCamInputs];
		deviceNames	= new string[numCamInputs];
		inputCam	= new sourceTracking[numCamInputs];
		
		// obtain cam devices id
		camDevices = fake.getAvailableIDs();
		
		// obtain cam devices names
		deviceNames = fake.getAvailableNames();
		
		fake.close();
	}else{
		numCamInputs = 0;
	}
	///////////////////////////////////////////////
	
	///////////////////////////////////////////////
	// retrieve dinamic multi panel(webcam & audio inputs) gui indexes
	if(!openniActivated){
		webcamsStartIndex	 = 1;
		audioInputStartIndex = 1 + numCamInputs;
	}else if(openniActivated){
		sensorKinectIndex	 = 1;
		webcamsStartIndex	 = sensorKinectIndex + 1;
		audioInputStartIndex = webcamsStartIndex + numCamInputs;
	}
	
	if(arduinoActivated){
		arduinoIndex = audioInputStartIndex + audioInputChannels;
		oscIndex	 = arduinoIndex + 1;
	}else if(!arduinoActivated){
		oscIndex = audioInputStartIndex + audioInputChannels;
	}
	
	///////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// init splash/intro image
	gamuzaLogo.loadImage(GAMUZA_LOGO);
	
	splashImage.loadImage(GAMUZA_SPLASH);
	if(trackingActivated && !openniActivated){
		splashWait = 5000*numCamInputs;
	}else if(openniActivated && !trackingActivated){
		splashWait = 20000 + 2000*audioInputChannels;
	}else if(openniActivated && trackingActivated){
		splashWait = 5000*numCamInputs + 20000 + 2000*audioInputChannels;
	}else{
		splashWait = 2000*audioInputChannels;
	}
	splashFinish = false;
	
	// empty containers
	_empty.loadImage("img/empty.png");
	emptyTexture.allocate(workingW,workingH,GL_RGB);
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// gui string vectors init
	setupStringVec();
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// Main GUI setup
	gamuzaMainColor  = simpleColor(216,64,64,255);
	gamuzaWhiteColor = simpleColor(240,240,240,255);
	gamuzaMarkColor	 = simpleColor(255,231,118,255);
	
	gui.loadFont(GAMUZA_FONT, 6);
	gui.setForegroundColor(simpleColor(90, 90, 90, 200),simpleColor(220, 220, 220, 160));
	gui.setBackgroundColor(simpleColor(20, 20, 20, 255));
	gui.setTextColor(gamuzaWhiteColor,simpleColor(240, 240, 240, 225));
	//gui.setOutlineColor(gamuzaWhiteColor);
	gui.setOutlineColor(simpleColor(20,20,20,255));
	
	if(isFullscreen){
		guiPosX = ((mainScreenW - MAIN_WINDOW_W)/2.0) + 1.0;
		guiPosY = ((mainScreenH - MAIN_WINDOW_H)/2.0) + 3.0;
		if(projectionScreenW < mainScreenW){
			lcPrevW	 = (projectionScreenW*mainScreenH)/projectionScreenH;
			lcPrevH	 = mainScreenH;
			lcPrevX	 = (mainScreenW-lcPrevW)/2.0;
			lcPrevY  = 0;
		}else{
			lcPrevW	 = mainScreenW;
			lcPrevH	 = (projectionScreenH*mainScreenW)/projectionScreenW;
			lcPrevX	 = 0;
			lcPrevY  = (mainScreenH-lcPrevH)/2.0;
		}
	}else{
		guiPosX = 0.0;
		guiPosY = 0.0;
		if(projectionScreenW < MAIN_WINDOW_W){
			lcPrevW	 = (projectionScreenW*MAIN_WINDOW_H)/projectionScreenH;
			lcPrevH	 = MAIN_WINDOW_H;
			lcPrevX	 = (MAIN_WINDOW_W-lcPrevW)/2.0;
			lcPrevY  = 0;
		}else{
			lcPrevW	 = MAIN_WINDOW_W;
			lcPrevH	 = (projectionScreenH*MAIN_WINDOW_W)/projectionScreenW;
			lcPrevX	 = 0;
			lcPrevY  = (MAIN_WINDOW_H-lcPrevH)/2.0;
		}
	}
	gui.setup(" -------------------------------------------------------", guiPosX, guiPosY, MAIN_WINDOW_W-2, MAIN_WINDOW_H-6);
	gui.setDraggable(false);
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// Live Coding GUI
	gui.setBackgroundColor(simpleColor(0, 0, 0, 255));
	gui.setOutlineColor(simpleColor(0,0,0,255));
	sprintf(temp," live coding [second screen fullscreen output @ %i x %i]",projectionScreenW,projectionScreenH);
	gui.addPanel(temp, 5);
	gui.setWhichPanel(temp);
	
	gui.setBackgroundColor(simpleColor(255, 255, 255, 20));
	gui.setOutlineColor(simpleColor(20,20,20,255));
	gui.setWhichColumn(0);
	//////////////////////////////////////////////
	gui.setTextColor(gamuzaMarkColor);
	sprintf(xml_name,"COMPUTE_FBO_TEXTURE");
	gui.addToggle("compute fbo texture", xml_name, true);
	gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
	sprintf(xml_name,"USE_SECONDARY_SCREEN");
	gui.addToggle("use secondary screen", xml_name, true);
	sprintf(xml_name,"USE_LIVECODING");
	gui.addToggle("use live coding", xml_name, false);
	//////////////////////////////////////////////
	gui.setTextColor(gamuzaMainColor);
	gui.addLabel("Grid Warping Settings");
	gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
	
	sprintf(xml_name,"DRAW_FBO_GRID");
	gui.addToggle("draw mapping grid", xml_name, false);
	sprintf(xml_name,"RESET_FBO_MAPPING_COORDS");
	gui.addToggle("reset mapping points", xml_name, false);
	//////////////////////////////////////////////
	gui.setTextColor(gamuzaMainColor);
	gui.addLabel("Calibration Screen Settings");
	gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
	
	sprintf(xml_name,"DRAW_FBO_CALIB_SCREEN");
	gui.addToggle("draw calibration screen", xml_name, false);
	sprintf(xml_name,"FBO_CALIB_SCREEN_SEL");
	gui.addTextDropDown("calibration screen selector", xml_name, 2, calibScreenUse);
	//////////////////////////////////////////////
	if(useShader){
		gui.setTextColor(gamuzaMainColor);
		gui.addLabel("Color Correction Settings");
		gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
		
		sprintf(xml_name,"OUTPUT_FBO_GAMMA");
		gui.addSlider("gamma correction",xml_name,fbo_gammaCorrection,1.0f,4.0f,false);
		sprintf(xml_name,"OUTPUT_FBO_BRIGHTNESS");
		gui.addSlider("brightness",xml_name,fbo_brightness,0.0f,4.0f,false);
		sprintf(xml_name,"OUTPUT_FBO_SATURATION");
		gui.addSlider("saturation",xml_name,fbo_saturation,0.0f,4.0f,false);
		sprintf(xml_name,"OUTPUT_FBO_CONTRAST");
		gui.addSlider("contrast",xml_name,fbo_contrast,0.0f,2.0f,false);	
		sprintf(xml_name,"OUTPUT_FBO_FILMBLEACH");
		gui.addSlider("film bleach",xml_name,fbo_filmBleach,0.0f,2.0f,false);
		sprintf(xml_name,"OUTPUT_FBO_FILM_TECHNICOLOR");
		gui.addSlider("film technicolor",xml_name,fbo_techniColor,0.0f,2.0f,false);
		sprintf(xml_name,"OUTPUT_FBO_FILM_WHITEDIFFUSION");
		gui.addSlider("film force black & white",xml_name,fbo_whiteDiffusion,0.0f,1.0f,false);
		sprintf(xml_name,"OUTPUT_FBO_FILM_EXPOSURE");
		gui.addSlider("film white exposure",xml_name,fbo_exposure,0.0f,10.0f,false);
		sprintf(xml_name,"OUTPUT_FBO_FILM_DIFFUSION");
		gui.addSlider("film white diffusion",xml_name,fbo_diffusion,0.0f,4.0f,false);
	}
	//////////////////////////////////////////////
	
	gui.setWhichColumn(1);
	//////////////////////////////////////////////
	gui.addDrawableRect("Output Projection Preview", &kuro, 512, 384);
	sprintf(xml_name,"USE_VSYNC");
	gui.addToggle("use vertical sync", xml_name, false);
	gui.addChartPlotter("FPS chart", guiStatVarPointer("gamuza FPS", &gamuzaRealFPS, GUI_VAR_FLOAT, true, 2), 512, 182, 200, 1, 120);
	//////////////////////////////////////////////
	
	gui.setWhichColumn(2);
	//////////////////////////////////////////////
	scriptsLister.listDir("scripts/");
	gui.addFileLister("script examples", &scriptsLister, 160, 562);
	sprintf(xml_name,"OPEN_SCRIPT_FILE_DIALOG");
	gui.addToggle("open script", xml_name, false);
	sprintf(xml_name,"SAVE_SCRIPT_FILE_DIALOG");
	gui.addToggle("save script", xml_name, false);
	
	gui.setWhichColumn(3);
	//////////////////////////////////////////////
	gui.enableIgnoreLayoutFlag();
	gui.addLogger("console", &logger, 380, 562);
	gui.disableIgnoreLayoutFlag();
	//////////////////////////////////////////////
	if(audioActivated && audioOutputChannels > 0){
		gui.setTextColor(gamuzaMainColor);
		gui.addLabel("Audio Output");
		gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
		
		sprintf(xml_name,"GAMUZA_MAIN_OUTPUT_VOLUME");
		gui.addSlider("main volume",xml_name,mainVolume,0.0f,1.0f,false);
	}
	
	//////////////////////////////////////////////
	// OpenNI SensorKinect GUI
	if(openniActivated){
		gui.setBackgroundColor(simpleColor(20, 20, 20, 255));
		gui.setOutlineColor(simpleColor(20,20,20,255));
		sprintf(temp," openni sensor kinect device");
		gui.addPanel(temp, 5);
		gui.setWhichPanel(temp);
		
		gui.setBackgroundColor(simpleColor(255, 255, 255, 20));
		gui.setOutlineColor(simpleColor(20,20,20,255));
		gui.setWhichColumn(0);
		//////////////////////////////////////////////
		gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
		
		gui.addDrawableRect("record image", &_empty, 320, 240);
		gui.setTextColor(gamuzaMarkColor);
		sprintf(xml_name,"COMPUTE_SENSOR_KINECT");
		gui.addToggle("compute sensor kinect", xml_name, true);
		gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
		gui.addDrawableRect("record depth", &_empty, 320, 240);
		sprintf(xml_name,"NEAR_THRESHOLD_SENSOR_KINECT");
		gui.addSlider("near treshold",xml_name,sensorKinect.nearThreshold,300,1000,true);
		sprintf(xml_name,"FAR_THRESHOLD_SENSOR_KINECT");
		gui.addSlider("far treshold",xml_name,sensorKinect.farThreshold,1000,3000,true);
		
		gui.setWhichColumn(1);
		//////////////////////////////////////////////
		gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
		
		gui.addDrawableRect("depth range mask", &sensorKinect.cleanImage, 320, 240);
		sprintf(xml_name,"USE_TRACKING_HANDS_SENSOR_KINECT");
		gui.addToggle("use kinect hands tracking", xml_name, true);
		gui.addDrawableRect("all users mask", &_empty, 320, 240);
		
		gui.setWhichColumn(2);
		//////////////////////////////////////////////
		gui.setTextColor(gamuzaMainColor);
		gui.addLabel("general settings");
		gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
		
		sprintf(xml_name,"CI_BLUR_SENSOR_KINECT");
		gui.addSlider("sensor kinect Blur", xml_name, sensorKinect.ciBlur, 0, 33, true);
		sprintf(xml_name,"CI_USE_ERODE_DILATE_SENSOR_KINECT");
		gui.addTextDropDown("erode | dilate ", xml_name, 2, erodeDilateUse);
		sprintf(xml_name,"CI_ERODE_SENSOR_KINECT");
		gui.addSlider("erode", xml_name, sensorKinect.ciErode, 0, 10, true);
		sprintf(xml_name,"CI_DILATE_SENSOR_KINECT");
		gui.addSlider("dilate", xml_name, sensorKinect.ciDilate, 0, 10, true);
		
		gui.setTextColor(gamuzaMainColor);
		gui.addLabel("motion detection settings");
		gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
		
		sprintf(xml_name,"M_THRESHOLD_SENSOR_KINECT");
		gui.addSlider("motion threshold", xml_name, sensorKinect.mThreshold, 1, 300, true);
		sprintf(xml_name,"MOTION_ON_HORIZON_SENSOR_KINECT");
		gui.addSlider("motion trigger low limit", xml_name, sensorKinect.onHorizon, 1, 100, true);
		sprintf(xml_name,"MOTION_OFF_HORIZON_SENSOR_KINECT");
		gui.addSlider("motion trigger range limit", xml_name, sensorKinect.offHorizon, 0, 200, true);
		gui.addDrawableRect("motion image", &sensorKinect.motionImg, 160, 120);
		//////////////////////////////////////////////
		
		gui.setWhichColumn(3);
		//////////////////////////////////////////////
		gui.setTextColor(gamuzaMainColor);
		gui.addLabel("blob tracking settings");
		gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
		
		sprintf(xml_name,"MIN_BLOB_AREA_SENSOR_KINECT");
		gui.addSlider("min blob",xml_name,sensorKinect.minBlobArea,2,5000,true);
		sprintf(xml_name,"MAX_BLOB_AREA_SENSOR_KINECT");
		gui.addSlider("max blob",xml_name,sensorKinect.maxBlobArea,0,100000,true);
		sprintf(xml_name,"CF_DETAIL_SENSOR_KINECT");
		gui.addTextDropDown("contour detail", xml_name, 0, cfDetailUse);
		sprintf(xml_name,"CF_SMOOTH_PCT_SENSOR_KINECT");
		gui.addSlider("contour smooth factor", xml_name,sensorKinect.smoothPct,0.01f,0.99f,false);
		sprintf(xml_name,"CF_TOLERANCE_SENSOR_KINECT");
		gui.addSlider("contour simple tolerance", xml_name,sensorKinect.tolerance,0.01f,20.0f,false);
		//////////////////////////////////////////////
		gui.setTextColor(gamuzaMainColor);
		gui.addLabel("computing algorithm selector");
		gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
		
		sprintf(xml_name,"COMPUTE_CF_SENSOR_KINECT");
		gui.addToggle("compute contour finder", xml_name, false);
		sprintf(xml_name,"COMPUTE_CG_SENSOR_KINECT");
		gui.addToggle("compute contour geometry", xml_name, false);
		sprintf(xml_name,"COMPUTE_OF_SENSOR_KINECT");
		gui.addToggle("compute optical flow", xml_name, false);
		sprintf(xml_name,"COMPUTE_TA_SENSOR_KINECT");
		gui.addToggle("compute trigger areas", xml_name, false);
		//////////////////////////////////////////////
		gui.setTextColor(gamuzaMainColor);
		gui.addLabel("osc data settings");
		gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
		
		sprintf(xml_name,"USE_KALMAN_FILTER_SENSOR_KINECT");
		gui.addToggle("use kalman filter", xml_name, false);
		sprintf(xml_name,"SMOOTHING_FACTOR_SENSOR_KINECT");
		gui.addSlider("smoothing factor",xml_name,sensorKinect._smoothingFactor,0.01f,0.99f,false);
		
		gui.setWhichColumn(4);
		//////////////////////////////////////////////
		gui.setTextColor(gamuzaMainColor);
		gui.addLabel("sensor kinect hardware");
		gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
		
		sprintf(temp,"sensor tilt");
		gui.addChartPlotter(temp, guiStatVarPointer("angle", &sensorKinect._osc_sensorTilt, GUI_VAR_FLOAT, true, 2), 180, 80, 200, 0.0f, 1.0f);
		sprintf(temp,"sensor accelerometer x");
		gui.addChartPlotter(temp, guiStatVarPointer("x", &sensorKinect._osc_sensorAcc.x, GUI_VAR_FLOAT, true, 2), 180, 80, 200, 0.0f, 1.0f);
		sprintf(temp,"sensor accelerometer y");
		gui.addChartPlotter(temp, guiStatVarPointer("y", &sensorKinect._osc_sensorAcc.y, GUI_VAR_FLOAT, true, 2), 180, 80, 200, 0.0f, 1.0f);
		sprintf(temp,"sensor accelerometer z");
		gui.addChartPlotter(temp, guiStatVarPointer("z", &sensorKinect._osc_sensorAcc.z, GUI_VAR_FLOAT, true, 2), 180, 80, 200, 0.0f, 1.0f);
		
	}
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// Video Tracking multi-panel GUI
	if(trackingActivated){
		for(unsigned int i=0;i<numCamInputs;i++){
			if(useVideoTest){
				inputCam[i].setupCam(i,workingW,workingH,camDevices[i],true,haarFinderFile);
			}else{
				inputCam[i].setupCam(i,workingW,workingH,camDevices[i],false,haarFinderFile);
			}
			
			gui.setBackgroundColor(simpleColor(0, 0, 0, 255));
			gui.setOutlineColor(simpleColor(0,0,0,255));
			sprintf(temp," CAM Device [%i] | %s",camDevices[i],deviceNames[i].c_str());
			gui.addPanel(temp, 5);
			
			gui.setWhichPanel(temp);
			gui.setBackgroundColor(simpleColor(255, 255, 255, 20));
			gui.setOutlineColor(simpleColor(20,20,20,255));
		
			gui.setWhichColumn(0);
			gui.addDrawableRect("color Input [eventually]", &inputCam[i].colorImg, 320, 240);
			
			gui.setTextColor(gamuzaMarkColor);
			sprintf(xml_name,"USE_DEVICE_%i",camDevices[i]);
			gui.addToggle("use device", xml_name, true);
			gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
			sprintf(xml_name,"UNDISTORT_INPUT_%i",camDevices[i]);
			gui.addToggle("undistort [lens correction]", xml_name, false);
			gui.addDrawableRect("b&w input", &inputCam[i].motionGUI, 160, 120);
			sprintf(xml_name,"RESET_INPUT_WARPING_DEV_%i",camDevices[i]);
			gui.addToggle("reset quad warping", xml_name, false);
			gui.addDrawableRect("warped RGB/HSV Input", &inputCam[i].colorImgHSV, 160, 120);
			
			gui.setWhichColumn(1);
			gui.addDrawableRect("Balanced Tracking", &inputCam[i].balancedTracking, 320, 240);
			sprintf(xml_name,"BG_CAPTURE_%i",camDevices[i]);
			gui.addToggle("capture background", xml_name, true);
			sprintf(xml_name,"BG_USE_STRECH_%i",camDevices[i]);
			gui.addToggle("use contrast strech", xml_name, true);
			gui.addDrawableRect("background subtraction", &inputCam[i].grayThresh, 160, 120);
			sprintf(xml_name,"USE_COLOR_TRACKING_%i",camDevices[i]);
			gui.addToggle("use color tracking", xml_name, true);
			gui.addDrawableRect("color Tracking", &inputCam[i].hsvTracking, 160, 120);
			
			gui.setWhichColumn(2);
			gui.setTextColor(gamuzaMainColor);
			gui.addLabel("general settings");
			gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
			
			//////////////////////////////////////////////
			sprintf(xml_name,"FLIP_SOURCE_USE_%i",camDevices[i]);
			gui.addTextDropDown("flip source image ", xml_name, 1, sourceFlipUse);
			//////////////////////////////////////////////
			sprintf(xml_name,"INPUT_BLUR_%i",camDevices[i]);
			gui.addSlider("source Blur", xml_name, inputCam[i].colorImgBlur, 0, 33, true);
			sprintf(xml_name,"SOURCE_COLOR_CORRECTION_USE_%i",camDevices[i]);
			gui.addTextDropDown("source color correction", xml_name, 1, inputColorCorrection);
			sprintf(xml_name,"CONTRAST_%i",camDevices[i]);
			gui.addSlider("contrast", xml_name, inputCam[i].contrast, 0.0, 1.0f, false);
			sprintf(xml_name,"BRIGHTNESS_%i",camDevices[i]);
			gui.addSlider("brightness", xml_name, inputCam[i].brightness, -1.0, 3.0f, false);
			sprintf(xml_name,"GAMMA_%i",camDevices[i]);
			gui.addSlider("gamma", xml_name, inputCam[i].gamma, 0.01, 3.0f, false);
			//////////////////////////////////////////////
			gui.setTextColor(gamuzaMainColor);
			gui.addLabel("background subtraction settings");
			gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
			
			sprintf(xml_name,"BGSUB_TECH_%i",camDevices[i]);
			gui.addTextDropDown("subtraction technique ", xml_name, 3, bgSubTechniques);
			sprintf(xml_name,"BGSUB_THRESHOLD_%i",camDevices[i]);
			gui.addSlider("subtraction threshold", xml_name, inputCam[i].threshold, 1, 254, true);
			sprintf(xml_name,"BGSUB_BLUR_%i",camDevices[i]);
			gui.addSlider("Blur", xml_name, inputCam[i].bgSubBlur, 0, 33, true);
			sprintf(xml_name,"BG_USE_ERODE_DILATE_%i",camDevices[i]);
			gui.addTextDropDown("erode | dilate ", xml_name, 2, erodeDilateUse);
			sprintf(xml_name,"BGSUB_ERODE_%i",camDevices[i]);
			gui.addSlider("erode", xml_name, inputCam[i].bgSubErode, 0, 10, true);
			sprintf(xml_name,"BGSUB_DILATE_%i",camDevices[i]);
			gui.addSlider("dilate", xml_name, inputCam[i].bgSubDilate, 0, 10, true);
			//////////////////////////////////////////////
			gui.setTextColor(gamuzaMainColor);
			gui.addLabel("osc data settings");
			gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
			sprintf(xml_name,"USE_KALMAN_FILTER_%i",camDevices[i]);
			gui.addToggle("use kalman filter", xml_name, false);
			sprintf(xml_name,"SMOOTHING_FACTOR_%i",camDevices[i]);
			gui.addSlider("smoothing factor",xml_name,inputCam[i]._smoothingFactor,0.01f,0.99f,false);
			//////////////////////////////////////////////
			
			gui.setWhichColumn(3);
			//////////////////////////////////////////////
			gui.setTextColor(gamuzaMainColor);
			gui.addLabel("color tracking settings");
			gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
			
			sprintf(xml_name,"HUE_%i",camDevices[i]);
			gui.addSlider("hue", xml_name, inputCam[i].hue, 0.0, 1.0f, false);
			sprintf(xml_name,"HUE_WIDTH_%i",camDevices[i]);
			gui.addSlider("hue range", xml_name, inputCam[i].hueWidth, 0.0, 1.0f, false);
			sprintf(xml_name,"SAT_%i",camDevices[i]);
			gui.addSlider("saturation", xml_name, inputCam[i].sat, 0.0, 1.0f, false);
			sprintf(xml_name,"SAT_WIDTH_%i",camDevices[i]);
			gui.addSlider("saturation range", xml_name, inputCam[i].satWidth, 0.0, 1.0f, false);
			sprintf(xml_name,"VAL_%i",camDevices[i]);
			gui.addSlider("value", xml_name, inputCam[i].val, 0.0, 1.0f, false);
			sprintf(xml_name,"VAL_WIDTH_%i",camDevices[i]);
			gui.addSlider("value range", xml_name, inputCam[i].valWidth, 0.0, 1.0f, false);
			sprintf(xml_name,"HSV_BLUR_%i",camDevices[i]);
			gui.addSlider("hsv Blur", xml_name, inputCam[i].hsvBlur, 0, 33, true);
			sprintf(xml_name,"HSV_USE_ERODE_DILATE_%i",camDevices[i]);
			gui.addTextDropDown("erode | dilate ", xml_name, 2, erodeDilateUse);
			sprintf(xml_name,"HSV_ERODE_%i",camDevices[i]);
			gui.addSlider("erode", xml_name, inputCam[i].hsvErode, 0, 10, true);
			sprintf(xml_name,"HSV_DILATE_%i",camDevices[i]);
			gui.addSlider("dilate", xml_name, inputCam[i].hsvDilate, 0, 10, true);
			//////////////////////////////////////////////
			gui.setTextColor(gamuzaMainColor);
			gui.addLabel("blob tracking settings");
			gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
			
			sprintf(xml_name,"MIN_BLOB_AREA_%i",camDevices[i]);
			gui.addSlider("min blob",xml_name,inputCam[i].minBlobArea,2,5000,true);
			sprintf(xml_name,"MAX_BLOB_AREA_%i",camDevices[i]);
			gui.addSlider("max blob",xml_name,inputCam[i].maxBlobArea,0,100000,true);
			sprintf(xml_name,"CF_DETAIL_%i",camDevices[i]);
			gui.addTextDropDown("contour detail", xml_name, 0, cfDetailUse);
			sprintf(xml_name,"CF_SMOOTH_PCT_%i",camDevices[i]);
			gui.addSlider("contour smooth factor", xml_name,inputCam[i].smoothPct,0.01f,0.99f,false);
			sprintf(xml_name,"CF_TOLERANCE_%i",camDevices[i]);
			gui.addSlider("contour simple tolerance", xml_name,inputCam[i].tolerance,0.01f,20.0f,false);
			//////////////////////////////////////////////
			
			gui.setWhichColumn(4);
			//////////////////////////////////////////////
			gui.setTextColor(gamuzaMainColor);
			gui.addLabel("motion detection settings");
			gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
			
			sprintf(xml_name,"M_THRESHOLD_%i",camDevices[i]);
			gui.addSlider("motion threshold", xml_name, inputCam[i].mThreshold, 1, 300, true);
			sprintf(xml_name,"M_NOISE_COMP_%i",camDevices[i]);
			gui.addSlider("motion noise compensation", xml_name, inputCam[i].mNoiseComp, 0, 1000, true);
			sprintf(xml_name,"MOTION_ON_HORIZON_%i",camDevices[i]);
			gui.addSlider("motion trigger low limit", xml_name, inputCam[i].onHorizon, 1, 100, true);
			sprintf(xml_name,"MOTION_OFF_HORIZON_%i",camDevices[i]);
			gui.addSlider("motion trigger range limit", xml_name, inputCam[i].offHorizon, 0, 200, true);
			//////////////////////////////////////////////
			gui.setTextColor(gamuzaMainColor);
			gui.addLabel("balanced tracking settings");
			gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
			
			sprintf(xml_name,"BT_BLUR_%i",camDevices[i]);
			gui.addSlider("balanced tracking Blur", xml_name, inputCam[i].btBlur, 0, 33, true);
			sprintf(xml_name,"BT_USE_ERODE_DILATE_%i",camDevices[i]);
			gui.addTextDropDown("erode | dilate ", xml_name, 2, erodeDilateUse);
			sprintf(xml_name,"BT_ERODE_%i",camDevices[i]);
			gui.addSlider("erode", xml_name, inputCam[i].btErode, 0, 10, true);
			sprintf(xml_name,"BT_DILATE_%i",camDevices[i]);
			gui.addSlider("dilate", xml_name, inputCam[i].btDilate, 0, 10, true);
			sprintf(xml_name,"BT_BALANCE1_%i",camDevices[i]);
			gui.addSlider("balance background sub.", xml_name, inputCam[i].balance1, 1, 254, true);
			sprintf(xml_name,"BT_BALANCE2_%i",camDevices[i]);
			gui.addSlider("balance color tracking", xml_name, inputCam[i].balance2, 1, 254, true);
			//////////////////////////////////////////////
			gui.setTextColor(gamuzaMainColor);
			gui.addLabel("computing algorithm selector");
			gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
			
			sprintf(xml_name,"COMPUTE_CF_%i",camDevices[i]);
			gui.addToggle("compute contour finder", xml_name, false);
			sprintf(xml_name,"COMPUTE_CG_%i",camDevices[i]);
			gui.addToggle("compute contour geometry", xml_name, false);
			sprintf(xml_name,"COMPUTE_OF_%i",camDevices[i]);
			gui.addToggle("compute optical flow", xml_name, false);
			sprintf(xml_name,"COMPUTE_FT_%i",camDevices[i]);
			gui.addToggle("compute haar finder", xml_name, false);
			sprintf(xml_name,"COMPUTE_TA_%i",camDevices[i]);
			gui.addToggle("compute trigger areas", xml_name, false);
			//////////////////////////////////////////////
			
		}
	}
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// Audio Source multi-panel GUI
	if(audioActivated){
		for(unsigned int i=0;i<audioInputChannels;i++){
		
			inputAudioCH[i].loadNoiseFilter();
			sprintf(temp," AUDIO input [channel %i]",i);
			gui.setBackgroundColor(simpleColor(40, 40, 0, 70));
			gui.setOutlineColor(simpleColor(40, 40, 0, 70));
			gui.addPanel(temp, 5);
			gui.setWhichPanel(temp);
			gui.setBackgroundColor(simpleColor(255, 255, 255, 20));
			gui.setOutlineColor(simpleColor(20,20,20,255));
		
			gui.setWhichColumn(0);
			//////////////////////////////////////////////
			gui.setTextColor(gamuzaMainColor);
			gui.addLabel("General Settings");
			
			gui.setTextColor(gamuzaMarkColor);
			sprintf(xml_name,"USE_AUDIO_INPUT_%i",i);
			gui.addToggle("use input channel", xml_name, false);
			gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
			
			sprintf(xml_name,"VOLUME_AUDIO_INPUT_%i",i);
			gui.addSlider("volume", xml_name, inputAudioCH[i]._volume,0.01f,1.0f,false);
			sprintf(xml_name,"MUTE_AUDIO_INPUT_%i",i);
			gui.addToggle("mute", xml_name, false);
			//////////////////////////////////////////////
			gui.setTextColor(gamuzaMainColor);
			gui.addLabel("Noise Reduction Settings");
			gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
			
			sprintf(xml_name,"REC_NOISE_FILTER_%i",i);
			gui.addToggle("rec noise", xml_name, false);
			sprintf(xml_name,"NOISE_REDUX_FACTOR_%i",i);
			gui.addSlider("redux factor", xml_name, inputAudioCH[i].reduxFactor,1.0f,10.0f,false);
			//////////////////////////////////////////////
			gui.setTextColor(gamuzaMainColor);
			gui.addLabel("3 band parametric equalization");
			gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
			
			
			sprintf(xml_name,"EQ_BAND1_CENTER_%i",i);
			gui.addSlider("band 1 center [bin]", xml_name, inputAudioCH[i].eqLOWCenter,0.0f,audioBufferSize/2.0f,false);
			sprintf(xml_name,"EQ_BAND1_AMPLITUDE_%i",i);
			gui.addSlider("band 1 amplitude [db]", xml_name, inputAudioCH[i].eqLOWAmplitude,-3.0f,6.0f,false);
			sprintf(xml_name,"EQ_BAND1_WIDTH_%i",i);
			gui.addSlider("band 1 width [q]", xml_name, inputAudioCH[i].eqLOWWidth,0.01f,audioBufferSize/4.0f,false);
			//////////////////////////////////////////////
			sprintf(xml_name,"EQ_BAND2_CENTER_%i",i);
			gui.addSlider("band 2 center [bin]", xml_name, inputAudioCH[i].eqMIDCenter,0.0f,audioBufferSize/2.0f,false);
			sprintf(xml_name,"EQ_BAND2_AMPLITUDE_%i",i);
			gui.addSlider("band 2 amplitude [db]", xml_name, inputAudioCH[i].eqMIDAmplitude,-3.0f,6.0f,false);
			sprintf(xml_name,"EQ_BAND2_WIDTH_%i",i);
			gui.addSlider("band 2 width [q]", xml_name, inputAudioCH[i].eqMIDWidth,0.01f,audioBufferSize/4.0f,false);
			//////////////////////////////////////////////
			sprintf(xml_name,"EQ_BAND3_CENTER_%i",i);
			gui.addSlider("band 3 center [bin]", xml_name, inputAudioCH[i].eqHICenter,0.0f,audioBufferSize/2.0f,false);
			sprintf(xml_name,"EQ_BAND3_AMPLITUDE_%i",i);
			gui.addSlider("band 3 amplitude [db]", xml_name, inputAudioCH[i].eqHIAmplitude,-3.0f,6.0f,false);
			sprintf(xml_name,"EQ_BAND3_WIDTH_%i",i);
			gui.addSlider("band 3 width [q]", xml_name, inputAudioCH[i].eqHIWidth,0.01f,audioBufferSize/4.0f,false);
			//////////////////////////////////////////////
			
			gui.setWhichColumn(1);
			//////////////////////////////////////////////
			gui.setTextColor(gamuzaMainColor);
			gui.addLabel("osc data settings");
			gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
			
			sprintf(xml_name,"USE_KALMAN_FILTER_AUDIOCH_%i",i);
			gui.addToggle("use kalman filter", xml_name, false);
			sprintf(xml_name,"SMOOTHING_FACTOR_AUDIOCH_%i",i);
			gui.addSlider("smoothing factor",xml_name,inputAudioCH[i]._smoothingFactor,0.01f,0.99f,false);
			//////////////////////////////////////////////
			gui.addChartPlotter("Volume chart", guiStatVarPointer("channel volume", &inputAudioCH[i]._osc_chVolume, GUI_VAR_FLOAT, true, 2), 180, 80, 200, 0.0f, 1.0f);
			gui.addChartPlotter("Pitch chart", guiStatVarPointer("channel pitch", &inputAudioCH[i]._s_chPitch, GUI_VAR_FLOAT, true, 2), 180, 80, 200, 20.0f, 20000.0f);
			
		}
	}
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// Serial GUI [Arduino]
	if(arduinoActivated){
		
		arduinoGraphics.loadImage("img/arduino.png");
		
		gui.setBackgroundColor(simpleColor(0, 0, 20, 255));
		gui.setOutlineColor(simpleColor(0, 0, 20, 255));
		sprintf(temp," arduino device connected at serial port %s | with baudrate %i", serialDevice.c_str(), baudRate);
		gui.addPanel(temp, 5);
		gui.setWhichPanel(temp);
		gui.setBackgroundColor(simpleColor(255, 255, 255, 20));
		gui.setOutlineColor(simpleColor(20,20,20,255));
		
		gui.setWhichColumn(0);
		//////////////////////////////////////////////
		gui.setTextColor(gamuzaMainColor);
		gui.addLabel("Analog values reading [0 | 5]");
		gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
		
		for(unsigned int i=0;i<6;i++){
			sprintf(temp,"analog pin %i ",i);
			gui.addChartPlotter(temp, guiStatVarPointer("Reading", &_s_analogPinValues[i], GUI_VAR_FLOAT, true, 2), 180, 80, 200, 0.0, 1023.0);
		}
		
		gui.setWhichColumn(1);
		//////////////////////////////////////////////
		gui.setTextColor(gamuzaMainColor);
		gui.addLabel("Analog pins control [0 | 5]");
		gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
		
		for(unsigned int i=0;i<6;i++){
			sprintf(xml_name,"ARDUINO_ANALOG_PIN_%i_MODE",i);
			sprintf(temp,"pin %i ",i);
			gui.addTextDropDown(temp, xml_name, 1, ardAnalogPinModes);
		}
		
		gui.setWhichColumn(2);
		//////////////////////////////////////////////
		gui.setTextColor(gamuzaMainColor);
		gui.addLabel("Arduino board");
		gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
		
		gui.setTextColor(gamuzaMarkColor);
		sprintf(xml_name,"USE_ARDUINO");
		gui.addToggle("use arduino", xml_name, true);
		gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
		gui.addDrawableRect(" ", &arduinoGraphics, 360, 475);
		//////////////////////////////////////////////
		gui.setTextColor(gamuzaMainColor);
		gui.addLabel("osc data settings");
		gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
		
		sprintf(xml_name,"USE_KALMAN_FILTER_ARDUINO");
		gui.addToggle("use kalman filter", xml_name, false);
		sprintf(xml_name,"SMOOTHING_FACTOR_ARDUINO");
		gui.addSlider("smoothing factor",xml_name,arduino_smoothingFactor,0.01f,0.99f,false);
		
		gui.setWhichColumn(3);
		//////////////////////////////////////////////
		gui.setTextColor(gamuzaMainColor);
		gui.addLabel("digital pins control [2 | 13]");
		gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
		
		//////////////////////////////////////////////
		for(unsigned int i=2;i<14;i++){
			sprintf(xml_name,"ARDUINO_DIGITAL_PIN_%i_MODE",i);
			if(i == 3 || i == 5 || i == 6 || i == 9 || i == 10 || i == 11){
				sprintf(temp,"pin [pwm] %i ",i);
				gui.addTextDropDown(temp, xml_name, 0, ardDigitalPinModesPWM);
			}else{
				sprintf(temp,"pin %i ",i);
				gui.addTextDropDown(temp, xml_name, 0, ardDigitalPinModes);
			}
		}
		
		gui.setWhichColumn(4);
		//////////////////////////////////////////////
		gui.setTextColor(gamuzaMainColor);
		gui.addLabel("digital values writing [2 | 13]");
		gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
		
		//////////////////////////////////////////////
		for(unsigned int i=2;i<14;i++){
			sprintf(xml_name,"ARDUINO_DIGITAL_PIN_%i_VALUE",i);
			if(i == 3 || i == 5 || i == 6 || i == 9 || i == 10 || i == 11){
				sprintf(temp,"pin %i pwm [0 254]",i);
				gui.panels.at(gui.currentPanel)->setElementYSpacing(-5);
				gui.addSlider(temp,xml_name,digitalPinValuesOutput[i-2],0,254,true);
				gui.setTextColor(simpleColor(255, 10, 10, 0));
				gui.setForegroundColor(simpleColor(90, 90, 90, 200),simpleColor(0, 255, 20, 255));
				gui.panels.at(gui.currentPanel)->setElementYSpacing(15);
				gui.addChartPlotter(temp, guiStatVarPointer(" ", &digitalPinValuesInput[i-2], GUI_VAR_INT, true, 2), 180, 8, 200, 0, 255);
				gui.setForegroundColor(simpleColor(90, 90, 90, 200),simpleColor(220, 220, 220, 160));
				gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
			}else{
				sprintf(temp,"pin %i low | high",i);
				gui.panels.at(gui.currentPanel)->setElementYSpacing(-5);
				gui.addToggle(temp, xml_name, true);
				gui.setTextColor(simpleColor(255, 10, 10, 0));
				gui.setForegroundColor(simpleColor(90, 90, 90, 200),simpleColor(0, 255, 20, 255));
				gui.panels.at(gui.currentPanel)->setElementYSpacing(15);
				gui.addChartPlotter(temp, guiStatVarPointer(" ", &digitalPinValuesInput[i-2], GUI_VAR_INT, true, 2), 180, 8, 200, 0, 1);
				gui.setForegroundColor(simpleColor(90, 90, 90, 200),simpleColor(220, 220, 220, 160));
				gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
			}
		}
		
	}
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// OSC GUI [Open Sound Control Output DATA]
	if(oscActivated){
		gui.setBackgroundColor(simpleColor(0, 0, 0, 255));
		gui.setOutlineColor(simpleColor(0,0,0,255));
		sprintf(temp," osc data sending at host %s : port %s ",host_number.c_str(),host_port.c_str());
		gui.addPanel(temp, 5);
		gui.setWhichPanel(temp);
		gui.setBackgroundColor(simpleColor(255, 255, 255, 20));
		gui.setOutlineColor(simpleColor(20,20,20,255));
		
		gui.setWhichColumn(0);
		
		//////////////////////////////////////////////
		gui.setTextColor(gamuzaMarkColor);
		sprintf(xml_name,"USE_OSC");
		gui.addToggle("use osc", xml_name, true);
		//////////////////////////////////////////////
		
		if(openniActivated){
			//////////////////////////////////////////////
			gui.setTextColor(gamuzaMainColor);
			gui.addLabel("openni sensor kinect osc data");
			gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
			
			sprintf(xml_name,"SENSOR_KINECT_SEND_OSC_BD");
			gui.addToggle("send blob detection data", xml_name, true);
			sprintf(xml_name,"SENSOR_KINECT_SEND_OSC_CF");
			gui.addToggle("send contour finder data", xml_name, true);
			sprintf(xml_name,"SENSOR_KINECT_SEND_OSC_CG");
			gui.addToggle("send contour geometry data", xml_name, true);
			sprintf(xml_name,"SENSOR_KINECT_SEND_OSC_OF");
			gui.addToggle("send optical flow LK data", xml_name, true);
			sprintf(xml_name,"SENSOR_KINECT_SEND_OSC_TA");
			gui.addToggle("send trigger areas data", xml_name, true);
			sprintf(xml_name,"SENSOR_KINECT_SEND_OSC_HT");
			gui.addToggle("send hand tracking data", xml_name, true);
			sprintf(xml_name,"SENSOR_KINECT_SEND_OSC_HW");
			gui.addToggle("send sensor hardware data", xml_name, true);
		
		}
		//////////////////////////////////////////////
		
		if(arduinoActivated){
			//////////////////////////////////////////////
			gui.setTextColor(gamuzaMainColor);
			gui.addLabel("arduino [serial port]");
			gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
			
			sprintf(xml_name,"ARDUINO_SEND_OSC_AAP");
			gui.addToggle("send analog pins data", xml_name, true);
			sprintf(xml_name,"ARDUINO_SEND_OSC_ADP");
			gui.addToggle("send digital pins data", xml_name, true);
			
		}
		//////////////////////////////////////////////
		
		if(trackingActivated){
			for(unsigned int i=0;i<numCamInputs;i++){
				gui.setWhichColumn(i+1);
				//////////////////////////////////////////////
				gui.setTextColor(gamuzaMainColor);
				sprintf(temp,"Dev.[%i] | %s",camDevices[i],deviceNames[i].c_str());
				gui.addLabel(temp);
				gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
				
				sprintf(xml_name,"WEBCAM_ID%i_SEND_OSC_MD",camDevices[i]);
				gui.addToggle("send motion detection data", xml_name, true);
				sprintf(xml_name,"WEBCAM_ID%i_SEND_OSC_BD",camDevices[i]);
				gui.addToggle("send blob detection data", xml_name, true);
				sprintf(xml_name,"WEBCAM_ID%i_SEND_OSC_CF",camDevices[i]);
				gui.addToggle("send contour finder data", xml_name, true);
				sprintf(xml_name,"WEBCAM_ID%i_SEND_OSC_CG",camDevices[i]);
				gui.addToggle("send contour geometry data", xml_name, true);
				sprintf(xml_name,"WEBCAM_ID%i_SEND_OSC_OF",camDevices[i]);
				gui.addToggle("send optical flow LK data", xml_name, true);
				sprintf(xml_name,"WEBCAM_ID%i_SEND_OSC_HF",camDevices[i]);
				gui.addToggle("send haar finder data", xml_name, true);
				sprintf(xml_name,"WEBCAM_ID%i_SEND_OSC_TA",camDevices[i]);
				gui.addToggle("send trigger areas data", xml_name, true);
				
			}
			
		}
		//////////////////////////////////////////////
		
		if(audioActivated){
			for(unsigned int i=0;i<audioInputChannels;i++){
				gui.setWhichColumn(i+1);
				//////////////////////////////////////////////
				gui.setTextColor(gamuzaMainColor);
				sprintf(temp,"AUDIO input [channel %i]",i);
				gui.addLabel(temp);
				gui.setTextColor(gamuzaWhiteColor,gamuzaMarkColor);
				
				sprintf(xml_name,"AUDIO_INPUT_CH%i_SEND_OSC_VD",i);
				gui.addToggle("send volume detection data", xml_name, true);
				sprintf(xml_name,"AUDIO_INPUT_CH%i_SEND_OSC_PD",i);
				gui.addToggle("send pitch detection data", xml_name, true);
				sprintf(xml_name,"AUDIO_INPUT_CH%i_SEND_OSC_BK",i);
				gui.addToggle("send bark scale data", xml_name, true);
				
			}
			
		}
		//////////////////////////////////////////////
		
	}
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// Gamuza Credits GUI
	gui.setBackgroundColor(simpleColor(20, 20, 20, 255));
	
	sprintf(temp," Gamuza Version 0.3 'Raven Shammy' Beta Testing");
	gui.addPanel(temp, 3);
	gui.setWhichPanel(temp);
	//////////////////////////////////////////////
	
	gui.loadSettings(GAMUZAGUI_SETTINGS);
	
	//////////////////////////////////////////////
	// GUI Events
	gui.setupEvents();
	gui.enableEvents();
	
	// enable all events for gui logger
	ofAddListener(gui.guiEvent, this, &gamuzaMain::eventsIn);
	
	// openNI sensor kinect GUI
	if(openniActivated){
		// create events to force contour finder computing
		sprintf(temp,"COMPUTE_CG_SENSOR_KINECT");
		ofAddListener(gui.createEventGroup(temp), this, &gamuzaMain::activateTrackingCF);
		sprintf(temp,"COMPUTE_TA_SENSOR_KINECT");
		ofAddListener(gui.createEventGroup(temp), this, &gamuzaMain::activateTrackingCF);
	}
	
	// Video Tracking multi-panel GUI
	if(trackingActivated){
		for(unsigned int i=0;i<numCamInputs;i++){ // for each input camera
			// create event for capture background
			sprintf(temp,"BG_CAPTURE_%i",camDevices[i]);
			ofAddListener(gui.createEventGroup(temp), this, &gamuzaMain::grabBackgroundEvent);
			// create event for reset input warping points
			sprintf(temp,"RESET_INPUT_WARPING_DEV_%i",camDevices[i]);
			ofAddListener(gui.createEventGroup(temp), this, &gamuzaMain::resetInputWarping);
			// create events to force contour finder computing
			sprintf(temp,"COMPUTE_CG_%i",camDevices[i]);
			ofAddListener(gui.createEventGroup(temp), this, &gamuzaMain::activateTrackingCF);
			sprintf(temp,"COMPUTE_TA_%i",camDevices[i]);
			ofAddListener(gui.createEventGroup(temp), this, &gamuzaMain::activateTrackingCF);
		}
	}
	
	// Audio Source multi-panel GUI
	if(audioActivated){
		for(unsigned int i=0;i<audioInputChannels;i++){ // for each audio input channel
			// create event for noise filtering
			sprintf(temp,"REC_NOISE_FILTER_%i",i);
			ofAddListener(gui.createEventGroup(temp), this, &gamuzaMain::recordNoise);
		}
	}
	
	// Arduino GUI
	if(arduinoActivated){
		// Serial GUI [Arduino] ANALOG PINS
		for(unsigned int i=0;i<6;i++){
			sprintf(temp,"ARDUINO_ANALOG_PIN_%i_MODE",i);
			ofAddListener(gui.createEventGroup(temp),this, &gamuzaMain::changeAnalogPinMode);
		}
	
		// Serial GUI [Arduino] DIGITAL PINS
		for(unsigned int i=2;i<14;i++){
			sprintf(temp,"ARDUINO_DIGITAL_PIN_%i_MODE",i);
			ofAddListener(gui.createEventGroup(temp),this, &gamuzaMain::changeDigitalPinMode);
			sprintf(temp,"ARDUINO_DIGITAL_PIN_%i_VALUE",i);
			ofAddListener(gui.createEventGroup(temp),this, &gamuzaMain::sendDigitalValue);
		}
	}
	
	// create event for reset output texture warping points
	sprintf(temp,"RESET_FBO_MAPPING_COORDS");
	ofAddListener(gui.createEventGroup(temp), this, &gamuzaMain::resetMappingPoints);
	//////////////////////////////////////////////
	
	// last line of gamuza setup
	flagSystemLoaded = true;
	ofSeedRandom();
	
}

//--------------------------------------------------------------
void gamuzaMain::setupStringVec(){
	
	// vector<string> of bgSubTechniques list
	bgSubTechniques.push_back("color abs");
	bgSubTechniques.push_back("b&w abs");
	bgSubTechniques.push_back("lighter than + FD");
	bgSubTechniques.push_back("lighter than");
	bgSubTechniques.push_back("darker than + FD");
	bgSubTechniques.push_back("darker than");
	// vector<string> of erodeDilateuse list
	erodeDilateUse.push_back("off");
	erodeDilateUse.push_back("erode");
	erodeDilateUse.push_back("dilate");
	erodeDilateUse.push_back("erode + dilate");
	// vector<string> of sourceFlipUse list
	sourceFlipUse.push_back("off");
	sourceFlipUse.push_back("vertical");
	sourceFlipUse.push_back("horizontal");
	sourceFlipUse.push_back("vertical + horizontal");
	// vector<string> of inputColorCorrection list
	inputColorCorrection.push_back("off");
	inputColorCorrection.push_back("apply bright./contr.");
	inputColorCorrection.push_back("apply gamma");
	inputColorCorrection.push_back("apply all");
	// vector<string> of cfDetailUse list
	cfDetailUse.push_back("raw");
	cfDetailUse.push_back("smooth");
	cfDetailUse.push_back("simple");
	// vector<string> of calibScreenUse list
	calibScreenUse.push_back("black");
	calibScreenUse.push_back("cross");
	calibScreenUse.push_back("white");
	calibScreenUse.push_back("test pattern color");
	calibScreenUse.push_back("test pattern b&w");
	calibScreenUse.push_back("webcam");
	// vector<string> of ardDigitalPinModes list
	ardDigitalPinModes.push_back("input");
	ardDigitalPinModes.push_back("output");
	// vector<string> of ardDigitalPinModesPWM list
	ardDigitalPinModesPWM.push_back("input");
	ardDigitalPinModesPWM.push_back("output");
	ardDigitalPinModesPWM.push_back("pwm");
	// vector<string> of ardAnalogPinModes list
	ardAnalogPinModes.push_back("report on");
	ardAnalogPinModes.push_back("report off");
	
}

//--------------------------------------------------------------
void gamuzaMain::updateGui(){
	
	char xml_name[256];
	
	//////////////////////////////////////////////
	// FBO Mapping texture preview & control
	sprintf(xml_name,"COMPUTE_FBO_TEXTURE");
	computeFBOTexture = gui.getValueI(xml_name);
	sprintf(xml_name,"USE_SECONDARY_SCREEN");
	useSecondaryScreen = gui.getValueI(xml_name);
	sprintf(xml_name,"DRAW_FBO_GRID");
	drawGrid = gui.getValueI(xml_name);
	//////////////////////////////////////////////
	sprintf(xml_name,"DRAW_FBO_CALIB_SCREEN");
	calibrationScreen = gui.getValueI(xml_name);
	sprintf(xml_name,"FBO_CALIB_SCREEN_SEL");
	calibScreenSel = gui.getValueI(xml_name);
	//////////////////////////////////////////////
	if(useShader){
		sprintf(xml_name,"OUTPUT_FBO_GAMMA");
		fbo_gammaCorrection = gui.getValueF(xml_name);
		sprintf(xml_name,"OUTPUT_FBO_BRIGHTNESS");
		fbo_brightness = gui.getValueF(xml_name);
		sprintf(xml_name,"OUTPUT_FBO_SATURATION");
		fbo_saturation = gui.getValueF(xml_name);
		sprintf(xml_name,"OUTPUT_FBO_CONTRAST");
		fbo_contrast = gui.getValueF(xml_name);
		//////////////////////////////////////////////
		sprintf(xml_name,"OUTPUT_FBO_FILMBLEACH");
		fbo_filmBleach = gui.getValueF(xml_name);
		sprintf(xml_name,"OUTPUT_FBO_FILM_TECHNICOLOR");
		fbo_techniColor = gui.getValueF(xml_name);
		sprintf(xml_name,"OUTPUT_FBO_FILM_WHITEDIFFUSION");
		fbo_whiteDiffusion = gui.getValueF(xml_name);
		sprintf(xml_name,"OUTPUT_FBO_FILM_EXPOSURE");
		fbo_exposure = gui.getValueF(xml_name);
		sprintf(xml_name,"OUTPUT_FBO_FILM_DIFFUSION");
		fbo_diffusion = gui.getValueF(xml_name);
	}
	//////////////////////////////////////////////
	sprintf(xml_name,"USE_VSYNC");
	useVSync = gui.getValueI(xml_name);
	//////////////////////////////////////////////
	sprintf(xml_name,"USE_LIVECODING");
	useLiveCoding = gui.getValueI(xml_name);
	//////////////////////////////////////////////
	if(audioActivated && audioOutputChannels > 0){
		sprintf(xml_name,"GAMUZA_MAIN_OUTPUT_VOLUME");
		mainVolume = gui.getValueF(xml_name);
	}
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// OpenNI SensorKinect GUI
	if(openniActivated){
		sprintf(xml_name,"COMPUTE_SENSOR_KINECT");
		sensorKinect.useKinect = gui.getValueI(xml_name);
		sprintf(xml_name,"USE_TRACKING_HANDS_SENSOR_KINECT");
		sensorKinect.isTrackingHands = gui.getValueI(xml_name);
		sprintf(xml_name,"NEAR_THRESHOLD_SENSOR_KINECT");
		sensorKinect.nearThreshold = gui.getValueF(xml_name);
		sprintf(xml_name,"FAR_THRESHOLD_SENSOR_KINECT");
		sensorKinect.farThreshold = gui.getValueF(xml_name);
		//////////////////////////////////////////////
		sprintf(xml_name,"CI_BLUR_SENSOR_KINECT");
		sensorKinect.ciBlur = gui.getValueI(xml_name);
		sprintf(xml_name,"CI_USE_ERODE_DILATE_SENSOR_KINECT");
		sensorKinect.ciErodeDilateUse = gui.getValueI(xml_name);
		sprintf(xml_name,"CI_ERODE_SENSOR_KINECT");
		sensorKinect.ciErode = gui.getValueF(xml_name);
		sprintf(xml_name,"CI_DILATE_SENSOR_KINECT");
		sensorKinect.ciDilate = gui.getValueF(xml_name);
		sprintf(xml_name,"M_THRESHOLD_SENSOR_KINECT");
		sensorKinect.mThreshold = gui.getValueI(xml_name);
		sprintf(xml_name,"MOTION_ON_HORIZON_SENSOR_KINECT");
		sensorKinect.onHorizon = gui.getValueI(xml_name);
		sprintf(xml_name,"MOTION_OFF_HORIZON_SENSOR_KINECT");
		sensorKinect.offHorizon = gui.getValueI(xml_name);	
		sprintf(xml_name,"USE_KALMAN_FILTER_SENSOR_KINECT");
		sensorKinect.useKalmanFilter = gui.getValueI(xml_name);
		sprintf(xml_name,"SMOOTHING_FACTOR_SENSOR_KINECT");
		sensorKinect._smoothingFactor = gui.getValueF(xml_name);
		//////////////////////////////////////////////
		sprintf(xml_name,"MIN_BLOB_AREA_SENSOR_KINECT");
		sensorKinect.minBlobArea = gui.getValueI(xml_name);
		sprintf(xml_name,"MAX_BLOB_AREA_SENSOR_KINECT");
		sensorKinect.maxBlobArea = gui.getValueI(xml_name);
		sprintf(xml_name,"CF_DETAIL_SENSOR_KINECT");
		sensorKinect.cfDetail = gui.getValueI(xml_name);
		sprintf(xml_name,"CF_SMOOTH_PCT_SENSOR_KINECT");
		sensorKinect.smoothPct = gui.getValueF(xml_name);
		sprintf(xml_name,"CF_TOLERANCE_SENSOR_KINECT");
		sensorKinect.tolerance = gui.getValueF(xml_name);
		sprintf(xml_name,"COMPUTE_CF_SENSOR_KINECT");
		sensorKinect.computeContourFinder = gui.getValueI(xml_name);
		sprintf(xml_name,"COMPUTE_CG_SENSOR_KINECT");
		sensorKinect.computeContourGeometry = gui.getValueI(xml_name);
		sprintf(xml_name,"COMPUTE_OF_SENSOR_KINECT");
		sensorKinect.computeOpticalFlow = gui.getValueI(xml_name);
		sprintf(xml_name,"COMPUTE_TA_SENSOR_KINECT");
		sensorKinect.computeTriggerAreas = gui.getValueI(xml_name);
	}
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// Video Tracking multi-panel GUI
	if(trackingActivated){
		for(unsigned int i=0;i<numCamInputs;i++){
			sprintf(xml_name,"USE_DEVICE_%i",camDevices[i]);
			inputCam[i].captureVideo = gui.getValueI(xml_name);
			//////////////////////////////////////////////
			sprintf(xml_name,"FLIP_SOURCE_USE_%i",camDevices[i]);
			inputCam[i]._sourceFlipUse = gui.getValueI(xml_name);
			//////////////////////////////////////////////
			sprintf(xml_name,"UNDISTORT_INPUT_%i",camDevices[i]);
			inputCam[i].undistortInput = gui.getValueI(xml_name);
			//////////////////////////////////////////////
			sprintf(xml_name,"INPUT_BLUR_%i",camDevices[i]);
			inputCam[i].colorImgBlur = gui.getValueI(xml_name);
			sprintf(xml_name,"SOURCE_COLOR_CORRECTION_USE_%i",camDevices[i]);
			inputCam[i].colorCorrectionUse = gui.getValueI(xml_name);
			sprintf(xml_name,"CONTRAST_%i",camDevices[i]);
			inputCam[i].contrast = gui.getValueF(xml_name);
			sprintf(xml_name,"BRIGHTNESS_%i",camDevices[i]);
			inputCam[i].brightness = gui.getValueF(xml_name);
			sprintf(xml_name,"GAMMA_%i",camDevices[i]);
			inputCam[i].gamma = gui.getValueF(xml_name);
			//////////////////////////////////////////////
			sprintf(xml_name,"USE_COLOR_TRACKING_%i",camDevices[i]);
			inputCam[i].useHsvTracking = gui.getValueI(xml_name);
			sprintf(xml_name,"HUE_%i",camDevices[i]);
			inputCam[i].hue = gui.getValueF(xml_name);
			sprintf(xml_name,"HUE_WIDTH_%i",camDevices[i]);
			inputCam[i].hueWidth = gui.getValueF(xml_name);
			sprintf(xml_name,"SAT_%i",camDevices[i]);
			inputCam[i].sat = gui.getValueF(xml_name);
			sprintf(xml_name,"SAT_WIDTH_%i",camDevices[i]);
			inputCam[i].satWidth = gui.getValueF(xml_name);
			sprintf(xml_name,"VAL_%i",camDevices[i]);
			inputCam[i].val = gui.getValueF(xml_name);
			sprintf(xml_name,"VAL_WIDTH_%i",camDevices[i]);
			inputCam[i].valWidth = gui.getValueF(xml_name);
			//////////////////////////////////////////////
			sprintf(xml_name,"MOTION_ON_HORIZON_%i",camDevices[i]);
			inputCam[i].onHorizon = gui.getValueI(xml_name);
			sprintf(xml_name,"MOTION_OFF_HORIZON_%i",camDevices[i]);
			inputCam[i].offHorizon = gui.getValueI(xml_name);
			//////////////////////////////////////////////
			sprintf(xml_name,"BGSUB_TECH_%i",camDevices[i]);
			inputCam[i].bgSubTech = gui.getValueI(xml_name);
			sprintf(xml_name,"BGSUB_THRESHOLD_%i",camDevices[i]);
			inputCam[i].threshold = gui.getValueI(xml_name);
			sprintf(xml_name,"BG_USE_STRECH_%i",camDevices[i]);
			inputCam[i].bgUseContrastStrech = gui.getValueI(xml_name);
			sprintf(xml_name,"BGSUB_BLUR_%i",camDevices[i]);
			inputCam[i].bgSubBlur = gui.getValueI(xml_name);
			sprintf(xml_name,"BG_USE_ERODE_DILATE_%i",camDevices[i]);
			inputCam[i].bgErodeDilateUse = gui.getValueI(xml_name);
			sprintf(xml_name,"BGSUB_ERODE_%i",camDevices[i]);
			inputCam[i].bgSubErode = gui.getValueI(xml_name);
			sprintf(xml_name,"BGSUB_DILATE_%i",camDevices[i]);
			inputCam[i].bgSubDilate = gui.getValueI(xml_name);
			//////////////////////////////////////////////
			sprintf(xml_name,"HSV_BLUR_%i",camDevices[i]);
			inputCam[i].hsvBlur = gui.getValueI(xml_name);
			sprintf(xml_name,"HSV_USE_ERODE_DILATE_%i",camDevices[i]);
			inputCam[i].hsvErodeDilateUse = gui.getValueI(xml_name);
			sprintf(xml_name,"HSV_ERODE_%i",camDevices[i]);
			inputCam[i].hsvErode = gui.getValueI(xml_name);
			sprintf(xml_name,"HSV_DILATE_%i",camDevices[i]);
			inputCam[i].hsvDilate = gui.getValueI(xml_name);
			//////////////////////////////////////////////
			sprintf(xml_name,"M_THRESHOLD_%i",camDevices[i]);
			inputCam[i].mThreshold = gui.getValueI(xml_name);
			sprintf(xml_name,"M_NOISE_COMP_%i",camDevices[i]);
			inputCam[i].mNoiseComp = gui.getValueI(xml_name);
			//////////////////////////////////////////////
			sprintf(xml_name,"BT_BLUR_%i",camDevices[i]);
			inputCam[i].btBlur = gui.getValueI(xml_name);
			sprintf(xml_name,"BT_USE_ERODE_DILATE_%i",camDevices[i]);
			inputCam[i].btErodeDilateUse = gui.getValueI(xml_name);
			sprintf(xml_name,"BT_ERODE_%i",camDevices[i]);
			inputCam[i].btErode = gui.getValueI(xml_name);
			sprintf(xml_name,"BT_DILATE_%i",camDevices[i]);
			inputCam[i].btDilate = gui.getValueI(xml_name);
			sprintf(xml_name,"BT_BALANCE1_%i",camDevices[i]);
			inputCam[i].balance1 = gui.getValueI(xml_name);
			sprintf(xml_name,"BT_BALANCE2_%i",camDevices[i]);
			inputCam[i].balance2 = gui.getValueI(xml_name);
			//////////////////////////////////////////////
			sprintf(xml_name,"MIN_BLOB_AREA_%i",camDevices[i]);
			inputCam[i].minBlobArea = gui.getValueI(xml_name);
			sprintf(xml_name,"MAX_BLOB_AREA_%i",camDevices[i]);
			inputCam[i].maxBlobArea = gui.getValueI(xml_name) + inputCam[i].minBlobArea;
			sprintf(xml_name,"CF_DETAIL_%i",camDevices[i]);
			inputCam[i].cfDetail = gui.getValueI(xml_name);
			sprintf(xml_name,"CF_SMOOTH_PCT_%i",camDevices[i]);
			inputCam[i].smoothPct = gui.getValueF(xml_name);
			sprintf(xml_name,"CF_TOLERANCE_%i",camDevices[i]);
			inputCam[i].tolerance = gui.getValueF(xml_name);
			//////////////////////////////////////////////
			sprintf(xml_name,"COMPUTE_CF_%i",camDevices[i]);
			inputCam[i].computeContourFinder = gui.getValueI(xml_name);
			sprintf(xml_name,"COMPUTE_CG_%i",camDevices[i]);
			inputCam[i].computeContourGeometry = gui.getValueI(xml_name);
			sprintf(xml_name,"COMPUTE_OF_%i",camDevices[i]);
			inputCam[i].computeOpticalFlow = gui.getValueI(xml_name);
			sprintf(xml_name,"COMPUTE_FT_%i",camDevices[i]);
			inputCam[i].computeHaarFinder = gui.getValueI(xml_name);
			sprintf(xml_name,"COMPUTE_TA_%i",camDevices[i]);
			inputCam[i].computeTriggerAreas = gui.getValueI(xml_name);
			//////////////////////////////////////////////
			sprintf(xml_name,"USE_KALMAN_FILTER_%i",camDevices[i]);
			inputCam[i].useKalmanFilter = gui.getValueI(xml_name);
			sprintf(xml_name,"SMOOTHING_FACTOR_%i",camDevices[i]);
			inputCam[i]._smoothingFactor = gui.getValueF(xml_name);
			//////////////////////////////////////////////
			
			inputCam[i].drawInfoGraphics = !gui.minimize;
		
		}
	}
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// Audio Source multi-panel GUI
	if(audioActivated){
		for(unsigned int i=0;i<audioInputChannels;i++){
			sprintf(xml_name,"USE_AUDIO_INPUT_%i",i);
			inputAudioCH[i].captureAudio = gui.getValueI(xml_name);
			//////////////////////////////////////////////
			sprintf(xml_name,"VOLUME_AUDIO_INPUT_%i",i);
			inputAudioCH[i]._volume = gui.getValueF(xml_name);
			sprintf(xml_name,"MUTE_AUDIO_INPUT_%i",i);
			inputAudioCH[i]._mute = gui.getValueI(xml_name);
			//////////////////////////////////////////////
			sprintf(xml_name,"NOISE_REDUX_FACTOR_%i",i);
			inputAudioCH[i].reduxFactor = gui.getValueF(xml_name);
			//////////////////////////////////////////////
			sprintf(xml_name,"EQ_BAND1_CENTER_%i",i);
			inputAudioCH[i].eqLOWCenter = gui.getValueF(xml_name);
			sprintf(xml_name,"EQ_BAND1_AMPLITUDE_%i",i);
			inputAudioCH[i].eqLOWAmplitude = gui.getValueF(xml_name);
			sprintf(xml_name,"EQ_BAND1_WIDTH_%i",i);
			inputAudioCH[i].eqLOWWidth = gui.getValueF(xml_name);
			//////////////////////////////////////////////
			sprintf(xml_name,"EQ_BAND2_CENTER_%i",i);
			inputAudioCH[i].eqMIDCenter = gui.getValueF(xml_name);
			sprintf(xml_name,"EQ_BAND2_AMPLITUDE_%i",i);
			inputAudioCH[i].eqMIDAmplitude = gui.getValueF(xml_name);
			sprintf(xml_name,"EQ_BAND2_WIDTH_%i",i);
			inputAudioCH[i].eqMIDWidth = gui.getValueF(xml_name);
			//////////////////////////////////////////////
			sprintf(xml_name,"EQ_BAND3_CENTER_%i",i);
			inputAudioCH[i].eqHICenter = gui.getValueF(xml_name);
			sprintf(xml_name,"EQ_BAND3_AMPLITUDE_%i",i);
			inputAudioCH[i].eqHIAmplitude = gui.getValueF(xml_name);
			sprintf(xml_name,"EQ_BAND3_WIDTH_%i",i);
			inputAudioCH[i].eqHIWidth = gui.getValueF(xml_name);
			//////////////////////////////////////////////
			sprintf(xml_name,"USE_KALMAN_FILTER_AUDIOCH_%i",i);
			inputAudioCH[i].useKalmanFilter = gui.getValueI(xml_name);
			sprintf(xml_name,"SMOOTHING_FACTOR_AUDIOCH_%i",i);
			inputAudioCH[i]._smoothingFactor = gui.getValueF(xml_name);
			
			inputAudioCH[i].drawGraphics = !gui.minimize;
		
		}
	}
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// Serial GUI [Arduino]
	if(arduinoActivated){
		sprintf(xml_name,"USE_ARDUINO");
		useArduino = gui.getValueI(xml_name);
		//////////////////////////////////////////////
		sprintf(xml_name,"USE_KALMAN_FILTER_ARDUINO");
		useKalmanFilterArduino = gui.getValueI(xml_name);
		sprintf(xml_name,"SMOOTHING_FACTOR_ARDUINO");
		arduino_smoothingFactor = gui.getValueF(xml_name);
		
	}
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// OSC GUI [Open Sound Control Output DATA]
	if(oscActivated){
		sprintf(xml_name,"USE_OSC");
		useOsc = gui.getValueI(xml_name);
		//////////////////////////////////////////////
		if(openniActivated){
			sprintf(xml_name,"SENSOR_KINECT_SEND_OSC_BD");
			sensorKinect.sendOsc_BD = gui.getValueI(xml_name);
			sprintf(xml_name,"SENSOR_KINECT_SEND_OSC_CF");
			sensorKinect.sendOsc_CF = gui.getValueI(xml_name);
			sprintf(xml_name,"SENSOR_KINECT_SEND_OSC_CG");
			sensorKinect.sendOsc_CG = gui.getValueI(xml_name);
			sprintf(xml_name,"SENSOR_KINECT_SEND_OSC_OF");
			sensorKinect.sendOsc_OF = gui.getValueI(xml_name);
			sprintf(xml_name,"SENSOR_KINECT_SEND_OSC_TA");
			sensorKinect.sendOsc_TA = gui.getValueI(xml_name);
			sprintf(xml_name,"SENSOR_KINECT_SEND_OSC_HT");
			sensorKinect.sendOsc_HT = gui.getValueI(xml_name);
			sprintf(xml_name,"SENSOR_KINECT_SEND_OSC_HW");
			sensorKinect.sendOsc_HW = gui.getValueI(xml_name);
		}
		//////////////////////////////////////////////
		if(trackingActivated){
			for(unsigned int i=0;i<numCamInputs;i++){
				sprintf(xml_name,"WEBCAM_ID%i_SEND_OSC_MD",camDevices[i]);
				inputCam[i].sendOsc_MD = gui.getValueI(xml_name);
				sprintf(xml_name,"WEBCAM_ID%i_SEND_OSC_BD",camDevices[i]);
				inputCam[i].sendOsc_BD = gui.getValueI(xml_name);
				sprintf(xml_name,"WEBCAM_ID%i_SEND_OSC_CF",camDevices[i]);
				inputCam[i].sendOsc_CF = gui.getValueI(xml_name);
				sprintf(xml_name,"WEBCAM_ID%i_SEND_OSC_CG",camDevices[i]);
				inputCam[i].sendOsc_CG = gui.getValueI(xml_name);
				sprintf(xml_name,"WEBCAM_ID%i_SEND_OSC_OF",camDevices[i]);
				inputCam[i].sendOsc_OF = gui.getValueI(xml_name);
				sprintf(xml_name,"WEBCAM_ID%i_SEND_OSC_HF",camDevices[i]);
				inputCam[i].sendOsc_HF = gui.getValueI(xml_name);
				sprintf(xml_name,"WEBCAM_ID%i_SEND_OSC_TA",camDevices[i]);
				inputCam[i].sendOsc_TA = gui.getValueI(xml_name);
		}
		}
		//////////////////////////////////////////////
		if(audioActivated){
			for(unsigned int i=0;i<audioInputChannels;i++){
				sprintf(xml_name,"AUDIO_INPUT_CH%i_SEND_OSC_VD",i);
				inputAudioCH[i].sendOsc_VD = gui.getValueI(xml_name);
				sprintf(xml_name,"AUDIO_INPUT_CH%i_SEND_OSC_PD",i);
				inputAudioCH[i].sendOsc_PD = gui.getValueI(xml_name);
				sprintf(xml_name,"AUDIO_INPUT_CH%i_SEND_OSC_BK",i);
				inputAudioCH[i].sendOsc_BK = gui.getValueI(xml_name);
			}
		}
		//////////////////////////////////////////////
		if(arduinoActivated){
			sprintf(xml_name,"ARDUINO_SEND_OSC_AAP");
			sendOsc_AAP = gui.getValueI(xml_name);
			sprintf(xml_name,"ARDUINO_SEND_OSC_ADP");
			sendOsc_ADP = gui.getValueI(xml_name);
		}
		
	}
	//////////////////////////////////////////////
	
}

//--------------------------------------------------------------
void gamuzaMain::drawGui(){
	
	char temp[128];
	string temp1;
	ostringstream temp2;
	
	glColor3f(1.0,1.0,1.0);
	ofNoFill();
	ofRect(0,0,mainScreenW-1,mainScreenH-1);
	
	////////////////////////////////////
	// panel tabs colorize
	if(!gui.minimize){
		ofEnableAlphaBlending();
		
		ofFill();
		// live Coding
		glColor4f(1.0,1.0,1.0,1.0);
		ofRect(gui.panelTabs[0].x+1, gui.panelTabs[0].y+1, gui.panelTabs[0].width-2, gui.panelTabs[0].height-2);
		// sensor kinect
		if(openniActivated){
			glColor4f(0.0,1.0,0.0,1.0);
			ofRect(gui.panelTabs[1].x+1, gui.panelTabs[1].y+1, gui.panelTabs[1].width-2, gui.panelTabs[1].height-2);
		}
		// webcams
		if(trackingActivated){
			glColor4f(0.847,0.25,0.25,1.0);
			for(unsigned int i=0;i<numCamInputs;i++){
				ofRect(gui.panelTabs[webcamsStartIndex+i].x+1, gui.panelTabs[webcamsStartIndex+i].y+1, gui.panelTabs[webcamsStartIndex+i].width-2, gui.panelTabs[webcamsStartIndex+i].height-2);
			}
		}
		// audio input
		if(audioActivated){
			glColor4f(1.0,0.906,0.463,1.0);
			for(unsigned int i=0;i<audioInputChannels;i++){
				ofRect(gui.panelTabs[audioInputStartIndex+i].x+1, gui.panelTabs[audioInputStartIndex+i].y+1, gui.panelTabs[audioInputStartIndex+i].width-2, gui.panelTabs[audioInputStartIndex+i].height-2);
			}
		}
		// arduino
		if(arduinoActivated){
			glColor4f(0.0,0.2,1.0,1.0);
			ofRect(gui.panelTabs[arduinoIndex].x+1, gui.panelTabs[arduinoIndex].y+1, gui.panelTabs[arduinoIndex].width-2, gui.panelTabs[arduinoIndex].height-2);
		}
		// osc
		if(oscActivated){
			glColor4f(0.4,0.4,0.4,1.0);
			ofRect(gui.panelTabs[oscIndex].x+1, gui.panelTabs[oscIndex].y+1, gui.panelTabs[oscIndex].width-2, gui.panelTabs[oscIndex].height-2);
		}
		// credits
		glColor4f(1.0,1.0,1.0,1.0);
		ofRect(gui.panelTabs[gui.panelTabs.size()-1].x+1, gui.panelTabs[gui.panelTabs.size()-1].y+1, gui.panelTabs[gui.panelTabs.size()-1].width-2, gui.panelTabs[gui.panelTabs.size()-1].height-2);
		
		ofDisableAlphaBlending();
	}
	////////////////////////////////////
	
	glPushMatrix();
	glTranslatef(guiPosX, guiPosY, 0.0f);
	
	////////////////////////////////////
	// MAIN TITLE
	sprintf(temp," GAMUZA[0.3] | <http://www.d3cod3.cc> | <http://www.d3cod3.org>");
	ofSetColor(216, 64, 64);
	fontSmall.drawString(temp,8,14);
	ofSetColor(255, 255, 255);
	////////////////////////////////////
	
	//////////////////////////////////////////////
	// OPENNI
	if(openniActivated){
		if(gui.getSelectedPanel() == sensorKinectIndex && !gui.minimize){
			if(sensorKinect.useKinect){
				sensorKinect.drawDevice();
				if(sensorKinect.computeContourFinder && sensorKinect.computeTriggerAreas){
					temp2 << "trigger areas: ";
					for(unsigned int t=0;t<TRIGGER_AREAS_NUM;t++){
						temp2 << t;
						temp2 << "[";
						sprintf(temp,"%i",sensorKinect.triggerState[t]);
						temp2 << temp;
						temp2 << "] ";
					}
					temp2 << endl;
					temp1 = temp2.str();
					sprintf(temp,"%s",temp1.c_str());
					glColor3f(1.0,1.0,1.0);
					fontSmall.drawString(temp, 370,334);
				}
			}else{
				ofEnableAlphaBlending();
				glColor4f(0.0,0.0,0.0,0.7);
				ofFill();
				ofRect(11,42,gui.boundingBox.width - gui.borderWidth*2 - 2, gui.boundingBox.height - gui.topSpacing*3 - 1);
				ofDisableAlphaBlending();
			}
		}
	}
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// Video Tracking Additional GUI
	if(trackingActivated){
		for(unsigned int i=0;i<numCamInputs;i++){
			sprintf(temp," CAM Device [%i] | %s",camDevices[i],deviceNames[i].c_str());
			if(gui.getSelectedPanelName() == temp){
				if(inputCam[gui.getSelectedPanel()-webcamsStartIndex].captureVideo){
					inputCam[gui.getSelectedPanel()-webcamsStartIndex].draw();
					if(inputCam[gui.getSelectedPanel()-webcamsStartIndex].drawInfoGraphics){
						sprintf(temp,"motion quantity : %i",inputCam[gui.getSelectedPanel()-webcamsStartIndex].numPixelsChanged);
						glColor3f(1.0,1.0,1.0);
						fontSmall.drawString(temp, 30, 550);
						if(inputCam[gui.getSelectedPanel()-webcamsStartIndex].computeContourFinder && inputCam[gui.getSelectedPanel()-webcamsStartIndex].computeTriggerAreas){
							temp2 << "trigger areas: ";
							for(unsigned int t=0;t<TRIGGER_AREAS_NUM;t++){
								temp2 << t;
								temp2 << "[";
								sprintf(temp,"%i",inputCam[gui.getSelectedPanel()-webcamsStartIndex].triggerState[t]);
								temp2 << temp;
								temp2 << "] ";
							}
							temp2 << endl;
							temp1 = temp2.str();
							sprintf(temp,"%s",temp1.c_str());
							fontSmall.drawString(temp, 370,340);
						}
					}
				}else{
					ofEnableAlphaBlending();
						glColor4f(0.0,0.0,0.0,0.7);
						ofFill();
						ofRect(11,42,gui.boundingBox.width - gui.borderWidth*2 - 2, gui.boundingBox.height - gui.topSpacing*3 - 1);
					ofDisableAlphaBlending();
				}
			}
		}
	}
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// AUDIO input  Additional GUI
	if(audioActivated){
		for(unsigned int i=0;i<audioInputChannels;i++){
			sprintf(temp," AUDIO input [channel %i]",i);
			if(gui.getSelectedPanelName() == temp && inputAudioCH[i].drawGraphics){
				if(inputAudioCH[i].noiseRec){
					ofSetColor(216, 64, 64);
					fontSmall.drawString("recording noise", 882,102);
					fontSmall.drawString("recording | click to stop", 106,245);
				}
				inputAudioCH[i].drawChannel(463,47,320,120);
				if(!inputAudioCH[i].captureAudio){
					ofEnableAlphaBlending();
					glColor4f(0.0,0.0,0.0,0.7);
					ofFill();
					ofRect(11,42,gui.boundingBox.width - gui.borderWidth*2 - 2, gui.boundingBox.height - gui.topSpacing*3 - 1);
					ofDisableAlphaBlending();
				}
			}
		}
		sprintf(temp," live coding [second screen fullscreen output @ %i x %i]",projectionScreenW,projectionScreenH);
		if(audioOutputChannels > 0 && gui.getSelectedPanelName() == temp){
			// draw audio output scope (multichannel mix)
			float stretch = 380 / (float)((audioBufferSize*audioOutputChannels) - 1);
			glPushMatrix();	
				glTranslatef(950, 676, 0.0f);
				glEnable(GL_BLEND);
				glColor4f(0.3f,0.3f,0.3f,0.15f);
				ofNoFill();
				glLineWidth(1.0f);
				ofBeginShape();
				for (unsigned int i = 0; i < (audioBufferSize*audioOutputChannels)-1; i++){
					ofVertex(i * stretch, hardClip(outputBufferCopy[i]) * 20);
					ofVertex((i + 1) * stretch, hardClip(outputBufferCopy[i+1]) * 20);
				}
				ofEndShape();
				glDisable(GL_BLEND);
			glPopMatrix();
		}
	}
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// Serial GUI [Arduino]
	if(arduinoActivated){
		if(gui.getSelectedPanel() == arduinoIndex && !gui.minimize){
			if(!useArduino){
				ofEnableAlphaBlending();
				glColor4f(0.0,0.0,0.0,0.7);
				ofFill();
				ofRect(11,42,gui.boundingBox.width - gui.borderWidth*2 - 2, gui.boundingBox.height - gui.topSpacing*3 - 1);
				ofDisableAlphaBlending();
			}
		}
	}
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// OSC GUI [Open Sound Control Output DATA]
	if(oscActivated){
		if(gui.getSelectedPanel() == oscIndex && !gui.minimize){
			if(!useOsc){
				ofEnableAlphaBlending();
				glColor4f(0.0,0.0,0.0,0.7);
				ofFill();
				ofRect(11,42,gui.boundingBox.width - gui.borderWidth*2 - 2, gui.boundingBox.height - gui.topSpacing*3 - 1);
				ofDisableAlphaBlending();
			}
		}
	}
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// Gamuza Credits Additional GUI
	if(gui.getSelectedPanel() == gui.panelTabs.size()-1){
		// Credits background
		ofEnableAlphaBlending();
		glColor4f(1.0,1.0,1.0,0.9);
		splashImage.draw((MAIN_WINDOW_W/2) - 256,(MAIN_WINDOW_H/2) - 153);
		ofDisableAlphaBlending();
	}else{
		// MAIN background
		ofEnableAlphaBlending();
		glColor4f(1.0,1.0,1.0,0.2);
		gamuzaLogo.draw((MAIN_WINDOW_W/2.0) - 216,(MAIN_WINDOW_H/2) - 215);
		ofDisableAlphaBlending();
	}
	//////////////////////////////////////////////
	
	glPopMatrix();
}

//--------------------------------------------------------------
void gamuzaMain::gamuzaFullscreen(){
	
	ofToggleFullscreen();
	if(isFullscreen){
		isFullscreen = false;
		if(projectionScreenW < MAIN_WINDOW_W){
			lcPrevW	 = (projectionScreenW*MAIN_WINDOW_H)/projectionScreenH;
			lcPrevH	 = MAIN_WINDOW_H;
			lcPrevX	 = (MAIN_WINDOW_W-lcPrevW)/2.0;
			lcPrevY  = 0;
		}else{
			lcPrevW	 = MAIN_WINDOW_W;
			lcPrevH	 = (projectionScreenH*MAIN_WINDOW_W)/projectionScreenW;
			lcPrevX	 = 0;
			lcPrevY  = (MAIN_WINDOW_H-lcPrevH)/2.0;
		}
		guiPosX = 0.0;
		guiPosY = 0.0;
		gui.setPosition(guiPosX, guiPosY);
		gui.setDimensions(MAIN_WINDOW_W-2, MAIN_WINDOW_H-6);
	}else{
		isFullscreen = true;
		if(projectionScreenW < mainScreenW){
			lcPrevW	 = (projectionScreenW*mainScreenH)/projectionScreenH;
			lcPrevH	 = mainScreenH;
			lcPrevX	 = (mainScreenW-lcPrevW)/2.0;
			lcPrevY  = 0;
		}else{
			lcPrevW	 = mainScreenW;
			lcPrevH	 = (projectionScreenH*mainScreenW)/projectionScreenW;
			lcPrevX	 = 0;
			lcPrevY  = (mainScreenH-lcPrevH)/2.0;
		}
		guiPosX = ((mainScreenW - MAIN_WINDOW_W)/2.0) + 1.0;
		guiPosY = ((mainScreenH - MAIN_WINDOW_H)/2.0) + 3.0;
		gui.setPosition(guiPosX, guiPosY);
		gui.setDimensions(MAIN_WINDOW_W-2, MAIN_WINDOW_H-6);
	}
	
}

//--------------------------------------------------------------
void gamuzaMain::resetWarpingPoints(int actualPanel){
	
	inputCam[actualPanel].sourceFrame.points[0].x = 0.0f;
	inputCam[actualPanel].sourceFrame.points[0].y = 0.0f;
	inputCam[actualPanel].sourceFrame.points[1].x = 320.0f;
	inputCam[actualPanel].sourceFrame.points[1].y = 0.0f;
	inputCam[actualPanel].sourceFrame.points[2].x = 320.0f;
	inputCam[actualPanel].sourceFrame.points[2].y = 240.0f;
	inputCam[actualPanel].sourceFrame.points[3].x = 0.0f;
	inputCam[actualPanel].sourceFrame.points[3].y = 240.0f;
	
}

//////////////////////////////////////////////////////////////// GUI Events
//--------------------------------------------------------------
void gamuzaMain::eventsIn(guiCallbackData & data){
	
	//lets send all events to our logger
	if( !data.isElement( "events logger" ) ){
		/*string logStr = data.getXmlName();
		
		for(int k = 0; k < data.getNumValues(); k++){
			logStr += " | " + data.getString(k);
		}
		
		logger.log(OF_LOG_NOTICE, " %s", logStr.c_str());*/
		
		if(data.isElement("script examples")){
			loadScript(scriptsLister.getSelectedPath());
			liveCoding.glEditor[liveCoding.currentEditor]->ClearAllText();
			liveCoding.pasteFromLuaScript(readScript(scriptsLister.getSelectedPath(),false));
		}
		
		if(data.isElement("OPEN_SCRIPT_FILE_DIALOG")){
			openFileDialog();
			gui.setValueB("OPEN_SCRIPT_FILE_DIALOG", false);
		}
		
		if(data.isElement("SAVE_SCRIPT_FILE_DIALOG")){
			saveFileDialog();
			gui.setValueB("SAVE_SCRIPT_FILE_DIALOG", false);
		}
		
		if(data.isElement("USE_LIVECODING") && data.getInt(0) == 1){
			gui.setValueB("DRAW_FBO_CALIB_SCREEN", false);
		}
		
		if(data.isElement("DRAW_FBO_CALIB_SCREEN") && data.getInt(0) == 1){
			gui.setValueB("USE_LIVECODING", false);
		}
		
	}
	
}

//--------------------------------------------------------------
void gamuzaMain::grabBackgroundEvent(guiCallbackData & data){
	
	if(gui.getSelectedPanel() >= webcamsStartIndex && gui.getSelectedPanel() < webcamsStartIndex + numCamInputs){
		char temp[256];
		sprintf(temp,"BG_CAPTURE_%i",camDevices[gui.getSelectedPanel()-webcamsStartIndex]);
		if(data.isElement(temp) && data.getInt(0) == 1 ){
			inputCam[gui.getSelectedPanel()-webcamsStartIndex].bLearnBackground = true;
			gui.setValueB(temp, false);
		}
	}
	
}

//--------------------------------------------------------------
void gamuzaMain::resetMappingPoints(guiCallbackData & data){
	
	char temp[256];
	sprintf(temp,"RESET_FBO_MAPPING_COORDS");
	if(data.isElement(temp) && data.getInt(0) == 1 ){
		// warped grid reset
		finalTextureMapping.reset(projectionScreenW,projectionScreenH);
		gui.setValueB(temp, false);
	}
	
}

//--------------------------------------------------------------
void gamuzaMain::activateTrackingCF(guiCallbackData & data){
	
	if(gui.getSelectedPanel() >= webcamsStartIndex && gui.getSelectedPanel() < webcamsStartIndex + numCamInputs){
		char temp[256], temp2[256],temp3[256];
		sprintf(temp,"COMPUTE_CG_%i",camDevices[gui.getSelectedPanel()-webcamsStartIndex]);
		sprintf(temp2,"COMPUTE_TA_%i",camDevices[gui.getSelectedPanel()-webcamsStartIndex]);
		sprintf(temp3,"COMPUTE_CF_%i",camDevices[gui.getSelectedPanel()-webcamsStartIndex]);
		if((data.isElement(temp) || data.isElement(temp2)) && data.getInt(0) == 1 ){
			gui.setValueB(temp3, true);
		}
	}
	
	if(gui.getSelectedPanel() == sensorKinectIndex){
		char temp[256], temp2[256],temp3[256];
		sprintf(temp,"COMPUTE_CG_SENSOR_KINECT");
		sprintf(temp2,"COMPUTE_TA_SENSOR_KINECT");
		sprintf(temp3,"COMPUTE_CF_SENSOR_KINECT");
		if((data.isElement(temp) || data.isElement(temp2)) && data.getInt(0) == 1 ){
			gui.setValueB(temp3, true);
		}
	}
	
}

//--------------------------------------------------------------
void gamuzaMain::resetInputWarping(guiCallbackData & data){
	
	char temp[256];
	if(gui.getSelectedPanel() >= webcamsStartIndex && gui.getSelectedPanel() < webcamsStartIndex + numCamInputs){
		sprintf(temp,"RESET_INPUT_WARPING_DEV_%i",camDevices[gui.getSelectedPanel()-webcamsStartIndex]);
		if(data.isElement(temp) && data.getInt(0) == 1){
			resetWarpingPoints(gui.getSelectedPanel()-webcamsStartIndex);
			gui.setValueB(temp, false);
		}
	}
}

//--------------------------------------------------------------
void gamuzaMain::recordNoise(guiCallbackData & data){
	
	if(gui.getSelectedPanel() >= audioInputStartIndex && gui.getSelectedPanel() < (audioInputStartIndex+audioInputChannels)){
		char temp[256];
		sprintf(temp,"REC_NOISE_FILTER_%i",gui.getSelectedPanel()-audioInputStartIndex);
		if(data.isElement(temp) && data.getInt(0) == 1 ){
			inputAudioCH[gui.getSelectedPanel()-audioInputStartIndex].recNoiseFilter();
			gui.setValueB(temp, false);
		}
	}
	
}

//--------------------------------------------------------------
void gamuzaMain::changeAnalogPinMode(guiCallbackData & data){
	
	char temp[256];
	for(unsigned int i=0;i<6;i++){
		sprintf(temp,"ARDUINO_ANALOG_PIN_%i_MODE",i);
		if(data.isElement(temp)){
			if(atoi(data.getString(0).c_str()) == 0){
				analogPinModes[i] = ARD_ON;
			}else if(atoi(data.getString(0).c_str()) == 1){
				analogPinModes[i] = ARD_OFF;
			}
			arduino.sendAnalogPinReporting(i, analogPinModes[i]);
		}
	}
}

//--------------------------------------------------------------
void gamuzaMain::changeDigitalPinMode(guiCallbackData & data){
	
	char temp[256];
	for(unsigned int i=2;i<14;i++){
		sprintf(temp,"ARDUINO_DIGITAL_PIN_%i_MODE",i);
		if(data.isElement(temp)){
			if(i == 3 || i == 5 || i == 6 || i == 9 || i == 10 || i == 11){
				if(atoi(data.getString(0).c_str()) == 0){
					digitalPinModes[i-2] = ARD_INPUT;
				}else if(atoi(data.getString(0).c_str()) == 1){
					digitalPinModes[i-2] = ARD_OUTPUT;
				}else if(atoi(data.getString(0).c_str()) == 2){
					digitalPinModes[i-2] = ARD_PWM;
				}
			}else{
				if(atoi(data.getString(0).c_str()) == 0){
					digitalPinModes[i-2] = ARD_INPUT;
				}else if(atoi(data.getString(0).c_str()) == 1){
					digitalPinModes[i-2] = ARD_OUTPUT;
				}
			}
			arduino.sendDigitalPinMode(i, digitalPinModes[i-2]);
		}
	}
}

//--------------------------------------------------------------
void gamuzaMain::sendDigitalValue(guiCallbackData & data){
	char temp[256];
	for(unsigned int i=2;i<14;i++){
		sprintf(temp,"ARDUINO_DIGITAL_PIN_%i_VALUE",i);
		if(data.isElement(temp)){
			if(digitalPinModes[i-2] == ARD_OUTPUT){
				if(i == 3 || i == 5 || i == 6 || i == 9 || i == 10 || i == 11){
					if(data.getInt(0) == 0){
						digitalPinValuesOutput[i-2] = data.getInt(0);
					}else{
						digitalPinValuesOutput[i-2] = 1;
					}
					arduino.sendDigital(i, digitalPinValuesOutput[i-2]);
				}else{
					digitalPinValuesOutput[i-2] = data.getInt(0);
					arduino.sendDigital(i, digitalPinValuesOutput[i-2]);
				}
			}else if(digitalPinModes[i-2] == ARD_PWM){
				digitalPinValuesOutput[i-2] = data.getInt(0);
				arduino.sendPwm(i, digitalPinValuesOutput[i-2]);
			}
		}
	}
}

//////////////////////////////////////////////////////////////// GUI Events

#endif
