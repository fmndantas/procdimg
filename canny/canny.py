import cv2
import numpy as np
import sys

def on_change(value):
	slide = cv2.getTrackbarPos(slide_name, title)
	print(f"inferior: {slide} | superior {3 * slide}")
	edges = cv2.Canny(image, slide, 3 * slide)
	cv2.imshow(title, edges)


if __name__ == '__main__':
	title = "canny"
	cv2.namedWindow(title, cv2.WINDOW_AUTOSIZE)
	image = cv2.imread(sys.argv[1], cv2.IMREAD_GRAYSCALE)
	cols, rows = image.shape
	border = np.zeros((cols, rows),)

	slide_name = "inferior"
	slider_max = 200

	cv2.createTrackbar(slide_name, title, 50, slider_max, on_change)

	on_change(0)
	cv2.waitKey(0)