#include "ofxAudioSample.h"


//--------------------------------------------------------------
ofxAudioSample::ofxAudioSample(){
    position = 0;
    speed = 1.0;
    soundStatus = NONE;
}

//--------------------------------------------------------------
ofxAudioSample::ofxAudioSample(string tmpPath){
    position = 0;
    speed = 1.0;
    soundStatus = NONE;
	
    myPath = tmpPath;
    read();
}

//--------------------------------------------------------------
ofxAudioSample::~ofxAudioSample(){
    delete myData;
    myChunkSize = NULL;
    mySubChunk1Size = NULL;
    myFormat = NULL;
    myChannels = NULL;
    mySampleRate = NULL;
    myByteRate = NULL;
    myBlockAlign = NULL;
    myBitsPerSample = NULL;
    myDataSize = NULL;
}

//--------------------------------------------------------------
void ofxAudioSample::setPath(string tmpPath){
    myPath = tmpPath;
	
}

//--------------------------------------------------------------
void ofxAudioSample::setLooping(bool loop){
    if(loop) {
        soundStatus |= LOOPING;
    }
    else {
        soundStatus &= ~LOOPING;
    }
}

//--------------------------------------------------------------
bool ofxAudioSample::getIsLooping(){
    if(soundStatus & LOOPING) return true;
    else return false;
}

//--------------------------------------------------------------
bool ofxAudioSample::getIsLoaded(){
    if(soundStatus & LOADED) return true;
    else return false;
}

//--------------------------------------------------------------
bool ofxAudioSample::getIsPlaying(){
    if(soundStatus & PLAYING) return true;
    else return false;
}

//--------------------------------------------------------------
bool ofxAudioSample::getIsPaused(){
    if(soundStatus & PAUSED) return true;
    else return false;
}

//--------------------------------------------------------------
bool ofxAudioSample::load(string tmpPath) {
    myPath = tmpPath;
	bool result = read();
	return result;
}

//--------------------------------------------------------------
void ofxAudioSample::generateWaveForm(vector<MiniMaxima> * _waveForm){
	
	_waveForm->clear();
	
	bool loopState = getIsLooping();
	setLooping(false);
	bool playState = getIsPlaying();
	double tmpSpeed = getSpeed();
	setSpeed(1.0f);
    play();
	// waveform display method based on this discussion http://answers.google.com/answers/threadview/id/66003.html
	
    double sampleL, sampleR;
	while ((long)position<getLength()) {
		
		MiniMaxima mm;
		mm.minL = mm.minR = mm.maxL = mm.maxR = 0;
		
		for (int i = 0; i < 256; i++){
			
		    if(myChannels == 1) {
                sampleL = update();
                sampleR = sampleL;
		    }
		    else {
                sampleL = update()*0.5;
                sampleR = update()*0.5;
		    }
			
			mm.minL = MIN(mm.minL, sampleL);
			mm.minR = MIN(mm.minR, sampleR);
			mm.maxL = MAX(mm.maxL, sampleL);
			mm.maxR = MAX(mm.maxR, sampleR);
			
		}
		
		_waveForm->push_back(mm);
		
 		//cout << mm.minR << " :: " << mm.maxR << " :: " << mm.minL << " :: " << mm.maxL << endl;
	}
	
	position = 0;
	setLooping(loopState);
	setSpeed(tmpSpeed);
	if(playState) play();
}

//--------------------------------------------------------------
void ofxAudioSample::drawWaveForm(int _x, int _y, int _w, int _h, vector<MiniMaxima> * _waveForm){
	
	float waveFormZoomX = (float)_waveForm->size()/(float)_w;
	
	glPushMatrix();
	
	glTranslated(_x, _y, 0);
	
	for (unsigned int i = 1; i < _waveForm->size(); i++){
	    if(myChannels == 1) {
            ofSetColor(0, 0, 255);
            ofLine((i-1)/waveFormZoomX, _h + (int)(_waveForm->at(i-1).minR*_h), i/waveFormZoomX, _h +  (int)(_waveForm->at(i).maxR*_h));
            ofLine(i/waveFormZoomX, _h + (int)(_waveForm->at(i).maxR*_h), i/waveFormZoomX, _h + (int) (_waveForm->at(i).minR*_h));
			
            //ofLine((i-1)/waveFormZoomX, (float)_h+_waveForm->at(i-1).minR*(float)_h, i/waveFormZoomX, (float)_h+_waveForm->at(i).maxR*(float)_h);
			// ofLine((i)/waveFormZoomX, (float)_h+_waveForm->at(i).maxR*(float)_h, (i)/waveFormZoomX, (float)_h+_waveForm->at(i).minR*(float)_h);
	    } else {
            ofSetColor(255, 0, 0);
            //ofLine((i-1)/waveFormZoomX, _waveForm->at(i).minR*(float)_h, i/waveFormZoomX, _waveForm->at(i).maxR*(float)_h);
            ofLine((i-1)/waveFormZoomX, (int)(_waveForm->at(i-1).minL*_h), i/waveFormZoomX, (int)(_waveForm->at(i).maxL*_h));
            ofLine(i/waveFormZoomX, (int)(_waveForm->at(i).maxL*_h), i/waveFormZoomX, (int) (_waveForm->at(i).minL*_h));
			
            ofSetColor(0, 0, 255);
            ofLine((i-1)/waveFormZoomX, _h + (int)(_waveForm->at(i-1).minR*_h), i/waveFormZoomX, _h +  (int)(_waveForm->at(i).maxR*_h));
            ofLine(i/waveFormZoomX, _h + (int)(_waveForm->at(i).maxR*_h), i/waveFormZoomX, _h + (int) (_waveForm->at(i).minR*_h));
	    }
	}
	
	ofSetColor(0, 255, 0);
	
	float waveFormDisplayScale = getLength()/_w;
	
    if(myChannels == 1) {
        ofLine(position/waveFormDisplayScale, -(float)_h*0.0, position/waveFormDisplayScale, (float)_h*2.0);
    }
    else
    {
        ofLine(position/waveFormDisplayScale, -(float)_h*0.5, position/waveFormDisplayScale, (float)_h*1.5);
    }
	
	glPopMatrix();
}

//--------------------------------------------------------------
void ofxAudioSample::stop(){
    position = 0;
    soundStatus &= ~PAUSED;
    soundStatus &= ~PLAYING;
}

//--------------------------------------------------------------
void ofxAudioSample::play(){
    if(speed > 0)
        position = 0;
    else
        position = getLength();
    soundStatus |= PLAYING;
}

//--------------------------------------------------------------
void ofxAudioSample::setPaused(bool bPaused){
    if(bPaused) {
        soundStatus |= PAUSED;
    }
    else {
        soundStatus &= ~PAUSED;
    }
}

//--------------------------------------------------------------
void ofxAudioSample::setSpeed(double spd){
    speed = spd;
}

//--------------------------------------------------------------
double ofxAudioSample::getSpeed(){
    return speed;
}

//--------------------------------------------------------------
int ofxAudioSample::getSampleRate(){
    return mySampleRate;
}

//--------------------------------------------------------------
double ofxAudioSample::update(){
    if(!(soundStatus & PLAYING)) return 0;
    if(soundStatus & PAUSED) return 0;
    if(!getIsLoaded()) return 0;
	
    long length=getLength();
	double remainder;
	short* buffer = (short *)myData;
	position=(position+speed);
	remainder = position - (long) position;
	
    // check if reached EOF
	if ((long) position>length) {
	    if(getIsLooping()) {
            position=0;
	    }
        else {
            soundStatus &= ~PLAYING;
            return 0;
	    }
	}
	
    //check if position less than zero (reverse)
	if ((long) position < 0) {
	    if(getIsLooping()) {
            position = length;
	    }
        else {
            soundStatus &= ~PLAYING;
            return 0;
	    }
	}
	
	output = (double) ((1.0-remainder) * buffer[1+ (long) position] + remainder * buffer[2+(long) position])/32767.0;//linear interpolation
	
	return(output);
}

//--------------------------------------------------------------
long ofxAudioSample::getLength(){
	long length;
	length=myDataSize*0.5;
	return(length);
}

//--------------------------------------------------------------
double ofxAudioSample::getPosition(){
    double pos = position/getLength();
    pos = ofClamp(pos,0.0,1.0);
	return pos;
}

//--------------------------------------------------------------
void ofxAudioSample::setPosition(double _position){
	double pct = ofClamp(_position,0.0,1.0);
	position = pct * getLength();
}

//--------------------------------------------------------------
char* ofxAudioSample::getSummary(){
    char *summary = new char[250];
    sprintf(summary, " Format: %d\n Channels: %d\n SampleRate: %d\n ByteRate: %d\n BlockAlign: %d\n BitsPerSample: %d\n DataSize: %d\n", myFormat, myChannels, mySampleRate, myByteRate, myBlockAlign, myBitsPerSample, myDataSize);
    std::cout << myDataSize;
    return summary;
}

//--------------------------------------------------------------
int ofxAudioSample::getChannels(){
    return myChannels;
}

//--------------------------------------------------------------
bool ofxAudioSample::save(){
    ofToDataPath(myPath);
    fstream myFile (myPath.c_str(), ios::out | ios::binary);
	
    // write the wav file per the wav file format
    myFile.seekp (0, ios::beg);
    myFile.write ("RIFF", 4);
    myFile.write ((char*) &myChunkSize, 4);
    myFile.write ("WAVE", 4);
    myFile.write ("fmt ", 4);
    myFile.write ((char*) &mySubChunk1Size, 4);
    myFile.write ((char*) &myFormat, 2);
    myFile.write ((char*) &myChannels, 2);
    myFile.write ((char*) &mySampleRate, 4);
    myFile.write ((char*) &myByteRate, 4);
    myFile.write ((char*) &myBlockAlign, 2);
    myFile.write ((char*) &myBitsPerSample, 2);
    myFile.write ("data", 4);
    myFile.write ((char*) &myDataSize, 4);
    myFile.write (myData, myDataSize);
	
    return true;
}

//--------------------------------------------------------------
bool ofxAudioSample::read(){
    myPath = ofToDataPath(myPath,true).c_str();
    ifstream inFile( myPath.c_str(), ios::in | ios::binary);
	
    ofLog(OF_LOG_NOTICE, "Reading file %s",myPath.c_str());
	
    if(!inFile.is_open())
    {
        ofLog(OF_LOG_ERROR,"Error opening file. File not loaded.");
        return false;
    }
	
    char id[4];
    inFile.read((char*) &id, 4);
    if(strncmp(id,"RIFF",4) != 0)
    {
        ofLog(OF_LOG_ERROR,"Error reading sample file. File is not a RIFF (.wav) file");
        return false;
    }
	
    inFile.seekg(4, ios::beg);
    inFile.read( (char*) &myChunkSize, 4 ); // read the ChunkSize
	
    inFile.seekg(16, ios::beg);
    inFile.read( (char*) &mySubChunk1Size, 4 ); // read the SubChunk1Size
	
    //inFile.seekg(20, ios::beg);
    inFile.read( (char*) &myFormat, sizeof(short) ); // read the file format.  This should be 1 for PCM
    if(myFormat != 1) {
        ofLog(OF_LOG_ERROR, "File format should be PCM, sample file failed to load.");
        return false;
    }
	
    //inFile.seekg(22, ios::beg);
    inFile.read( (char*) &myChannels, sizeof(short) ); // read the # of channels (1 or 2)
	
    //inFile.seekg(24, ios::beg);
    inFile.read( (char*) &mySampleRate, sizeof(int) ); // read the Samplerate
	
    //inFile.seekg(28, ios::beg);
    inFile.read( (char*) &myByteRate, sizeof(int) ); // read the byterate
	
    //inFile.seekg(32, ios::beg);
    inFile.read( (char*) &myBlockAlign, sizeof(short) ); // read the blockalign
	
    //inFile.seekg(34, ios::beg);
    inFile.read( (char*) &myBitsPerSample, sizeof(short) ); // read the bitsperSample
	
    inFile.seekg(40, ios::beg);
    inFile.read( (char*) &myDataSize, sizeof(int) ); // read the size of the data
    //cout << myDataSize << endl;

    // read the data chunk
    myData = new char[myDataSize];
    inFile.seekg(44, ios::beg);
    inFile.read(myData, myDataSize);

    inFile.close(); // close the input file

    soundStatus |= LOADED;

    return true; // this should probably be something more descriptive
}
