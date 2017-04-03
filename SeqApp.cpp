#include "SeqApp.h"


/*
 * main function
 */
int main()
{
	Mat img_src, img_blur, img_mat, img_thresh, img_thin, img_final;

	/*
	 * read fingerprint image
	 */
	img_src = imread("../101_2.tif", CV_LOAD_IMAGE_GRAYSCALE);

	
	/*
	 * perform image enhancement
	 */
	// perform blurring of the image using a median filter
	medianBlur(img_src, img_blur, 3);

	// convert the blurred image to a double precision matrix
	img_blur.convertTo(img_mat, CV_64FC1);

	// normalize the image matrix
	Mat img_norm = normalize(img_mat);

	// calculate ridge orientation
	Mat orientation = ridge_orient(img_norm, 1.0, 5.0, 5.0);
	
        // perform ridge filtering to get the final enhanced image
        Mat img_filtered = ridge_filter(img_norm, orientation, 0.4, 0.4);
	
	// generate mask
	Mat mask = mask_gen(img_mat);
	Mat img_enhanced = mask.mul(img_filtered);

	// convert image format from double to uchar
	img_enhanced.convertTo(img_enhanced, CV_8UC1);

	// obtain binary image from the enhanced image
	threshold(img_enhanced, img_thresh, 100, 255, CV_THRESH_BINARY);

	// perform thinning of the binary image
	thinning(img_thresh, img_thin);
	
	// convert the thinned image to a double precision matrix
	img_thin.convertTo(img_final, CV_64FC1);

	
	/*
	 * identify minutiae points 
	 */
	




	
#if 0	
	//TODO read/write orientations to/from file
	// write:
	FileStorage fs1("f0001_01.xml", FileStorage::WRITE);
	fs1 << "f0001_01" << orientation;

	// read:
	FileStorage fs2("f0001_01.xml", FileStorage::READ);
	fs2["f0001_01"] >> new_orientation;
#endif	

	
	imshow("fingerprint", img_thin);
	waitKey(0);

	return 0;
}
