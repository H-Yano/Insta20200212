#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	int span = 20;
	for (int x = -200; x <= 200; x += span) {

		for (int y = -200; y <= 200; y += span) {

			for (int z = -200; z <= 200; z += span) {

				bool x_flag = x < -140 || x > 140;
				bool y_flag = y < -140 || y > 140;
				bool z_flag = z < -140 || z > 140;

				if (((x_flag || y_flag) && z_flag) || ((y_flag || z_flag) && x_flag)) {

					this->location_list.push_back(glm::vec3(x, y, z));
				}
			}
		}
	}

	for (auto& location : this->location_list) {

		vector<int> next_index = vector<int>();
		int index = -1;
		for (auto& other : this->location_list) {

			index++;
			if (location == other) { continue; }

			float distance = glm::distance(location, other);
			if (distance <= span) {

				next_index.push_back(index);
			}
		}

		this->next_index_list.push_back(next_index);
	}

	for (int i = 0; i < 1500; i++) {

		this->actor_list.push_back(make_unique<Actor>(this->location_list, this->next_index_list, this->destination_list));
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	this->face.clear();
	this->frame.clear();

	int frame_span = 15;
	int prev_index_size = 0;

	if (ofGetFrameNum() % frame_span == 0) {

		prev_index_size = this->destination_list.size();
	}

	for (auto& actor : this->actor_list) {

		actor->update(frame_span, this->location_list, this->next_index_list, this->destination_list);
		this->setBoxToMesh(this->face, this->frame, actor->getLocation(), 20);
	}

	if (prev_index_size != 0) {

		this->destination_list.erase(this->destination_list.begin(), this->destination_list.begin() + prev_index_size);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	ofSetColor(39);
	this->face.draw();

	ofSetColor(239);
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setBoxToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float size) {

	this->setBoxToMesh(face_target, frame_target, location, size, size, size);
}

//--------------------------------------------------------------
void ofApp::setBoxToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float height, float width, float depth) {

	int index = face_target.getVertices().size();

	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.99, height * 0.5 * 0.99, depth * -0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.99, height * 0.5 * 0.99, depth * -0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.99, height * 0.5 * 0.99, depth * 0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.99, height * 0.5 * 0.99, depth * 0.5 * 0.99));

	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.99, height * -0.5 * 0.99, depth * -0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.99, height * -0.5 * 0.99, depth * -0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.99, height * -0.5 * 0.99, depth * 0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.99, height * -0.5 * 0.99, depth * 0.5 * 0.99));

	face_target.addIndex(index + 0); face_target.addIndex(index + 1); face_target.addIndex(index + 2);
	face_target.addIndex(index + 0); face_target.addIndex(index + 2); face_target.addIndex(index + 3);

	face_target.addIndex(index + 4); face_target.addIndex(index + 5); face_target.addIndex(index + 6);
	face_target.addIndex(index + 4); face_target.addIndex(index + 6); face_target.addIndex(index + 7);

	face_target.addIndex(index + 0); face_target.addIndex(index + 4); face_target.addIndex(index + 1);
	face_target.addIndex(index + 4); face_target.addIndex(index + 5); face_target.addIndex(index + 1);

	face_target.addIndex(index + 1); face_target.addIndex(index + 5); face_target.addIndex(index + 6);
	face_target.addIndex(index + 6); face_target.addIndex(index + 2); face_target.addIndex(index + 1);

	face_target.addIndex(index + 2); face_target.addIndex(index + 6); face_target.addIndex(index + 7);
	face_target.addIndex(index + 7); face_target.addIndex(index + 3); face_target.addIndex(index + 2);

	face_target.addIndex(index + 3); face_target.addIndex(index + 7); face_target.addIndex(index + 4);
	face_target.addIndex(index + 4); face_target.addIndex(index + 0); face_target.addIndex(index + 3);

	frame_target.addVertex(location + glm::vec3(width * -0.5, height * 0.5, depth * -0.5));
	frame_target.addVertex(location + glm::vec3(width * 0.5, height * 0.5, depth * -0.5));
	frame_target.addVertex(location + glm::vec3(width * 0.5, height * 0.5, depth * 0.5));
	frame_target.addVertex(location + glm::vec3(width * -0.5, height * 0.5, depth * 0.5));

	frame_target.addVertex(location + glm::vec3(width * -0.5, height * -0.5, depth * -0.5));
	frame_target.addVertex(location + glm::vec3(width * 0.5, height * -0.5, depth * -0.5));
	frame_target.addVertex(location + glm::vec3(width * 0.5, height * -0.5, depth * 0.5));
	frame_target.addVertex(location + glm::vec3(width * -0.5, height * -0.5, depth * 0.5));

	frame_target.addIndex(index + 0); frame_target.addIndex(index + 1);
	frame_target.addIndex(index + 1); frame_target.addIndex(index + 2);
	frame_target.addIndex(index + 2); frame_target.addIndex(index + 3);
	frame_target.addIndex(index + 3); frame_target.addIndex(index + 0);

	frame_target.addIndex(index + 4); frame_target.addIndex(index + 5);
	frame_target.addIndex(index + 5); frame_target.addIndex(index + 6);
	frame_target.addIndex(index + 6); frame_target.addIndex(index + 7);
	frame_target.addIndex(index + 7); frame_target.addIndex(index + 4);

	frame_target.addIndex(index + 0); frame_target.addIndex(index + 4);
	frame_target.addIndex(index + 1); frame_target.addIndex(index + 5);
	frame_target.addIndex(index + 2); frame_target.addIndex(index + 6);
	frame_target.addIndex(index + 3); frame_target.addIndex(index + 7);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}