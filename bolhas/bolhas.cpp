#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image;
  Vec3b val;
  image = imread("bolhas.png", CV_LOAD_IMAGE_COLOR);
  if(!image.data)
    cout << "nao abriu bolhas.png";

  namedWindow("janela", WINDOW_AUTOSIZE);
  for(int i=200; i<210; i++){
    for(int j=10; j<200; j++){
      image.at<uchar>(i, j) = 0;
    }
  }

  imshow("janela", image);
  waitKey();

  val[0] = 0;
  val[1] = 0;
  val[2] = 255;

  for(int i=200; i<210; i++){
    for(int j=10; j<200; j++){
      image.at<Vec3b>(i, j) = val;
    }
  }

  imshow("janela", image);
  waitKey();

  return 0;
}
