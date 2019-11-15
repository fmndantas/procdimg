import cv2
import numpy as np
import sys

if __name__ == '__main__':
	title = "pontilhismo"
	cv2.namedWindow(title, cv2.WINDOW_AUTOSIZE)
	image = cv2.imread(sys.argv[1], cv2.IMREAD_GRAYSCALE)
	cols, rows = image.shape
	cv2.imshow(title, image)
	cv2.waitKey(0)
	
	points = np.copy(image)
	points[...] = 220

	step, jitter, radius = 5, 3, 3
	
	xrange = np.arange(rows/step) * step + step/2
	yrange = np.arange(cols/step) * step + step/2

	np.random.shuffle(xrange)

	for i in xrange:
		np.random.shuffle(yrange)
		for j in yrange:
			displacement = np.random.randint(0, 2*jitter) - jitter + 1
			i = int(i + displacement if i + displacement < rows else rows - 1)
			j = int(j + displacement if j + displacement < cols else cols - 1)
			cv2.circle(points, (j, i), radius, int(image[i, j]), -1)

	cv2.imshow(title, points)
	cv2.waitKey(0)