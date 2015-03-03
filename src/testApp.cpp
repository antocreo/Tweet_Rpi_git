
/*
 Original code from:
https://github.com/pelayomendez/ofxTwitter
 
 Author: Anto+Creo
 
 Adaptation of the example from ofxTwitter.
 This app will allow you to shot a picture and post it automatically on your twitter profile.
 The setup allows serial communication. In my case with Arduino.
 This code has been prepared to work on a Raspberry Pi
 
 Keywords: Twitter, Rpi, Raspberry Pi, serial communication, Arduino, RaspberryPi
 
 This code it was prepared to make OF working on a Raspberry Pi and communicating with Arduino through Serial port.
 The project is part of the MFA Computtional Arts at GOldsmiths College London 2014/15
 
 Links: http://koenagashi.tumblr.com/
 by Antonio Daniele
 
 Notes: NO bugs detected.
 
 */



#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(50);
    
    ofSetVerticalSync(true);
    
    
    ofBackground(255);
    ofSetFrameRate(30);
    
    string const CONSUMER_KEY = "yourconsumer key here";
    string const CONSUMER_SECRET = "your consumer secret here";
    
    twitterClient.authorize(CONSUMER_KEY, CONSUMER_SECRET);

    actualTweet = 0;
    
    ////////////////SERIAL///////////
    
    ofSetVerticalSync(true);
    ofSetWindowPosition(100, 100);
    
    bSendSerialMessage = false;
    //ofSetLogLevel(OF_LOG_VERBOSE);
    
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    
    // this should be set to whatever com port your serial device is connected to.
    // (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
    // arduino users check in arduino app....
    int baud = 9600;
    serial.setup(0, baud); //open the first device
    //serial.setup("COM4", baud); // windows example
    //serial.setup("/dev/tty.usbserial-A4001JEC", baud); // mac osx example
    //serial.setup("/dev/ttyUSB0", baud); //linux example
    
    incomingByte =0;
    
    ////////////////SERIAL END///////////

    
    
    
    //////////camera grabber/////////
    
    camWidth        = 320;	// try to grab at this size.
    camHeight 		= 240;
    vidGrabber.listDevices();
//    vector <ofVideoDevice> videoList = vidGrabber.listDevices();

    vidGrabber.setDeviceID(1);  //external camera
//    vidGrabber.setDeviceID(0); //iSight

    vidGrabber.setDesiredFrameRate(60);
    vidGrabber.initGrabber(camWidth,camHeight);
    finalImage.allocate(camWidth, camHeight, OF_IMAGE_COLOR);
    saveImage = false;
    counter = 0;
    
    ////////// end of camera grabber/////////
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    /////////serial read from arduino///////
    incomingByte = serial.readByte();

    
    if ( incomingByte == OF_SERIAL_NO_DATA ){
        
        /// do nothing
        
    }
    else if (incomingByte == 36){
        printf(">>>>>>>>>>>>>>>>>>incomingByte %d \n", incomingByte);
//        savePicture();  //try this one if saveImage doesn;t work
        saveImage = true;       //this one should prevent the multi-posting.
        
    }
    else if(incomingByte== 18){
        printf("incomingByte %d \n", incomingByte);
        
        binarySeq.push_back(1);
    }
    else if(incomingByte== 19){
        printf("incomingByte %d \n", incomingByte);

        
            binarySeq.push_back(0);

        }
    
//        for (int i=0; i<binarySeq.size(); i++) {
//            printf("incomingByte %d \n", binarySeq[i]);
//
//        }
  

    
    ///////////////
    
    
    vidGrabber.update();
    if (vidGrabber.isFrameNew())
    {
        if(saveImage)
        {
            savePicture();
        }
    }

    
}


//--------------------------------------------------------------
void testApp::savePicture(){
    
    counter++;
    finalImage.getPixelsRef() = vidGrabber.getPixelsRef();
    saveImage = false;
    //            finalImage.saveImage("images/test"+ ofToString(counter)+ ".jpg");
    
    string nameOfImage =
        ofToString(ofGetDay())
        + ofToString(ofGetMonth())
        + ofToString(ofGetYear())
        + ofToString(ofGetHours())
        + ofToString(ofGetMinutes())
        + ofToString(ofGetSeconds());
    
    
    finalImage.saveImage("images/test_" + nameOfImage + "_.jpg");
    
    
    
    string imgPosted = "imagesPost/test_" + ofToString(counter) + "_.jpg";
    finalImage.saveImage(imgPosted);
    
//    twitterClient.postStatus("Tweetting from OF! YO!", imgPosted);
    twitterClient.postStatus(nameOfImage, imgPosted);
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
   
    
    
    ofSetColor(0);
    string info;
    info += "Adaptation from ofxTwitter post example:";
    info += "\nPress 'q' post a 'test' status";
    info += "\nPress h' to obtain info about media_upload service and image size support";
    info += "\nPress s' to shot a picture and post it online";

    ofDrawBitmapString(info, ofPoint(20,20));
    
    twitterClient.printDebugInfo();
    
    
    ofSetColor(255);
    vidGrabber.draw(0,200);
//    finalImage.draw(camWidth,0);
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
    if(key == 'q') {
        twitterClient.postStatus("Let it Brain! Liberati!");
    }
    
    
    if(key == 'h') {
        twitterClient.config.printPhotoSizesInfo();
    }
    
    if (key == 's')
        saveImage = true;

    
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
