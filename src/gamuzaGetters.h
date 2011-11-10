#ifndef _GAMUZAGETTERS
#define _GAMUZAGETTERS

// constants
#include "gamuzaConstants.h"
#include "gamuza.h"

extern gamuza * gapp; // external reference to gamuza app, declared in main.cpp

//--------------------------------------------------------------
// GRAPHICS SECTION
//--------------------------------------------------------------
void gaBackground(float g, float a){
	ofFill();
	glColor4f(g,g,g,a);
	ofRect(0,0,gapp->gamuzaBase.projectionScreenW,gapp->gamuzaBase.projectionScreenH);
}

void gaBackground(float r, float g, float b, float a){
	ofFill();
	glColor4f(r,g,b,a);
	ofRect(0,0,gapp->gamuzaBase.projectionScreenW,gapp->gamuzaBase.projectionScreenH);
}

ofTexture gaGetWebcamTexture(int _id){
	if(_id < gapp->gamuzaBase.numCamInputs && gapp->gamuzaBase.trackingActivated && gapp->gamuzaBase.inputCam[_id].captureVideo){
		return gapp->gamuzaBase.inputCam[_id].camTexture;
	}else{
		return gapp->gamuzaBase.emptyTexture;
	}
}

//--------------------------------------------------------------
// AUDIO SYNTH SECTION
//--------------------------------------------------------------
void gaSetupOSC(int _wt, float _freq){
	if(gapp->gamuzaBase.audioActivated && gapp->gamuzaBase.audioOutputChannels > 0){
		gapp->gamuzaBase.addAudioModule(_wt, _freq);
	}
}

void gaSetupMonoOSC(int _wt, float _freq, int _channel){
	if(gapp->gamuzaBase.audioActivated && gapp->gamuzaBase.audioOutputChannels > 0){
		gapp->gamuzaBase.addAudioModule(_wt, _freq, _channel);
	}
}

void gaSetOscVolume(int _pos, float _vol){
	if(gapp->gamuzaBase.audioActivated && gapp->gamuzaBase.audioOutputChannels > 0){
		if(_pos < gapp->gamuzaBase.audioModules.size()){
			if(_vol > 1.0 || _vol < -1.0){
				gapp->gamuzaBase.audioModules[_pos].setVolume(1.0);
			}else{
				gapp->gamuzaBase.audioModules[_pos].setVolume(_vol);
			}
		}
	}
}

void gaSetOscFrequency(int _pos, float _freq){
	if(gapp->gamuzaBase.audioActivated && gapp->gamuzaBase.audioOutputChannels > 0){
		if(_pos < gapp->gamuzaBase.audioModules.size()){
			gapp->gamuzaBase.audioModules[_pos].setFrequency(_freq);
		}
	}
}

void gaSetOscWaveType(int _pos, int _wt){
	if(gapp->gamuzaBase.audioActivated && gapp->gamuzaBase.audioOutputChannels > 0){
		if(_pos < gapp->gamuzaBase.audioModules.size()){
			gapp->gamuzaBase.audioModules[_pos].setWave(_wt);
		}
	}
}

void gaSetOscTuning(int _pos, int _tuning){
	if(gapp->gamuzaBase.audioActivated && gapp->gamuzaBase.audioOutputChannels > 0){
		if(_pos < gapp->gamuzaBase.audioModules.size()){
			if(_tuning <= 1.0 && _tuning >= 0.0){
				gapp->gamuzaBase.audioModules[_pos].setTuning(_tuning);
			}
		}
	}
}

float gaNote(int _note){
	if(_note <= SI_8 && _note >= DO_0){
		return noteToHarmonicFrequency(_note);
	}else{
		return 0.0f;
	}
}

//--------------------------------------------------------------
// OPENNI SENSOR KINECT SECTION
//--------------------------------------------------------------
int getONIRunningBlob(){
	if(gapp->gamuzaBase.openniActivated && gapp->gamuzaBase.sensorKinect.useKinect){
		return gapp->gamuzaBase.sensorKinect.runningBlobs;
	}else{
		return 0;
	}
}

float getONIBlobX(int _bId){
	if(gapp->gamuzaBase.openniActivated && _bId >= 0 && _bId < MAX_USERS_HARDLIMIT){
		if(gapp->gamuzaBase.sensorKinect.useKinect && gapp->gamuzaBase.sensorKinect.computeContourFinder){
			for(unsigned int e = 0; e < gapp->gamuzaBase.sensorKinect.runningBlobs; e++){
				if(gapp->gamuzaBase.sensorKinect.blobsOrder[e] == _bId){
					return gapp->gamuzaBase.sensorKinect._osc_blobInfo[e].center.x;
				}
			}
		}else{
			return 0.0f;
		}
	}else{
		return 0.0f;
	}
	
}

float getONIBlobY(int _bId){
	if(gapp->gamuzaBase.openniActivated && _bId >= 0 && _bId < MAX_USERS_HARDLIMIT){
		if(gapp->gamuzaBase.sensorKinect.useKinect && gapp->gamuzaBase.sensorKinect.computeContourFinder){
			for(unsigned int e = 0; e < gapp->gamuzaBase.sensorKinect.runningBlobs; e++){
				if(gapp->gamuzaBase.sensorKinect.blobsOrder[e] == _bId){
					return gapp->gamuzaBase.sensorKinect._osc_blobInfo[e].center.y;
				}
			}
		}else{
			return 0.0f;
		}
	}else{
		return 0.0f;
	}
	
}

float getONIBlobW(int _bId){
	if(gapp->gamuzaBase.openniActivated && _bId >= 0 && _bId < MAX_USERS_HARDLIMIT){
		if(gapp->gamuzaBase.sensorKinect.useKinect && gapp->gamuzaBase.sensorKinect.computeContourFinder){
			for(unsigned int e = 0; e < gapp->gamuzaBase.sensorKinect.runningBlobs; e++){
				if(gapp->gamuzaBase.sensorKinect.blobsOrder[e] == _bId){
					return gapp->gamuzaBase.sensorKinect._osc_blobInfo[e].size.width;
				}
			}
		}else{
			return 0.0f;
		}
	}else{
		return 0.0f;
	}
	
}

float getONIBlobH(int _bId){
	if(gapp->gamuzaBase.openniActivated && _bId >= 0 && _bId < MAX_USERS_HARDLIMIT){
		if(gapp->gamuzaBase.sensorKinect.useKinect && gapp->gamuzaBase.sensorKinect.computeContourFinder){
			for(unsigned int e = 0; e < gapp->gamuzaBase.sensorKinect.runningBlobs; e++){
				if(gapp->gamuzaBase.sensorKinect.blobsOrder[e] == _bId){
					return gapp->gamuzaBase.sensorKinect._osc_blobInfo[e].size.height;
				}
			}
		}else{
			return 0.0f;
		}
	}else{
		return 0.0f;
	}
	
}

float getONIBlobAngle(int _bId){
	if(gapp->gamuzaBase.openniActivated && _bId >= 0 && _bId < MAX_USERS_HARDLIMIT){
		if(gapp->gamuzaBase.sensorKinect.useKinect && gapp->gamuzaBase.sensorKinect.computeContourFinder){
			for(unsigned int e = 0; e < gapp->gamuzaBase.sensorKinect.runningBlobs; e++){
				if(gapp->gamuzaBase.sensorKinect.blobsOrder[e] == _bId){
					return gapp->gamuzaBase.sensorKinect._osc_blobInfo[e].angle;
				}
			}
		}else{
			return 0.0f;
		}
	}else{
		return 0.0f;
	}
	
}

vector<ofVec2f> getONIBlobContour(int _bId){
	if(gapp->gamuzaBase.openniActivated && _bId >= 0 && _bId < MAX_USERS_HARDLIMIT){
		if(gapp->gamuzaBase.sensorKinect.useKinect && gapp->gamuzaBase.sensorKinect.computeContourFinder){
			for(unsigned int e = 0; e < gapp->gamuzaBase.sensorKinect.runningBlobs; e++){
				if(gapp->gamuzaBase.sensorKinect.blobsOrder[e] == _bId){
					if(gapp->gamuzaBase.sensorKinect.cfDetail == 1){
						return gapp->gamuzaBase.sensorKinect._osc_contourSmooth[e];
					}else if(gapp->gamuzaBase.sensorKinect.cfDetail == 2){
						return gapp->gamuzaBase.sensorKinect._osc_contourSimple[e];
					}
				}
			}
		}else{
			return (vector<ofVec2f>)0.0f;
		}
	}else{
		return (vector<ofVec2f>)0.0f;
	}
}

vector<ofVec4f> getONIBlobGeometry(int _bId){
	if(gapp->gamuzaBase.openniActivated && _bId >= 0 && _bId < MAX_USERS_HARDLIMIT){
		if(gapp->gamuzaBase.sensorKinect.useKinect && gapp->gamuzaBase.sensorKinect.computeContourFinder && gapp->gamuzaBase.sensorKinect.computeContourGeometry){
			for(unsigned int e = 0; e < gapp->gamuzaBase.sensorKinect.runningBlobs; e++){
				if(gapp->gamuzaBase.sensorKinect.blobsOrder[e] == _bId){
					return gapp->gamuzaBase.sensorKinect._osc_blobGeom[e];
				}
			}
		}else{
			return (vector<ofVec4f>)0.0f;
		}
	}else{
		return (vector<ofVec4f>)0.0f;
	}
}

ofVec2f* getONIOpticalFlow(){
	if(gapp->gamuzaBase.openniActivated){
		if(gapp->gamuzaBase.sensorKinect.useKinect && gapp->gamuzaBase.sensorKinect.computeOpticalFlow){
			return gapp->gamuzaBase.sensorKinect._osc_opfVel;
		}else{
			return NULL;
		}
	}else{
		return NULL;
	}
}

bool getONITrigger(int _aID){
	if(gapp->gamuzaBase.openniActivated && _aID < TRIGGER_AREAS_NUM){
		if(gapp->gamuzaBase.sensorKinect.useKinect && gapp->gamuzaBase.sensorKinect.computeContourFinder && gapp->gamuzaBase.sensorKinect.computeTriggerAreas){
			return gapp->gamuzaBase.sensorKinect.triggerState[_aID];
		}else{
			return -1;
		}
	}else{
		return -1;
	}
}

ofVec3f getONIHand(int hand){
	if(gapp->gamuzaBase.openniActivated){
		if(gapp->gamuzaBase.sensorKinect.useKinect && gapp->gamuzaBase.sensorKinect.isTrackingHands){
			return gapp->gamuzaBase.sensorKinect._osc_hands_Pos[hand];
		}else{
			return (ofVec3f)0.0f;
		}
	}else{
		return (ofVec3f)0.0f;
	}
}

ofVec3f getONIAccelerometer(){
	if(gapp->gamuzaBase.openniActivated){
		if(gapp->gamuzaBase.sensorKinect.useKinect){
			return gapp->gamuzaBase.sensorKinect._osc_sensorAcc;
		}else{
			return (ofVec3f)0.0f;
		}
	}else{
		return (ofVec3f)0.0f;
	}
}

float getONITilt(){
	if(gapp->gamuzaBase.openniActivated){
		if(gapp->gamuzaBase.sensorKinect.useKinect){
			return gapp->gamuzaBase.sensorKinect._osc_sensorTilt;
		}else{
			return 0.0f;
		}
	}else{
		return 0.0f;
	}
}

//--------------------------------------------------------------
// WEBCAM TRACKING SECTION
//--------------------------------------------------------------
float gaCamMotionQ(int _dID){
	if(gapp->gamuzaBase.trackingActivated && _dID < gapp->gamuzaBase.numCamInputs){
		if(gapp->gamuzaBase.inputCam[_dID].captureVideo){
			return gapp->gamuzaBase.inputCam[_dID]._osc_MDQ;
		}else{
			return 0.0f;
		}
	}else{
		return 0.0f;
	}
}

float gaCamMotionX(int _dID){
	if(gapp->gamuzaBase.trackingActivated && _dID < gapp->gamuzaBase.numCamInputs){
		if(gapp->gamuzaBase.inputCam[_dID].captureVideo){
			return gapp->gamuzaBase.inputCam[_dID]._osc_MDCM.x;
		}else{
			return 0.0f;
		}
	}else{
		return 0.0f;
	}
}

float gaCamMotionY(int _dID){
	if(gapp->gamuzaBase.trackingActivated && _dID < gapp->gamuzaBase.numCamInputs){
		if(gapp->gamuzaBase.inputCam[_dID].captureVideo){
			return gapp->gamuzaBase.inputCam[_dID]._osc_MDCM.x;
		}else{
			return 0.0f;
		}
	}else{
		return 0.0f;
	}
}

int gaCamRunningBlob(int _dID){
	if(gapp->gamuzaBase.trackingActivated && gapp->gamuzaBase.inputCam[_dID].captureVideo && _dID < gapp->gamuzaBase.numCamInputs){
		return gapp->gamuzaBase.inputCam[_dID].runningBlobs;
	}else{
		return 0;
	}
}

float gaCamBlobX(int _dID,int _bId){
	if(gapp->gamuzaBase.trackingActivated && _bId >= 0 && _bId < gapp->gamuzaBase.inputCam[_dID].runningBlobs){
		if(gapp->gamuzaBase.inputCam[_dID].captureVideo && _dID < gapp->gamuzaBase.numCamInputs && gapp->gamuzaBase.inputCam[_dID].computeContourFinder){
			for(unsigned int e = 0; e < gapp->gamuzaBase.inputCam[_dID].runningBlobs; e++){
				if(gapp->gamuzaBase.inputCam[_dID].blobsOrder[e] == _bId){
					return gapp->gamuzaBase.inputCam[_dID]._osc_blobInfo[e].center.x;
				}
			}
		}else{
			return 0.0f;
		}
	}else{
		return 0.0f;
	}
	
}

float gaCamBlobY(int _dID,int _bId){
	if(gapp->gamuzaBase.trackingActivated && _bId >= 0 && _bId < gapp->gamuzaBase.inputCam[_dID].runningBlobs){
		if(gapp->gamuzaBase.inputCam[_dID].captureVideo && _dID < gapp->gamuzaBase.numCamInputs && gapp->gamuzaBase.inputCam[_dID].computeContourFinder){
			for(unsigned int e = 0; e < gapp->gamuzaBase.inputCam[_dID].runningBlobs; e++){
				if(gapp->gamuzaBase.inputCam[_dID].blobsOrder[e] == _bId){
					return gapp->gamuzaBase.inputCam[_dID]._osc_blobInfo[e].center.y;
				}
			}
		}else{
			return 0.0f;
		}
	}else{
		return 0.0f;
	}
	
}

float gaCamBlobW(int _dID,int _bId){
	if(gapp->gamuzaBase.trackingActivated && _bId >= 0 && _bId < gapp->gamuzaBase.inputCam[_dID].runningBlobs){
		if(gapp->gamuzaBase.inputCam[_dID].captureVideo && _dID < gapp->gamuzaBase.numCamInputs && gapp->gamuzaBase.inputCam[_dID].computeContourFinder){
			for(unsigned int e = 0; e < gapp->gamuzaBase.inputCam[_dID].runningBlobs; e++){
				if(gapp->gamuzaBase.inputCam[_dID].blobsOrder[e] == _bId){
					return gapp->gamuzaBase.inputCam[_dID]._osc_blobInfo[e].size.width;
				}
			}
		}else{
			return 0.0f;
		}
	}else{
		return 0.0f;
	}
	
}


float gaCamBlobH(int _dID,int _bId){
	if(gapp->gamuzaBase.trackingActivated && _bId >= 0 && _bId < gapp->gamuzaBase.inputCam[_dID].runningBlobs){
		if(gapp->gamuzaBase.inputCam[_dID].captureVideo && _dID < gapp->gamuzaBase.numCamInputs && gapp->gamuzaBase.inputCam[_dID].computeContourFinder){
			for(unsigned int e = 0; e < gapp->gamuzaBase.inputCam[_dID].runningBlobs; e++){
				if(gapp->gamuzaBase.inputCam[_dID].blobsOrder[e] == _bId){
					return gapp->gamuzaBase.inputCam[_dID]._osc_blobInfo[e].size.height;
				}
			}
		}else{
			return 0.0f;
		}
	}else{
		return 0.0f;
	}
	
}

float gaCamBlobAngle(int _dID,int _bId){
	if(gapp->gamuzaBase.trackingActivated && _bId >= 0 && _bId < gapp->gamuzaBase.inputCam[_dID].runningBlobs){
		if(gapp->gamuzaBase.inputCam[_dID].captureVideo && _dID < gapp->gamuzaBase.numCamInputs && gapp->gamuzaBase.inputCam[_dID].computeContourFinder){
			for(unsigned int e = 0; e < gapp->gamuzaBase.inputCam[_dID].runningBlobs; e++){
				if(gapp->gamuzaBase.inputCam[_dID].blobsOrder[e] == _bId){
					return gapp->gamuzaBase.inputCam[_dID]._osc_blobInfo[e].angle;
				}
			}
		}else{
			return 0.0f;
		}
	}else{
		return 0.0f;
	}
	
}

vector<ofVec2f> gaCamBlobContour(int _dID,int _bId){
	if(gapp->gamuzaBase.trackingActivated && _bId >= 0 && _bId < gapp->gamuzaBase.inputCam[_dID].runningBlobs){
		if(gapp->gamuzaBase.inputCam[_dID].captureVideo && _dID < gapp->gamuzaBase.numCamInputs && gapp->gamuzaBase.inputCam[_dID].computeContourFinder){
			for(unsigned int e = 0; e < gapp->gamuzaBase.inputCam[_dID].runningBlobs; e++){
				if(gapp->gamuzaBase.inputCam[_dID].blobsOrder[e] == _bId){
					if(gapp->gamuzaBase.inputCam[_dID].cfDetail == 1){
						return gapp->gamuzaBase.inputCam[_dID]._osc_contourSmooth[e];
					}else if(gapp->gamuzaBase.inputCam[_dID].cfDetail == 2){
						return gapp->gamuzaBase.inputCam[_dID]._osc_contourSimple[e];
					}
				}
			}
		}else{
			return (vector<ofVec2f>)0.0f;
		}
	}else{
		return (vector<ofVec2f>)0.0f;
	}
}

vector<ofVec4f> gaCamBlobGeometry(int _dID,int _bId){
	if(gapp->gamuzaBase.trackingActivated && _bId >= 0 && _bId < gapp->gamuzaBase.inputCam[_dID].runningBlobs){
		if(gapp->gamuzaBase.inputCam[_dID].captureVideo && _dID < gapp->gamuzaBase.numCamInputs && gapp->gamuzaBase.inputCam[_dID].computeContourFinder && gapp->gamuzaBase.inputCam[_dID].computeContourGeometry){
			for(unsigned int e = 0; e < gapp->gamuzaBase.inputCam[_dID].runningBlobs; e++){
				if(gapp->gamuzaBase.inputCam[_dID].blobsOrder[e] == _bId){
					return gapp->gamuzaBase.inputCam[_dID]._osc_blobGeom[e];
				}
			}
		}else{
			return (vector<ofVec4f>)0.0f;
		}
	}else{
		return (vector<ofVec4f>)0.0f;
	}
}

ofVec2f* gaCamOpticalFlow(int _dID){
	if(gapp->gamuzaBase.trackingActivated && _dID < gapp->gamuzaBase.numCamInputs){
		if(gapp->gamuzaBase.inputCam[_dID].captureVideo && gapp->gamuzaBase.inputCam[_dID].computeOpticalFlow){
			return gapp->gamuzaBase.inputCam[_dID]._osc_opfVel;
		}else{
			return NULL;
		}
	}else{
		return NULL;
	}
}

ofVec4f gaCamHaar(int _dID, int _hID){
	if(gapp->gamuzaBase.trackingActivated && _dID < gapp->gamuzaBase.numCamInputs){
		if(gapp->gamuzaBase.inputCam[_dID].captureVideo && _hID < gapp->gamuzaBase.inputCam[_dID].numFace && gapp->gamuzaBase.inputCam[_dID].computeHaarFinder){
			return gapp->gamuzaBase.inputCam[_dID]._osc_ftInfo[_hID];
		}else{
			return (ofVec4f)0.0f;
		}
	}else{
		return (ofVec4f)0.0f;
	}
}

bool gaCamTrigger(int _dID, int _aID){
	if(gapp->gamuzaBase.trackingActivated && _dID < gapp->gamuzaBase.numCamInputs && _aID < TRIGGER_AREAS_NUM){
		if(gapp->gamuzaBase.inputCam[_dID].captureVideo && gapp->gamuzaBase.inputCam[_dID].computeContourFinder && gapp->gamuzaBase.inputCam[_dID].computeTriggerAreas){
			return gapp->gamuzaBase.inputCam[_dID].triggerState[_aID];
		}else{
			return -1;
		}
	}else{
		return -1;
	}
}

//--------------------------------------------------------------
// AUDIO INPUT SECTION
//--------------------------------------------------------------
float gaGetVolume(int _ch){
	if(gapp->gamuzaBase.audioActivated && _ch < gapp->gamuzaBase.audioInputChannels){
		if(gapp->gamuzaBase.inputAudioCH[_ch].captureAudio){
			return gapp->gamuzaBase.inputAudioCH[_ch]._osc_chVolume;
		}else{
			return 0.0f;
		}
	}else{
		return 0.0f;
	}
}

float gaGetPitch(int _ch){
	if(gapp->gamuzaBase.audioActivated && _ch < gapp->gamuzaBase.audioInputChannels){
		if(gapp->gamuzaBase.inputAudioCH[_ch].captureAudio){
			return gapp->gamuzaBase.inputAudioCH[_ch]._osc_chPitch;
		}else{
			return 0.0f;
		}
	}else{
		return 0.0f;
	}
}

float gaGetFFTBin(int _ch, int _pos){
	if(gapp->gamuzaBase.audioActivated && _ch < gapp->gamuzaBase.audioInputChannels && _pos < BARK_SCALE_CRITICAL_BANDS){
		if(gapp->gamuzaBase.inputAudioCH[_ch].captureAudio){
			return gapp->gamuzaBase.inputAudioCH[_ch]._osc_barkBins[_pos];
		}else{
			return 0.0f;
		}
	}else{
		return 0.0f;
	}
	
}

float* gaGetFFTBins(int _ch){
	if(gapp->gamuzaBase.audioActivated && _ch < gapp->gamuzaBase.audioInputChannels){
		if(gapp->gamuzaBase.inputAudioCH[_ch].captureAudio){
			return  gapp->gamuzaBase.inputAudioCH[_ch]._osc_barkBins;
		}else{
			return NULL;
		}
	}else{
		return NULL;
	}
	
}
//--------------------------------------------------------------
// ARDUINO SECTION
//--------------------------------------------------------------
float gaGetAArduinoPin(int _pin){
	if(gapp->gamuzaBase.arduinoActivated && _pin >= 0 && _pin <= 5){
		return gapp->gamuzaBase._osc_analogPinValues[_pin];
	}else{
		return 0.0f;
	}
}
int gaGetDArduinoPin(int _pin){
	if(gapp->gamuzaBase.arduinoActivated && _pin >= 2 && _pin <= 13){
		return gapp->gamuzaBase._osc_digitalPinValuesInput[_pin-2];
	}
}

void gaSetDArduinoPin(int _pin,int val){
	if(gapp->gamuzaBase.arduinoActivated && _pin >= 2 && _pin <= 13){
		if(gapp->gamuzaBase.digitalPinModes[_pin-2] == ARD_OUTPUT){
			if(val > 1){
				val = 1;
			}
			gapp->gamuzaBase.digitalPinValuesOutput[_pin-2] = val;
			gapp->gamuzaBase.arduino.sendDigital(_pin, gapp->gamuzaBase.digitalPinValuesOutput[_pin-2]);
		}else  if(gapp->gamuzaBase.digitalPinModes[_pin-2] == ARD_PWM){
			gapp->gamuzaBase.digitalPinValuesOutput[_pin-2] = val;
			gapp->gamuzaBase.arduino.sendPwm(_pin, gapp->gamuzaBase.digitalPinValuesOutput[_pin-2]);
		}
	}
}

#endif