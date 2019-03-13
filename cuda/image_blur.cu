#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include "src/blur.h"

int main(int argc, char const *argv[]){

	cv::Mat src_in = cv::imread("tmp.jpg", cv::IMREAD_COLOR);
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
	pixel *blur = new pixel[cols*rows];
	float block = 32;

	unsigned i, j; //tmp;
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
		    in[(i*cols)+j].set_r((float)src_in.at<cv::Vec3b>(i, j)[0]);
		    in[(i*cols)+j].set_g((float)src_in.at<cv::Vec3b>(i, j)[1]);
		    in[(i*cols)+j].set_b((float)src_in.at<cv::Vec3b>(i, j)[2]);
		}
	}	

	kblur(in, blur, cols, rows, block);

	cv::Mat src_out(rows, cols, CV_8UC3, cv::Scalar(0,0,0));
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
		    src_out.at<cv::Vec3b>(i, j)[0] = blur[(i*cols)+j].get_r();
		    src_out.at<cv::Vec3b>(i, j)[1] = blur[(i*cols)+j].get_g();
		    src_out.at<cv::Vec3b>(i, j)[2] = blur[(i*cols)+j].get_b();
		}
	}

	cv::namedWindow("window_blur", CV_WINDOW_NORMAL);
	cv::imshow("window_blur", src_out);

	cv::waitKey(0);
	cv::destroyWindow("window_in");
	cv::destroyWindow("window_blur");

	delete in;
	delete blur;
	return 0;
}