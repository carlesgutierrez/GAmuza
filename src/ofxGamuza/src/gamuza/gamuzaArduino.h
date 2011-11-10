#ifndef _GAMUZAARDUINO
#define _GAMUZAARDUINO

#include "gamuzaConstants.h"

//////////////////////////////////////////////
// Arduino vars
ofxXmlSettings			pinsSettings;
bool					bSetupArduino;
bool					saveArduinoPinsState;
//////////////////////////////////////////////

//--------------------------------------------------------------
void gamuzaMain::setupArduino(){
	
	// connect arduino board via serial port
	arduino.connect(serialDevice, baudRate);
	
	// pins status/values arrays setup
	digitalPinModes					= new int[12];
	digitalPinValuesInput			= new int[12];
	digitalPinValuesOutput			= new int[12];
	analogPinModes					= new int[6];
	analogPinValues					= new int[6];
	
	loadArduinoSetting();
	
	bSetupArduino			= false;
	saveArduinoPinsState	= false;
	
	// osc data vars setup
	useKalmanFilterArduino			= false;
	_kalman_analogPinValues			= new float[6];
	_s_analogPinValues				= new float[6];
	_osc_analogPinValues			= new float[6];
	_osc_digitalPinValuesInput		= new int[12];
	
	for(unsigned int i=0;i<6;i++){
		kanalogPinValues[i]	= new ofxCvKalman(0);
	}
	
	sendOsc_AAP				= false;
	sendOsc_ADP				= false;
	
}

//--------------------------------------------------------------
void gamuzaMain::updateArduino(){
	
	if(arduino.isArduinoReady()){
		// first pins setup
		if(!bSetupArduino){
			printf("\nARDUINO connection established\n");
			logger.log(OF_LOG_NOTICE, " ARDUINO connection established");
			// init pins
			for(unsigned int i=2;i<14;i++){
				arduino.sendDigitalPinMode(i, digitalPinModes[i-2]);
				if(digitalPinModes[i-2] == ARD_PWM){
					arduino.sendPwm(i, digitalPinValuesOutput[i-2]);
				}else if(digitalPinModes[i-2] == ARD_OUTPUT){
					arduino.sendDigital(i, digitalPinValuesOutput[i-2]);
				}
			}
			for(unsigned int i=0;i<6;i++){
				arduino.sendAnalogPinReporting(i, analogPinModes[i]);
			}
			bSetupArduino = true;
		}
		
		if(useArduino){
			arduino.update();
			
			// manage digital pins readings
			for(unsigned int i=2;i<14;i++){
				if(digitalPinModes[i-2] == ARD_INPUT){
					digitalPinValuesInput[i-2] = arduino.getDigital(i);
				}else if(digitalPinModes[i-2] == ARD_PWM){
					digitalPinValuesInput[i-2] = arduino.getPwm(i);
				}else if(digitalPinModes[i-2] == ARD_OUTPUT){
					digitalPinValuesInput[i-2] = digitalPinValuesOutput[i-2];
				}
			}
			
			// manage analog pins readings
			for(unsigned int i=0;i<6;i++){
				if(analogPinModes[i] == ARD_ON){
					analogPinValues[i] = arduino.getAnalog(i);
				}else{
					analogPinValues[i] = 0;
				}
			}
			
			if(saveArduinoPinsState){
				saveArduinoSetting();
				saveArduinoPinsState = false;
			}
			
			// prepare data for osc sending(eventually)
			prepareArduinoOscData();
		}
		
	}
	
}

//--------------------------------------------------------------
void gamuzaMain::prepareArduinoOscData(){
	
	for(unsigned int i=0;i<6;i++){
		if(useKalmanFilterArduino){
			_kalman_analogPinValues[i] = kanalogPinValues[i]->correct((float)analogPinValues[i]);
		}else{
			_kalman_analogPinValues[i] = (float)analogPinValues[i];
		}
		_s_analogPinValues[i]	= _s_analogPinValues[i]*arduino_smoothingFactor + (1.0-arduino_smoothingFactor)*_kalman_analogPinValues[i];
		_osc_analogPinValues[i] = ofNormalize(_s_analogPinValues[i],0.0,1023.0);
	}
	
	for(unsigned int i=0;i<12;i++){
		_osc_digitalPinValuesInput[i] = digitalPinValuesInput[i];
	}
	
}

//--------------------------------------------------------------
void gamuzaMain::loadArduinoSetting(){
	
	char temp[128];
	
	pinsSettings.loadFile(GAMUZA_ARDUINO_PINS);
	
	for(unsigned int i=0;i<6;i++){
		sprintf(temp,"analog_%i",i);
		analogPinModes[i] = pinsSettings.getValue(temp, 0, 0);
	}
	
	for(unsigned int i=2;i<14;i++){
		sprintf(temp,"digital_%i",i);
		digitalPinModes[i-2] = pinsSettings.getValue(temp, 0, 0);
		sprintf(temp,"digital_%i_value",i);
		digitalPinValuesOutput[i-2] = pinsSettings.getValue(temp, 0, 0);
	}
	
}

//--------------------------------------------------------------
void gamuzaMain::saveArduinoSetting(){
	
	char temp[128];
	
	for(unsigned int i=0;i<6;i++){
		sprintf(temp,"analog_%i",i);
		pinsSettings.setValue(temp,analogPinModes[i]);
	}
	
	for(unsigned int i=2;i<14;i++){
		sprintf(temp,"digital_%i",i);
		pinsSettings.setValue(temp,digitalPinModes[i-2]);
		sprintf(temp,"digital_%i_value",i);
		pinsSettings.setValue(temp,digitalPinValuesInput[i-2]);
	}
	
	pinsSettings.saveFile(GAMUZA_ARDUINO_PINS);
	
	printf("ARDUINO Pins Mode SAVED\n");
	logger.log(OF_LOG_NOTICE, " ARDUINO Pins Mode SAVED");
	
}


#endif
