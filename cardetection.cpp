#include "cardetection.h"

CarDetection::CarDetection(string filepath, int roadType)
{
    this->roadType = roadType;

    VideoCapture videoSample(filepath);
    this->videoSample = videoSample;
    VideoCapture trainset(filepath);
    substractor = cv::createBackgroundSubtractorKNN();
    trainSubtractor(trainset, substractor, 500);
}
void CarDetection::showSampleVideo()
{
    Mat frame;
    do
    {
        outputVid >> frame;
        if(frame.empty())
        {
            destroyAllWindows();
            break;
        }
        imshow("video", frame);
        if(waitKey(30) >= 0)
        {
            destroyAllWindows();
            break;
        }
    }
    while(!frame.empty());
}
void CarDetection::trainSubtractor(VideoCapture vid, Ptr<BackgroundSubtractor> &sbstr, int history)
{
    Mat frame, none;
    for(int i = 0; i < history; i++)
    {
        vid >> frame;
        resize(frame, frame, Size(700, 400));
        sbstr->apply(frame, none, 0.001);
    }
}
Mat CarDetection::detectCarHorizontally(Mat original, int distance)
{
    Mat frame, frameBackground;
    Mat kernel = getStructuringElement( MORPH_ELLIPSE, Size( 2, 2 ), Point( 1, 1 ) );
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    frame = original.clone();
    resize(frame, frame, Size(700, 400));
    substractor->apply(frame, frameBackground);

    //Morphological operations
    threshold(frameBackground, frameBackground, 244, 255, THRESH_BINARY);
    erode(frameBackground, frameBackground, kernel, Point(-1,-1), 1);
    dilate(frameBackground, frameBackground, kernel, Point(-1,-1), 3);

    //Finding objects
    findContours(frameBackground, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    for(unsigned int i = 0; i< contours.size(); i++ )
        {
            if(contourArea(contours[i]) > frame.rows/12 * frame.cols/12)
            {
                drawContours(frameBackground, contours,i, ( 255), CV_FILLED);
            }
        }
    findContours(frameBackground, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );
    vector<Point2f>center( contours.size() );
    vector<float>radius( contours.size() );

    line(original, Point(original.size().width*0.15, original.size().height*0.75), Point(original.size().width * 0.85, original.size().height * 0.75), Scalar( 0, 0, 255 ),5,8,0);
    //Detecting cars
    for( unsigned int i = 0; i< contours.size(); i++ )
    {
        if(contourArea(contours[i]) > frame.rows/20* frame.cols/20)
        {
            approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
            boundRect[i] = boundingRect( Mat(contours_poly[i]) );
            minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
            Point2f carCenter = center[i];
            Car temp(carCenter.x, carCenter.y, radius[i]);
            bool found = false;
            for(auto &j : cars)
            {
                if(pow(carCenter.x - j.centerX, 2) + pow(carCenter.y - j.centerY, 2) < pow(j.radius, 2) && !j.updated)
                {
                    j.update(carCenter.x, carCenter.y, radius[i]);
                    found = true;
                }

            }
            if(!found && !(temp.centerY < frame.size().height*0.75 + 20 && temp.centerY > frame.size().height*0.75 - 20))
                cars.push_back(temp);
        }
    }
    //Counting cars

    for(auto &j : cars)
    {
        j.updated = false;
        if(j.centerY < frame.size().height*0.75 + 10 && j.centerY > frame.size().height*0.75 - 10 && !j.spotted)
        {
            j.spotted = true;
        }

    }
    for(auto &j : cars)
    {

        if(j.spotted && j.history > 7 && !j.counted)
        {
            j.counted = true;
            carNumber++;
        }

    }
    string info = "Number of cars: ";
    info += std::to_string(carNumber);
    putText(original, info, Point(10, 20), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0,0,0), 1, CV_AA);
    return original;

}
Mat CarDetection::detectCarVertically(Mat original, int distance)
{
    Mat frame, frameBackground;
    Mat kernel = getStructuringElement( MORPH_ELLIPSE, Size( 2, 2 ), Point( 1, 1 ) );
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    frame = original.clone();
    resize(frame, frame, Size(700, 400));

    //Background subtraction
    substractor->apply(frame, frameBackground);

    //Morphologica operations
    threshold(frameBackground, frameBackground, 244, 255, THRESH_BINARY);
    erode(frameBackground, frameBackground, kernel, Point(-1,-1), 1);
    dilate(frameBackground, frameBackground, kernel, Point(-1,-1), 3);

    //Finding objects
    findContours(frameBackground, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    for( unsigned int i = 0; i< contours.size(); i++ )
        {
            if(contourArea(contours[i]) > frame.rows/12 * frame.cols/12)
            {
                drawContours(frameBackground, contours,i, (255), CV_FILLED);
            }
        }
    findContours(frameBackground, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );
    vector<Point2f>center( contours.size() );
    vector<float>radius( contours.size() );

    line(original, Point(original.size().width*0.3, original.size().height*0.1), Point(original.size().width * 0.3, original.size().height * 0.9), Scalar( 0, 0, 255 ),5,8,0);

    //Detecting cars
    for( unsigned int i = 0; i< contours.size(); i++ )
    {
        if(contourArea(contours[i]) > frame.rows/20* frame.cols/20)
        {
            approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
            boundRect[i] = boundingRect( Mat(contours_poly[i]) );
            minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
            Point2f carCenter = center[i];
            Car temp(carCenter.x, carCenter.y, radius[i]);
            bool found = false;
            bool allowed = true;
            for(auto &j : cars)
            {
                if(pow(carCenter.x - j.centerX, 2) + pow(carCenter.y - j.centerY, 2) < pow(j.radius, 2) && !j.updated)
                {
                    j.update(carCenter.x, carCenter.y, radius[i]);
                    found = true;
                }
                else if(pow(carCenter.x - j.centerX, 2) + pow(carCenter.y - j.centerY, 2) < pow(j.radius, 2) && j.updated)
                {
                    allowed = false;
                }
            }
            if(!found && allowed && (temp.centerX < frame.size().width*0.3 + 20 && temp.centerX > frame.size().width*0.3 - 20))
                cars.push_back(temp);

        }
    }

    //Counting cars
    for(auto &j : cars)
    {
        j.updated = false;
        if((j.centerX < frame.size().width*0.3 + 10 && j.centerX > frame.size().width*0.3 - 10 ) && !j.spotted)
        {
            j.spotted = true;
        }

    }
    for(auto &j : cars)
    {

        if(j.spotted  && j.history > 7 &&!j.counted)
        {
            j.counted = true;
            carNumber++;
        }

    }
    string info = "Number of cars: ";
    info += std::to_string(carNumber);
    putText(original, info, Point(10, 20), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0,0,0), 1, CV_AA);
    return original;
}

void CarDetection::detectCars()
{
    Mat original;
    int frames_per_second = 30;
    videoSample >> original;
    VideoWriter oVideoWriter("output.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), frames_per_second, original.size(), true);
    do
    {
        if(roadType == 1)
            original = detectCarHorizontally(original, 0.8);
        else
            original = detectCarVertically(original, 0.8);
        oVideoWriter.write(original);
        videoSample >> original;

    }
    while(!original.empty());
    oVideoWriter.release();
}
