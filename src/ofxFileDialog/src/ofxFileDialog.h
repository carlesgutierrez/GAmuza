/*
 *  ofxFileDialog.h
 *
 *  original version by timknapen on 07/05/10.
 *  http://forum.openframeworks.cc/index.php?PHPSESSID=ngecnj1itpajpbbj5crl4u90c5&/topic,955.0.html
 *	www.wereldderindianen.be
 *	code stolen from 'mantissa' over here:
 *	http://www.openframeworks.cc/forum/viewtopic.php?p=5028#p5028
 *
 */


#ifndef _OFX_FILE_DIALOG_
#define _OFX_FILE_DIALOG_

#include "ofMain.h"


class ofxFileDialog{

		public:

			static int openFile(string& URL);
			static int saveFile(string& URL, string& fileURL);


};

#endif
