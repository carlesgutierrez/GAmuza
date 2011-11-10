#include "gamuzaMain.h"
#include "gamuzaGui.h"
#include "gamuzaFBO.h"
#include "gamuzaAudio.h"
#include "gamuzaArduino.h"
#include "gamuzaOSC.h"

//--------------------------------------------------------------
void gamuzaMain::setup(){

	flagSystemLoaded	= false; // first line of gamuza setup

	//////////////////////////////////////////////
	// load settings from xml
	loadGamuzaSettings();
	if(START_WITH_FULLSCREEN){
		isFullscreen = true;
	}else{
		isFullscreen = false;
	}
	// automation
	if(autoPilot){
		gamuzaFullscreen();
	}
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// init general openframeworks settings
	ofSetFrameRate(FPS);
	ofEnableSmoothing();
	ofSetLogLevel(OF_LOG_VERBOSE);
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// AUDIO
	if(audioActivated){
		gamuzaSetup.lock();
		setupAudio();
		gamuzaSetup.unlock();
	}
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// OPENNI
	if(openniActivated){
		gamuzaSetup.lock();
		sensorKinect.setupDevice(workingW,workingH,useKinectInfrared,sensorKinectLedState);
		gamuzaSetup.unlock();
	}
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// FBO second screen output texture
	gamuzaSetup.lock();
	setupFBO();
	gamuzaSetup.unlock();
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// OSC data sending @ host_number(IP),host_port
	if(oscActivated){
		gamuzaSetup.lock();
		setupOSC();
		gamuzaSetup.unlock();
	}
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// ARDUINO
	if(arduinoActivated){
		gamuzaSetup.lock();
		setupArduino();
		gamuzaSetup.unlock();
	}
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// GUI
	// include setup of webcams & audio inputs
	gamuzaSetup.lock();
	setupGui();
	gamuzaSetup.unlock();
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// SYSTEM
	currentSavedFrame = 0;
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// set log level to error only
	ofSetLogLevel(OF_LOG_ERROR);
	//////////////////////////////////////////////

}

//--------------------------------------------------------------
void gamuzaMain::update(){

	//////////////////////////////////////////////
	// INIT PROGRAM | splash image control
	if(ofGetElapsedTimeMillis() > splashWait){
		splashFinish = true;
		if(flagSystemLoaded){
			flagSystemLoaded = false;
			if(audioActivated){
				soundStream.start();
				printf("\nsoundStream STARTED\n\n");
				logger.log(99, " soundStream STARTED");
				for(unsigned int i = 0; i < audioInputChannels; i++){
					inputAudioCH[i].computeChannel = true;
				}
				computeAudioInput	= true;
				computeAudioOutput	= true;

			}
		}
		//////////////////////////////////////////////
		// Main settings control
		ofSetVerticalSync(useVSync);
		//////////////////////////////////////////////

		//////////////////////////////////////////////
		// GUI update
		updateGui();
		gui.update();
		//////////////////////////////////////////////

		//////////////////////////////////////////////
		// OPENNI
		if(openniActivated && sensorKinect.useKinect){
			sensorKinect.updateDevice();
		}
		//////////////////////////////////////////////

		//////////////////////////////////////////////
		// WEBCAM input devices update
		if(trackingActivated){
			for(unsigned int i=0;i<numCamInputs;i++){
				if(inputCam[i].captureVideo){
					inputCam[i].update();
				}
			}
		}
		//////////////////////////////////////////////

		//////////////////////////////////////////////
		// ARDUINO update
		if(arduinoActivated && useArduino){
			updateArduino();
		}
		//////////////////////////////////////////////

		//////////////////////////////////////////////
		// OSC update
		if(oscActivated && useOsc){
			updateOSC();
		}
		//////////////////////////////////////////////
	}
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// Various reference var
	gamuzaRealFPS = ofGetFrameRate();
	//////////////////////////////////////////////

}

//--------------------------------------------------------------
void gamuzaMain::draw(){

	if(splashFinish){

		ofBackground(0,0,0);
		//////////////////////////////////////////////
		// GUI draw
		if(!liveCodingMode && !autoPilot){
			gui.draw();
			drawGui();
		}
		//////////////////////////////////////////////

		//////////////////////////////////////////////
		// FBO texture draw
		drawFBO();
		//////////////////////////////////////////////

	}else{
		ofBackground(20,20,20);
		ofEnableAlphaBlending();
		if(isFullscreen){
			// image
			glColor4f(1.0,1.0,1.0,0.9);
			splashImage.draw((mainScreenW/2) - 256,(mainScreenH/2) - 153);
			// loading bar
			glColor4f(0.847,0.25,0.25,0.8);
			ofRect((mainScreenW/2) - 240,(mainScreenH/2) - 144,(ofGetElapsedTimeMillis()*1.0f/splashWait)*480.0f,6);
			ofDisableAlphaBlending();
		}else{
			// image
			glColor4f(1.0,1.0,1.0,0.9);
			splashImage.draw((MAIN_WINDOW_W/2) - 256,(MAIN_WINDOW_H/2) - 153);
			// loading bar
			glColor4f(0.847,0.25,0.25,0.8);
			ofRect((MAIN_WINDOW_W/2) - 240,(MAIN_WINDOW_H/2) - 144,(ofGetElapsedTimeMillis()*1.0f/splashWait)*480.0f,6);
			ofDisableAlphaBlending();
		}
	}

}

//--------------------------------------------------------------
void gamuzaMain::keyPressed(int key){

	// LIVE CODING
	if(computeFBOTexture && useLiveCoding){
		liveCoding.keyPressed(key);
		lua.scriptKeyPressed(key);
	}

}

//--------------------------------------------------------------
void gamuzaMain::keyReleased(int key){

	bool alt = gamuzaKmap.isAltDown();

	// fullscreen toggle
	if(alt && (key == 'f' || key == 'F')){
		gamuzaFullscreen();
	}

	// LIVE CODING
	if(alt && (key == 'j' || key == 'J')){
		liveCodingMode = !liveCodingMode;
	}

	// show/hide script code
	if(alt && (key == 'w' || key == 'W')){
		viewCode = !viewCode;
	}

	// open file dialog
	if(alt && (key == 'd' || key == 'D')){
		openFileDialog();
	}

	// save frame
	if(alt && (key == 'o' || key == 'O')){
		saveFrame();
	}

	// print frame
	if(alt && (key == 'p' || key == 'P')){
		printFrame();
	}

}

//--------------------------------------------------------------
void gamuzaMain::mouseMoved(int x, int y){

	//////////////////////////////TESTING
	//printf("%i - %i\n",x,y);
	//////////////////////////////TESTING

	// mapping grid
	if(drawGrid){
		finalTextureMapping.mouseMoved(x, y);
	}

	// Live Coding
	if(computeFBOTexture && useLiveCoding){
		lua.scriptMouseMoved(x, y);
	}

}

//--------------------------------------------------------------
void gamuzaMain::mouseDragged(int x, int y, int button){

	// gui interface
	gui.mouseDragged(x, y, button);

	if(openniActivated){
		if(gui.getSelectedPanel() == sensorKinectIndex){
			sensorKinect.mouseDragged(x-guiPosX, y-guiPosY, button);
		}
	}

	if(trackingActivated){
		if(gui.getSelectedPanel() >= webcamsStartIndex && gui.getSelectedPanel() < webcamsStartIndex + numCamInputs){
			inputCam[gui.getSelectedPanel()-webcamsStartIndex].mouseDragged(x-guiPosX, y-guiPosY, button);
		}
	}

	// Mapping
	if(drawGrid){
		finalTextureMapping.mouseDragged(x, y);
	}

	// Live Coding
	if(computeFBOTexture && useLiveCoding){
		lua.scriptMouseDragged(x, y, button);
	}

}

//--------------------------------------------------------------
void gamuzaMain::mousePressed(int x, int y, int button){

	// gui interface
	gui.mousePressed(x, y, button);

	//////////////////////////////////////////////////
	// save EXTRA GUI settings
	if(gui.saveDown && gui.getSelectedPanel() == 0){
		if(computeFBOTexture){
			saveMappingSettings = true;
		}
	}

	if(openniActivated){
		if(gui.getSelectedPanel() == sensorKinectIndex){
			sensorKinect.mousePressed(x-guiPosX, y-guiPosY, button);
			if(gui.saveDown){
				sensorKinect.saveAllSettings = true;
			}
		}
	}

	if(trackingActivated){
		if(gui.getSelectedPanel() >= webcamsStartIndex && gui.getSelectedPanel() < webcamsStartIndex + numCamInputs){
			inputCam[gui.getSelectedPanel()-webcamsStartIndex].mousePressed(x-guiPosX, y-guiPosY, button);

			if(gui.saveDown){
				inputCam[gui.getSelectedPanel()-webcamsStartIndex].saveAllSettings = true;
			}

		}
	}

	if(arduinoActivated){
		if(gui.saveDown && gui.getSelectedPanel() == arduinoIndex){
			if(useArduino){
				saveArduinoPinsState = true;
			}
		}
	}
	// save EXTRA GUI settings
	//////////////////////////////////////////////////

	// Mapping
	if(drawGrid){
		finalTextureMapping.mousePressed(x, y);
	}

	// Live Coding
	if(computeFBOTexture && useLiveCoding){
		lua.scriptMousePressed(x, y, button);
	}

}

//--------------------------------------------------------------
void gamuzaMain::mouseReleased(int x, int y, int button){

	// gui interface
	gui.mouseReleased();

	if(openniActivated){
		if(gui.getSelectedPanel() == sensorKinectIndex){
			sensorKinect.mouseReleased(x-guiPosX, y-guiPosY, button);
		}
	}

	if(trackingActivated){
		if(gui.getSelectedPanel() >= webcamsStartIndex && gui.getSelectedPanel() < webcamsStartIndex + numCamInputs){
			inputCam[gui.getSelectedPanel()-webcamsStartIndex].mouseReleased(x-guiPosX, y-guiPosY, button);
		}
	}

	// Mapping
	if(drawGrid){
		finalTextureMapping.mouseReleased(x, y);
	}

	// Live Coding
	if(computeFBOTexture && useLiveCoding){
		lua.scriptMouseReleased(x, y, button);
	}

}

//--------------------------------------------------------------
void gamuzaMain::windowResized(int w, int h){

}

//--------------------------------------------------------------
void gamuzaMain::dragEvent(ofDragInfo dragInfo){

}

//--------------------------------------------------------------
void gamuzaMain::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void gamuzaMain::saveFrame(){

	string _d = getDateTimeAsString("%d_%m_%Y");
	string start = "export/frames/frame"+ofToString(currentSavedFrame);
	string fin = start+"_"+_d+".jpg";

	currentSavedFrame++;

	tempFrame.grabScreen(mainScreenW+1,0,projectionScreenW,projectionScreenH);
	tempFrame.saveImage(fin.c_str());

	printf("Frame SAVED\n");

	logger.log(99," Frame SAVED");
}

//--------------------------------------------------------------
void gamuzaMain::printFrame(){
	#if defined (TARGET_OSX) || defined(TARGET_LINUX)
		tempFrame.grabScreen(mainScreenW+1,0,projectionScreenW,projectionScreenH);
		tempFrame.saveImage("export/frames/printFrame.jpg");

		string commandStr = "lp ../../../data/export/frames/printFrame.jpg";
		system(commandStr.c_str());

		printf("Sending frame for Printing\n");

		logger.log(99," Sending frame for Printing");
	#endif
}

//--------------------------------------------------------------
void gamuzaMain::openFileDialog(){

	string URL;

	int response = ofxFileDialog::openFile(URL);
	if(response){
		loadScript(URL);
		logger.log(99, " %s", URL.c_str());
		logger.log(99, " OPEN SCRIPT:");
		liveCoding.glEditor[liveCoding.currentEditor]->ClearAllText();
		liveCoding.pasteFromLuaScript(readScript(URL,true));
	}else{
		logger.log(99, " OPEN SCRIPT Canceled.");
	}

}

//--------------------------------------------------------------
void gamuzaMain::saveFileDialog(){

	string folderURL;
	string fileName;

	int response = ofxFileDialog::saveFile(folderURL, fileName);
	if(response){
	    #ifdef TARGET_OSX
		logger.log(99, " %s%s", folderURL.c_str(),fileName.c_str());
		logger.log(99, " SAVE SCRIPT:");
		liveCoding.saveToFile(fileName,folderURL);
		#endif

		#ifdef TARGET_LINUX
		logger.log(99, " %s", fileName.c_str());
		logger.log(99, " SAVE SCRIPT:");
		liveCoding.saveToFile(folderURL);
		#endif

		scriptsLister.refreshDir();

	}else{
		logger.log(99, " SAVE SCRIPT Canceled");
	}

}

//--------------------------------------------------------------
void gamuzaMain::exit(){

	// close arduino connection
	if(arduinoActivated){
		arduino.disconnect();
	}

	if(audioActivated && audioOutputChannels > 0){
		audioModules.clear();
	}

	// call the script's exit() function
	lua.scriptExit();

	// clear the lua state
	lua.clear();

}

//--------------------------------------------------------------
void gamuzaMain::loadGamuzaSettings(){

	setting_data.loadFile(GAMUZA_SETTINGS);

	//////////////////////////////////////////////
	// get SCREENS settings
	mainScreenW				= setting_data.getValue("ms_width",0,0);
	mainScreenH				= setting_data.getValue("ms_height",0,0);
	projectionScreenW		= setting_data.getValue("ps_width",0,0);
	projectionScreenH		= setting_data.getValue("ps_height",0,0);
	INVprojectionScreenW	= 1.0f/projectionScreenW;
	INVprojectionScreenH	= 1.0f/projectionScreenH;

	FPS = setting_data.getValue("fps",0,0);
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// get AUTOMATION settings
	autoPilot				= setting_data.getValue("auto_pilot",0,0);
	autoLoadScript			= setting_data.getValue("autoload_script",0,0);
	autoScriptFile			= setting_data.getValue("script_file"," ",0);
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// get SENSOR KINECT TRACKING settings
	useKinectInfrared		= setting_data.getValue("use_infrared",0,0);
	sensorKinectLedState	= setting_data.getValue("led_state",0,0);
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// get WEBCAM TRACKING settings
	trackingActivated		= setting_data.getValue("tracking_activated",0,0);
	openniActivated			= setting_data.getValue("openni_activated",0,0);
	useVideoTest			= setting_data.getValue("video_test",0,0);
	workingW				= setting_data.getValue("capture_width",0,0);
	workingH				= setting_data.getValue("capture_height",0,0);
	totPixels				= workingW*workingH;

	haarFinderFile			= setting_data.getValue("haar_finder_file"," ",0);
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// get output FBO texture settings
	useVideoFile			= setting_data.getValue("use_video_file",0,0);
	fboVideoFile			= setting_data.getValue("fbo_video_file"," ",0);
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// get MAPPING settings
	gridRes					= setting_data.getValue("grid_res",0,0);
	fboNumSamples			= setting_data.getValue("fbo_num_samples",0,0);

	if(gridRes > 20){
		gridRes = 20;
	}
	if(gridRes < 1){
		gridRes = 1;
	}
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// get AUDIO settings
	audioActivated			= setting_data.getValue("audio_activated",0,0);
	audioDevID				= setting_data.getValue("audio_Dev_ID",0,0);
	audioOutputChannels		= setting_data.getValue("output_ch",0,0);
	audioInputChannels		= setting_data.getValue("input_ch",0,0);
	audioSamplingRate		= setting_data.getValue("sampling_rate",0,0);
	audioBufferSize			= setting_data.getValue("buffer_size",0,0);
	audioNumBuffers			= setting_data.getValue("num_buffers",0,0);
	fftWindowUse			= setting_data.getValue("fft_window",0,0);

	if(!audioActivated){
		audioInputChannels = 0;
		audioOutputChannels = 0;
	}
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// get ARDUINO settings
	arduinoActivated		= setting_data.getValue("arduino_activated",0,0);
	serialDevice			= setting_data.getValue("serial_device_name"," ",0);
	baudRate				= setting_data.getValue("baud_rate",0,0);
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// get OSC settings
	oscActivated			= setting_data.getValue("osc_activated",0,0);
	host_number				= setting_data.getValue("host_ip"," ",0);
	host_port				= setting_data.getValue("host_port"," ",0);
	//////////////////////////////////////////////

}

