#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

bool houve_movimento(Mat hist, Mat cp_hist, float maxima_diferenca=4, float threshold=8){
  /*
  Detecta mudança de movimento baseado nos histogramas passado e atual
  
  Parametros
  ----------
    hist: histograma do frame atual
    cp_hist: histograma do frame passado
    maxima_diferenca: valor limiar para que a frequencia de uma escala de cinza em hist
                      ser considerada diferente da frequencia da mesma escala de cinza
                      em cp_hist
    threshold: valor limiar de diferenças para que seja estabelecido que houve movimento

  Retorna
  -------
    true se a quantidade de mudancas > threshold, false, c.c.
  */
  if ((hist.rows != cp_hist.rows) || (hist.cols != cp_hist.cols)){
    return -1;
  }
  int mudancas = 0;
  for(int i=0; i<hist.rows; i++){
    if ((hist.at<float>(i) - cp_hist.at<float>(i)) > maxima_diferenca){
      mudancas++;
    }
  }
  if (mudancas > threshold){
    return true;
  }
  return false;
}

int main(int argc, char** argv){
  Mat image;
  int width, height;
  VideoCapture cap;
  vector<Mat> planes;
  Mat hist, cp_hist;
  int nbins = 64;
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;
  Mat hsvImg;
  string text_status;

  cap.open(0);

  if(!cap.isOpened()){
    cout << "cameras indisponiveis";
    return -1;
  }

  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  while(1){
    cap >> image;
    split (image, planes);

    calcHist(&planes[0], 1, 0, Mat(), hist, 1,
             &nbins, &histrange,
             uniform, acummulate);

    normalize(hist, hist, 0, 256, NORM_MINMAX, -1, Mat());
    
    if (houve_movimento(hist, cp_hist)){
      text_status = "movimento detectado";
    }else{
      text_status = "";
    }

    cp_hist = hist.clone();

    putText(image, text_status, Point2f(100, 480), FONT_HERSHEY_PLAIN, 2,  Scalar(0, 0, 255), 2, 8, false);
    imshow("image", image);
    if(waitKey(1) == 27) break;
  }
  return 0;
}