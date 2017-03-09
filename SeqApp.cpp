#include <iostream>
#include <cmath>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

#define PI 3.14159265

using namespace std;
using namespace cv;
int main()
{
	int i, j;
	Mat img, img_db;
	
	// read fingerprint image
	img = imread("f0001_01.png", CV_LOAD_IMAGE_GRAYSCALE);
	
	// convert image to double precision
	img.convertTo(img_db, CV_64FC1);

	// initialize matrices for calculation of pixel orientation
	Mat Gx(img_db.cols, img_db.rows, CV_64FC1);
	Mat Gy(img_db.cols, img_db.rows, CV_64FC1);
	Mat Vx(img_db.cols, img_db.rows, CV_64FC1);
	Mat Vy(img_db.cols, img_db.rows, CV_64FC1);
	Mat orientation(img_db.cols, img_db.rows, CV_64FC1);

	// computing image gradient with Sobel operator
	Sobel(img_db, Gx, CV_64F, 1, 0, 3);
	Sobel(img_db, Gy, CV_64F, 0, 1, 3);
	
	// determine pixel orientation, adjust it to [0, 180] degree range
	Vx = 2*Gx.mul(Gy);
	Vy = Gx.mul(Gx) - Gy.mul(Gy);
	for(int i = 0; i < Gx.rows; i++)
		for(int j= 0; j< Gx.cols; j++)
		{
			orientation.at<double>(i,j) = 90 + atan2(Vx.at<double>(i,j),Vy.at<double>(i,j)) * 0.5 * 180/PI ;
		}

#if 0
	// write:
FileStorage fs("myfile.txt",FileStorage::WRITE);
fs << "mat1" << orientation;

// read:
FileStorage fs("myfile.txt",FileStorage::READ);
fs["mat1"] >> m;
#endif



	imshow("fingerprint",img);
	waitKey(0);
	

	return 0;
}

