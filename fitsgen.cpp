#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mathutils.hpp"
using namespace std;

//these values are relative to image dimensions
const double MIN_STAR_RAD = 0.01;
const double MAX_STAR_RAD = 0.03;

//format is [x, y, radius]
void generate_body_info_list(int *data_array, int nbodies, int ncols, int nrows, int minrad, int maxrad){
	int range = maxrad - minrad;
	for (int i = 0; i < nbodies; i++){
		int radius = rand() % range + minrad;
		data_array[i * 3] = rand() % (ncols - radius * 2) + radius;
		data_array[i * 3 + 1] = rand() % (nrows - radius * 2) + radius;
		data_array[i * 3 + 2] = radius;
	}
}

void overlay_star(int *data_array, int cx, int cy, int radius, int nrows, int ncols, int lum_add){
	for (int star_x = -radius; star_x <= radius; star_x++){
		for (int star_y = -radius; star_y <= radius; star_y++){
			int d2 = star_x * star_x + star_y * star_y;
			if (d2 <= radius * radius){
				data_array[(star_y + cy) * ncols + (cx + star_x)] = radius * radius - d2 + lum_add;
			}
		}
	}
}

void generate_starfield(int *data_array, int *star_loc_list, int nrows, int ncols, int nstars){

	for (int i = 0; i < nrows; i++){
		for (int j = 0; j < ncols; j++){
			data_array[i * ncols + j] = 0;
		}
	}

	for (int star_n = 0; star_n	< nstars; star_n++){
		int cx = star_loc_list[star_n * 3];
		int cy = star_loc_list[star_n * 3 + 1];
		int radius = star_loc_list[star_n * 3 + 2];

		overlay_star(data_array, cx, cy, radius, nrows, ncols, 0);
	}
}



void write_image_data(int *data_array, int nrows, int ncols, string outfilepath){
	ofstream outfile;
	outfile.open(outfilepath, ios::binary);
	for (int i = 0; i < nrows; i++){
		for (int j = 0; j < ncols; j++){
			outfile.write(reinterpret_cast<const char *>(&data_array[i * ncols + j]), sizeof(int));
		}
	}
	outfile.close();
}

void supernova_sequence(char* directory){
	int nstars = 20;

	int imgsize = 1000;

	int nrows = imgsize;
	int ncols = imgsize;

	int star_loc_list[nstars * 3];

	//we could also use ncols here since we implicitly assume nrows = ncols
	int minrad = MIN_STAR_RAD * nrows;
	int maxrad = MAX_STAR_RAD * nrows;
	int range = maxrad - minrad;

	generate_body_info_list(star_loc_list, nstars, nrows, ncols, minrad, maxrad);

	int arr[imgsize * imgsize];
	generate_starfield(arr, star_loc_list, imgsize, imgsize, 20);

	int target_radius = MIN_STAR_RAD * nrows;
	int target_x = rand() % (ncols - target_radius * 2) + target_radius;
	int target_y = rand() % (nrows - target_radius * 2) + target_radius;
	int max_lum_add = 9 * target_radius * target_radius;

	for (int img_num = 0; img_num < 100; img_num++){
		overlay_star(arr, target_x, target_y, 10, imgsize, imgsize, supernova_light_curve(0, 100, img_num, max_lum_add));
		string fname = directory;
		fname = fname + "/data" + to_string(img_num) + ".txt";
		write_image_data(arr, imgsize, imgsize, fname);
	}
}

int main(int argc, char **argv){
	srand(time(NULL));

	define_light_curves();

	cout << "Running FITS generation sequence..." << endl;
	cout << "Sequence type " << argv[1] << endl;
	cout << "Target file is " << argv[2] << endl;

	supernova_sequence("./sample_sn");

	return 0;
}