#ifndef _GAMUZAOSC
#define _GAMUZAOSC

#include "gamuzaConstants.h"


/*
 OSC buffer size = 4096 = 4k
 
 */
//////////////////////////////////////////////
// OSC vars
ofxOscSender			sender;

ofxOscMessage			osc_message;
ofxOscBundle			osc_bundle;
//////////////////////////////////////////////

//--------------------------------------------------------------
void gamuzaMain::setupOSC(){
	
	sender.setup(host_number.c_str(),atoi(host_port.c_str()));
	
}

//--------------------------------------------------------------
void gamuzaMain::updateOSC(){
	
	char osc_name[256];
	
	////////////////////////////////////////////
	// send openni sensorkinect data
	if(openniActivated){
		if(sensorKinect.useKinect){
			// sending blob detection data
			if(sensorKinect.sendOsc_BD && sensorKinect.computeContourFinder){
				for (unsigned int e = 0; e < sensorKinect.runningBlobs; e++){
					if(sensorKinect.blobsOrder[e] < MAX_USERS_HARDLIMIT){
						osc_message.clear();
						sprintf(osc_name,"/oscBD_OPENNI/Blob%i/ORDER",sensorKinect.blobsOrder[e]);
						osc_message.setAddress(osc_name);
						osc_message.addIntArg(sensorKinect.blobsOrder[e]);
						osc_bundle.addMessage(osc_message);
						osc_message.clear();
						sprintf(osc_name,"/oscBD_OPENNI/Blob%i/X",sensorKinect.blobsOrder[e]);
						osc_message.setAddress(osc_name);
						osc_message.addFloatArg(sensorKinect._osc_blobInfo[e].center.x);
						osc_bundle.addMessage(osc_message);
						osc_message.clear();
						sprintf(osc_name,"/oscBD_OPENNI/Blob%i/Y",sensorKinect.blobsOrder[e]);
						osc_message.setAddress(osc_name);
						osc_message.addFloatArg(sensorKinect._osc_blobInfo[e].center.y);
						osc_bundle.addMessage(osc_message);
						osc_message.clear();
						sprintf(osc_name,"/oscBD_OPENNI/Blob%i/WIDTH",sensorKinect.blobsOrder[e]);
						osc_message.setAddress(osc_name);
						osc_message.addFloatArg(sensorKinect._osc_blobInfo[e].size.width);
						osc_bundle.addMessage(osc_message);
						osc_message.clear();
						sprintf(osc_name,"/oscBD_OPENNI/Blob%i/HEIGHT",sensorKinect.blobsOrder[e]);
						osc_message.setAddress(osc_name);
						osc_message.addFloatArg(sensorKinect._osc_blobInfo[e].size.height);
						osc_bundle.addMessage(osc_message);
						osc_message.clear();
						sprintf(osc_name,"/oscBD_OPENNI/Blob%i/ANGLE",sensorKinect.blobsOrder[e]);
						osc_message.setAddress(osc_name);
						osc_message.addFloatArg(sensorKinect._osc_blobInfo[e].angle);
						osc_bundle.addMessage(osc_message);
						sendBuffer();
						cleanBuffer();
					}
				}
			}
			
			// sending contour finder data
			if(sensorKinect.sendOsc_CF && sensorKinect.computeContourFinder){
				for (unsigned int e = 0; e < sensorKinect.runningBlobs; e++){
					if(sensorKinect.cfDetail == 1 && sensorKinect.blobsOrder[e] < MAX_USERS_HARDLIMIT){
						osc_message.clear();
						sprintf(osc_name,"/oscCF_OPENNI/Blob%i/NumPoints",sensorKinect.blobsOrder[e]);
						osc_message.setAddress(osc_name);
						osc_message.addIntArg(sensorKinect.contourSmooth[sensorKinect.blobsOrder[e]].size());
						osc_bundle.addMessage(osc_message);
						for(unsigned int j = 0; j < sensorKinect.contourSmooth[sensorKinect.blobsOrder[e]].size(); j++){
							osc_message.clear();
							sprintf(osc_name,"/oscCF_OPENNI/Blob%i/Point%i/X",sensorKinect.blobsOrder[e],j);
							osc_message.setAddress(osc_name);
							osc_message.addFloatArg(sensorKinect._osc_contourSmooth[sensorKinect.blobsOrder[e]].at(j).x);
							osc_bundle.addMessage(osc_message);
							osc_message.clear();
							sprintf(osc_name,"/oscCF_OPENNI/Blob%i/Point%i/Y",sensorKinect.blobsOrder[e],j);
							osc_message.setAddress(osc_name);
							osc_message.addFloatArg(sensorKinect._osc_contourSmooth[sensorKinect.blobsOrder[e]].at(j).y);
							osc_bundle.addMessage(osc_message);
							sendBuffer();
							cleanBuffer();
						}
					}else if(sensorKinect.cfDetail == 2 && sensorKinect.blobsOrder[e] < MAX_USERS_HARDLIMIT){
						osc_message.clear();
						sprintf(osc_name,"/oscCF_OPENNI/Blob%i/NumPoints",sensorKinect.blobsOrder[e]);
						osc_message.setAddress(osc_name);
						osc_message.addIntArg(sensorKinect.contourSimple[sensorKinect.blobsOrder[e]].size());
						osc_bundle.addMessage(osc_message);
						for(unsigned int j = 0; j < sensorKinect.contourSimple[sensorKinect.blobsOrder[e]].size(); j++){
							osc_message.clear();
							sprintf(osc_name,"/oscCF_OPENNI/Blob%i/Point%i/X",sensorKinect.blobsOrder[e],j);
							osc_message.setAddress(osc_name);
							osc_message.addFloatArg(sensorKinect._osc_contourSimple[sensorKinect.blobsOrder[e]].at(j).x);
							osc_bundle.addMessage(osc_message);
							osc_message.clear();
							sprintf(osc_name,"/oscCF_OPENNI/Blob%i/Point%i/Y",sensorKinect.blobsOrder[e],j);
							osc_message.setAddress(osc_name);
							osc_message.addFloatArg(sensorKinect._osc_contourSimple[sensorKinect.blobsOrder[e]].at(j).y);
							osc_bundle.addMessage(osc_message);
							sendBuffer();
							cleanBuffer();
						}
					}
				}
			}
			
			// sending contour geometry data
			if(sensorKinect.sendOsc_CG && sensorKinect.computeContourFinder && sensorKinect.computeContourGeometry){
				for (unsigned int e = 0; e < sensorKinect.runningBlobs; e++){
					if(sensorKinect.blobsOrder[e] < MAX_USERS_HARDLIMIT){
						osc_message.clear();
						sprintf(osc_name,"/oscCG_OPENNI/Blob%i/NumLines",sensorKinect.blobsOrder[e]);
						osc_message.setAddress(osc_name);
						osc_message.addIntArg(sensorKinect.geomLines[sensorKinect.blobsOrder[e]].size());
						osc_bundle.addMessage(osc_message);
						for(unsigned int j = 0; j < sensorKinect.geomLines[sensorKinect.blobsOrder[e]].size(); j++){
							osc_message.clear();
							sprintf(osc_name,"/oscCG_OPENNI/Blob%i/Line%i/X",sensorKinect.blobsOrder[e],j);
							osc_message.setAddress(osc_name);
							osc_message.addFloatArg(sensorKinect._osc_blobGeom[sensorKinect.blobsOrder[e]].at(j).x);
							osc_bundle.addMessage(osc_message);
							osc_message.clear();
							sprintf(osc_name,"/oscCG_OPENNI/Blob%i/Line%i/Y",sensorKinect.blobsOrder[e],j);
							osc_message.setAddress(osc_name);
							osc_message.addFloatArg(sensorKinect._osc_blobGeom[sensorKinect.blobsOrder[e]].at(j).y);
							osc_bundle.addMessage(osc_message);
							osc_message.clear();
							sprintf(osc_name,"/oscCG_OPENNI/Blob%i/Line%i/Z",sensorKinect.blobsOrder[e],j);
							osc_message.setAddress(osc_name);
							osc_message.addFloatArg(sensorKinect._osc_blobGeom[sensorKinect.blobsOrder[e]].at(j).z);
							osc_bundle.addMessage(osc_message);
							osc_message.clear();
							sprintf(osc_name,"/oscCG_OPENNI/Blob%i/Line%i/W",sensorKinect.blobsOrder[e],j);
							osc_message.setAddress(osc_name);
							osc_message.addFloatArg(sensorKinect._osc_blobGeom[sensorKinect.blobsOrder[e]].at(j).w);
							osc_bundle.addMessage(osc_message);
							sendBuffer();
							cleanBuffer();
						}
					}
				}
			}
			
			// sending optical flow data
			if(sensorKinect.sendOsc_OF && sensorKinect.computeOpticalFlow){
				osc_message.clear();
				sprintf(osc_name,"/oscOF_OPENNI/PixelGrid");
				osc_message.setAddress(osc_name);
				osc_message.addIntArg((int)sensorKinect.opticalFlowXGrid);
				osc_message.addIntArg((int)sensorKinect.opticalFlowYGrid);
				osc_bundle.addMessage(osc_message);
				sendBuffer();
				cleanBuffer();
				for(unsigned int o=0;o<sensorKinect.opticalFlowYGrid;o++){ // Y
					for(unsigned int oo=0;oo<sensorKinect.opticalFlowXGrid;oo++){ // X
						osc_message.clear();
						sprintf(osc_name,"/oscOF_OPENNI/PixelVel_X%i_Y%i",oo,o);
						osc_message.setAddress(osc_name);
						osc_message.addFloatArg(sensorKinect._osc_opfVel[oo+(sensorKinect.opticalFlowXGrid*o)].x);
						osc_message.addFloatArg(sensorKinect._osc_opfVel[oo+(sensorKinect.opticalFlowXGrid*o)].y);
						osc_bundle.addMessage(osc_message);
						sendBuffer();
						cleanBuffer();
					}
				}
			}
			
			// sending trigger areas data
			if(sensorKinect.sendOsc_TA && sensorKinect.computeContourFinder && sensorKinect.computeTriggerAreas){
				osc_message.clear();
				sprintf(osc_name,"/oscTA_OPENNI");
				osc_message.setAddress(osc_name);
				for(unsigned int t=0;t<TRIGGER_AREAS_NUM;t++){
					osc_message.addIntArg((int)sensorKinect.triggerState[t]);
				}
				osc_bundle.addMessage(osc_message);
				sendBuffer();
				cleanBuffer();
			}
			
			// sending sensor kinect hands tracking data
			if(sensorKinect.sendOsc_HT && sensorKinect.isTrackingHands){
				for(unsigned int i=0;i<sensorKinect.maxHands;i++){
					osc_message.clear();
					sprintf(osc_name,"/oscHT_OPENNI/Hand%i_XYZ",i);
					osc_message.setAddress(osc_name);
					osc_message.addFloatArg(sensorKinect._osc_hands_Pos[i].x);
					osc_message.addFloatArg(sensorKinect._osc_hands_Pos[i].y);
					osc_message.addFloatArg(sensorKinect._osc_hands_Pos[i].z);
					osc_bundle.addMessage(osc_message);
					sendBuffer();
					cleanBuffer();
				}
			}
			
			// sending sensor hardware data
			if(sensorKinect.sendOsc_HW){
				osc_message.clear();
				sprintf(osc_name,"/oscHW_OPENNI/Accelerometer");
				osc_message.setAddress(osc_name);
				osc_message.addFloatArg(sensorKinect._osc_sensorAcc.x);
				osc_message.addFloatArg(sensorKinect._osc_sensorAcc.y);
				osc_message.addFloatArg(sensorKinect._osc_sensorAcc.z);
				osc_bundle.addMessage(osc_message);
				sendBuffer();
				cleanBuffer();
				osc_message.clear();
				sprintf(osc_name,"/oscHW_OPENNI/TiltSensor");
				osc_message.setAddress(osc_name);
				osc_message.addFloatArg(sensorKinect._osc_sensorTilt);
				osc_bundle.addMessage(osc_message);
				sendBuffer();
				cleanBuffer();
			}
			
		}
	}
	////////////////////////////////////////////
	
	////////////////////////////////////////////
	// send webcam analysis data
	if(trackingActivated){
		for(unsigned int i=0;i<numCamInputs;i++){
			if(inputCam[i].captureVideo){
				// sending motion detection data
				if(inputCam[i].sendOsc_MD){
					osc_message.clear();
					sprintf(osc_name,"/oscMD_DEV%i/Q",i);
					osc_message.setAddress(osc_name);
					osc_message.addFloatArg(inputCam[i]._osc_MDQ);
					osc_bundle.addMessage(osc_message);
					osc_message.clear();
					sprintf(osc_name,"/oscMD_DEV%i/X",i);
					osc_message.setAddress(osc_name);
					osc_message.addFloatArg(inputCam[i]._osc_MDCM.x);
					osc_bundle.addMessage(osc_message);
					osc_message.clear();
					sprintf(osc_name,"/oscMD_DEV%i/Y",i);
					osc_message.setAddress(osc_name);
					osc_message.addFloatArg(inputCam[i]._osc_MDCM.y);
					osc_bundle.addMessage(osc_message);
					sendBuffer();
					cleanBuffer();
					
				}
				
				// sending blob detection data
				if(inputCam[i].sendOsc_BD && inputCam[i].computeContourFinder){
					for (unsigned int e = 0; e < inputCam[i].runningBlobs; e++){
						if(inputCam[i].blobsOrder[e] < MAX_USERS_HARDLIMIT){
							osc_message.clear();
							sprintf(osc_name,"/oscBD_DEV%i/Blob%i/ORDER",i,inputCam[i].blobsOrder[e]);
							osc_message.setAddress(osc_name);
							osc_message.addIntArg(inputCam[i].blobsOrder[e]);
							osc_bundle.addMessage(osc_message);
							osc_message.clear();
							sprintf(osc_name,"/oscBD_DEV%i/Blob%i/X",i,inputCam[i].blobsOrder[e]);
							osc_message.setAddress(osc_name);
							osc_message.addFloatArg(inputCam[i]._osc_blobInfo[e].center.x);
							osc_bundle.addMessage(osc_message);
							osc_message.clear();
							sprintf(osc_name,"/oscBD_DEV%i/Blob%i/Y",i,inputCam[i].blobsOrder[e]);
							osc_message.setAddress(osc_name);
							osc_message.addFloatArg(inputCam[i]._osc_blobInfo[e].center.y);
							osc_bundle.addMessage(osc_message);
							osc_message.clear();
							sprintf(osc_name,"/oscBD_DEV%i/Blob%i/WIDTH",i,inputCam[i].blobsOrder[e]);
							osc_message.setAddress(osc_name);
							osc_message.addFloatArg(inputCam[i]._osc_blobInfo[e].size.width);
							osc_bundle.addMessage(osc_message);
							osc_message.clear();
							sprintf(osc_name,"/oscBD_DEV%i/Blob%i/HEIGHT",i,inputCam[i].blobsOrder[e]);
							osc_message.setAddress(osc_name);
							osc_message.addFloatArg(inputCam[i]._osc_blobInfo[e].size.height);
							osc_bundle.addMessage(osc_message);
							osc_message.clear();
							sprintf(osc_name,"/oscBD_DEV%i/Blob%i/ANGLE",i,inputCam[i].blobsOrder[e]);
							osc_message.setAddress(osc_name);
							osc_message.addFloatArg(inputCam[i]._osc_blobInfo[e].angle);
							osc_bundle.addMessage(osc_message);
							sendBuffer();
							cleanBuffer();
						}
					}
					
				}
				
				// sending contour finder data
				if(inputCam[i].sendOsc_CF && inputCam[i].computeContourFinder){
					for (unsigned int e = 0; e < inputCam[i].runningBlobs; e++){
						if(inputCam[i].cfDetail == 1 && inputCam[i].blobsOrder[e] < MAX_USERS_HARDLIMIT){
							osc_message.clear();
							sprintf(osc_name,"/oscCF_DEV%i/Blob%i/NumPoints",i,inputCam[i].blobsOrder[e]);
							osc_message.setAddress(osc_name);
							osc_message.addIntArg(inputCam[i].contourSmooth[inputCam[i].blobsOrder[e]].size());
							osc_bundle.addMessage(osc_message);
							for(unsigned int j = 0; j < inputCam[i].contourSmooth[inputCam[i].blobsOrder[e]].size(); j++){
								osc_message.clear();
								sprintf(osc_name,"/oscCF_DEV%i/Blob%i/Point%i/X",i,inputCam[i].blobsOrder[e],j);
								osc_message.setAddress(osc_name);
								osc_message.addFloatArg(inputCam[i]._osc_contourSmooth[inputCam[i].blobsOrder[e]].at(j).x);
								osc_bundle.addMessage(osc_message);
								osc_message.clear();
								sprintf(osc_name,"/oscCF_DEV%i/Blob%i/Point%i/Y",i,inputCam[i].blobsOrder[e],j);
								osc_message.setAddress(osc_name);
								osc_message.addFloatArg(inputCam[i]._osc_contourSmooth[inputCam[i].blobsOrder[e]].at(j).y);
								osc_bundle.addMessage(osc_message);
								sendBuffer();
								cleanBuffer();
							}
						}else if(inputCam[i].cfDetail == 2 && inputCam[i].blobsOrder[e] < MAX_USERS_HARDLIMIT){
							osc_message.clear();
							sprintf(osc_name,"/oscCF_DEV%i/Blob%i/NumPoints",i,inputCam[i].blobsOrder[e]);
							osc_message.setAddress(osc_name);
							osc_message.addIntArg(inputCam[i].contourSimple[inputCam[i].blobsOrder[e]].size());
							osc_bundle.addMessage(osc_message);
							for(unsigned int j = 0; j < inputCam[i].contourSimple[inputCam[i].blobsOrder[e]].size(); j++){
								osc_message.clear();
								sprintf(osc_name,"/oscCF_DEV%i/Blob%i/Point%i/X",i,inputCam[i].blobsOrder[e],j);
								osc_message.setAddress(osc_name);
								osc_message.addFloatArg(inputCam[i]._osc_contourSimple[inputCam[i].blobsOrder[e]].at(j).x);
								osc_bundle.addMessage(osc_message);
								osc_message.clear();
								sprintf(osc_name,"/oscCF_DEV%i/Blob%i/Point%i/Y",i,inputCam[i].blobsOrder[e],j);
								osc_message.setAddress(osc_name);
								osc_message.addFloatArg(inputCam[i]._osc_contourSimple[inputCam[i].blobsOrder[e]].at(j).y);
								osc_bundle.addMessage(osc_message);
								sendBuffer();
								cleanBuffer();
							}
						}
					}
				}
				
				// sending contour geometry data
				if(inputCam[i].sendOsc_CG && inputCam[i].computeContourFinder && inputCam[i].computeContourGeometry){
					for (unsigned int e = 0; e < inputCam[i].runningBlobs; e++){
						if(inputCam[i].blobsOrder[e] < MAX_USERS_HARDLIMIT){
							osc_message.clear();
							sprintf(osc_name,"/oscCG_DEV%i/Blob%i/NumLines",i,inputCam[i].blobsOrder[e]);
							osc_message.setAddress(osc_name);
							osc_message.addIntArg(inputCam[i].geomLines[inputCam[i].blobsOrder[e]].size());
							osc_bundle.addMessage(osc_message);
							for(unsigned int j = 0; j < inputCam[i].geomLines[inputCam[i].blobsOrder[e]].size(); j++){
								osc_message.clear();
								sprintf(osc_name,"/oscCG_DEV%i/Blob%i/Line%i/X",i,inputCam[i].blobsOrder[e],j);
								osc_message.setAddress(osc_name);
								osc_message.addFloatArg(inputCam[i]._osc_blobGeom[inputCam[i].blobsOrder[e]].at(j).x);
								osc_bundle.addMessage(osc_message);
								osc_message.clear();
								sprintf(osc_name,"/oscCG_DEV%i/Blob%i/Line%i/Y",i,inputCam[i].blobsOrder[e],j);
								osc_message.setAddress(osc_name);
								osc_message.addFloatArg(inputCam[i]._osc_blobGeom[inputCam[i].blobsOrder[e]].at(j).y);
								osc_bundle.addMessage(osc_message);
								osc_message.clear();
								sprintf(osc_name,"/oscCG_DEV%i/Blob%i/Line%i/Z",i,inputCam[i].blobsOrder[e],j);
								osc_message.setAddress(osc_name);
								osc_message.addFloatArg(inputCam[i]._osc_blobGeom[inputCam[i].blobsOrder[e]].at(j).z);
								osc_bundle.addMessage(osc_message);
								osc_message.clear();
								sprintf(osc_name,"/oscCG_DEV%i/Blob%i/Line%i/W",i,inputCam[i].blobsOrder[e],j);
								osc_message.setAddress(osc_name);
								osc_message.addFloatArg(inputCam[i]._osc_blobGeom[inputCam[i].blobsOrder[e]].at(j).w);
								osc_bundle.addMessage(osc_message);
								sendBuffer();
								cleanBuffer();
							}
						}
					}
				}
				
				// sending optical flow data
				if(inputCam[i].sendOsc_OF && inputCam[i].computeOpticalFlow){
					for(unsigned int o=0;o<inputCam[i].opticalFlowYGrid;o++){ // Y
						for(unsigned int oo=0;oo<inputCam[i].opticalFlowXGrid;oo++){ // X
							osc_message.clear();
							sprintf(osc_name,"/oscOF_DEV%i/PixelVel_X%i_Y%i",i,oo,o);
							osc_message.setAddress(osc_name);
							osc_message.addFloatArg(inputCam[i]._osc_opfVel[oo+(inputCam[i].opticalFlowXGrid*o)].x);
							osc_message.addFloatArg(inputCam[i]._osc_opfVel[oo+(inputCam[i].opticalFlowXGrid*o)].y);
							osc_bundle.addMessage(osc_message);
							sendBuffer();
							cleanBuffer();
						}
					}
				}
				
				// sending haar finder data
				if(inputCam[i].sendOsc_HF && inputCam[i].computeHaarFinder){
					osc_message.clear();
					sprintf(osc_name,"/oscHF_DEV%i/NumFaces",i);
					osc_message.setAddress(osc_name);
					osc_message.addFloatArg(inputCam[i].numFace);
					osc_bundle.addMessage(osc_message);
					for(unsigned int f = 0; f < inputCam[i].numFace; f++){
						osc_message.clear();
						sprintf(osc_name,"/oscHF_DEV%i/Face%i",i,f);
						osc_message.setAddress(osc_name);
						osc_message.addFloatArg(inputCam[i]._osc_ftInfo[f].x);
						osc_message.addFloatArg(inputCam[i]._osc_ftInfo[f].y);
						osc_message.addFloatArg(inputCam[i]._osc_ftInfo[f].z);
						osc_message.addFloatArg(inputCam[i]._osc_ftInfo[f].w);
						osc_bundle.addMessage(osc_message);
						sendBuffer();
						cleanBuffer();
					}
				}
				
				// sending trigger areas data
				if(inputCam[i].sendOsc_TA && inputCam[i].computeContourFinder && inputCam[i].computeTriggerAreas){
					osc_message.clear();
					sprintf(osc_name,"/oscTA_DEV%i",i);
					osc_message.setAddress(osc_name);
					for(unsigned int t=0;t<TRIGGER_AREAS_NUM;t++){
						osc_message.addIntArg((int)inputCam[i].triggerState[t]);
					}
					osc_bundle.addMessage(osc_message);
					sendBuffer();
					cleanBuffer();
				}
			}
		}
	}
	////////////////////////////////////////////
	
	////////////////////////////////////////////
	// send audio input analysis data
	if(audioActivated){
		for(unsigned int i=0;i<audioInputChannels;i++){
			if(inputAudioCH[i].captureAudio){
				// send channel volume detection
				if(inputAudioCH[i].sendOsc_VD){
					osc_message.clear();
					sprintf(osc_name,"/oscVOL_CH%i",i);
					osc_message.setAddress(osc_name);
					osc_message.addFloatArg(inputAudioCH[i]._osc_chVolume);
					osc_bundle.addMessage(osc_message);
					sendBuffer();
					cleanBuffer();
				}
				// send channel pitch detection
				if(inputAudioCH[i].sendOsc_PD){
					osc_message.clear();
					sprintf(osc_name,"/oscPITCH_CH%i",i);
					osc_message.setAddress(osc_name);
					osc_message.addFloatArg(inputAudioCH[i]._osc_chPitch);
					osc_bundle.addMessage(osc_message);
					sendBuffer();
					cleanBuffer();
				}
				// send channel bark scale bins power (psychoacoustical fft scale)
				if(inputAudioCH[i].sendOsc_BK){
					osc_message.clear();
					sprintf(osc_name,"/oscBK_CH%i/BarkBins",i);
					osc_message.setAddress(osc_name);
					for(unsigned int e = 0; e < BARK_SCALE_CRITICAL_BANDS; e++){
						osc_message.addFloatArg(inputAudioCH[i]._osc_barkBins[e]);
					}
					osc_bundle.addMessage(osc_message);
					sendBuffer();
					cleanBuffer();
				}
			}
		}
	}
	////////////////////////////////////////////
	
	////////////////////////////////////////////
	// send arduino data
	if(arduinoActivated){
		if(useArduino){
			// send analog pins readings
			if(sendOsc_AAP){
				osc_message.clear();
				sprintf(osc_name,"/oscAAP");
				osc_message.setAddress(osc_name);
				for(unsigned int i=0;i<6;i++){
					osc_message.addFloatArg(_osc_analogPinValues[i]);
				}
				osc_bundle.addMessage(osc_message);
				sendBuffer();
				cleanBuffer();
			}
			// send digital pins readings
			if(sendOsc_ADP){
				osc_message.clear();
				sprintf(osc_name,"/oscADP");
				osc_message.setAddress(osc_name);
				for(unsigned int i=2;i<14;i++){
					osc_message.addIntArg(_osc_digitalPinValuesInput[i-2]);
				}
				osc_bundle.addMessage(osc_message);
				sendBuffer();
				cleanBuffer();
			}
		}
	}
	////////////////////////////////////////////
	
	
}

//--------------------------------------------------------------
void gamuzaMain::sendBuffer(){
	
	////////////////////////////////////////////
	// send OSC data bundle
	sender.sendBundle(osc_bundle);
	////////////////////////////////////////////
	
}

//--------------------------------------------------------------
void gamuzaMain::cleanBuffer(){
	
	////////////////////////////////////////////
	// clean OSC buffer
	osc_bundle.clear();
	osc_message.clear();
	////////////////////////////////////////////
	
}

#endif
