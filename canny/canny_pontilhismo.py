import cv2
import numpy as np
import sys


def on_change(value):
	slide = cv2.getTrackbarPos(slide_name, title)
	edges = cv2.Canny(image, slide, 3 * slide)
	points = np.copy(image)
	points[...] = 220
	np.random.shuffle(xrange)
	for i in xrange:
		np.random.shuffle(yrange)
		for j in yrange:
			displacement = np.random.randint(0, 2*jitter) - jitter + 1
			i = int(i + displacement if i + displacement < rows else rows - 1)
			j = int(j + displacement if j + displacement < cols else cols - 1)
			cv2.circle(points, (j, i), radius, int(image[i, j]), -1)
	for i in range(rows):
		for j in range(cols):
			if edges[i, j] == 255:
				cv2.circle(points, (j, i), 1, 100, 0)
	cv2.imshow(title, points)


if __name__ == '__main__':
	title = "Pontilhismo com Canny"
	cv2.namedWindow(title, cv2.WINDOW_AUTOSIZE)
	image = cv2.imread(sys.argv[1], cv2.IMREAD_GRAYSCALE)
	cols, rows = image.shape

	step, jitter, radius = 5, 8, 5
	xrange = np.arange(rows/step) * step + step/2
	yrange = np.arange(cols/step) * step + step/2

	slide_name = "Threshold inferior"
	slider_max = 200

	cv2.createTrackbar(slide_name, title, 197, slider_max, on_change)

	on_change(0)
	cv2.waitKey(0)