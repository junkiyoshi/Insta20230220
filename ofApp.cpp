#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofSetLineWidth(3);
	ofEnableDepthTest();

	auto ico_sphere = ofIcoSpherePrimitive(1, 5);

	this->number_of_sphere = 350;
	while (this->sphere_list.size() < this->number_of_sphere) {

		auto tmp_location = this->make_point(300, ofRandom(0, 60), ofRandom(360), ofRandom(360));
		auto radius = this->sphere_list.size() < 110 ? ofRandom(20, 60) : ofRandom(2, 20);

		bool flag = true;
		for (int i = 0; i < this->sphere_list.size(); i++) {

			if (glm::distance(tmp_location, get<1>(this->sphere_list[i])) < get<2>(this->sphere_list[i]) + radius) {

				flag = false;
				break;
			}
		}

		if (flag) {

			ofColor color;
			color.setHsb(ofRandom(255), 200, 255);

			auto size = (radius * 2) / sqrt(3);

			this->sphere_list.push_back(make_tuple(color, tmp_location, size));
		}
	}

	this->random_seed = 39;
}

//--------------------------------------------------------------
void ofApp::update() {

	if (ofGetFrameNum() % 60 == 0) {

		this->random_seed = ofRandom(1000);
	}

	ofSeedRandom(this->random_seed);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateZ(ofGetFrameNum() * 0.66666666666666666);



	for (int i = 0; i < this->sphere_list.size(); i++) {

		auto location = get<1>(this->sphere_list[i]);
		auto size = get<2>(this->sphere_list[i]);

		ofPushMatrix();
		ofTranslate(location);

		ofRotateZ(ofRandom(360));
		ofRotateY(ofRandom(360));
		ofRotateX(ofRandom(360));

		ofColor color;
		color.setHsb(ofRandom(255), 255, 255);

		auto param = ofGetFrameNum() % 60;
		float scale = 1;
		if (param < 30) {

			scale = ofMap(param, 0, 30, 0, size);
		}
		else {

			scale = ofMap(param, 30, 60, size, 0);
		}

		vector<glm::vec2> vertices;
		for (int deg = 0; deg < 360; deg += 1) {

			vertices.push_back(this->make_point(deg * DEG_TO_RAD) / 20 * scale);
		}


		ofFill();
		ofSetColor(color, 128);

		ofBeginShape();
		ofVertices(vertices);
		ofEndShape(true);

		ofNoFill();
		ofSetColor(color);

		ofBeginShape();
		ofVertices(vertices);
		ofEndShape(true);

		ofPopMatrix();
	}

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v) {

	// 数学デッサン教室 描いて楽しむ数学たち　P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u)) * cos(v);
	auto y = (R + r * cos(u)) * sin(v);
	auto z = r * sin(u);

	return glm::vec3(x, y, z);
}

//--------------------------------------------------------------
//Reference by https://twitter.com/shiffman/status/1095764239665512453
glm::vec2 ofApp::make_point(float theta) {

	float x = 16 * (pow(sin(theta), 3));
	float y = 13 * cos(theta) - 5 * cos(2 * theta) - 2 * cos(3 * theta) - cos(4 * theta);
	return glm::vec2(x, -y);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}