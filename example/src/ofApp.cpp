#include "ofApp.h"

float getNearestVertex(const ofMesh& mesh, const ofVec2f& target, int& vertexIndex) {
	float bestDistance = 0;
	for(int i = 0; i < mesh.getNumVertices(); i++) {
		float distance = target.distance(mesh.getVertex(i));
		if(distance < bestDistance || i == 0) {
			bestDistance = distance;
			vertexIndex = i;
		}
	}
	return bestDistance;
}

ofMesh makeGrid(ofRectangle square, int nHoriz, int nVert) {
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	for (int j = 0; j < nVert; j++){
    for (int i = 0; i < nHoriz; i++){
			float x = ofMap(i, 0, nHoriz-1, square.x, square.x + square.width);
			float y = ofMap(j, 0, nVert-1, square.y, square.y + square.height);
			mesh.addVertex(ofPoint(x,y));
		}
	}
	for ( unsigned int y = 0; y < (nVert-1); y++ ) {
		for ( unsigned int x = 0; x < nHoriz-1; x++ ) {
			unsigned int nRow1 = y * nHoriz;
			unsigned int nRow2 = (y+1) * nHoriz;
			mesh.addIndex(nRow1 + x);
			mesh.addIndex(nRow2 + x + 1);
			mesh.addIndex(nRow1 + x + 1);
			mesh.addIndex(nRow1 + x);
			mesh.addIndex(nRow2 + x);
			mesh.addIndex(nRow2 + x + 1);
		}
	}
	return mesh;
}

void ofApp::setup(){
	ofSetVerticalSync(true);
	selected = false;
	ofMesh mesh = makeGrid(ofRectangle(100,100,600,600), 10, 10);
	puppet.setup(mesh);
	
	puppet.setControlPoint(0); selectedVertices.insert(0);
	puppet.setControlPoint(9); selectedVertices.insert(9);
}

void ofApp::update(){
	puppet.update();
}

void ofApp::draw(){
	ofBackground(0);
	ofSetColor(255);
	puppet.drawWireframe();
	
	ofNoFill();
	ofSetColor(ofColor::red);
	for(set<unsigned int>::iterator itr = selectedVertices.begin(); itr != selectedVertices.end(); itr++) {
		ofVec2f cur = puppet.getDeformedMesh().getVertex(*itr);
		ofCircle(cur, 5); 
	}
}

void  ofApp::mousePressed(int x, int y, int button){
	float distance = getNearestVertex(puppet.getDeformedMesh(), ofVec2f(x, y), selectedVertex);
	if(distance < 10) {
		if(button == 0) {
			selected = true;
			selectedVertices.insert(selectedVertex);
		} else if(button == 2) {
			if(selectedVertices.find(selectedVertex) != selectedVertices.end()) {
				selectedVertices.erase(selectedVertex);
				puppet.removeControlPoint(selectedVertex);
			}
		}
	} else {
		selected = false;
	}
}

void  ofApp::mouseDragged(int x, int y, int button){
	if(selected) {
		puppet.setControlPoint(selectedVertex, ofVec2f(x, y));
	}
}

void  ofApp::mouseReleased(int x, int y, int button){
	selected = false;
}











