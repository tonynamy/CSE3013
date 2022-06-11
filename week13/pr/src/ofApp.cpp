/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.

	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

	http://www.spout.zeal.co

	=========================================================================
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
	=========================================================================

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"
#include <iostream>
using namespace std;
//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isdfs = false;
	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth() - windowWidth) / 2, (ofGetScreenHeight() - windowHeight) / 2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked

	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS", false, false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();

} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if (title == "Open") {
		readFile();
	}
	if (title == "Exit") {
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if (title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			DFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;

	}

	if (title == "Show BFS") {
		doTopmost(bChecked); // Use the checked value directly

	}

	if (title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if (title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j;

	// TO DO : DRAW MAZE; 
	// add code here

	if (isOpen) {

		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {

				if (maze[i * WIDTH + j] & WALL_UP) ofDrawLine(j * MAZE_SIZE, i * MAZE_SIZE, (j + 1) * MAZE_SIZE, i * MAZE_SIZE);
				if (maze[i * WIDTH + j] & WALL_DOWN) ofDrawLine(j * MAZE_SIZE, (i + 1) * MAZE_SIZE, (j + 1) * MAZE_SIZE, (i + 1) * MAZE_SIZE);
				if (maze[i * WIDTH + j] & WALL_LEFT) ofDrawLine(j * MAZE_SIZE, i * MAZE_SIZE, j * MAZE_SIZE, (i + 1) * MAZE_SIZE);
				if (maze[i * WIDTH + j] & WALL_RIGHT) ofDrawLine((j + 1) * MAZE_SIZE, i * MAZE_SIZE, (j + 1) * MAZE_SIZE, (i + 1) * MAZE_SIZE);

			}
		}
	}

	if (isdfs)
	{
		dfsRouteColor = ofColor(236, 152, 196);
		escapeRouteColor = ofColor(129, 205, 251);

		ofSetColor(escapeRouteColor);
		ofSetLineWidth(5);

		if (isOpen)
			dfsdraw();
		else
			cout << "You must open file first" << endl;
	}
	if (bShowInfo) {
		// Show keyboard duplicates of menu functions
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight() - 20);
	}

} // end Draw


void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if (bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else {
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU));
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth() - ofGetWidth()) / 2, (ofGetScreenHeight() - ofGetHeight()) / 2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if (bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if (bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if (GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	// Escape key exit has been disabled but it can be checked here
	if (key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if (bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			ofExit();
		}
	}

	// Remove or show screen info
	if (key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if (key == 'f') {
		bFullscreen = !bFullscreen;
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
bool ofApp::readFile()
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		printf("file name is %s\n", fileName);
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
				isOpen = 1;
			}

			ofBuffer buffer(file);

			// Input_flag is a variable for indication the type of input.
			// If input_flag is zero, then work of line input is progress.
			// If input_flag is one, then work of dot input is progress.
			int input_flag = 0;

			// Idx is a variable for index of array.
			int idx = 0;

			// Read file line by line
			int cnt = 0;

			// TO DO
			vector<string> lines;
			for (auto line : buffer.getLines()) {
				lines.push_back(line);
			}

			HEIGHT = (lines.size() - 1) / 2;
			WIDTH = (lines[0].size() - 1) / 2;

			input = (char**)malloc(sizeof(char*) * lines.size());
			maze = (int*)malloc(sizeof(int) * HEIGHT * WIDTH);
			route = (int*)malloc(sizeof(int) * HEIGHT * WIDTH);

			memset(maze, 0, sizeof(int) * HEIGHT * WIDTH);
			memset(route, 0, sizeof(int) * HEIGHT * WIDTH);

			for (int i = 0; i < lines.size(); i++) {
				input[i] = (char*)malloc(sizeof(char) * lines[i].size());
				strcpy(input[i], lines[i].c_str());
			}

			for (int i = 0; i < HEIGHT; i++) {
				for (int j = 0; j < WIDTH; j++) {
					int y = 1 + i * 2;
					int x = 1 + j * 2;

					if (input[y - 1][x] == '-') maze[i * WIDTH + j] |= WALL_UP;
					if (input[y + 1][x] == '-') maze[i * WIDTH + j] |= WALL_DOWN;
					if (input[y][x - 1] == '|') maze[i * WIDTH + j] |= WALL_LEFT;
					if (input[y][x + 1] == '|') maze[i * WIDTH + j] |= WALL_RIGHT;

				};
			}

			/* DEBUG
			for (int i = 0; i < lines.size(); i++) {
				cout << input[i] << "\n";
			}

			for (int i = 0; i < HEIGHT; i++) {
				for (int j = 0; j < WIDTH; j++) {
					bitset<4> x(maze[i * WIDTH + j]);
					cout << x << " ";
				}
				cout << "\n";
			}
			*/

			return true;
		}
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
}
void ofApp::freeMemory() {

	//TO DO
	if (isOpen) {
		for (int i = 0; i < HEIGHT * 2 + 1; i++) {
			free(input[i]);
		}
		free(input);
		free(route);
		free(maze);
	}
}

bool ofApp::DFS()
{
	// DFS 탐색 스택
	stack<pair<int, int>> dfsStack;
	dfsStack.push({ 0, 0 });

	// 탈출 경로 스택
	stack<pair<int, int>> escapeStack;

	while (!dfsStack.empty()) {

		auto top = dfsStack.top();
		dfsStack.pop();

		int i = top.first;
		int j = top.second;

		// 방문한 경로이면 건너뛰기
		if (route[i * WIDTH + j] & ROUTE_BFS) {
			continue;
		}

		// 우선 탈출 경로에 추가
		escapeStack.push(top);

		// 방문 표시
		route[i * WIDTH + j] |= ROUTE_BFS;

		// 종점이면 break
		if (i == HEIGHT - 1 && j == WIDTH - 1) break;

		int originalSize = dfsStack.size();

		if (!(maze[i * WIDTH + j] & WALL_UP) && ! (route[(i - 1)*WIDTH+j] & ROUTE_BFS)) {
			dfsStack.push({ i - 1, j });
		}
		if (!(maze[i * WIDTH + j] & WALL_DOWN) && ! (route[(i + 1) * WIDTH + j] & ROUTE_BFS)) {
			dfsStack.push({ i + 1, j });
		}
		if (!(maze[i * WIDTH + j] & WALL_LEFT) && ! (route[i * WIDTH + j - 1] & ROUTE_BFS)) {
			dfsStack.push({ i, j - 1 });
		}
		if (!(maze[i * WIDTH + j] & WALL_RIGHT) && ! (route[i * WIDTH + j + 1] & ROUTE_BFS)) {
			dfsStack.push({ i, j + 1 });
		}

		// 더 이상 진행할 길이 없고
		// dfsStack이 비어있지 않은 경우
		if (originalSize == dfsStack.size() && !dfsStack.empty()) {

			top = dfsStack.top();

			i = top.first;
			j = top.second;

			// dfs 마지막 탐색 지점을 갈 수 있는 정점을 방문할 때까지 탈출 경로 stack pop
			while (!escapeStack.empty()) {

				auto rTop = escapeStack.top();
				int ri = rTop.first;
				int rj = rTop.second;

				bool isAdjacent = false;
				int di[4] = { 1 , -1, 0, 0 };
				int dj[4] = { 0, 0, 1, -1 };
				int dw[4] = { WALL_DOWN, WALL_UP, WALL_RIGHT, WALL_LEFT };

				for (int k = 0; k < 4; k++) {
					if (i == ri + di[k] && j == rj + dj[k] && !(maze[ri * WIDTH + rj] & dw[k])) {
						isAdjacent = true;
						break;
					}
				}

				if (isAdjacent) break;

				escapeStack.pop();
			}

		}
	}

	// 탈출 경로들에 flag 추가
	while (!escapeStack.empty()) {
		auto top = escapeStack.top();
		escapeStack.pop();

		int i = top.first;
		int j = top.second;

		route[i * WIDTH + j] |= ROUTE_ESCAPE;
	}

	isDFS = 1;
	isdfs = true;

	return 1;
}

void ofApp::dfsdraw()
{
	//TO DO 
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {

			int di[4] = { 1 , -1, 0, 0 };
			int dj[4] = { 0, 0, 1, -1 };
			int dw[4] = { WALL_DOWN, WALL_UP, WALL_RIGHT, WALL_LEFT };

			int cx = j * MAZE_SIZE + MAZE_SIZE / 2;
			int cy = i * MAZE_SIZE + MAZE_SIZE / 2;

			for (int k = 0; k < 4; k++) {

				int ni = i + di[k];
				int nj = j + dj[k];

				if (ni < 0 || ni >= HEIGHT || j < 0 || nj >= WIDTH || !route[ni*WIDTH+nj] || maze[i * WIDTH + j] & dw[k]) continue;

				int dx = nj * MAZE_SIZE + MAZE_SIZE / 2;
				int dy = ni * MAZE_SIZE + MAZE_SIZE / 2;

				if (route[i * WIDTH + j] & ROUTE_ESCAPE && route[ni * WIDTH + nj] & ROUTE_ESCAPE) {
					ofSetColor(escapeRouteColor);
				}
				else {
					ofSetColor(dfsRouteColor);
				}

				ofDrawLine(cx, cy, dx, dy);

			}
		}
	}
}

