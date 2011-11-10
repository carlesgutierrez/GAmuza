#ifndef _GAMUZAAUDIO
#define _GAMUZAAUDIO

#include "gamuzaConstants.h"

//////////////////////////////////////////////
// Audio INPUT/OUTPUT stream vars
int						outputBufferCounter;
int						inputBufferCounter;
//////////////////////////////////////////////

//--------------------------------------------------------------
void gamuzaMain::setupAudio(){
	
	//////////////////////////////////////////
	// inicializo el DSP de audio
	computeAudioInput		= false;
	computeAudioOutput		= false;
	outputBufferCounter		= 0;
	inputBufferCounter		= 0;
	audioModulesNum			= 0;
	outputBufferCopy		= new float[audioBufferSize*audioOutputChannels];
	
	printf("\n");
	soundStream.setDeviceID(audioDevID);
	soundStream.setup(this,audioOutputChannels,audioInputChannels,audioSamplingRate,audioBufferSize, audioNumBuffers);
	
	inputAudioCH = new audioInputChannel[audioInputChannels];
	
	for(int i=0; i<audioInputChannels; i++){
		inputAudioCH[i].setupChannel(i,audioSamplingRate,audioBufferSize,audioInputChannels,fftWindowUse);
	}
	
	soundStream.stop();
	
	if(audioOutputChannels > 0){
		gamuzaDSP.setupDSP(audioOutputChannels);
		gamuzaAMP.setup(audioOutputChannels);
	}
	//////////////////////////////////////////
	
}

//--------------------------------------------------------------
void gamuzaMain::audioIn(float * input, int bufferSize, int nChannels){
	
	// samples are "interleaved"
	//  L	R	L	R	L	R
	// CH1 CH2 CH1 CH2 CH1 CH2
	//  0   1   2   3   4   5
	if(computeAudioInput){
		if(audioInputChannels > 0){
			///////////////////////////////////////////
			// audio analysis here
			for(int i = 0; i < audioInputChannels; i++){
				if(inputAudioCH[i].captureAudio && inputAudioCH[i].computeChannel){
					inputAudioCH[i].captureChannel(input);
				}
			}
			///////////////////////////////////////////
			inputBufferCounter++;
		}
	}
	
}

//--------------------------------------------------------------
void gamuzaMain::audioOut(float * output, int bufferSize, int nChannels){
	
	// samples are "interleaved"
	//  L	R	L	R	L	R
	// CH1 CH2 CH1 CH2 CH1 CH2
	//  0   1   2   3   4   5
	if(computeAudioOutput){
		if(audioOutputChannels > 0){
			gamuzaDSP.clearBuffer(output, bufferSize);
			///////////////////////////////////////////
			// audio synthesis here
			for(int i = 0; i < audioModules.size(); i++){
				audioModules[i].addToSoundBuffer(output, bufferSize, gamuzaDSP.numOscInCh);
			}
			///////////////////////////////////////////
			gamuzaAMP.addToSoundBuffer(output, bufferSize, mainVolume);
			
			if(ofGetFrameNum()%2 == 0){
				memcpy(outputBufferCopy, output, bufferSize * nChannels * sizeof(float));
			}
			
			outputBufferCounter++;
		}
	}
	
	
	
}

//--------------------------------------------------------------
void gamuzaMain::addAudioModule(int _wT, float _freq){
	audioModule	aM;
	aM.setup(audioSamplingRate,audioOutputChannels, _wT, _freq, AMP_TYPE_MULTI);
	audioModules.push_back(aM);
	gamuzaDSP.addOsc();
	audioModulesNum++;
}

//--------------------------------------------------------------
void gamuzaMain::addAudioModule(int _wT, float _freq,int _ch){
	if(_ch < audioOutputChannels){
		audioModule	aM;
		aM.setup(audioSamplingRate,audioOutputChannels, _wT, _freq, AMP_TYPE_MONO, _ch);
		audioModules.push_back(aM);
		gamuzaDSP.addOsc(_ch);
		audioModulesNum++;
	}
}

//--------------------------------------------------------------
void gamuzaMain::resetAudioOutput(){
	audioModules.clear();
	audioModulesNum = 0;
	gamuzaDSP.resetOsc();
}

#endif
