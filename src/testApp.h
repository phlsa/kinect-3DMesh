#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "Smoother.h"
#include "ofxSTL.h"

struct SmoothPoint {
	int x;
	int y;
	Smoother z;
};

class testApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
	ofxKinect		kinect;
	int 			camWidth;
	int 			camHeight;
	float			threshFront;
	float			threshBack;
	ofImage			kinectImage;
	
	ofMesh			mesh;
	int				res3D;
	float			depthFactor;
	
	vector<SmoothPoint> points;
	
	bool			shouldSave;
	bool			useColor;
};