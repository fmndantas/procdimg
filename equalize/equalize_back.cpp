#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){

  VideoCapture cap;
  int height, width;
  int histSize = 256;
  float range[] = {0, histSize};
  const float* histRange = {range};
  Mat image, copy;
  Mat histogram, histogram_copy;
  bool uniform = true, accumulate = false;
  int histdisplay_width = 128, histdisplay_height = 68;
  
  cap.open(0);

  if(!cap.isOpened()){
    cout << "cameras indisponiveis";
    return -1;
  }

  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  while(1){
    cap >> image;
    cvtColor(image, image, CV_BGR2GRAY);
    
    copy = image.clone();
    
    Mat histImage(histdisplay_height, histdisplay_width, CV_8U, Scalar(0, 0, 0));
    Mat histImageCopy(histdisplay_height, histdisplay_width, CV_8U, Scalar(0, 0, 0));

    calcHist(&image, 1, 0, Mat(), histogram, 1, &histSize, &histRange, uniform, accumulate);
    histogram_copy = histogram.clone(); 
    normalize(histogram_copy, histogram_copy, 0, histImage.rows, NORM_MINMAX, -1, Mat());

    equalizeHist(image, image); 
  
    calcHist(&image, 1, 0, Mat(), histogram, 1, &histSize, &histRange, uniform, accumulate);
    normalize(histogram, histogram, 0, histImage.rows, NORM_MINMAX, -1, Mat());

    for(int i=0; i<histSize; i++){
      line(
        histImage, 
        Point(i, histdisplay_height),
        Point(i, histdisplay_height - cvRound(histogram.at<float>(i))),
        Scalar(255, 255, 255), 1, 8, 0
      );
      line(
        histImageCopy,
        Point(i, histdisplay_height),
        Point(i, histdisplay_height - cvRound(histogram_copy.at<float>(i))),
        Scalar(255, 255, 255), 1, 8, 0  
      );
    }

    histImage.copyTo(image(Rect(0, 0, histImage.cols, histImage.rows)));
    histImageCopy.copyTo(copy(Rect(0, 0, histImage.cols, histImage.rows)));

    imshow("equalizada", image);
    imshow("nao-equalizada", copy);

    if(waitKey(1) == 27) break;
  }
  return 0;
}