/*
 * Code to calculate ridge orientation field
 */
#include <iostream>
#include <opencv2/opencv.hpp>


#define PI 3.14159265


using namespace cv;


/*
 * function to calculate gradient in x and y direction of the image
 */
void gradient(const Mat image, Mat xGradient, Mat yGradient)
{
	xGradient = Mat::zeros(image.rows, image.cols, CV_64FC1);
	yGradient = Mat::zeros(image.rows, image.cols, CV_64FC1);

	for (int i = 1; i < image.rows - 1; i++)
	{
		for (int j = 1; j < image.cols - 1; j++)
		{
			double xPixel1 = image.at<double>(i, j - 1);
			double xPixel2 = image.at<double>(i, j + 1);

			double yPixel1 = image.at<double>(i - 1, j);
			double yPixel2 = image.at<double>(i + 1, j);

			double xGrad = 0.0;
			double yGrad = 0.0;

			if (j == 0)
			{
				xPixel1 = image.at<double>(i, j);
				xGrad = xPixel2 - xPixel1;
			}
			else if (j == image.cols - 1)
			{
				xPixel2 = image.at<double>(i, j);
				xGrad = xPixel2 - xPixel1;
			}
			else
			{
				xGrad = 0.5 * (xPixel2 - xPixel1);
			}

			if (i == 0)
			{
				yPixel1 = image.at<double>(i, j);
				yGrad = yPixel2 - yPixel1;
			}
			else if (i == image.rows - 1)
			{
				yPixel2 = image.at<double>(i, j);
				yGrad = yPixel2 - yPixel1;
			}
			else
			{
				yGrad = 0.5 * (yPixel2 - yPixel1);
			}

			xGradient.at<double>(i, j) = xGrad;
			yGradient.at<double>(i, j) = yGrad;
		}
	}
}


/*
 * function to calculate ridge orientation field
 */
Mat ridge_orient(Mat im, double gradientsigma, double blocksigma, double orientsmoothsigma)
{
        Mat orientim , grad_x, grad_y, denom, sin2theta, cos2theta;
	int size1 = 6 * round(gradientsigma);

	if (size1 % 2 == 0)
	{
		size1 += 1;
	}

	// define gaussian kernel
	Mat gaussKernelX = getGaussianKernel(size1, gradientsigma, CV_64FC1);
	Mat gaussKernelY = getGaussianKernel(size1, gradientsigma, CV_64FC1);
	Mat gaussKernel = gaussKernelX * gaussKernelY.t();

	// peform gaussian filtering
	Mat fx, fy;
	Mat kernelx = (Mat_<double>(1, 3) << -0.5, 0, 0.5);
	Mat kernely = (Mat_<double>(3, 1) << -0.5, 0, 0.5);
	filter2D(gaussKernel, fx, -1, kernelx);
	filter2D(gaussKernel, fy, -1, kernely);

	// gradient of gaussian
	gradient(gaussKernel, fx, fy);

	grad_x.convertTo(grad_x, CV_64FC1);
	grad_y.convertTo(grad_y, CV_64FC1);

	filter2D(im, grad_x, -1, fx, Point(-1, -1), 0, BORDER_DEFAULT); // gradient of the image in x
	filter2D(im, grad_y, -1, fy, Point(-1, -1), 0, BORDER_DEFAULT); // gradient of the image in y

	Mat grad_xx, grad_xy, grad_yy;
	multiply(grad_x, grad_x, grad_xx);
	multiply(grad_x, grad_y, grad_xy);
	multiply(grad_y, grad_y, grad_yy);

	// now smooth the covariance data to perform a weighted summation of the data
	int size2 = 6 * round(blocksigma);

	if (size2 % 2 == 0)
	{
		size2 += 1;
	}

	Mat gaussKernelX2 = getGaussianKernel(size2, blocksigma, CV_64FC1);
	Mat gaussKernelY2 = getGaussianKernel(size2, blocksigma, CV_64FC1);
	Mat gaussKernel2 = gaussKernelX2 * gaussKernelY2.t();

	filter2D(grad_xx, grad_xx, -1, gaussKernel2, Point(-1, -1), 0, BORDER_DEFAULT);
	filter2D(grad_xy, grad_xy, -1, gaussKernel2, Point(-1, -1), 0, BORDER_DEFAULT);
	filter2D(grad_yy, grad_yy, -1, gaussKernel2, Point(-1, -1), 0, BORDER_DEFAULT);

	grad_xy *= 2;

	// analytic solution of principal direction
	Mat G1, G2, G3;
	multiply(grad_xy, grad_xy, G1);
	G2 = grad_xx - grad_yy;
	multiply(G2, G2, G2);

	G3 = G1 + G2;
	sqrt(G3, denom);

	divide(grad_xy, denom, sin2theta);
	divide(grad_xx - grad_yy, denom, cos2theta);


	int size3 = 6 * round(orientsmoothsigma);

	if (size3 % 2 == 0)
	{
		size3 += 1;
	}

	Mat gaussKernelX3 = getGaussianKernel(size3, orientsmoothsigma, CV_64FC1);
	Mat gaussKernelY3 = getGaussianKernel(size3, orientsmoothsigma, CV_64FC1);
	Mat gaussKernel3 = gaussKernelX3 * gaussKernelY3.t();

	filter2D(cos2theta, cos2theta, -1, gaussKernel3, Point(-1, -1), 0, BORDER_DEFAULT);
	filter2D(sin2theta, sin2theta, -1, gaussKernel3, Point(-1, -1), 0, BORDER_DEFAULT);

	sin2theta.convertTo(sin2theta, CV_64FC1);
	cos2theta.convertTo(cos2theta, CV_64FC1);
	orientim = Mat::zeros(sin2theta.rows, sin2theta.cols, CV_64FC1);

	for (int i = 0; i < sin2theta.rows; i++)
	{
		for (int j = 0; j < sin2theta.cols; j++)
		{
			double sinPix = sin2theta.at<double>(i, j);
			double cosPix = cos2theta.at<double>(i, j);
			double pix = PI / 2 + std::atan2(sinPix, cosPix) / 2;
			orientim.at<double>(i, j) = pix;
		}
	}

	return orientim;
}
