#ifndef CARDETECTION_H
#define CARDETECTION_H

#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/bgsegm.hpp>

#include "car.h"

using namespace std;
using namespace cv;

class CarDetection
{
private:
    Ptr<BackgroundSubtractor> substractor;
    VideoCapture videoSample;
    VideoCapture trainset;
    VideoCapture outputVid;
    vector<Car> cars;
    int carNumber = 0;
    int roadType;
public:
    CarDetection(string filepath, int roadType);
    void showSampleVideo();
    void detectCars();
    void trainSubtractor(VideoCapture vid,Ptr<BackgroundSubtractor> &sbstr, int history );
    Mat detectCarHorizontally(Mat original, int distance);
    Mat detectCarVertically(Mat original, int distance);

};

#endif // CARDETECTION_H
