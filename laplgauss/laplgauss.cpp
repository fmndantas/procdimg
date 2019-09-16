#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void printmask(Mat m){
  cout << "\n";
  for(int i=0; i<m.rows; i++){
    for(int j=0; j<m.cols; j++){
      cout << m.at<float>(i,j) << ",";
    }
    cout << endl;
  }
}

Mat make_mask(float vec[], int size, bool average=false, float factor=1.0){
  Mat tmp = Mat(size, size, CV_32F, vec);
  if(average){
    Mat aux;
    scaleAdd(tmp, 1/factor, Mat::zeros(3, 3, CV_32F), aux);
    swap(aux, tmp);
  }
  return tmp;
}

void menu(){
  system("clear");
  cout <<
	"\na - Calcular modulo\n"
  "m - Media\n"
  "g - Gauss\n"
  "v - Vertical\n"
	"h - Horizontal\n"
  "l - Laplaciano\n"
  "d - Laplaciano do gaussiano\n"
	"esc - Sair\n";
}

int main(int argvc, char** argv){
  
  VideoCapture video;
  
  float media[] = {
    1,1,1,
		1,1,1,
		1,1,1
  };

  float gauss[] = {
    1,2,1,
    2,4,2,
    1,2,1
  };

  float horizontal[]={
    -1,0,1,
	  -2,0,2,
	  -1,0,1
  };

  float vertical[]={
    -1,-2,-1,
		0,0,0,
		1,2,1
  };

  float laplacian[]={
    0,-1,0,
    -1,4,-1,
    0,-1,0
  };
	
  Mat cap, frame, frame32f, frameFiltered;
  Mat mask(3, 3, CV_32F);
  Mat result, result1;
  double width, height, min, max;
  int absolut;
  char key;
  
  video.open(0); 
  if(!video.isOpened()) 
    return -1;
  width=video.get(CV_CAP_PROP_FRAME_WIDTH);
  height=video.get(CV_CAP_PROP_FRAME_HEIGHT);
  std::cout << "largura = " << width << "\n" << endl;
  std::cout << "altura = " << height << "\n" << endl;

  mask = make_mask(media, 3, true, 9.0);  //starts with media

  absolut = 1;

  menu();

  for(;;){
    video >> cap; 
    cvtColor(cap, frame, CV_BGR2GRAY);
    flip(frame, frame, 1);
    imshow("original", frame);
    
    frame.convertTo(frame32f, CV_32F);
    
    if(key != 'd'){
      filter2D(frame32f, frameFiltered,
        frame32f.depth(), mask, Point(1,1), 0);
    }else{
      mask = make_mask(gauss, 3, true, 16.0);
      filter2D(frame32f, frameFiltered,
        frame32f.depth(), mask, Point(1,1), 0);
      mask = make_mask(laplacian, 3);
      filter2D(frame32f, frameFiltered,
        frame32f.depth(), mask, Point(1,1), 0);
    }
        
    if(absolut){
      frameFiltered=abs(frameFiltered);
    }

    frameFiltered.convertTo(result, CV_8U);
    
    imshow("filtroespacial", result);
    
    key = (char) waitKey(10);
    
    if(key == 27) break;
    
    switch(key){
      case 'a':
        menu();
        cout << "\nopcao: modulo " << absolut << "\n";
        absolut = !absolut;
        break;
      case 'm':
  	    menu();
        cout << "\nopcao: media\n";
        mask = make_mask(media, 3, true, 9.0);
        printmask(mask);
        break;
      case 'g':
        menu();
        cout << "\nopcao: gauss\n";
        mask = make_mask(gauss, 3, true, 16.0);
        printmask(mask);
        break;
      case 'h':
        menu();
        cout << "\nopcao: horizontal\n";
        mask = make_mask(horizontal, 3);
        printmask(mask);
        break;
      case 'v':
        menu();
        cout << "\nopcao: vertical\n";
        mask = make_mask(vertical, 3);
        printmask(mask);
        break;
      case 'l':
        menu();
        cout << "\nopcao: laplaciano\n";
        mask = make_mask(laplacian, 3);
        printmask(mask);
        break;
      case 'd':
        menu();
        cout << "\nopcao: laplaciano do gaussiano\n";
        break;
      default:
        break;
    }
  }
  return 0;
}
