#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace cv;
using namespace std;

Mat filtered, original_32f, out1, out2, tiltshifted;
Mat channels[3], aux_channels[3];
Mat kernel(3, 3, CV_32F);

float mascara[] = {  // kernel para filtragem
	1, 2, 1,
	2, 4, 2,
	1, 2, 1
};

Mat original = imread("image.jpg");

int decay_slider = 0, decay_slider_max = 100;
int offset_slider = 0, offset_slider_max = original.rows/2;
int alpha_slider = 0, alpha_slider_max = 100;
int average_slider = 0, average_slider_max = 300;

double alpha;
double dcy; 
double avg; 
int offset;

char trackbar_name[50];

char key;

Mat alpha_mask = Mat::zeros(Size(original.rows, original.cols), CV_8U);
Mat c_alpha_mask = Mat::zeros(Size(original.rows, original.cols), CV_8U);			

double alphafunc(double d, int offset, int x, int rows){
	double ptop = offset;
	double pbottom = rows - offset;
	return 0.5 * (tanh((x - ptop)/d) - tanh((x - pbottom)/d));
}

void make_mask(Mat* mask, double d, int offset){
	int rows = mask->rows;
	int cols = mask->cols;
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			mask->at<uchar>(i, j) = int(255 * alphafunc(d, offset, i, rows));
		}
	}
}

void make_negative(Mat* destiny, Mat source){
	for(int i = 0; i < destiny->rows; i++){
		for (int j = 0; j < destiny->cols; j++){
			destiny->at<uchar>(i, j) = 255 - source.at<uchar>(i, j);	
		}
	}
};

Mat make_kernel(float vec[], int size, bool mascara=false, float factor=1.0){
  Mat tmp = Mat(size, size, CV_32F, vec);
  if(mascara){
    Mat aux;
    scaleAdd(tmp, 1/factor, Mat::zeros(size, size, CV_32F), aux);
    swap(aux, tmp);
  }
  return tmp;
}

void on_trackbar_change(int, void*){
	alpha = (double) alpha_slider/alpha_slider_max;
	dcy = (double) decay_slider;
	offset = (double) offset_slider;
	avg = (double) average_slider/100; 
	
	make_mask(&alpha_mask, dcy, offset);
	make_negative(&c_alpha_mask, alpha_mask);
	split(original, channels);
	multiply(alpha_mask, channels[0], aux_channels[0], 1/255.0);	
	multiply(alpha_mask, channels[1], aux_channels[1], 1/255.0);
	multiply(alpha_mask, channels[2], aux_channels[2], 1/255.0);
	merge(aux_channels, 3, out1);
	
	original.convertTo(original_32f, CV_32F);
	kernel = make_kernel(mascara, 3, true, 16.0);
	for(int i=0; i<10; i++){
		filter2D(original_32f, filtered, original_32f.depth(), kernel, Point(1, 1), 0);
		original_32f = filtered.clone();	
	}
	filtered.convertTo(out2, CV_8U);

	split(out2, channels);
	multiply(c_alpha_mask, channels[0], aux_channels[0], 1/255.0);	
	multiply(c_alpha_mask, channels[1], aux_channels[1], 1/255.0);
	multiply(c_alpha_mask, channels[2], aux_channels[2], 1/255.0);
	merge(aux_channels, 3, out2);

	addWeighted(out1, alpha, out2, 1 - alpha, 0.0, tiltshifted);

	addWeighted(tiltshifted, avg, tiltshifted, 0.0, 0.0, tiltshifted);

	imshow("tiltshift", tiltshifted);
}

int main(int argvc, char** argv){
	
	namedWindow("tiltshift", WINDOW_AUTOSIZE);

	sprintf(trackbar_name, "Alpha %d", alpha_slider_max);
	createTrackbar(trackbar_name, "tiltshift", &alpha_slider, alpha_slider_max, on_trackbar_change);

	sprintf(trackbar_name, "Decay %d", decay_slider_max);
	createTrackbar(trackbar_name, "tiltshift", &decay_slider, decay_slider_max, on_trackbar_change);

	sprintf(trackbar_name, "Offset %d", offset_slider_max);
	createTrackbar(trackbar_name, "tiltshift", &offset_slider, offset_slider_max, on_trackbar_change);

	sprintf(trackbar_name, "Average %d", average_slider_max);
	createTrackbar(trackbar_name, "tiltshift", &average_slider, average_slider_max, on_trackbar_change);

	on_trackbar_change(alpha_slider, 0);

	key = waitKey(0);
	if(key == 's'){
		cout << "saving edited copy" << endl;
		imwrite("image-edited.jpg", tiltshifted);
	}

	return 0;
}