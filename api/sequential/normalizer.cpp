/*
 * Code to normalize an image matrix
 */
#include <iostream>
#include <opencv2/opencv.hpp>


using namespace cv;


/*
 * function to calculate standard deviation of the image matrix
 */
static double deviation(Mat img, double avg)
{
	double sdev = 0.0;
	double var = 0.0;
	double sd = 0.0;

	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			double pixel = img.at<double>(i, j);
			double dev = (pixel - avg) * (pixel - avg);
			sdev = sdev + dev;
		}
	}

	var = sdev / (img.rows * img.cols - 1);
	sd = std::sqrt(var);

	return sd;
}


/*
 * function to normalize an image matrix
 */
Mat normalize(const Mat img)
{
	Scalar avg = mean(img);
	Mat img_norm = img - avg[0];
	Scalar mean_norm = mean(img_norm);
	double stddev = deviation(img_norm, mean_norm[0]);
	img_norm = img_norm / stddev;
	return img_norm;
}
