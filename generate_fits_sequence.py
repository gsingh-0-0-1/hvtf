import numpy as np
from astropy.io import fits
import sys
import os
import subprocess
import datetime
import matplotlib.pyplot as plt

C_TO_COMPILE = ['fitsgen.cpp']

print("Compiling generation code...")
for f in C_TO_COMPILE:
	subprocess.run(['g++', '-o', f.replace(".cpp", ""), f])

ARGS = sys.argv

OPTION = ARGS[1]

OUTDIR = ARGS[2]

subprocess.run(['rm', '-rf', OUTDIR])
os.makedirs(OUTDIR)

subprocess.run(['./fitsgen', OPTION, OUTDIR + "/data.txt"])

for i in range(100):

	data = np.fromfile(OUTDIR + "/data" + str(i) + ".txt", dtype = np.uint32)

	data = data.reshape((int(data.shape[0] ** 0.5), int(data.shape[0] ** 0.5)))


	hdu = fits.PrimaryHDU(data)
	hdul = fits.HDUList([hdu])
	hdul.writeto(OUTDIR + "/" + str(i) + ".fits")

	plt.imshow(data)
	plt.pause(0.01)
	plt.clf()
