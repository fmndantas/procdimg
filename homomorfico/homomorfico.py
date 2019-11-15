import sys
import cv2
import numpy as np


def on_change(value):
	gamma_h = cv2.getTrackbarPos(gammah_name, title) / 100
	gamma_l = cv2.getTrackbarPos(gammal_name, title) / 100
	d0 = cv2.getTrackbarPos(d0_name, title)
	c = cv2.getTrackbarPos(c_name, title)
	H = (gamma_h - gamma_l) * (1 - np.exp(-c * ((d / d0) ** 2))) + gamma_l
	dft_filt = dft * H
	dft_filt = np.fft.ifftshift(dft_filt)
	filtered_image = np.fft.ifft2(dft_filt)
	final_image = np.exp(np.abs(filtered_image))
	final_image = cv2.normalize(final_image, None, 0, 1, cv2.NORM_MINMAX)
	cv2.imshow(title, final_image)


title = 'filtro homomorfico'
cv2.namedWindow(title, cv2.WINDOW_AUTOSIZE)

gammah_max = 100
gammal_max = 100
c_max = 100
d0_max = 100

gammah_name = "gammah % = "
gammal_name = "gammal % = "
c_name = "c = "
d0_name = "d0 = "

image = cv2.imread(sys.argv[1], cv2.IMREAD_GRAYSCALE)
image = cv2.resize(image, (512, 512), interpolation=cv2.INTER_AREA)
modified_image = np.log(image + 1.)
dft = np.fft.fft2(modified_image)
dft = np.fft.fftshift(dft)

P, Q = dft.shape[:2]
v, u = np.meshgrid(np.arange(Q), np.arange(P))
d = np.hypot(u - P / 2, v - Q / 2)

cv2.createTrackbar(gammah_name, title, 50, gammah_max, on_change)
cv2.createTrackbar(gammal_name, title, 50, gammal_max, on_change)
cv2.createTrackbar(c_name, title, 50, c_max, on_change)
cv2.createTrackbar(d0_name, title, 50, d0_max, on_change)

on_change(0)
cv2.waitKey(0)
