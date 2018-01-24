import sys
import numpy as np
import cv2

def main(argv):
   if len(sys.argv) < 2 or len(sys.argv) > 3:
      sys.exit("Usage: python kmeans.py <image>.jpg")
   img = cv2.imread(argv[1])
   Z = img.reshape((-1,3))

   # convert to np.float32
   Z = np.float32(Z)

   # define criteria, number of clusters(K) and apply kmeans()
   criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 10, 1.0)
   if len(sys.argv) is 3:
      K = int(argv[2])
   else:
      K = 4
   ret,label,center=cv2.kmeans(Z,K,None,criteria,10,cv2.KMEANS_RANDOM_CENTERS)

   # Now convert back into uint8, and make original image
   center = np.uint8(center)
   res = center[label.flatten()]
   res2 = res.reshape((img.shape))
   res2 = cv2.resize(res2, (1200, 800))

   cv2.imshow('output img',res2)
   cv2.waitKey(0)
   cv2.destroyAllWindows()

if __name__ == "__main__":
   main(sys.argv)