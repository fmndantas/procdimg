#include <iostream>
#include <opencv2/opencv.hpp>
#include <sstream>

#define BRANCO 255
#define PRETO 0

using namespace cv;
using namespace std;

Mat load_img(string dir){
  Mat image;
  image = imread(dir, CV_LOAD_IMAGE_GRAYSCALE);
  return image;
}

bool pixel_eh_cinza(Mat image, int i, int j){
  return image.at<uchar>(i, j) > PRETO && image.at<uchar>(i, j) < BRANCO;
}

Mat colorir_background(Mat image){
  CvPoint p;
  p.x = 0;
  p.y = 0;
  floodFill(image, p, BRANCO);
  return image;
}

Mat remover_bolhas_das_bordas(Mat image){
  CvPoint p;

  for (int j = 0; j < image.cols; j++) {
    p.x = j;
    p.y = 0;
    floodFill(image, p, PRETO);
  }

  for (int j = 0; j < image.cols; j++) {
    p.x = j;
    p.y = image.rows - 1;
    floodFill(image, p, PRETO);
  }

  for (int i = 0; i < image.rows; i++) {
    p.x = 0;
    p.y = i;
    floodFill(image, p, PRETO);
  }

  for (int i = 0; i < image.rows; i++) {
    p.x = image.cols - 1;
    p.y = i;
    floodFill(image, p, PRETO);
  }
  return image;
}

Mat labeling(Mat image, int* objs){
  CvPoint p;
  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
      if(image.at<uchar>(i, j) == BRANCO){
        *objs += 1;
        p.x = j;
        p.y = i;
        floodFill(image, p, *objs);
      }
    }
  }
  return image;
}

Mat contar_buracos(Mat image, int* objs_esburacados){
  CvPoint p;
  cout << *objs_esburacados;
  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
      p.x = j;
      p.y = i;
      if(image.at<uchar>(i, j) == PRETO){
        if(pixel_eh_cinza(image, i, j - 1)){
          floodFill(image, p, BRANCO);
          p.x--;
          floodFill(image, p, BRANCO);
          *objs_esburacados += 1;
        }
      }
    }
  }
  return image;
}

int main(int argc, char *argv[]){

  Mat image = load_img("bolhas.png");

  int objs = 0;
  int objs_esburacados = 0;

  namedWindow("labeling", WINDOW_AUTOSIZE);

  imshow("labeling", image);
  waitKey();

  image = remover_bolhas_das_bordas(image);

  imshow("labeling", image);
  waitKey();

  image = labeling(image, &objs);

  imshow("labeling", image);
  waitKey();

  image = colorir_background(image);

  imshow("labeling", image);
  waitKey();

  image = contar_buracos(image, &objs_esburacados);

  cout << "Quantidade de objetos: " << objs << endl;
  cout << "Quantidade de objetos esburacados: " << objs_esburacados << endl;

  imshow("labeling", image);
  waitKey();

  return 0;
}
