/*
 * Code to use Sobel operator to find gradient of a grayscale 
 * image and then return the pixel orientation matrix
 */
#include <opencv2/opencv.hpp>


#define PI 3.14159265


using namespace cv;


/*
 * function to compute pixel orientation
 */
Mat compute_orientation(const Mat matrix)
{
        // initialize matrices for calculation of pixel orientation
        Mat Gx(matrix.cols, matrix.rows, CV_64FC1);
        Mat Gy(matrix.cols, matrix.rows, CV_64FC1);
        Mat Vx(matrix.cols, matrix.rows, CV_64FC1);
        Mat Vy(matrix.cols, matrix.rows, CV_64FC1);
        Mat orientation(matrix.cols, matrix.rows, CV_64FC1);


        // computing image gradient with Sobel operator
        Sobel(matrix, Gx, CV_64F, 1, 0, 3);
        Sobel(matrix, Gy, CV_64F, 0, 1, 3);


        // determine pixel orientation, adjust it to [0, 180] degree range
        Vx = 2 * Gx.mul(Gy);
        Vy = Gx.mul(Gx) - Gy.mul(Gy);

        for(int i = 0; i < Gx.rows; i++)
                for(int j = 0; j < Gx.cols; j++)
                        orientation.at<double>(i, j) = 90 + atan2(Vx.at<double>(i, j),\
                                        Vy.at<double>(i, j)) * 0.5 * 180 / PI ;


        return orientation;
}
