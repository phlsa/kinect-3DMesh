#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	kinect.init();
	kinect.setVerbose( true );
	kinect.open();
	
	camWidth = kinect.getWidth();
	camHeight = kinect.getHeight();
	
	threshFront = 40;
	threshBack = 90;
	
	res3D = 4;
	depthFactor = 6.5;
	useColor = false;
	
	for ( int y = 0; y < camHeight / res3D; y++ ) {
		for ( int x = 0; x < camWidth / res3D; x++ ) {
			SmoothPoint p;
			p.x = x;
			p.y = y;
			p.z = Smoother( 10 );
			points.push_back( p );
		}
	}
}

//--------------------------------------------------------------
void testApp::update(){
	int pointIndex = 0;
	kinect.update();
	if ( kinect.isFrameNew() ) {
		mesh.clear();
		
		float * distancePixels = kinect.getDistancePixels();
		unsigned char * depthPixels = kinect.getDepthPixels();
		
		// Alle Pixel durchgehen
		for ( int i = 0; i < camWidth*camHeight; i++ ) {
			// Pixel zurŸcksetzen, wenn er au§erhalb der Thresholds liegt
			if ( distancePixels[i] < threshFront || distancePixels[i] > threshBack ) {
				distancePixels[i] = threshBack;
				depthPixels[i] = 0;
			}
			
			// 3D-Daten erzeugen
			int x = i % camWidth;
			int y = i / camWidth;
			if ( x % res3D == 0 && y % res3D == 0 ) {
				float z = threshBack - distancePixels[ i ];
			//	mesh.addVertex( ofVec3f( x, y, z * depthFactor ) );
				points[ pointIndex ].z.addValue( z * depthFactor );
				mesh.addVertex( ofVec3f( x, y, points[ pointIndex ].z.getValue() ) );
				
				if ( useColor ) {
					float brightness = abs( ofMap( points[ pointIndex ].z.getValue() - points[ pointIndex ].z.getLast(), 0, 20, 0, 1.0 ) );
					if ( brightness < 0.5 ) brightness = 0;
					ofFloatColor pixelColor( brightness );
					if ( points[ pointIndex ].z.getValue() == 0 ) pixelColor.a = 0;
					mesh.addColor( pixelColor );
				}
				
				pointIndex++;
			}
		}
		// Bereinigte Tiefenpixel in ein Bild schreiben
		kinectImage.setFromPixels( depthPixels, camWidth, camHeight, OF_IMAGE_GRAYSCALE );
		
		// STL Export
		ofxSTLExporter exporter;
		exporter.beginModel( "kinect" );
		
		// Dreiecke erzeugen
		int meshResX = camWidth / res3D;
		int meshResY = camHeight / res3D;
		for ( int y = 1; y < meshResY; y++ ) {
			for ( int x = 1; x < meshResX; x++ ) {
				mesh.addTriangle( x+y*meshResX, (x-1)+y*meshResX, x+(y-1)*meshResX );
				mesh.addTriangle( (x-1)+(y-1)*meshResX, (x-1)+y*meshResX, x+(y-1)*meshResX );
				
				// STL export
				if ( shouldSave ) {
					exporter.addTriangle( mesh.getVertex( x+y*meshResX ), 
										 mesh.getVertex( (x-1)+y*meshResX ), 
										 mesh.getVertex( x+(y-1)*meshResX ), 
										 ofVec3f(0, 0, 1) );
					exporter.addTriangle( mesh.getVertex( (x-1)+(y-1)*meshResX ), 
										 mesh.getVertex( (x-1)+y*meshResX ), 
										 mesh.getVertex( x+(y-1)*meshResX ), 
										 ofVec3f(0, 0, 1) );
				}
			}
		}
		
		// STL export
		if ( shouldSave ) {
			exporter.useASCIIFormat( true );
			exporter.saveModel( "kinectTestModel.stl" );
			printf( "saved model\n" );
			shouldSave = false;
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground( 0, 0, 0 );
	mesh.setMode( OF_PRIMITIVE_TRIANGLES );
	
	glEnable( GL_DEPTH_TEST );
	ofTranslate( ofGetWidth()/2, ofGetHeight()/2 );
	ofRotate( ofMap( ofGetMouseX(), 0, ofGetWidth(), -180, 180 ), 0, 1, 0 );
	ofRotate( ofMap( ofGetMouseY(), 0, ofGetHeight(), -180, 180 ), 1, 0, 0 );
	ofTranslate( -camWidth/2, -camHeight/2 );
	ofSetColor( 255, 255, 255 );
	mesh.draw();
	ofSetColor( 255, 0, 0 );
	glTranslatef( 0, 0, 1 );
	mesh.drawWireframe();
	ofSetColor( 50, 50, 50 );
	kinectImage.draw( 0, 0 );
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if ( key == 's' ) {
		shouldSave = true;
	} else if ( key == 'c' ) {
		useColor = !useColor;
	} else if ( key == '+' ) {
		threshBack += 1;
	} else if ( key == '-' ) {
		threshBack -= 1;
	} else if ( key == '.' ) {
		threshFront += 1;
	} else if ( key == ',' ) {
		threshFront -= 1;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}