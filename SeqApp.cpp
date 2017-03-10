#include <iostream>
#include <cmath>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace std;
using namespace cv;


/*
 * function to perform thinning of the binary image
 */
void thinning(const Mat& src, Mat& dst);


/*
 * function to compute orientation of the pixels
 */
Mat compute_orientation(const Mat matrix);


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
	orientation = compute_orientation(img_mat);

	
	// obtain binary image from the blurred image
	threshold(img_blur, img_thresh, 100, 255, CV_THRESH_BINARY);
	
	
	// perform thinning of the binary image
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

