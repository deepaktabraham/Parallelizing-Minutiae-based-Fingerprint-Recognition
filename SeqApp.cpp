#include <iostream>
#include <cmath>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"


#define PI 3.14159265


using namespace std;
using namespace cv;


/*
 * Zhang-Suen algorithm for thinning binary images
 * -- defined in "api/thin.cpp" --
 */
void thinning(const cv::Mat& src, cv::Mat& dst);


/*
 * function to compute orientation of the pixels
 */
Mat get_orientation(const Mat matrix)
{
	// initialize matrices for calculation of pixel orientation
	Mat Gx(matrix.cols, matrix.rows, CV_64FC1);
	Mat Gy(matrix.cols, matrix.rows, CV_64FC1);
	Mat Vx(matrix.cols, matrix.rows, CV_64FC1);
	Mat Vy(matrix.cols, matrix.rows, CV_64FC1);
	Mat orientation(matrix.cols, matrix.rows, CV_64FC1);


	// computing image gradient with Sobel operator
	Sobel(matrix, Gx, CV_64F, 1, 0, 3);
	Sobel(matrix, Gy, CV_64F, 0, 1, 3);

	
	// determine pixel orientation, adjust it to [0, 180] degree range
	Vx = 2 * Gx.mul(Gy);
	Vy = Gx.mul(Gx) - Gy.mul(Gy);

	for(int i = 0; i < Gx.rows; i++)
		for(int j = 0; j < Gx.cols; j++)
			orientation.at<double>(i, j) = 90 + atan2(Vx.at<double>(i, j),\
					Vy.at<double>(i, j)) * 0.5 * 180 / PI ;
	
	return orientation;
}


/*
 * main function
 */
int main()
{
	Mat img, img_blur, img_mat, img_thresh, img_thin;
	Mat orientation;

	
	// read fingerprint image
	img = imread("f0001_01.png", CV_LOAD_IMAGE_GRAYSCALE);

	
	// perform gaussian blurring of the image
	GaussianBlur(img, img_blur, Size(13, 13), 3, 3);
	
	
	// convert blurred image to a double precision matrix
	img_blur.convertTo(img_mat, CV_64FC1);

	// compute orientation of the pixels	
	orientation = get_orientation(img_mat);

	// perform thresholding on the blurred image
	threshold(img_blur, img_thresh, 100, 255, CV_THRESH_BINARY);
	
	
	// thin the thresholded image
	thinning(img_thresh, img_thin);

#if 0
	// write:
	FileStorage fs("f0001_01.xml", FileStorage::WRITE);
	fs << "f0001_01" << orientation;

	// read:
	FileStorage fs("f0001_01.xml", FileStorage::READ);
	fs["f0001_01"] >> new_orientation;
#endif

	

	imshow("fingerprint", img_thin);
	waitKey(0);
	

	return 0;
}

