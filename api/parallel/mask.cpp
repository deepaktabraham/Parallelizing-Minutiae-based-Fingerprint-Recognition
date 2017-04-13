/*
 * Code to generate mask
 */
#include <opencv2/opencv.hpp>


#define SIZE 11	// (SIZE x SIZE) should be odd


using namespace cv;


/*
 * function to generate mask
 */
Mat mask_gen(const Mat img_mat)
{
	int r = 0, c = 0; 
	vector<Mat> img_blk;
	// add padding to the matrix
	Mat img_pad(img_mat.rows + SIZE - 1, img_mat.cols + SIZE - 1, CV_64FC1);
	copyMakeBorder(img_mat, img_pad, (SIZE-1)/2, (SIZE-1)/2, (SIZE-1)/2, (SIZE-1)/2, BORDER_REPLICATE);

	// get blocks of size 11x11 from the padded matrix
	for (int y = 0; y < img_pad.rows - SIZE + 1; y++)
	{
		for (int x = 0; x < img_pad.cols - SIZE + 1; x++)
		{
                        img_blk.push_back(img_pad(Rect(x, y, SIZE, SIZE)).clone());
		}
	}

	// obtain mask based on standard deviation
	Mat mask_mat(img_mat.rows, img_mat.cols, CV_64FC1);
	Scalar mean, stddev;
	for(int i = 0; i < img_blk.size(); i++)
	{
		meanStdDev(img_blk[i], mean, stddev, Mat());

		if(stddev[0] > 10)
		{
			mask_mat.at<double>(r, c) = 1.0;
		}
		
		else
		{
			mask_mat.at<double>(r, c) = 0.0;
		}
		
		c++;
		if(c == mask_mat.cols)
		{
			c=0;
			r++;
		}
	}

	// convert mask matrix to grayscale image
	Mat mask(img_mat.rows, img_mat.cols, CV_8UC1);
	mask_mat.convertTo(mask, CV_64FC1, 1, 0);


	// perform morphological operations on the mask
	Mat const structure_elem1 = getStructuringElement(MORPH_ELLIPSE, Size(4, 4));
	Mat const structure_elem2 = getStructuringElement(MORPH_ELLIPSE, Size(14, 14));
	Mat open_result, close_result;
	morphologyEx(mask, open_result, MORPH_OPEN, structure_elem1);
	morphologyEx(open_result, close_result, MORPH_CLOSE, structure_elem2);
	
	return close_result;
}
