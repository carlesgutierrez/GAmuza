/*
 GAmuza v.03 see <http://gamuza.d3cod3.org>
 
 GAmuza is released under the MIT License.
 
 Copyright (c) 2011 Emanuele Mazza <http://www.d3cod3.org>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 GAmuza includes ofxXmlSettings, an openframeworks add-on that include tinyxml.
 tinyxml is provided 'as-is' under the terms in LICENSE.TINYXML.
 The original distribution of tinyxml can be found at
 http://www.sourceforge.net/projects/tinyxml
 
 GAmuza includes ofxOpenCv, an openframeworks add-on that include OpenCv.
 OpenCv is licensed under BSD LICENSE <http://en.wikipedia.org/wiki/BSD_licenses>.
 The original distribution of OpenCv can be found at
 http://opencv.willowgarage.com/wiki/
 
 GAmuza includes ofxLua, an openframeworks add-on. ofxLua is licensed under the
 terms provided in LICENSE.OFXLUA. The original distribution of ofxLua can be
 found at https://github.com/danomatika/ofxLua
 
 GAmuza includes code from Lua. Lua is licensed under the terms provided in
 LICENSE.LUA. The original distribution of Lua can be found at
 http://www.lua.org/
 
 GAmuza includes code from Luabind. Luabind is licensed under the terms provided in
 LICENSE.LUABIND. The original distribution of Luabind can be found at
 http://www.rasterbar.com/products/luabind
 
 GAmuza includes code from openFrameworks. openFrameworks is licensed under the
 terms provided in LICENSE.OF. The original distribution of openFrameworks can
 can be found at http://www.openframeworks.cc
 
 */

#ifndef _GAMUZA
#define _GAMUZA

#include "ofMain.h"
#include "gamuzaMain.h"

class gamuza : public gamuzaMain{

	public:
		
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void audioIn(float *input, int bufferSize, int nChannels); 
		void audioOut(float *output, int bufferSize, int nChannels);

		///////////////////////////////////////
		gamuzaMain				gamuzaBase;
		///////////////////////////////////////

};

#endif
