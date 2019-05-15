/*

#include<iostream>
#include<opencv.hpp>
using namespace std;
using namespace cv;

int main()
{
	//-- 读图
	Mat img1 = imread("qw.jpg");
	Mat img2 = imread("qa.jpg");
	imshow("qw", img1);
	//GaussianBlur(img1, img1, Size(9, 9),0,0);
	//-- 初始化
	vector<KeyPoint> keypoints_1, keypoints_2;
	Mat des1, des2;
	Ptr<FeatureDetector> detector = ORB::create(); //  ::  类ORB中的成员create()
	//Ptr detector = FeatureDetector::create("ORB");
	//Ptr<FeatureDetector> descriptor = ORB::create();
	Ptr<DescriptorExtractor> descriptor = ORB::create();  //opencv中的Ptr指其使用的智能指针，指的是Template class for smart reference-counting pointers（智能指针模板类）ptr是在访问图片中像素时的操作，如image.ptr<uchar>(5),指的是访问image图片的第6行像素值
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");

	detector->detect(img1,keypoints_1);//c++:  ->  指向结构体的指针
  	detector->detect(img2, keypoints_2);  //检测FAST角点位置

	descriptor->compute(img1, keypoints_1, des1);// 计算brief 描述子
	descriptor->compute(img2, keypoints_2, des2);

	vector<DMatch> matches,goodmatches;//实例化一个匹配器
	Mat out,goodout;
	drawKeypoints(img1, keypoints_1, out, Scalar::all(-1),DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
	imshow("out1", out);
	matcher->match(des1, des2, matches);
	//处理匹配
	
	drawMatches(img1, keypoints_1, img2, keypoints_2,matches , out);
	imshow("out", out);




	//优化匹配
	int mindis=40, maxdis=42;//初始化一个值，随机，应小于最大值，大于最小值
	
	//2选一   查找最大最小值
	mindis = min_element(matches.begin(), matches.end())->distance;
	maxdis = max_element(matches.begin(), matches.end())->distance;
	
	//for (int i = 0; i <=end(matches)-begin(matches); i++)   //此处范围超过500 会翻车 原因未知，报错   vector subscript out of range
	//{
		//double dist = matches[i].distance;
		//if (dist < mindis) 	mindis = dist;
		//if (dist > maxdis) 	maxdis = dist;
	//}

	cout << mindis << "\t" << maxdis << endl;
	
	for (int i = 0; i <= 499; i++)     //此处范围超过500 会翻车 原因未知，报错   vector subscript out of range
		if (matches[i].distance <= max(2 * mindis, 20))
				goodmatches.push_back(matches[i]);

	drawMatches(img1, keypoints_1, img2, keypoints_2,goodmatches, goodout);
	imshow("goodout", goodout);

	waitKey(0);
}

*/




#include<iostream>
#include<opencv.hpp>
#include<opencv_modules.hpp>

using namespace std;
using namespace cv;

int main()
{
	Mat a,b,a1,b1,a2,b2;
	Mat out,goodout;
	int mins = 30, maxs = 32;
	vector<KeyPoint> key1, key2;
	vector<DMatch>matches,goodmatches;
	Ptr<FeatureDetector> detector = ORB::create();
	Ptr<DescriptorExtractor> exator = ORB::create();
	Ptr<DescriptorMatcher>macher = DescriptorMatcher::create("BruteForce-Hamming");

	a = imread("shot.png", 1);
	detector->detect(a, key1);
	drawKeypoints(a, key1, a1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	imshow("a1", a1);

	exator->compute(a, key1, a2);

	VideoCapture cap(0);
	cap.set(CAP_PROP_XI_HDR, 0);
	cap.set(CAP_PROP_FPS, 60);
	for (int i = 0; i < 1000; i++)
	{
		cap >> b;
		//imshow("b", b);
		detector->detect(b, key2);
		drawKeypoints(b, key2, b1, Scalar(-1), DrawMatchesFlags::DEFAULT);
		imshow("b1", b1);
		
		exator->compute(b, key2, b2);
		
		macher->match(a2, b2, matches);
		drawMatches(a, key1, b, key2, matches,out);
		imshow("out", out);
		
		mins = min_element(matches.begin(), matches.end())->distance;
		maxs = max_element(matches.begin(), matches.end())->distance;

		for (int i = 0; i < b.rows; i++)
		{
			int dist = matches[i].distance;
			if (mins > dist)	mins = dist;
			//if (maxs < dist)	maxs = dist;
			//cout << i << endl;
		}
		cout << mins << "\t" << maxs<<endl;
		
		for (int i = 0; i < 500; i++)
			if (matches[i].distance <= 20)	goodmatches.push_back(matches[i]);

		drawMatches(a, key1, b, key2, goodmatches, goodout);
		imshow("goodout", goodout);
		waitKey(10);
	}
}