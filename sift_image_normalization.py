from astropy.io import fits
import numpy as np
import matplotlib.pyplot as plt
import os

def normalize(img_dat, header_dat):
    result = np.array()
    return result

#find all .fits files
fits_files = []
for root, dirs, files in os.walk(os.getcwd()):
    for file in files:
        if file.endswith(".fits"):
             fits_files.append(os.path.join(root, file))

result_image_data = np.array()

#iterate through files and normalize
for file in fits_files:
    header = fits.getheader(file)
    image_data = fits.getdata(file)
    result_image_data.append(normalize(image_data, header))
    plt.imshow(image_data)
    plt.show()
