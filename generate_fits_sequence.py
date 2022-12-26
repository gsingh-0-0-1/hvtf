import numpy as np
from astropy.io import fits
import sys
import os
import subprocess
import datetime
from tqdm import tqdm

C_TO_COMPILE = ['fitsgen.cpp']

print("Compiling generation code...")
for f in C_TO_COMPILE:
	subprocess.run(['g++', '-o', f.replace(".cpp", ""), f])

ARGS = sys.argv

try:
	OPTION = ARGS[1]
	OUTDIR = ARGS[2]
except Exception:
	OPTION = "SN"
	OUTDIR = "sample_sn"

subprocess.run(['rm', '-rf', OUTDIR])
os.makedirs(OUTDIR)

subprocess.run(['./fitsgen', OPTION, OUTDIR])

print("Rewriting data as fits...")

NFITS = len([el for el in os.listdir(OUTDIR) if 'fits' in el])

for i in tqdm(range(NFITS)):

	data = np.fromfile(OUTDIR + "/data" + str(i) + ".txt", dtype = np.uint32)

	data = data.reshape((int(data.shape[0] ** 0.5), int(data.shape[0] ** 0.5)))


	hdu = fits.PrimaryHDU(data)
	hdul = fits.HDUList([hdu])
	hdul.writeto(OUTDIR + "/" + str(i) + ".fits")
