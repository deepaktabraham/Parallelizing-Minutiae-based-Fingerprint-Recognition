/*
 * Code for performing image enhancement using 
 * orientation image and frequency
 */
#include <iostream>
#include <opencv2/opencv.hpp>


#define PI 3.14159265


using namespace cv;


/*
 * global
 */
static Mat meshX, meshY;


/*
 * function equivalent to MATLAB's 'meshgrid' function
 */
static void meshgrid(int size)
{
	Range xr = Range(-size, size);
	Range yr = Range(-size, size);

	std::vector<int>t_x, t_y;

	int i, j;
	for (i = xr.start; i <= xr.end; i++)
	{
		t_x.push_back(i);
	}

	for (j = yr.start; j <= yr.end; j++)
	{
		t_y.push_back(j);
	}

	Mat xgv = Mat(t_x);
	Mat ygv = Mat(t_y);

	repeat(xgv.reshape(1, 1), ygv.total(), 1, meshX);
	repeat(ygv.reshape(1, 1).t(), 1, xgv.total(), meshY);
}


/*
 * function to perform ridge filtering
 */
Mat ridge_filter(const Mat inputImage, const Mat orientationImage, double kx, double ky)
{
	int angleInc = 3; // fixed angle increment between filter orientations in degrees

	// frequency defined statically as 0.11
        Mat frequency = Mat::ones(inputImage.rows, inputImage.cols, inputImage.type());
        frequency *= 0.11;

	int rows = inputImage.rows;
	int cols = inputImage.cols;

	Mat enhancedImage = Mat::zeros(rows, cols, CV_64FC1);
	std::vector<int> validr;
	std::vector<int> validc;
	std::vector<Mat> filter;

	int ind = 1;
	double freq = frequency.at<double>(1, 1);
	double unfreq = freq;

	Mat freqindex = Mat::ones(100, 1, CV_64FC1);

	double sigmax = (1 / freq) * kx;
	double sigmay = (1 / freq) * ky;

	int sizek = round(3 * (std::max(sigmax, sigmay)));
	meshgrid(sizek);

	Mat reffilter = Mat::zeros(meshX.rows, meshX.cols, CV_64FC1);

	meshX.convertTo(meshX, CV_64FC1);
	meshY.convertTo(meshY, CV_64FC1);

	int i,j;
	#pragma omp parallel for private(i,j)
	for (i = 0; i < meshX.rows; i++)
	{
		for (j = 0; j < meshX.cols; j++)
		{
			double pixVal2 = -0.5*(meshX.at<double>(i, j)*meshX.at<double>(i, j) / (sigmax*sigmax) +
					meshY.at<double>(i, j)*meshY.at<double>(i, j) / (sigmay*sigmay));
			double pixVal = std::exp(pixVal2);
			double cosVal = 2 * PI * unfreq * meshX.at<double>(i, j);
			reffilter.at<double>(i, j) = pixVal * cos(cosVal);
		}
	}

	for (int m = 0; m < 180 / angleInc; m++)
	{
		double angle = -(m*angleInc + 90);
		Mat rot_mat = getRotationMatrix2D(Point((double)(reffilter.rows / 2.0F), (double)(reffilter.cols / 2.0F)), angle, 1.0);
		Mat rotResult;
		warpAffine(reffilter, rotResult, rot_mat, reffilter.size());
		filter.push_back(rotResult);
	}

	// find indices of matrix points greater than maxsize from the image boundary
	int maxsize = sizek;
	
	// convert orientation matrix values from radians to an index value 
	// that corresponds to round(degrees/angleInc)
	int maxorientindex = std::round(180 / angleInc);

	Mat orientindex(rows, cols, CV_64FC1);

	int y, x;
	for (y = 0; y < rows; y++)
	{
		for (x = 0; x < cols; x++)
		{
			if (x > maxsize && x < rows - maxsize && y > maxsize && y < cols - maxsize)
			{
				validr.push_back(y);
				validc.push_back(x);
			}

			int orientpix = static_cast<int>(std::round(orientationImage.at<double>(y, x) / PI * 180 / angleInc));

			if (orientpix < 0)
			{
				orientpix += maxorientindex;
			}
			if (orientpix >= maxorientindex)
			{
				orientpix -= maxorientindex;
			}

			orientindex.at<double>(y, x) = orientpix;
		}
	}

	// Finally, do the filtering
	int m,n,k;
	for (k = 0; k < validr.size(); k++)
	{
		int s = sizek;
		int r = validr[k];
		int c = validc[k];
		int filterindex = freqindex.at<double>(std::round(frequency.at<double>(r, c) * 100));

		Rect roi(c - s - 1, r - s - 1, meshX.cols, meshX.rows);
		Mat subim(meshX.rows, meshX.cols, CV_64FC1);

		for (m = r - s; m < r + s; m++)
		{
			for (n = c - s; n < c + s; n++)
			{
				int tmpRow = m - r + s;
				int tmpCol = n - c + s;

				if (tmpRow < subim.rows && tmpCol < subim.cols && m < inputImage.rows && n < inputImage.cols)
				{
					subim.at<double>(m - r + s, n - c + s) = inputImage.at<double>(m, n);
				}
			}
		}

		Mat subFilter = filter.at(orientindex.at<double>(r, c));
		Mat mulResult;
		multiply(subim, subFilter, mulResult);

		Scalar resultSum = sum(mulResult);
		double value = resultSum[0];
		if (value > 0)
		{
			enhancedImage.at<double>(r, c) = 255;
		}
	}

	Mat aux = enhancedImage.rowRange(0, rows).colRange(0, sizek + 1);
	aux.setTo(255);

	aux = enhancedImage.rowRange(0, sizek + 1).colRange(0, cols);
	aux.setTo(255);

	aux = enhancedImage.rowRange(rows - sizek, rows).colRange(0, cols);
	aux.setTo(255);

	aux = enhancedImage.rowRange(0, rows).colRange(cols - 2 * (sizek + 1) - 1, cols);
	aux.setTo(255);

	return enhancedImage;
}
