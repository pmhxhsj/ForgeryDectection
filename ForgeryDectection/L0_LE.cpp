#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

void L0_LE_class();
void block_count();

int main() {

	L0_LE_class();
	//block_count();
}

void L0_LE_class() {


	Mat src = imread("im43_t.bmp", 0);

	if (src.empty())
	{
		cout << "the image is not exist" << endl;
	}


	Mat srcDCT;   //복사되어 수정할 이미지
	src.convertTo(srcDCT, CV_32F); // dct는 32F or 64F 깊이


	Mat L0_LE(src.size().height, src.size().width, CV_32F);
	Mat bloc(8, 8, CV_32F);


	float L0 = 0;

	for (int i = 0; i < src.size().height - 8; i++) // 8단위로 이동하는 for문(cols)
	{
		for (int j = 0; j < src.size().width - 8; j++) // 8단위로 이동하는 for문(rows)
		{
			Mat Li_block = srcDCT(Rect(j, i, 8, 8));
			dct(Li_block, bloc);
			//cout << bloc << endl;
			for (int x = 0; x < 8; x++)
			{
				for (int y = 0; y < 8; y++)
				{
					if (round(bloc.at<float>(y, x)) != 0) {
						L0++;
					} // 8 X 8 이동하면서 0이 아닌 수가 있으면 L0에 1씩 추가
				}
			}

			L0_LE(Rect(j, i, 1, 1)).at<float>(0, 0) = L0 / 64; // L0_LE(Rect(j,i,1,1)에 8x8 L0값 입력

			L0 = 0; // L0 값 0으로 초기화
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
			for (int x = 0; x < LE_block.size().height; x++) {   //최솟값 구하는 for문
				for (int y = 0; y < LE_block.size().width; y++) {
					minimum = min(LE_block.at<float>(y, x), minimum); // 8x8 블록에서 minimum값 찾기

				}
			}
			//cout << minimum << endl;
			for (int x = 0; x < LE_block.size().height; x++) {   //최솟값 구하는 for문
				for (int y = 0; y < LE_block.size().width; y++) {
					if (LE_block.at<float>(y, x) == minimum) {
						minimum_num++;   // 8x8불록에서 minimum값의 개수 찾기
					}
					// cout << minimum << endl;
				}
			}

			for (int a = 0; a < LE_block.size().height; a++) {
				for (int b = 0; b < LE_block.size().width; b++) {
					if (minimum_num == 1) {                // 8x8 블록에 minimum값이 1개면
						if (LE_block.at<float>(b, a) == minimum) {
							local_minima(Rect(j, i, 8, 8)).at<float>(b, a) = minimum; // minimum값 보존
						}
						else local_minima(Rect(j, i, 8, 8)).at<float>(b, a) = 255; // 나머지 100으로 채움
					}
					else local_minima(Rect(j, i, 8, 8)).at<float>(b, a) = 255; // minimum값이 여러개면 다 100으로 채움
				}
			}




			minimum = 1000;
			minimum_num = 0;
		}
	}

	//int h = src.size().height;
	//int w = src.size().width;

	//Mat local_minima_3;
	Mat local_minima_3 = imread("bigwhite.jpg", 0);
	//resize(img1, local_minima_3, Size(w, h));


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
	}

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


	//윤곽선
	//선만 남기고 점 없애기(함수 사용할려고)
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

	//픽셀 값 확인
	/*for (int x = 0; x < src.size().height; x++) {
	   for (int y = 0; y < src.size().width; y++) {
		  if (img.at<uchar>(y, x) != 0 && img.at<uchar>(y, x) != 255)
			 cout << img.at<uchar>(y, x);
	   }
	}*/

	// 교수님 말씀대로 코드짜기
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
				if ((ex.at<uchar>(j, i - 8) == 255 && ex.at<uchar>(j, i) == 0) ||   //해당 픽셀이 검정색(선)이고 왼쪽으로 8개 간 픽셀이 하얀색(바탕)이거나 
					(ex.at<uchar>(j - 8, i) == 255 && ex.at<uchar>(j, i) == 0) ||   //해당 픽셀이 검정색(선)이고 위쪽으로 8개 간 픽셀이 하얀색(바탕)이거나 
					(ex.at<uchar>(j, i + 8) == 255 && ex.at<uchar>(j, i) == 0) ||   //해당 픽셀이 검정색(선)이고 오른쪽으로 8개 간 픽셀이 하얀색(바탕)이거나
					(ex.at<uchar>(j + 8, i) == 255 && ex.at<uchar>(j, i) == 0))   //해당 픽셀이 검정색(선)이고 아래쪽으로 8개 간 픽셀이 하얀색(바탕)이면
					src.at<uchar>(j, i) = 255;   //원본이미지에 하얀색 픽셀로 표시한다.
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


	//ex.at<uchar>(5, 5) = 0;//검정색


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

/*void block_count() {

   Mat src = imread("im35_t_mask.bmp", 0);
   for (int i = 0; i < src.size().height; i += 8) {
	  for (int j = 0; j < src.size().width; j += 8) {
		 rectangle(src, Point(j, i), Point(j + 8, i + 8), Scalar(22), 1);
	  }
   }

   int sum = 0;
   int block = 0;
   for (int i = 0; i < src.size().height; i += 8) {
	  for (int j = 0; j < src.size().width; j += 8) {
		 for (int x = 0; x < 8; x++) {
			for (int y = 0; y < 8; y++) {
			   if (src(Rect(j, i, 8, 8)).at<uchar>(y, x) == 255) {
				  sum = sum + 1;
			   }

			}

		 }
		 if (sum == 48) {
			block++;
		 }
		 sum = 0;
	  }

   }
   cout << block / 2 << endl;
   imshow("src", src);
   waitKey();

};*/