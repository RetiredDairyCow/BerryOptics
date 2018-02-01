#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int mask_red(Mat new_image);
int mask_green(Mat new_image);
int draw_histogram(Mat new_image);
int circle_red(Mat new_image);

int main(int argc, char** argv)
{
	Mat src = imread(argv[1], 1);

	// Create a matrix entry for each pixel
	Mat samples(src.rows * src.cols, 3, CV_32F);
	for (int y = 0; y < src.rows; y++)
		for (int x = 0; x < src.cols; x++)
			for (int z = 0; z < 3; z++)
				samples.at<float>(y + x * src.rows, z) = src.at<Vec3b>(y, x)[z];


	int clusterCount = 12;
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

	namedWindow("original image", CV_WINDOW_NORMAL);
	imshow("original image", src);
	namedWindow("kmeans image", CV_WINDOW_NORMAL);
	imshow("kmeans image", new_image);

	draw_histogram(new_image);
	mask_red(new_image);
	mask_green(new_image);

	waitKey(0);

	circle_red(new_image);

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

	namedWindow("Masked red", CV_WINDOW_NORMAL);
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

	namedWindow("Masked green", CV_WINDOW_NORMAL);
	imshow("Masked green", mask);

	return 0;
}

int draw_histogram(Mat src)
{
	Mat dst;

	if (!src.data)
	{
		return -1;
	}

	/// Separate the image in 3 places ( B, G and R )
	vector<Mat> bgr_planes;
	split(src, bgr_planes);

	/// Establish the number of bins
	int histSize = 256;

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 };
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;

	Mat b_hist, g_hist, r_hist;

	/// Compute the histograms:
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

	// Draw the histograms for B, G and R
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	/// Draw for each channel
	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}

	/// Display
	namedWindow("calcHist", CV_WINDOW_NORMAL);
	imshow("calcHist", histImage);

	return 0;
}

int circle_red(Mat new_image)
{
	// Convert input image to HSV
 	cv::Mat hsv_image;
	cv::cvtColor(new_image, hsv_image, cv::COLOR_BGR2HSV);
	
	// Threshold the HSV image, keep only the red pixels
	cv::Mat lower_red_hue_range;
	cv::Mat upper_red_hue_range;
	cv::inRange(hsv_image, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), lower_red_hue_range);
	cv::inRange(hsv_image, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), upper_red_hue_range);

	// Combine the above two images
	cv::Mat red_hue_image;
	cv::addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);

	cv::GaussianBlur(red_hue_image, red_hue_image, cv::Size(9, 9), 2, 2);

	// Use the Hough transform to detect circles in the combined threshold image
	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(red_hue_image, circles, CV_HOUGH_GRADIENT, 1, red_hue_image.rows / 8, 100, 20, 0, 0);

	// Loop over all detected circles and outline them on the original image
	if (circles.size() == 0)
		std::exit(-1);

	for (size_t current_circle = 0; current_circle < circles.size(); ++current_circle)
	{
		cv::Point center(std::round(circles[current_circle][0]), std::round(circles[current_circle][1]));
		int radius = std::round(circles[current_circle][2]);
		cv::circle(new_image, center, radius, cv::Scalar(0, 255, 0), 5);
	}

	namedWindow("Attempt circle red", CV_WINDOW_NORMAL);
	imshow("Attempt circle red", new_image);
}