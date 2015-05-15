#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    camera.setPosition(ofPoint(ofGetWidth()/2, ofGetHeight()/2, zoom));
    ofSetFrameRate(100);
    //http://www.learnopengles.com/android-lesson-eight-an-introduction-to-index-buffer-objects-ibos/
    // when drawing 2 rows of squares, the lower vertices of the first row overlap the upper
    // vertices of the second row. We do not draw this vertices twice, but we use indexes
    // to tell the mesh that the vertices in the middle can be used by both rows
    
    // grid vars
    cols = 60;
    rows = 60;
    dim_cells = width/float(cols);
    
    // perlin noise values that affect the
    // initial position of the vertices in the grid
    frequence = 0.8;
    time_x = 1;
    time_y = 10;
    time_z = 100;
    
    // perlin noise update values, initialization
    max_z = 300.00;
    min_z = 50.00;
    
    
    //mesh population
    int current_row = 0;
    int current_col = 0;
    for (int x = 0; x <= cols; x++) {
        for (int y = 0; y <= rows; y++) {
            float noise_x = ofSignedNoise(time_x);
            float noise_y = ofSignedNoise(time_y);
            float noise_z = ofSignedNoise(time_z);
            float tmp_x = ofMap(noise_x * frequence, 0, 1, 0, dim_cells);
            float tmp_y = ofMap(noise_y * frequence, 0, 1, 0, dim_cells);
            float tmp_z = ofMap(noise_z * frequence, 0, 1, min_z, max_z);
            ofVec3f v = ofVec3f(current_row + tmp_x,
                                current_col + tmp_y,
                                tmp_z);
            // uncomment this line to avoid noise in the initial setup
            //ofVec3f v = ofVec3f(current_row, current_col, 0);
            mesh.addVertex(v);
            current_col += dim_cells;
            time_y += 0.1;
            time_x += 0.1;
            time_z += 0.1;
        }
        current_col = 0;
        current_row += dim_cells;
    }
    cout << mesh.getNumVertices() << endl;
    //Adding index, square per square, re-using vertices
    int current_index = 0;
    for (int x = 0; x < cols; x++) {
        for (int y = 0; y < rows; y++) {
            //first triangle
            mesh.addTriangle(current_index,
                             current_index + 1,
                             (current_index + rows)+1
                             );
            //second triangle
            mesh.addTriangle(
                             (current_index + 1),
                             ((current_index + rows)+1),
                             (current_index + rows)+2
                             );
            current_index += 1;
        }
        current_index += 1;
    }

    
	ofSetVerticalSync(true); 
	ofEnableSmoothing();
    //GUI
    //set some sketch parameters
    //Background Color 
    red = 233; 
    blue = 240; 
    green = 52; 
    alpha = 200;
    backgroundColor = ofColor(233, 52, 27);
    
    gui = new ofxUISuperCanvas("VARIABLE BINDING");
    gui->addSpacer();
    gui->addLabel("BACKGROUND", OFX_UI_FONT_MEDIUM);
    gui->addSpacer();
    gui->addSlider("Z_OFF_INC", 0.001, 0.009, z_off_increment);
    gui->addSlider("AMPLITUDE", min_z, max_z, amplitude );
    gui->addSlider("FREQ", 0.01, 30.00, freq);
    gui->addSpacer();
    gui->addSlider("BGR", 0, 255, backgroundColor.r);
    gui->addSlider("BGG", 0, 255, backgroundColor.g);
    gui->addSlider("BGB", 0, 255, backgroundColor.b);
    gui->addSpacer();
    gui->addToggle( "DRAW_FACES", false);
    gui->addToggle( "CAMERA", false);
    gui->addSlider("ZOOM", 10, 600, zoom);
    gui->addSpacer();
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);
    gui->loadSettings("guiSettings.xml");
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void ofApp::update(){
    //iteration through all the vertices of the mesh and reposition.
    int numVerts = mesh.getNumVertices();
    
    for (int i=0; i<numVerts; ++i) {
        ofVec3f vert = mesh.getVertex(i);
        float time = ofGetElapsedTimef() * 0.02;
        float noiseValue = ofSignedNoise(time* freq *z_off)*amplitude;
        //float noiseValue = ofSignedNoise(time*freq)*amplitude;
        //cout << noiseValue << endl;
        vert.z = noiseValue;

        // change the color of the vertice depending on its z value
        float mapped_color = ofMap(vert.z, -min_z, max_z, 0,255);
        //cout << mapped_color << endl;
        mesh.addColor(ofColor(0, 0, mapped_color));
        
        z_off += z_off_increment;
        mesh.setVertex(i, vert);
    }
    
}


//--------------------------------------------------------------
void ofApp::draw()
{    	
	ofBackground(backgroundColor);     
	ofPushStyle();
	
    //disable camera movement because it messup with ofxUi
    if(camera_mouse) {
        camera.enableMouseInput();
    }else{
        camera.disableMouseInput();
    }

    ofEnableDepthTest();
    camera.begin();
        ofPushMatrix();
            ofRotateX(-60);
            ofTranslate(-200, 20, 2);
            if(draw_faces){
                mesh.drawFaces();
            }else{
                mesh.drawWireframe();
            }
        ofPopMatrix();
    camera.end();
    ofDisableDepthTest();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    
	ofPopStyle();
}
//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
	

	if(name == "Z_OFF_INC")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		z_off_increment = slider->getScaledValue();
	}

	else if(name == "BGR")
	{
		ofxUISlider *rslider = (ofxUISlider *) e.widget; 
		backgroundColor.r = rslider->getScaledValue();
	}
    else if(name == "BGG")
    {
        ofxUISlider *rslider = (ofxUISlider *) e.widget;
        backgroundColor.g = rslider->getScaledValue();
    }
    else if(name == "BGB")
    {
        ofxUISlider *rslider = (ofxUISlider *) e.widget;
        backgroundColor.b = rslider->getScaledValue();
    }
    else if(name == "AMPLITUDE")
    {
        ofxUISlider *rslider = (ofxUISlider *) e.widget;
        amplitude = rslider->getScaledValue();
    }
    else if(name == "FREQ")
    {
        ofxUISlider *rslider = (ofxUISlider *) e.widget;
        freq = rslider->getScaledValue();
    }
    else if(name == "DRAW_FACES")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.getToggle();
        draw_faces = toggle->getValue();
    }
    else if(name == "CAMERA")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.getToggle();
        camera_mouse = toggle->getValue();
    }
    else if(name == "ZOOM")
    {
        ofxUISlider *rslider = (ofxUISlider *) e.widget;
        zoom = rslider->getScaledValue();
    }
}
//--------------------------------------------------------------
void ofApp::exit()
{
    gui->saveSettings("guiSettings.xml");
    delete gui; 
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch (key) 
    {            
        case 'g':
        {
            gui->toggleVisible(); 
        }
            break; 
        default:
            break;
    }
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{ 
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
 
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
 

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}