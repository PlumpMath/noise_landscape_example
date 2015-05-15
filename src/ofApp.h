#pragma once 

#include "ofMain.h"
#include "ofxUI.h"

class ofApp : public ofBaseApp 
{
public:
	void setup();
	void update();
	void draw();
	void exit(); 

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	ofxUISuperCanvas *gui;
	void guiEvent(ofxUIEventArgs &e);

    
    int width = ofGetWidth();
    int height = ofGetHeight();
    
    ofVboMesh mesh;
    vector<ofVec3f> net;
    ofEasyCam camera;
    //non usata
    ofNode lookat;
    
    //grid variables
    int cols;
    int rows;
    float dim_cells;
    
    //noise variables
    //1 noise positioning
    float time_x;
    float time_y;
    float time_z;
    float frequence;
    
    //2noise updating
    float z_off;
    int min_z;
    int max_z;
    float z_off_increment;
    float amplitude;
    float freq;
    float speed;
    int zoom;
    bool draw_faces;
    bool camera_mouse;
    

	float red, green, blue, alpha;
    ofColor backgroundColor;
};