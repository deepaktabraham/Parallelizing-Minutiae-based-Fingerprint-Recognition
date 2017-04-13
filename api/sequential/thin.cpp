/*
 * Code for thinning a binary image using the Zhang-Suen algorithm
 *
 * Author:  Nash (nash [at] opencv-code [dot] com) 
 * Website: http://opencv-code.com
 */
#include <opencv2/opencv.hpp>


// [DEEPAK] modified to conform with the 
// coding conventions in the main program
using namespace cv; 


/*
 * function to perform one thinning iteration
 */
static void thinningIteration(Mat& img, int iter)
{
	CV_Assert(img.channels() == 1);
	CV_Assert(img.depth() != sizeof(uchar));
	CV_Assert(img.rows > 3 && img.cols > 3);

	Mat marker = Mat::zeros(img.size(), CV_8UC1);

	int nRows = img.rows;
	int nCols = img.cols;

	if (img.isContinuous()) 
	{
		nCols *= nRows;
		nRows = 1;
	}

	int x, y;
	uchar *pAbove;
	uchar *pCurr;
	uchar *pBelow;
	uchar *nw, *no, *ne;    // north (pAbove)
	uchar *we, *me, *ea;
	uchar *sw, *so, *se;    // south (pBelow)

	uchar *pDst;

	// initialize row pointers
	pAbove = NULL;
	pCurr  = img.ptr<uchar>(0);
	pBelow = img.ptr<uchar>(1);

	for (y = 1; y < img.rows-1; ++y) 
	{
		// shift the rows up by one
		pAbove = pCurr;
		pCurr  = pBelow;
		pBelow = img.ptr<uchar>(y+1);

		pDst = marker.ptr<uchar>(y);

		// initialize col pointers
		no = &(pAbove[0]);
		ne = &(pAbove[1]);
		me = &(pCurr[0]);
		ea = &(pCurr[1]);
		so = &(pBelow[0]);
		se = &(pBelow[1]);

		for (x = 1; x < img.cols-1; ++x) 
		{
			// shift col pointers left by one (scan left to right)
			nw = no;
			no = ne;
			ne = &(pAbove[x+1]);
			we = me;
			me = ea;
			ea = &(pCurr[x+1]);
			sw = so;
			so = se;
			se = &(pBelow[x+1]);

			int A  = (*no == 0 && *ne == 1) + (*ne == 0 && *ea == 1) + 
				(*ea == 0 && *se == 1) + (*se == 0 && *so == 1) + 
				(*so == 0 && *sw == 1) + (*sw == 0 && *we == 1) +
				(*we == 0 && *nw == 1) + (*nw == 0 && *no == 1);
			int B  = *no + *ne + *ea + *se + *so + *sw + *we + *nw;
			int m1 = iter == 0 ? (*no * *ea * *so) : (*no * *ea * *we);
			int m2 = iter == 0 ? (*ea * *so * *we) : (*no * *so * *we);

			if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
				pDst[x] = 1;
		}
	}

	img &= ~marker;
}


/*
 * function for thinning the given binary image
 *
 * Parameters:
 * 	src:  source image
 * 	dst:  destination image
 */
void thinning(const Mat& src, Mat& dst)
{
	dst = src.clone();
	dst /= 255;         // convert to binary image

	Mat prev = Mat::zeros(dst.size(), CV_8UC1);
	Mat diff;

	do 
	{
		thinningIteration(dst, 0);
		thinningIteration(dst, 1);
		absdiff(dst, prev, diff);
		dst.copyTo(prev);
	}while (countNonZero(diff) > 0);

	dst *= 255;
}
