#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include "mathutils.hpp"
#include "loadfits.hpp"
using namespace std;

//these values are relative to image dimensions
const double MIN_STAR_RAD = 0.002;
const double MAX_STAR_RAD = 0.006;

void arrcp(int* arr1, int* arr2, int n){
	for (int i = 0; i < n; i++){
		arr2[i] = arr1[i];
	}
}

void arr_from_vec(vector<int> arr1, int* arr2, int n){
	for (int i = 0; i < n; i++){
		arr2[i] = arr1.at(i);
	}
}

//format is [x, y, radius]
void generate_body_info_list(int* data_array, int nbodies, int ncols, int nrows, int minrad, int maxrad){
	int range = maxrad - minrad;
	for (int i = 0; i < nbodies; i++){
		int radius = rand() % range + minrad;
		data_array[i * 3] = rand() % (ncols - radius * 2) + radius;
		data_array[i * 3 + 1] = rand() % (nrows - radius * 2) + radius;
		data_array[i * 3 + 2] = radius;
	}
}

void overlay_star(int* data_array, int cx, int cy, int radius, int nrows, int ncols, int peak_lum){
	for (int star_x = -radius; star_x <= radius; star_x++){
		for (int star_y = -radius; star_y <= radius; star_y++){
			int d2 = star_x * star_x + star_y * star_y;
			if (d2 <= radius * radius){
				int lum = peak_lum * (radius * radius - d2) / (radius * radius);
				data_array[(star_y + cy) * ncols + (cx + star_x)] += lum;
			}
		}
	}
}

void generate_starfield(int* data_array, int *star_loc_list, int nrows, int ncols, int nstars){

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



void write_image_data(int* data_array, int nrows, int ncols, string outfilepath){
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
	//int nstars = 100;

	vector<int> data_vec = load_fits_file("andromeda.fits");

	int imgsize = pow(data_vec.size(), 0.5);

	int* arr = new int[imgsize * imgsize];

	arr_from_vec(data_vec, arr, data_vec.size());

	int nrows = imgsize;
	int ncols = imgsize;

	//int star_loc_list[nstars * 3];

	//we could also use ncols here since we implicitly assume nrows = ncols
	int minrad = MIN_STAR_RAD * nrows;
	int maxrad = MAX_STAR_RAD * nrows;
	int range = maxrad - minrad;

	//generate_body_info_list(star_loc_list, nstars, nrows, ncols, minrad, maxrad);

	//generate_starfield(arr, star_loc_list, imgsize, imgsize, nstars);

	int target_radius = rand() % range + minrad;
	int target_x = rand() % (ncols - target_radius * 2) + target_radius;
	int target_y = rand() % (nrows - target_radius * 2) + target_radius;

	int magnitude_increase = 2;

	int curve_max_lum = pow(10, magnitude_increase) * target_radius * target_radius;

	int N_IMGS = 10;

	cout << endl;

	for (int img_num = 0; img_num < N_IMGS; img_num++){
		int star_max_lum = supernova_light_curve(0, N_IMGS, img_num, curve_max_lum);

		int* newarr = new int[imgsize * imgsize];
		arrcp(arr, newarr, imgsize * imgsize);

		overlay_star(newarr, target_x, target_y, target_radius, imgsize, imgsize, star_max_lum);

		string fname = directory;
		fname = fname + "/data" + to_string(img_num) + ".txt";
		
		cout << "\rWriting data for image " << img_num + 1 << " / " << N_IMGS << "..." << flush;
		write_image_data(newarr, imgsize, imgsize, fname);
	}
	cout << endl;
}

int main(int argc, char** argv){
	srand(time(NULL));

	define_light_curves();

	cout << "Running FITS generation sequence..." << endl;
	cout << "Sequence type " << argv[1] << endl;
	cout << "Target directory is " << argv[2] << endl;

	supernova_sequence(argv[2]);

	return 0;
}