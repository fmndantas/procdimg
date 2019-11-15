#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace cv;
using namespace std;

Mat filtered, frame_32f, out1, out2, tiltshifted;
Mat channels[3], aux_channels[3];
Mat kernel(3, 3, CV_32F);

float mascara[] = {
	1, 2, 1,
	2, 4, 2,
	1, 2, 1
};

VideoCapture cap("video.mpg");
int width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
int hght = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

Mat frame;

int decay_slider = 40, decay_slider_max = 100;
int offset_slider = 250, offset_slider_max = hght/2;
int average_slider = 200, average_slider_max = 300;
int height_slider = 50, height_slider_max = 100;

double dcy;
double avg; 
double height;
int offset;

char trackbar_name[50];

char key;
			
Mat alpha_mask = Mat::zeros(Size(width, hght), CV_8U);
Mat c_alpha_mask = Mat::zeros(Size(width, hght), CV_8U);

double alphafunc(double d, int offset, int i, int rows, double height){
	double ptop = height * (rows - offset);
	double pbottom = ptop + offset;
	return 0.5 * (tanh((i - ptop)/d) - tanh((i - pbottom)/d));
}

void make_mask(Mat* mask, double d, int offset, double height){
	int rows = mask->rows;
	int cols = mask->cols;
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			mask->at<uchar>(i, j) = int(255 * alphafunc(d, offset, i, rows, height));
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

Mat make_kernel(float vec[], int size, bool average=false, float factor=1.0){
  Mat tmp = Mat(size, size, CV_32F, vec);
  if(average){
    Mat aux;
    scaleAdd(tmp, 1/factor, Mat::zeros(size, size, CV_32F), aux);
    swap(aux, tmp);
  }
  return tmp;
}

void video_processing(int, void*){
	cap >> frame;
	if(!frame.empty()){
		dcy = (double) decay_slider;
		avg = (double) average_slider/100;
		offset = (int) offset_slider;
		height = (double) height_slider/100;

		make_mask(&alpha_mask, dcy, offset, height);
		make_negative(&c_alpha_mask, alpha_mask);
		split(frame, channels);
		multiply(alpha_mask, channels[0], aux_channels[0], 1/255.0);	
		multiply(alpha_mask, channels[1], aux_channels[1], 1/255.0);
		multiply(alpha_mask, channels[2], aux_channels[2], 1/255.0);
		merge(aux_channels, 3, out1);
		
		frame.convertTo(frame_32f, CV_32F);
		for(int i=0; i<10; i++){
			filter2D(frame_32f, filtered, frame_32f.depth(), kernel, Point(1, 1), 0);
			frame_32f = filtered.clone();	
		}
		filtered.convertTo(out2, CV_8U);

		split(out2, channels);
		multiply(c_alpha_mask, channels[0], aux_channels[0], 1/255.0);	
		multiply(c_alpha_mask, channels[1], aux_channels[1], 1/255.0);
		multiply(c_alpha_mask, channels[2], aux_channels[2], 1/255.0);
		merge(aux_channels, 3, out2);

		addWeighted(out1, 0.5, out2, 0.5, 0.0, tiltshifted);
		addWeighted(tiltshifted, avg, tiltshifted, 0.0, 0.0, tiltshifted);
	}else{}
}

int main(int argvc, char** argv){
	
	namedWindow("tiltshiftvideo", WINDOW_AUTOSIZE);

	kernel = make_kernel(mascara, 3, true, 16.0);

	sprintf(trackbar_name, "Decay %d", decay_slider_max);
	createTrackbar(trackbar_name, "tiltshiftvideo", &decay_slider, decay_slider_max, video_processing);

	sprintf(trackbar_name, "Offset %d", offset_slider_max);
	createTrackbar(trackbar_name, "tiltshiftvideo", &offset_slider, offset_slider_max, video_processing);

	sprintf(trackbar_name, "Height %d", height_slider_max);
	createTrackbar(trackbar_name, "tiltshiftvideo", &height_slider, height_slider_max, video_processing);

	sprintf(trackbar_name, "Average %d", average_slider_max);
	createTrackbar(trackbar_name, "tiltshiftvideo", &average_slider, average_slider_max, video_processing);

	while(1){
		video_processing(decay_slider, 0);
		if(frame.empty()) break;
		imshow("tiltshiftvideo", tiltshifted);
		key = waitKey(50);
		if(key == 27 || key == 's'){
			destroyAllWindows();
			break;	
		} 

	}

	if(key == 's'){
		cout << "Salvando video" << endl;
		VideoCapture cap("video.mpg");
		cap >> frame;

		VideoWriter writer;
		double fps = 10.0;
		writer.open("video-edited.mp4", -1, fps, frame.size(), true);
		
		for(;;){
			cap >> frame;
			if(frame.empty()){
				destroyAllWindows();
				break;
			}
			video_processing(decay_slider, 0);
			writer.write(tiltshifted);
			imshow("tiltshiftvideo", tiltshifted);
			if(waitKey(1) == 27) break;
		}
	}

	return 0;
}