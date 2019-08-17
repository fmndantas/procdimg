#include <iostream>
#include <opencv2/opencv.hpp>
#include <sstream>

using namespace cv;
using namespace std;

Mat load_img(string dir){
  Mat image;
  image = imread(dir, CV_LOAD_IMAGE_COLOR);
  return image;
}

Mat* swapped_quadrants(Mat source){
  int height = source.rows;
  int midx = height / 2;
  int width = source.cols;
  int midy = width / 2;
  Mat* slices = new Mat[4];
  Mat A = source.operator()(Range(midx, height), Range(midy, width));
  Mat B = source.operator()(Range(midx, height), Range(0, midy));
  Mat C = source.operator()(Range(0, midx), Range(midy, width));
  Mat D = source.operator()(Range(0, midx), Range(0, midy));
  slices[0] = A;
  slices[1] = B;
  slices[2] = C;
  slices[3] = D;
  return slices;
}

Mat swapped_img(Mat source){
  Mat H1, H2, V;
  Mat* slices = swapped_quadrants(source);
  hconcat(slices[0], slices[1], H1);
  hconcat(slices[2], slices[3], H2);
  vconcat(H1, H2, V);
  return V;
};

int main(int argc, char *argv[]){
  Mat image = load_img("biel.png");
  Mat swapped = swapped_img(image);

  if (image.rows != swapped.rows || image.cols != swapped.cols){
    cout << "erro nas dimensões das imagens" << endl;
    return -1;
  }

  namedWindow("swap", WINDOW_AUTOSIZE);

  imshow("swap", swapped);
  waitKey();

  return 0;
}
