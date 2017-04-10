#include "SeqApp.h"
struct minutiae_t
{
	int type;	// 1 - ridge ending
			// 2 - bifurcation
	int x, y;
	double theta;
};

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
 * main function
 */
int main()
{
	Mat img_src, img_blur, img_src_mat, img_thresh, img_enhanced, bin_matrix, d_bin_matrix;

	/*
	 * read fingerprint image
	 */
	img_src = imread("../101_1.tif", CV_LOAD_IMAGE_GRAYSCALE);

	
	/*
	 * perform image enhancement
	 */
	// perform blurring of the image using a median filter
	medianBlur(img_src, img_blur, 3);

	// convert the blurred image to a double precision matrix
	img_blur.convertTo(img_src_mat, CV_64FC1);

	// normalize the image matrix
	Mat img_norm = normalize(img_src_mat);

	// calculate ridge orientation
	Mat orientation = ridge_orient(img_norm, 1.0, 5.0, 5.0);
	
        // perform ridge filtering to get the final enhanced image
        Mat img_filtered = ridge_filter(img_norm, orientation, 0.4, 0.4);
	
	// generate mask
	Mat mask = mask_gen(img_src_mat);

	// apply mask
	Mat img_masked = mask.mul(img_filtered);

	// convert image format from double to uchar
	img_masked.convertTo(img_masked, CV_8UC1);

	// obtain binary image from the enhanced image
	threshold(img_masked, img_thresh, 100, 255, CV_THRESH_BINARY);

	// perform thinning of the binary image
	thinning(img_thresh, img_enhanced);
	
	// convert to a binary matrix
	threshold(img_enhanced, bin_matrix, 254, 1, THRESH_BINARY);
	
	// convert the binary matrix to a double precision
	bin_matrix.convertTo(d_bin_matrix, CV_64FC1);


	/*
	 * identify minutiae points 
	 */
	// dynamic allocation of memory for storing minutiae details
	vector<struct minutiae_t> minutiae;
	struct minutiae_t minutiae_point;

	// classify minutiae points 
	int k=0;
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
				k++;
			}
		}
	}



#if 0	
	// write:
	FileStorage db("database/109_3.xml", FileStorage::WRITE); 	//TODO handle filenames
	db<<"num_items"<<(int)k;
	db<<"minutiae"<<"[";
	for(int i=0; i<k; i++)
	{
		db<<"{";
		db<<"type"<<(int)minutiae[i].type;
  		db<<"x"<<(int)minutiae[i].x;
		db<<"y"<<(int)minutiae[i].y;
		db<<"theta"<<(double)minutiae[i].theta;
		db<<"}";
	}
	db<<"]";
	db.release();
#endif	


#if 1
	// read:
	string dir = string(DATABASE);
	vector<string> files = vector<string>();
	ostringstream filepath;

	DIR *dp = opendir(dir.c_str());
	struct dirent *dirp;
	if(dp == NULL) 
	{
		cout<<"Error opening "<<dir<<"\n";
		return errno;
	}

	while ((dirp = readdir(dp)) != NULL) 
	{
		if(strncmp(dirp->d_name, ".", 1) != 0) 
		{
			files.push_back(dirp->d_name);
		}
	}
	closedir(dp);

	
	vector<vector <struct minutiae_t>> db_data;
	vector<struct minutiae_t> points;
	int num_items, id;
	for (int i = 0; i<files.size(); i++)
	{
		filepath.clear();
		filepath.str("");
		filepath<<DATABASE<<files[i];
		FileStorage db(filepath.str(), FileStorage::READ);

		if(!db.isOpened())
		{
			cout<<"Database is empty\n";
			return -1;
		}

		num_items= 0;
		db["num_items"] >> num_items;	//TODO everything is read here... why???
		cout<<"error below!\n";
		db_data.push_back(points);
		
		FileNode fn = db["minutiae"];
		id=0;
		for (FileNodeIterator it = fn.begin(); it != fn.end(); it++,id++)
		{
			FileNode item = *it;
			db_data[i].push_back(minutiae_point);
			item["type"] >> db_data[i][id].type;
			item["x"] >> db_data[i][id].x;
			item["y"] >> db_data[i][id].y;
			item["theta"] >> db_data[i][id].theta;
		}

	}


	// single minutiae matching
	if(db_data[0] == minutiae)
		cout<<"Matching!!!\n";
	else
		cout<<"ERROR!!!\n";

#endif
	//imshow("fingerprint", img_enhanced);
	//waitKey(0);
	return 0;
}
