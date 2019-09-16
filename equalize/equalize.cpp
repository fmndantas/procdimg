#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat histograma_acumulado(Mat hist){
  for(int i=1; i<hist.rows; i++){
    hist.at<float>(i) += hist.at<float>(i - 1);
  }
  return hist;
}

int main(int argc, char** argv){
  Mat image, cp;
  int width, height;
  VideoCapture cap;
  vector<Mat> planes;
  Mat histR, histG, histB;
  int nbins = 256;
  float range[] = {0, 256};
  const float *histrange = {range};
  bool uniform = true;
  bool acummulate = false;
  CvPoint p;

  cap.open(0);

  if(!cap.isOpened()){
    cout << "cameras indisponiveis";
    return -1;
  }

  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  int histw = nbins, histh = nbins/2;
  Mat histImgR(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgG(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgB(histh, histw, CV_8UC3, Scalar(0,0,0));

  while(1){
    cap >> image;
    cap >> cp;
    split (image, planes);
    calcHist(&planes[0], 1, 0, Mat(), histR, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[1], 1, 0, Mat(), histG, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[2], 1, 0, Mat(), histB, 1,
             &nbins, &histrange,
             uniform, acummulate);

    histR = histograma_acumulado(histR);
    histG = histograma_acumulado(histG);
    histB = histograma_acumulado(histB);
    
    normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    normalize(histG, histG, 0, histImgG.rows, NORM_MINMAX, -1, Mat());
    normalize(histB, histB, 0, histImgB.rows, NORM_MINMAX, -1, Mat());

    for(int i=0; i<image.rows; i++){
      for(int j=0; j<image.cols; j++){
        Vec3b color = image.at<Vec3b>(Point(j, i));
        color[0] = histR.at<float>(color[0]);
        color[1] = histG.at<float>(color[1]);
        color[2] = histB.at<float>(color[2]);
        image.at<Vec3b>(Point(j, i)) = color;
      }  
    }

    histImgR.setTo(Scalar(0));
    histImgG.setTo(Scalar(0));
    histImgB.setTo(Scalar(0));

    for(int i=0; i<nbins; i++){
      line(histImgR,
           Point(i, histh),
           Point(i, histh-cvRound(histR.at<float>(i))),
           Scalar(0, 0, 255), 1, 4, 0);
      line(histImgG,
           Point(i, histh),
           Point(i, histh-cvRound(histG.at<float>(i))),
           Scalar(0, 255, 0), 1, 4, 0);
      line(histImgB,
           Point(i, histh),
           Point(i, histh-cvRound(histB.at<float>(i))),
           Scalar(255, 0, 0), 1, 4, 0);
    }

    histImgR.copyTo(image(Rect(0, 0       ,nbins, histh)));
    histImgG.copyTo(image(Rect(0, histh   ,nbins, histh)));
    histImgB.copyTo(image(Rect(0, 2*histh ,nbins, histh)));
  
    imshow("equalizada", image);
    imshow("nao-equalizada", cp);
    if(waitKey(1) == 27) break;
  }
  return 0;
}
