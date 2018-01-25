#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

int mask_red(Mat new_image);
int mask_green(Mat new_image);

int main(int argc, char** argv)
{
	Mat src = imread(argv[1], 1);
	Mat samples(src.rows * src.cols, 3, CV_32F);
	for (int y = 0; y < src.rows; y++)
		for (int x = 0; x < src.cols; x++)
			for (int z = 0; z < 3; z++)
				samples.at<float>(y + x * src.rows, z) = src.at<Vec3b>(y, x)[z];


	int clusterCount = 20;
	Mat labels;
	int attempts = 1;
	Mat centers;
	kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);


	Mat new_image(src.size(), src.type());
	for (int y = 0; y < src.rows; y++)
		for (int x = 0; x < src.cols; x++)
		{
			int cluster_idx = labels.at<int>(y + x * src.rows, 0);
			new_image.at<Vec3b>(y, x)[0] = centers.at<float>(cluster_idx, 0);
			new_image.at<Vec3b>(y, x)[1] = centers.at<float>(cluster_idx, 1);
			new_image.at<Vec3b>(y, x)[2] = centers.at<float>(cluster_idx, 2);
		}
	imshow("original image", src);
	imshow("kmeans image", new_image);

	mask_red(new_image);
	mask_green(new_image);

	waitKey(0);
}

// Masks the ripe
int mask_red(Mat new_image)
{
	Mat3b hsv;
	cvtColor(new_image, hsv, COLOR_BGR2HSV);

	Mat1b mask1, mask2;
	inRange(hsv, Scalar(0, 70, 50), Scalar(10, 255, 255), mask1);
	inRange(hsv, Scalar(170, 70, 50), Scalar(180, 255, 255), mask2);

	Mat1b mask = mask1 | mask2;

	imshow("Masked red", mask);

	return 0;
}

// Masks the unripe
int mask_green(Mat new_image)
{
	Mat3b hsv;
	cvtColor(new_image, hsv, COLOR_BGR2HSV);

	Mat1b mask1, mask2;
	inRange(hsv, Scalar(20, 38, 70), Scalar(35, 255, 255), mask1);
	//inRange(hsv, Scalar(70, 170, 50), Scalar(255, 180, 255), mask2);

	Mat1b mask = mask1; // | mask2;

	imshow("Masked green", mask);

	return 0;
}