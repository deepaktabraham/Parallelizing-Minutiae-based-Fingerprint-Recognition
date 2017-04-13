#ifndef H_SEQ_APP
#define H_SEQ_APP


#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#define DATABASE "xml_database/"


using namespace std;
using namespace cv;


/*
 * structure for minutiae points
 */
struct minutiae_t
{
        int type;       // 1 - ridge ending
                        // 2 - bifurcation
        int x, y;	// position
        double theta;	// orientation
};


/*
 * function to normalize an image matrix
 */
Mat normalize(const Mat img);


/*
 * function to calculate ridge orientation
 */
Mat ridge_orient(const Mat img, double gradient_sigma, double block_sigma, double orient_smooth_sigma);


/*
 * function to perform ridge filtering
 */
Mat ridge_filter(const Mat img, const Mat img_orient, double kx, double ky);


/* 
 * function to generate mask
 */
Mat mask_gen(const Mat img);


/*
 * function to perform thinning of the binary image
 */
void thinning(const Mat& src, Mat& dst);


/*
 * function to compute orientation of the pixels
 */
Mat compute_orientation(const Mat matrix);


/*
 * function to obtain a vector of files in the given directory
 */
int get_files(string dir, vector<string>* files);


/*
 * operator overloading for struct minutiae_t comparison
 */
bool operator==(const vector<struct minutiae_t> & lhs, const vector<struct minutiae_t> & rhs);


/*
 * function to identify and classify minutiae points
 */
vector<struct minutiae_t> minutiae_classify(const Mat d_bin_matrix, const Mat orientation);
#endif
