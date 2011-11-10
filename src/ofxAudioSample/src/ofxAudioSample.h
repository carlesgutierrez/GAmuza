#ifndef _OFXAUDIOSAMPLE
#define _OFXAUDIOSAMPLE

// sound stream capabilities from libsndfile
#include "sndfile.hh"

#include "ofMain.h"

typedef struct{
	
	double minL;
	double maxL;
	double minR;
	double maxR;
	
}MiniMaxima;

class ofxAudioSample{
	
	public:
	
		
		ofxAudioSample();
		ofxAudioSample(string tmpPath);
	
		~ofxAudioSample();
	
		string	getPath() { return myPath;}
		bool    getIsLooping();
		bool    getIsLoaded();
		bool    getIsPlaying();
		bool    getIsPaused();
		double	getPosition();
		double  getSpeed();
		
		void	setPath(string newPath);
		void	setLooping(bool loop);
		void	setPosition(double _position);
		void    setPaused(bool bPaused);
		void    setSpeed(double speed);
		
		bool	load(string tmpPath);
		bool	read();
		void	play();
		void    stop();
		double  update();
		
		bool	save();
		char*	getSummary();
		int		getChannels();
		int		getSampleRate();
		long	getLength();
		
		void	generateWaveForm(vector<MiniMaxima> * _WaveForm);
		void	drawWaveForm(int _x, int _y, int _w, int _h, vector<MiniMaxima> * _WaveForm);
		
		
		char* 	myData;

	private:
	
		enum SoundFlags { NONE = 0, LOADED = 1, PLAYING = 2, PAUSED = 4, LOOPING = 8 };
		//Good = Open | Edit | Save | Close,
		//Bad = Corrupt | Busy
		
		string			myPath;
		int				myChunkSize;
		int				mySubChunk1Size;
		short			myFormat;
		short			myChannels;
		int				mySampleRate;
		int				myByteRate;
		short			myBlockAlign;
		short			myBitsPerSample;
		int				myDataSize;
		double			position;
		double			speed;
		double			output;
		bool			isLooping;
		unsigned char	soundStatus;
	
};

#endif