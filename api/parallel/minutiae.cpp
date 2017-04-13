#include "SeqApp.h"


/*
 * operator overloading for struct minutiae_t comparison
 */
bool operator==(const vector<struct minutiae_t> & lhs, const vector<struct minutiae_t> & rhs)
{
        if(lhs.size() != rhs.size())
                return false;

        for(int i=0; i<lhs.size(); i++)
        {
                if(lhs[i].type != rhs[i].type ||
                        lhs[i].x != rhs[i].x ||
                        lhs[i].y != rhs[i].y ||
                        lhs[i].theta != rhs[i].theta)
                                return false;
        }

        return true;
}


/*
 * function to identify and classify minutiae points
 */
vector<struct minutiae_t> minutiae_classify(const Mat d_bin_matrix, const Mat orientation)
{
        vector<struct minutiae_t> minutiae;
        struct minutiae_t minutiae_point;
	
        // classify minutiae points
        for(int i=1; i<d_bin_matrix.rows-1; i++)
        {
                for(int j=1; j<d_bin_matrix.cols-1; j++)
                {
                        if(d_bin_matrix.at<double>(i, j) == 1)
                        {
                                // for bifurcation
                                if(
                                        (d_bin_matrix.at<double>(i, j-1) == 1 &&
                                         d_bin_matrix.at<double>(i-1, j+1) == 1 &&
                                         d_bin_matrix.at<double>(i+1, j+1) == 1) ||

                                        (d_bin_matrix.at<double>(i-1, j) == 1 &&
                                         d_bin_matrix.at<double>(i+1, j-1) == 1 &&
                                         d_bin_matrix.at<double>(i+1, j+1) == 1) ||

                                        (d_bin_matrix.at<double>(i-1, j-1) == 1 &&
                                         d_bin_matrix.at<double>(i+1, j-1) == 1 &&
                                         d_bin_matrix.at<double>(i, j+1) == 1) ||

                                        (d_bin_matrix.at<double>(i-1, j-1) == 1 &&
                                         d_bin_matrix.at<double>(i-1, j+1) == 1 &&
                                         d_bin_matrix.at<double>(i+1, j) == 1) ||

                                        (d_bin_matrix.at<double>(i-1, j-1) == 1 &&
                                         d_bin_matrix.at<double>(i+1, j) == 1 &&
                                         d_bin_matrix.at<double>(i, j+1) == 1) ||
					
					(d_bin_matrix.at<double>(i, j-1) == 1 &&
                                         d_bin_matrix.at<double>(i-1, j) == 1 &&
                                         d_bin_matrix.at<double>(i+1, j+1) == 1) ||

                                        (d_bin_matrix.at<double>(i-1, j) == 1 &&
                                         d_bin_matrix.at<double>(i+1, j-1) == 1 &&
                                         d_bin_matrix.at<double>(i, j+1) == 1) ||

                                        (d_bin_matrix.at<double>(i, j-1) == 1 &&
                                         d_bin_matrix.at<double>(i+1, j) == 1 &&
                                         d_bin_matrix.at<double>(i-1, j+1) == 1) ||

                                        (d_bin_matrix.at<double>(i-1, j-1) == 1 &&
                                         d_bin_matrix.at<double>(i+1, j-1) == 1 &&
                                         d_bin_matrix.at<double>(i+1, j+1) == 1) ||

                                        (d_bin_matrix.at<double>(i-1, j-1) == 1 &&
                                         d_bin_matrix.at<double>(i-1, j+1) == 1 &&
                                         d_bin_matrix.at<double>(i+1, j-1) == 1) ||

                                        (d_bin_matrix.at<double>(i-1, j+1) == 1 &&
                                         d_bin_matrix.at<double>(i+1, j-1) == 1 &&
                                         d_bin_matrix.at<double>(i+1, j+1) == 1) ||

                                        (d_bin_matrix.at<double>(i-1, j-1) == 1 &&
                                         d_bin_matrix.at<double>(i-1, j+1) == 1 &&
                                         d_bin_matrix.at<double>(i+1, j+1) == 1) ||

                                        (d_bin_matrix.at<double>(i, j-1) == 1 &&
                                         d_bin_matrix.at<double>(i, j+1) == 1 &&
                                         d_bin_matrix.at<double>(i+1, j) == 1) ||
					
					
                                        (d_bin_matrix.at<double>(i, j-1) == 1 &&
                                         d_bin_matrix.at<double>(i, j+1) == 1 &&
                                         d_bin_matrix.at<double>(i-1, j) == 1) ||

                                        (d_bin_matrix.at<double>(i-1, j) == 1 &&
                                         d_bin_matrix.at<double>(i+1, j) == 1 &&
                                         d_bin_matrix.at<double>(i, j-1) == 1) ||

                                        (d_bin_matrix.at<double>(i-1, j) == 1 &&
                                         d_bin_matrix.at<double>(i+1, j) == 1 &&
                                         d_bin_matrix.at<double>(i, j+1) == 1)
                                )
                                {
                                        minutiae_point.type = 2;
                                }

                                // for ridge ending
                                else
                                {
                                        minutiae_point.type = 1;

                                }

                                minutiae_point.x = i;
                                minutiae_point.y = j;
                                minutiae_point.theta = orientation.at<double>(i, j);
                                minutiae.push_back(minutiae_point);
                        }
                }
        }
	
	return minutiae;
}
