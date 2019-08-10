#include <iostream>
#include <opencv2/opencv.hpp>
#include<sstream>

using namespace cv;
using namespace std;

int* definir_pontos(int argc, char* argv[]){
  stringstream ss;
  int* pontos = new int[argc];
  for (int i = 0; i < argc - 1; i++) {
    ss.clear();
    ss.str(argv[i + 1]);
    ss >> pontos[i];
  }
  return pontos;
}

int main(int argc, char *argv[]){
  Mat image;
  image = imread("biel.png", CV_LOAD_IMAGE_COLOR);
  if(!image.data)
    cout << "nao abriu biel.png";

  int* pontos = definir_pontos(argc, argv);

  namedWindow("regions", WINDOW_AUTOSIZE);
  for(int i = pontos[0]; i < pontos[2]; i++){
    for(int j = pontos[1]; j < pontos[3]; j++){
      image.at<uchar>(i, j) = 255 - image.at<uchar>(i, j);
    }
  }

  imshow("regions", image);
  waitKey();

  return 0;
}
