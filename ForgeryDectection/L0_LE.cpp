#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

void L0_LE_class();

int main() {

	L0_LE_class();
}

void L0_LE_class() {


	Mat src = imread("im43_t.bmp", 0);

	if (src.empty())
	{
		cout << "the image is not exist" << endl;
	}


	Mat srcDCT;   //Image to be copied and modified
	src.convertTo(srcDCT, CV_32F); // change depth for dct


	Mat L0_LE(src.size().height, src.size().width, CV_32F);
	Mat bloc(8, 8, CV_32F);


	float L0 = 0;

	for (int i = 0; i < src.size().height - 8; i++) //cols
	{
		for (int j = 0; j < src.size().width - 8; j++) //(rows)
		{
			Mat Li_block = srcDCT(Rect(j, i, 8, 8)); //Split for LE calculation
			dct(Li_block, bloc); // DCT progress
			
			for (int x = 0; x < 8; x++)
			{
				for (int y = 0; y < 8; y++)
				{
					if (round(bloc.at<float>(y, x)) != 0) {
						L0++;
					} 
				}
			}

			L0_LE(Rect(j, i, 1, 1)).at<float>(0, 0) = L0 / 64; // Enter 8x8 L0 value in L0_LE(Rect(j,i,1,1)

			L0 = 0; //Initialize L0 value to 0
		}
	}

	Mat local_minima(src.size().height, src.size().width, CV_32F);
	Mat LE_block(8, 8, CV_32F);
	float minimum = 1000;
	float minimum_num = 0;
	for (int i = 0; i < L0_LE.size().height - 8; i += 8) {
		for (int j = 0; j < L0_LE.size().width - 8; j += 8) {
			Mat LE_block = L0_LE(Rect(j, i, 8, 8));
			//cout << LE_block << endl;
			for (int x = 0; x < LE_block.size().height; x++) {   //Find the minimum
				for (int y = 0; y < LE_block.size().width; y++) {
					minimum = min(LE_block.at<float>(y, x), minimum); // Finding the minimum value in an 8x8 block

				}
			}
			for (int x = 0; x < LE_block.size().height; x++) {   //최솟값 구하는 for문
				for (int y = 0; y < LE_block.size().width; y++) {
					if (LE_block.at<float>(y, x) == minimum) {
						minimum_num++;   // Finding the number of minimum values in 8x8 blocks
					}
			
				}
			}

			for (int a = 0; a < LE_block.size().height; a++) {
				for (int b = 0; b < LE_block.size().width; b++) {
					if (minimum_num == 1) {              
						if (LE_block.at<float>(b, a) == minimum) {
							local_minima(Rect(j, i, 8, 8)).at<float>(b, a) = minimum; // If there is 1 minimum value in an 8x8 block, the minimum value is preserved.
						}
						else local_minima(Rect(j, i, 8, 8)).at<float>(b, a) = 255; // Fill with remaining 255
					}
					else local_minima(Rect(j, i, 8, 8)).at<float>(b, a) = 255; // If there are multiple minimum values, fill them all with 255
				}
			}




			minimum = 1000; // Initializing minimum values
			minimum_num = 0; // Initializing minimum_num values
		}
	}

	
	Mat local_minima_3 = imread("bigwhite.jpg", 0); // bigwhite.jpg = 4460x3360 total pixel value of 255 images
	


	for (int i = 0; i < src.size().height - 16; i += 8) {
		for (int j = 0; j < src.size().width - 16; j += 8) {
			for (int x = 0; x < 8; x++) {
				for (int y = 0; y < 8; y++) {

					if ((local_minima(Rect(j, i, 8, 8)).at<float>(y, x) != 255 && local_minima(Rect(j + 8, i, 8, 8)).at<float>(y, x) != 255) &&
						(local_minima(Rect(j + 16, i, 8, 8)).at<float>(y, x) != 255 ||
							local_minima(Rect(j + 8, i + 8, 8, 8)).at<float>(y, x) != 255 ||
							local_minima(Rect(j, i + 8, 8, 8)).at<float>(y, x) != 255 ||
							local_minima(Rect(j + 16, i + 8, 8, 8)).at<float>(y, x) != 255)) {

						rectangle(local_minima_3, Point(j + y, i + x), Point(j + y + 8, i + x + 8), Scalar(0), 1);
						rectangle(local_minima_3, Point(j + y + 8, i + x), Point(j + y + 16, i + x + 8), Scalar(0), 1);



					}
					else if ((local_minima(Rect(j, i, 8, 8)).at<float>(y, x) != 255 && local_minima(Rect(j, i + 8, 8, 8)).at<float>(y, x) != 255) &&
						(local_minima(Rect(j + 8, i, 8, 8)).at<float>(y, x) != 255 ||
							local_minima(Rect(j + 8, i + 8, 8, 8)).at<float>(y, x) != 255 ||
							local_minima(Rect(j + 8, i + 16, 8, 8)).at<float>(y, x) != 255 ||
							local_minima(Rect(j, i + 16, 8, 8)).at<float>(y, x) != 255)) {

						rectangle(local_minima_3, Point(j + y, i + x), Point(j + y + 8, i + x + 8), Scalar(0), 1);
						rectangle(local_minima_3, Point(j + y, i + x + 8), Point(j + y + 8, i + x + 16), Scalar(0), 1);

					}
				}
			}
		}
	}  // Draw a rectangle using the L0 value of the copy paste forged area

	Mat local_minima_4 = local_minima_3.clone();

	for (int i = 0; i < src.size().height; i += 8) {
		for (int j = 0; j < src.size().width; j += 8) {
			for (int x = 0; x < 8; x++) {
				for (int y = 0; y < 8; y++) {
					if (local_minima_3(Rect(j, i, 8, 8)).at<uchar>(0, 0) == 0) {

						local_minima_4(Rect(j, i, 8, 8)).at<uchar>(0, 0) = 0;

						if (y != 0 && y != 7 && x != 0 && x != 7 && local_minima_3(Rect(j, i, 8, 8)).at<uchar>(y, x) == 0) {

							local_minima_4(Rect(j, i, 8, 8)).at<uchar>(y, x) = 0;

						}
						else {
							local_minima_4(Rect(j, i, 8, 8)).at<uchar>(y, x) = 255;

						}


					}
				}
			}
		}
	}


	for (int i = 0; i < src.size().height - 8; i++) {
		for (int j = 0; j < src.size().width - 8; j++) {
			if (j % 8 != 0 && i % 8 != 0) {
				if (local_minima_4.at<uchar>(j, i) != 255 && local_minima_4.at<uchar>(j + 2, i) != 255 ||
					local_minima_4.at<uchar>(j, i) != 255 && local_minima_4.at<uchar>(j + 3, i) != 255) {
					local_minima_4.at<uchar>(j + 1, i) = 0;
				}
				if (local_minima_4.at<uchar>(j, i) != 255 && local_minima_4.at<uchar>(j, i + 2) != 255 ||
					local_minima_4.at<uchar>(j, i) != 255 && local_minima_4.at<uchar>(j, i + 3) != 255) {
					local_minima_4.at<uchar>(j, i + 1) = 0;
				}
			}
		}
	}
	int sum = 0;

	for (int i = 0; i < src.size().width - 8; i++) {
		for (int j = 0; j < src.size().height - 8; j++) {
			if (local_minima_4.at<uchar>(j, i) == 0 && local_minima_4.at<uchar>(j + 1, i) == 0 && local_minima_4.at<uchar>(j + 2, i) == 0 && local_minima_4.at<uchar>(j + 3, i) == 0
				&& local_minima_4.at<uchar>(j, i + 1) == 0 && local_minima_4.at<uchar>(j, i + 2) == 0 && local_minima_4.at<uchar>(j, i + 3) == 0) {
				sum++;
			}
		}
	}


	Mat img = local_minima_4.clone();

	for (int i = 0; i < src.size().height; i += 8) {
		for (int j = 0; j < src.size().width; j += 8) {
			if ((local_minima_4.at<uchar>(j, i) == 0 && local_minima_4.at<uchar>(j + 1, i) == 0 && local_minima_4.at<uchar>(j + 2, i) == 0) ||
				(local_minima_4.at<uchar>(j, i) == 0 && local_minima_4.at<uchar>(j, i + 1) == 0 && local_minima_4.at<uchar>(j, i + 2) == 0))
				/*if ((local_minima_4.at<uchar>(j, i) == 0 && local_minima_4.at<uchar>(j + 1, i) == 255 ) ||
				   (local_minima_4.at<uchar>(j, i) == 0 && local_minima_4.at<uchar>(j, i + 1) == 255 ))*/
				break;

			else
				img.at<uchar>(j, i) = 255;
		}
	}


	/*for (int x = 0; x < src.size().height; x++) {
	   for (int y = 0; y < src.size().width; y++) {
		  if (img.at<uchar>(y, x) != 0 && img.at<uchar>(y, x) != 255)
			 cout << img.at<uchar>(y, x);
	   }
	}*/


	Mat ex = local_minima_4.clone();

	for (int i = 0; i < src.size().height; i += 8) {
		for (int j = 0; j < src.size().width; j += 8) {
			if (ex.at<uchar>(j, i) == 0 && ex.at<uchar>(j + 1, i) == 0 && ex.at<uchar>(j + 2, i) == 0)
				break;

			else
				ex.at<uchar>(j, i) = 255;
		}
	}

	for (int i = 0; i < src.size().height; i++) {
		for (int j = 0; j < src.size().width; j++) {
			if (ex.at<uchar>(j, i) == 0)
				if ((ex.at<uchar>(j, i - 8) == 255 && ex.at<uchar>(j, i) == 0) ||   
					(ex.at<uchar>(j - 8, i) == 255 && ex.at<uchar>(j, i) == 0) ||  
					(ex.at<uchar>(j, i + 8) == 255 && ex.at<uchar>(j, i) == 0) || 
					(ex.at<uchar>(j + 8, i) == 255 && ex.at<uchar>(j, i) == 0))   
					src.at<uchar>(j, i) = 255; 
		}
	}

	//cout << src.size() << endl;
	//cout << local_minima_4.size() << endl;

	//int h = src.size().height;
	//int w = src.size().width;

	// 관심영역 설정 (set ROI (X, Y, W, H)).
	Rect rect(0, 0, 1280, 760);
	// 관심영역 자르기 (Crop ROI).
	Mat img2 = img(rect);


	vector<vector<Point>> contours;
	findContours(img, contours, RETR_LIST, CHAIN_APPROX_NONE);


	//cvtColor(src, ex, COLOR_BGR2GRAY);
	cvtColor(img, ex, COLOR_GRAY2BGR);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar c(rand() & 255, rand() & 255, rand() & 255);
		drawContours(ex, contours, i, c, 2);
	}


	


	//cout << sum << endl;

	//cout << src << endl;
	//cout << minimum1 << endl;
	//cout << local_minima_3 << endl;
	//cout << L0_LE << endl;
	//cout << srcDCT <<endl;
	imshow("src", src);
	namedWindow("src", WINDOW_NORMAL);
	resizeWindow("src", src.size().width, src.size().height);
	//imshow("local_minima", local_minima);
	//imshow("L0_LE", L0_LE);
	//imshow("local_minima3", local_minima_3);
	//imshow("forgery detection", local_minima_4);
	//imshow("src", src);   
	imshow("forgery detection", img2);
	namedWindow("forgery detection", WINDOW_NORMAL);
	resizeWindow("forgery detection", src.size().width, src.size().height);
	waitKey();
};

