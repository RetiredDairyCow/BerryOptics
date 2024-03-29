// test_program.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/*
 *Loads the original image into the Mat object.
 *
 *@param *image the image object loaded by the user
 *@param image_name the name of the image to load
 *Note: if the image is a colored image the image is read in BGR format,
 *this is how opencv reads in images.
*/
void get_original_image(Mat *image, string image_name)
{
	const int ORIGINAL_FORMAT = -1;
	*image = imread(image_name, ORIGINAL_FORMAT);

	if ((*image).empty())
	{
		fprintf(stderr, "Error: The image could not be read.\n");
		exit(EXIT_FAILURE);
	}

}
/*
 *Note:
 *In opencv: Hue is in the range of 0-180
 *Saturation is in the range of 0-255
 *Value is in the range of 0 - 255
*/
Mat convert_to_hsv(Mat *image)
{
	Mat hsv;
	cvtColor(*image, hsv, COLOR_BGR2HSV);
	return hsv;
}

void convert_to_lab(Mat *image)
{
	cvtColor(*image, *image, COLOR_BGR2Lab);
}

void display_image(Mat image, String title)
{
	imshow(title, image);
}
/*
 *Splits the image into its color values
 *
 *@param *image the image we want to split
 *@param *split_image the Mat object that has the same dimesnsion as
 *the *image parameter
 *
*/
void split_the_image(Mat *image, Mat *split_image)
{
	split(*image, split_image);
}

/*
*Merge the multidimensional image into one image
*
*@param *image the image we want to merge
*@param count number of dimensions of image
*@return merged_image the mereged image
*
*/
Mat merge_split_image(Mat *image, int count)
{
	Mat merged_image;
	merge(image, count, merged_image);
	return merged_image;
}

int get_height(Mat image)
{
	return image.rows;
}

int get_width(Mat image)
{
	return image.cols;
}

int get_channels(Mat image)
{
	return image.channels();
}

void k_means(Mat *image);
void in_range(Mat *image);
void debug(Mat *image);
void print_pixel_values(Mat *image, int index_to_print);
int main(int argc, char* argv[])
{
	Mat image;
	Mat split_image[3];
	//get_original_image(&image, "strawberrie.jpg");
	get_original_image(&image, "strawberrie.jpg");
	display_image(image, "Original");
	in_range(&image);
	//display_image(image,"Original Image");
	//convert_to_hsv(&image);
	//split_the_image(&image, split_image);
	//debug(split_image);
	waitKey(0);
	return EXIT_SUCCESS;
}

void in_range(Mat *image)
{
	Mat hsv_image;
	Mat lower_red_hue_range;
	Mat upper_red_hue_range;
	Mat img_threshold;
	int lower_bound_red_1 = 0;
	int lower_bound_red_2 = 10;
	int upper_bound_red_1 = 160;
	int upper_bound_red_2 = 180;

	hsv_image = convert_to_hsv(image);
	inRange(hsv_image, Scalar(lower_bound_red_1, 100, 100), Scalar(lower_bound_red_2, 255, 255), 
		lower_red_hue_range);
	inRange(hsv_image, Scalar(upper_bound_red_1, 100, 100), Scalar(upper_bound_red_2, 255, 255), 
		upper_red_hue_range);
	
	img_threshold = max(lower_red_hue_range, upper_red_hue_range);
	display_image(img_threshold, "Image Threshold");
}

void debug(Mat *image)
{
	
}

void print_pixel_values(Mat *image, int index_to_print)
{
	int height = get_height(*image);
	int width = get_width(*image);
	int channels = get_channels(*image);
	int count = 0;
	int check = 0;
	Mat hsv;
	//printf("%i\n", height);
	//printf("%i\n", width);
	//printf("%i\n", channels);
	display_image(image[0], "Original H");
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			check = image[index_to_print].at<uchar>(i, j);
			if ((check > 0 && check < 20) || (check > 160 && check < 180))
			{
				image[index_to_print].at<uchar>(i, j) = 60;
			}
			//if (image[index_to_print].at<uchar>(i, j) < 10)
			//{
				//image[index_to_print].at<uchar>(i, j) = 60;
			//}
			//printf("%i ", image[index_to_print].at<uchar>(i, j));
			/*
			if (count == 25)
			{
				printf("\n");
				count = 0;
			}
			count++;
			*/
		}
		//count = 0;
		//printf("\n");
	}
	display_image(image[0],"Modified H");
	merge(image, 3, hsv);
	display_image(hsv, "HSV");
	cvtColor(hsv, hsv, COLOR_HSV2BGR);
	display_image(hsv, "H chnage BGR");
}

void k_means(Mat *image)
{

}