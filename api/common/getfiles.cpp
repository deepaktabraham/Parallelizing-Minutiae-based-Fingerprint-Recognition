#include <iostream>
#include <string.h>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>


using namespace std;


/*
 * function to obtain a vector of files in the given directory
 */
int get_files(string dir, vector<string>* files)
{
	// open the database 
	DIR *dp = opendir(dir.c_str());
	struct dirent *dirp;
	if(dp == NULL) 
	{
		cout<<"ERROR: failed to open "<<dir<<"\n";
		return -1;
	}

	while ((dirp = readdir(dp)) != NULL) 
	{
		if(strncmp(dirp->d_name, ".", 1) != 0) 
		{
			files->push_back(dirp->d_name);
		}
	}

	closedir(dp);
	return 0;
}
