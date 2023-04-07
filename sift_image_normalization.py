from astropy.io import fits
import numpy as np
import matplotlib.pyplot as plt
import os
from PIL import Image
import sys

def data_scale(img_dat, minval, maxval):
    #scale data to [0,1] range
    return (img_dat - minval)/(maxval - minval)

def normalize(img_dat, header_dat):
    #try:
    #    maxval = header_dat['DATAMAX']
    #    minval = header_dat['DATAMIN']
    #except KeyError:#for when datamax/datamin don't exist
    #    return
    maxval = np.max(img_dat)
    minval = np.min(img_dat)
    result = data_scale(img_dat, minval, maxval)
    #convert to jpg
    image_data = (255*result).astype(np.uint8)[::-1,:]
    image = Image.fromarray(image_data, 'L')
    return image

#find all .fits files
fits_files = []
for root, dirs, files in os.walk(os.getcwd()):
    for file in files:
        if file.endswith(".fits"):
             fits_files.append(os.path.join(root, file))

result_image_data = []

#iterate through files and normalize
for file in fits_files:
    header = fits.getheader(file)
    try:
        image_data = fits.getdata(file)
    except TypeError:
        continue
    #result_image_data.append(normalize(image_data, header))
    plt.imshow(image_data)
    plt.show()
    image_name = os.path.basename(file).replace('.fits', '.jpg')
    #try:
    if not os.path.exists("training"):
        os.makedirs("training")
    normalize(image_data, header).save("training/"+image_name)
    #except Exception as e:
     #   print(e)
      #  continue
