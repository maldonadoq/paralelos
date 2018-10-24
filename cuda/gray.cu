#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include "src/gray.h"

int main(int argc, char const *argv[]){

	cv::Mat src_in = cv::imread("file.png", cv::IMREAD_COLOR);
	if(src_in.empty()){
		std::cout << "Error : Image cannot be loaded..!!n";
		return 0;
	}

	cv::namedWindow("window_in", CV_WINDOW_NORMAL);
	cv::imshow("window_in", src_in);

	unsigned cols = src_in.cols;
	unsigned rows = src_in.rows;

	std::cout << "cols: " << cols << "\n";
	std::cout << "rows: " << rows << "\n";

	pixel *in = new pixel[cols*rows];
	float *out = new float[cols*rows];


	unsigned tmp, i, j;
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
		    in[(i*cols)+j].set_r((float)src_in.at<cv::Vec3b>(i, j)[0]);
		    in[(i*cols)+j].set_g((float)src_in.at<cv::Vec3b>(i, j)[1]);
		    in[(i*cols)+j].set_b((float)src_in.at<cv::Vec3b>(i, j)[2]);
		}
	}

	gray_scale(in, out, cols, rows);

	cv::Mat src_out(rows,cols, CV_8UC3, cv::Scalar(0,0,0));	
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
			tmp = (i*cols)+j;
			src_out.at<cv::Vec3b>(i, j)[0] = out[tmp];
			src_out.at<cv::Vec3b>(i, j)[1] = out[tmp];
			src_out.at<cv::Vec3b>(i, j)[2] = out[tmp];
		}
	}

	cv::namedWindow("window_out", CV_WINDOW_NORMAL);
	cv::imshow("window_out", src_out);

	cv::waitKey(0);
	cv::destroyWindow("window_in");
	cv::destroyWindow("window_out");

	delete in;
	delete out;
	return 0;
}