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


	Mat srcDCT;   //����Ǿ� ������ �̹���
	src.convertTo(srcDCT, CV_32F); // dct�� 32F or 64F ����


	Mat L0_LE(src.size().height, src.size().width, CV_32F);
	Mat bloc(8, 8, CV_32F);


	float L0 = 0;

	for (int i = 0; i < src.size().height - 8; i++) // 8������ �̵��ϴ� for��(cols)
	{
		for (int j = 0; j < src.size().width - 8; j++) // 8������ �̵��ϴ� for��(rows)
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
					} // 8 X 8 �̵��ϸ鼭 0�� �ƴ� ���� ������ L0�� 1�� �߰�
				}
			}

			L0_LE(Rect(j, i, 1, 1)).at<float>(0, 0) = L0 / 64; // L0_LE(Rect(j,i,1,1)�� 8x8 L0�� �Է�

			L0 = 0; // L0 �� 0���� �ʱ�ȭ
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
			for (int x = 0; x < LE_block.size().height; x++) {   //�ּڰ� ���ϴ� for��
				for (int y = 0; y < LE_block.size().width; y++) {
					minimum = min(LE_block.at<float>(y, x), minimum); // 8x8 ��Ͽ��� minimum�� ã��

				}
			}
			//cout << minimum << endl;
			for (int x = 0; x < LE_block.size().height; x++) {   //�ּڰ� ���ϴ� for��
				for (int y = 0; y < LE_block.size().width; y++) {
					if (LE_block.at<float>(y, x) == minimum) {
						minimum_num++;   // 8x8�ҷϿ��� minimum���� ���� ã��
					}
					// cout << minimum << endl;
				}
			}

			for (int a = 0; a < LE_block.size().height; a++) {
				for (int b = 0; b < LE_block.size().width; b++) {
					if (minimum_num == 1) {                // 8x8 ��Ͽ� minimum���� 1����
						if (LE_block.at<float>(b, a) == minimum) {
							local_minima(Rect(j, i, 8, 8)).at<float>(b, a) = minimum; // minimum�� ����
						}
						else local_minima(Rect(j, i, 8, 8)).at<float>(b, a) = 255; // ������ 100���� ä��
					}
					else local_minima(Rect(j, i, 8, 8)).at<float>(b, a) = 255; // minimum���� �������� �� 100���� ä��
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


	//������
	//���� ����� �� ���ֱ�(�Լ� ����ҷ���)
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

	//�ȼ� �� Ȯ��
	/*for (int x = 0; x < src.size().height; x++) {
	   for (int y = 0; y < src.size().width; y++) {
		  if (img.at<uchar>(y, x) != 0 && img.at<uchar>(y, x) != 255)
			 cout << img.at<uchar>(y, x);
	   }
	}*/

	// ������ ������� �ڵ�¥��
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
				if ((ex.at<uchar>(j, i - 8) == 255 && ex.at<uchar>(j, i) == 0) ||   //�ش� �ȼ��� ������(��)�̰� �������� 8�� �� �ȼ��� �Ͼ��(����)�̰ų� 
					(ex.at<uchar>(j - 8, i) == 255 && ex.at<uchar>(j, i) == 0) ||   //�ش� �ȼ��� ������(��)�̰� �������� 8�� �� �ȼ��� �Ͼ��(����)�̰ų� 
					(ex.at<uchar>(j, i + 8) == 255 && ex.at<uchar>(j, i) == 0) ||   //�ش� �ȼ��� ������(��)�̰� ���������� 8�� �� �ȼ��� �Ͼ��(����)�̰ų�
					(ex.at<uchar>(j + 8, i) == 255 && ex.at<uchar>(j, i) == 0))   //�ش� �ȼ��� ������(��)�̰� �Ʒ������� 8�� �� �ȼ��� �Ͼ��(����)�̸�
					src.at<uchar>(j, i) = 255;   //�����̹����� �Ͼ�� �ȼ��� ǥ���Ѵ�.
		}
	}

	//cout << src.size() << endl;
	//cout << local_minima_4.size() << endl;

	//int h = src.size().height;
	//int w = src.size().width;

	// ���ɿ��� ���� (set ROI (X, Y, W, H)).
	Rect rect(0, 0, 1280, 760);
	// ���ɿ��� �ڸ��� (Crop ROI).
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


	//ex.at<uchar>(5, 5) = 0;//������


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