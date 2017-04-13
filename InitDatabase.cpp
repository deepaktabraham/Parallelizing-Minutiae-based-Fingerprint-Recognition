#include "SeqApp.h"


/*
 * main function to initialize the XML database
 */
int main(int argc, char* argv[])
{
	/*
	 * check command-line arguments
	 */
	if(argc != 2)
	{
		cout<<"ERROR: invalid arguments\n";
		cout<<"Usage: ./built/InitDatabase <path_to_image_database>\n";
		return -1;
	}


	/* 
	 * check image database
	 */
	vector<string> files = vector<string>();
	string dir (argv[1]);
	if(argv[1][strlen(argv[1])-1] != '/')
	{
		dir.append("/");
	}
	
	if(get_files(dir.c_str(), &files) != 0)
	{
		// exit due to error
		return -1;
	}	
	
	// check if image database is empty
	if(files.size() == 0)
	{
		cout<<"ERROR: image Database is empty\n";
		return -1;
	}

	ostringstream file_path, new_path;
        for (int i = 0; i<files.size(); i++)
        {
		Mat img_src, img_blur, img_src_mat, img_thresh, img_enhanced, bin_matrix, d_bin_matrix;
		string result;
		
		/*
		 * read fingerprint image from image database
		 */
                file_path.clear();
                file_path.str("");
                file_path<<dir.c_str()<<files[i];
		img_src = imread(file_path.str(), CV_LOAD_IMAGE_GRAYSCALE);

		
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
		 * write minutiae details in to XML database
	 	 */
                new_path.clear();
                new_path.str("");
                new_path<<DATABASE<<files[i];
		result.append((new_path.str()).substr(0, new_path.str().length() - 3));
		result.append(XML); 
		FileStorage db(result, FileStorage::WRITE);
		db<<"num_items"<<(int)minutiae.size();
		db<<"minutiae"<<"[";
		for(int i=0; i<minutiae.size(); i++)
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


	}
		
	return 0;
}
