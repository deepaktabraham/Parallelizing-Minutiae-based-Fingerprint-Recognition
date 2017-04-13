#include "SeqApp.h"


/*
 * main function
 */
int main(int argc, char* argv[])
{
	Mat img_src, img_blur, img_src_mat, img_thresh, img_enhanced, bin_matrix, d_bin_matrix;

	/*
	 * check command-line arguments
	 */
	if(argc != 2)
	{
		cout<<"ERROR: invalid arguments\n";
		cout<<"Usage: ./built/SeqApp <fingerprint_image>\n";
		return -1;
	}


	/*
	 * read fingerprint image
	 */
	img_src = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);


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
	 * identify and classify the minutiae points
	 */
	vector<struct minutiae_t> minutiae = minutiae_classify(d_bin_matrix, orientation);


	/* 
	 * read and compare with minutiae details in the database
	 */
	vector<string> files = vector<string>();
	
	// get list of files in the XML database
	if(get_files(string(DATABASE), &files) != 0)
	{
		// exit due to error
		return -1;
	}	

	// check if XML database is empty
	if(files.size() == 0)
	{
		cout<<"ERROR: XML Database is empty... run InitDatabase\n";
		return -1;
	}	

	vector<vector <struct minutiae_t>> db_data;
	vector<struct minutiae_t> db_minutiae_set;
	struct minutiae_t db_minutiae_point;
	ostringstream file_path;
	int num_items, id;
	for (int i = 0; i<files.size(); i++)
	{
		id=0;

		// read file from the XML database
		file_path.clear();
		file_path.str("");
		file_path<<DATABASE<<files[i];
		FileStorage db(file_path.str(), FileStorage::READ);

		if(!db.isOpened())
		{
			cout<<"ERROR: failed to open file in the XML database\n";
			return -1;
		}

		db["num_items"] >> num_items;
		db_data.push_back(db_minutiae_set);

		FileNode fn = db["minutiae"];

		for (FileNodeIterator it = fn.begin(); it != fn.end(); it++,id++)
		{
			FileNode item = *it;
			db_data[i].push_back(db_minutiae_point);
			item["type"] >> db_data[i][id].type;
			item["x"] >> db_data[i][id].x;
			item["y"] >> db_data[i][id].y;
			item["theta"] >> db_data[i][id].theta;
		}
		db.release();


		// compare minutiae points obtained for the given image with values read from the XML database
		if(db_data[i] == minutiae)
		{
			cout<<"Matching with "<<files[i].substr(0, files[i].length() - 4)<<" !!!\n";
			return 0;
		}
	}

	cout<<"Match not found!!!\n";
	return 0;
}
