import glob
import imageio
import cv2
import numpy as np
import sys
import os

n_clusters = 8  
n_rodadas = 1

image = cv2.imread(sys.argv[1], cv2.IMREAD_COLOR)
rows, cols = image.shape[:2]

z = np.float32(image.reshape((-1, 3)))

criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 10000, 1e-4)
flags = cv2.KMEANS_RANDOM_CENTERS

images = []

for i in range(10):
	compactness, label, centers = cv2.kmeans(z, n_clusters, None, criteria, n_rodadas, flags)
	centers = np.uint8(centers)
	restored = centers[label.flatten()].reshape((image.shape))
	path = f"tmp-{i}.jpg"
	cv2.imwrite(path, restored)
	images.append(imageio.imread(path))
	cv2.destroyAllWindows()

assert len(images) == 10

imageio.mimsave('./kmeans.gif', images)

tmp = glob.glob('tmp*')
for path in tmp:
	os.remove(path)