#ifndef H_SEQ_APP
#define H_SEQ_APP

#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#define SIZE 11	// (SIZE x SIZE) should be odd


using namespace std;
using namespace cv;


/*
 * function to normalize an image matrix
 */
Mat normalize(const Mat img);


/*
 * function to calculate ridge orientation field
 */
Mat ridge_orient(const Mat img, double gradient_sigma, double block_sigma, double orient_smooth_sigma);


/*
 * function to perform ridge filtering
 */
Mat ridge_filter(const Mat img, const Mat img_orient, double kx, double ky);


/*
 * function to perform thinning of the binary image
 */
void thinning(const Mat& src, Mat& dst);


/*
 * function to compute orientation of the pixels
 */
Mat compute_orientation(const Mat matrix);

#endif
