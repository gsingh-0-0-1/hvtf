#ifndef LOADFITS_HPP
#define LOADFITS_HPP

#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <map>
#include <vector>
using namespace std;

const int FITS_KEYWORD_SIZE = 80;
const int FITS_HEADER_BLOCK_SIZE = FITS_KEYWORD_SIZE * 36;

map<string, string> get_fits_header(string fpath){

	map<string, string> header;
	ifstream infile(fpath, ios::binary);
	while (true){
		char buffer[FITS_KEYWORD_SIZE];
		infile.read(buffer, FITS_KEYWORD_SIZE);
		string key = "";
		string val = "";
		bool add_to_key = true;
		for (char c : buffer){
			//don't bother adding spaces
			if (c == ' '){
				continue;
			}
			//switch to value when = shows up
			if (c == '='){
				add_to_key = false;
				continue;
			}
			//ignore comments
			if (c == '/'){
				break;
			}

			if (add_to_key){
				key = key + c;
			}
			else{
				val = val + c;
			}
		}

		if (key == "END"){
			break;
		}
		header.insert(pair<string, string>(key, val));
	}
	infile.close();

	return header;
}

vector<int> load_fits_file(string fpath){
	map<string, string> header = get_fits_header(fpath);
	int nkeys = header.size();
	int nblocks_header = nkeys * FITS_KEYWORD_SIZE / FITS_HEADER_BLOCK_SIZE;
	nblocks_header++;

	auto bpx_keyval = header.find("BITPIX");
	int bit_per_pix = stoi(bpx_keyval -> second);
	if (bit_per_pix < 0){
		bit_per_pix = bit_per_pix * -1;
	}
	int nbytes = bit_per_pix / 8;

	int nsamples = stoi(header.find("NAXIS1") -> second) * stoi(header.find("NAXIS2") -> second);

	ifstream infile(fpath, ios::binary);
	char buffer[nblocks_header * FITS_HEADER_BLOCK_SIZE];
	infile.read(buffer, nblocks_header * FITS_HEADER_BLOCK_SIZE);

	vector<int> data_array;
	for (int i = 0; i < nsamples; i++){

		char val[nbytes];
		infile.read(val, nbytes);

		int pushval = 0;

		for (int j = 0; j < nbytes; j++){
			pushval = pushval + (val[nbytes - j - 1] * pow(256, j));
		}

		data_array.push_back(pushval);
	}
	infile.close();


	ofstream outfile("new.fits", ios::binary);
	for (int i = 0; i < nsamples; i++){
		outfile.write(reinterpret_cast<const char *>(&data_array.at(i)), 2);
	}
	outfile.close();

	return data_array;
}


#endif