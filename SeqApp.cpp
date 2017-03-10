#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#define BLK_SIZE 3


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

int GolayL(IplImage* src, IplImage* dst);

/*
 * main function
 */
int main()
{
	Mat img, img_blur, img_hist, img_mat, img_thresh, img_thin;
	Mat orientation, new_orientation;
	//vector<Mat> img_blk;
	//Scalar mean, stddev;
	
	// read fingerprint image
	img = imread("../f0001_01.png", CV_LOAD_IMAGE_GRAYSCALE);



#if 0
	//TODO normalize the image
	for (int y = 0; y < img.rows - BLK_SIZE + 1; y++)
	{
		for (int x = 0; x < img.cols - BLK_SIZE + 1; x++)
		{
                        img_blk.push_back(img(Rect(x, y, BLK_SIZE, BLK_SIZE)).clone());
		}
	}



	// --- should be same??	
	for(int i = 0; i < BLK_SIZE; i++)
	{
		cout<<"\n";
		for(int j = 0; j < BLK_SIZE; i++)
			cout<<img.at<int>(i, j)<<" ";
	}
	cout<<"\n"<<img_blk[0]<<"\n";
	// ---



	#if 0		
	for(int i = 0; i < img_blk.size(); i++)
	{
		meanStdDev(img_blk[i], mean, stddev, Mat());

		if(stddev[0] > 0.05)
		{
			// something goes in here
		}
	}
	#endif	
	
#endif

	
	// perform gaussian blurring of the image
	GaussianBlur(img, img_blur, Size(3, 3), 0, 0);
	

	// perform histogram equalization of the blurred image
	equalizeHist(img_blur, img_hist);


	// obtain binary image from the equalized image
	threshold(img_hist, img_thresh, 100, 255, CV_THRESH_BINARY);

	
	// perform thinning of the binary image
	thinning(img_thresh, img_thin);


	// convert original image to a double precision matrix
	img.convertTo(img_mat, CV_64FC1);
	
	
	// compute orientation of the pixels	
	orientation = compute_orientation(img_mat);

	// write:
	FileStorage fs("f0001_01.xml", FileStorage::WRITE);
	fs << "f0001_01" << orientation;

	// read:
	FileStorage fs("f0001_01.xml", FileStorage::READ);
	fs["f0001_01"] >> new_orientation;
	

	imshow("fingerprint", img_thin);
	waitKey(0);

	return 0;
}

