#include<opencv2\opencv.hpp>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<iostream>
#include<string>

using namespace std;
using namespace cv;

Point pt1(0 , 0);
Point pt2(0 , 0);

bool is_selecting = false;

bool flag1 = true;
bool flag2 = true;

string s_input = "sample/";	//源图像所在文件路径
string s_output = "result/";	//输出图像文件路径
string s_format = ".jpg";	//图像格式
int n = 52;			//图片最大编号
int m = 1;			//图片最小编号

ostringstream oss_input;	//拼接源图像路径
ostringstream oss_output;	//拼接输出图像路径

//鼠标监听事件
void onMouseCallback(int mouseEvent , int x , int y , int flags , void* params)
{
	switch (mouseEvent)
	{
		case CV_EVENT_LBUTTONDOWN:	//鼠标左键按下
			pt1 = Point(x , y);
			pt2 = Point(x , y);
			is_selecting = true;
			break;
		case CV_EVENT_MOUSEMOVE:	//鼠标移动
			if (is_selecting)
				pt2 = Point(x , y);
			break;
		case CV_EVENT_LBUTTONUP:	//鼠标左键松开
			pt2 = Point(x , y);
			is_selecting = false;
			break;
		default:
			break;
	}
}

//连接字符串
void jointString(int i)
{
	oss_input.str("");
	oss_output.str("");
	oss_input << s_input << i << s_format;
	oss_output << s_output << i << s_format;
}

//处理越界的点
void processPoint(Point &p1 , Point &p2 , Mat image)
{
	if (pt1.x < 0)
		pt1.x = 0;
	else if (pt1.x>image.cols)
		pt1.x = image.cols;

	if (pt2.x < 0)
		pt2.x = 0;
	else if (pt2.x>image.cols)
		pt2.x = image.cols;

	if (pt1.y < 0)
		pt1.y = 0;
	else if (pt1.y>image.rows)
		pt1.y = image.rows;

	if (pt2.y < 0)
		pt2.y = 0;
	else if (pt2.y>image.rows)
		pt2.y = image.rows;
}

int main(void)
{
	int i = m;	//当前图片编号

	jointString(i);	//生成源图像和输出图像路径

	Mat image = imread(oss_input.str());	//导入图像
	Mat show_image;				//备份图像
	Mat saveImage(32 , 32 , image.type());	//目标图像,大小32*32px

	namedWindow("image");				//设置窗口名称
	setMouseCallback("image" , onMouseCallback);	//为窗口设置监听事件

	while (flag1)
	{
		show_image = image.clone();

		//绘制矩形
		rectangle(show_image , pt1 , pt2 , Scalar(255 , 255 , 255));

		imshow("image" , show_image);

		//获取键盘输入
		int key = waitKey(10);

		//处理不合法的点
		processPoint(pt1 , pt2 , show_image);

		//获取兴趣区域
		Mat roiImage(image , Rect(min(pt1.x , pt2.x) , min(pt1.y , pt2.y) , abs(pt2.x - pt1.x) , abs(pt2.y - pt1.y)));

		flag2 = true;
		while (flag2)
		{
			switch (key)
			{
				case 115:	//按s键,保存截图
					flag2 = false;

					//缩放图像
					resize(roiImage , saveImage , saveImage.size() , 0 , 0 , INTER_LINEAR);
					imwrite(oss_output.str() , saveImage);
					break;
				case 110:	//按n键,下一张图片
					flag2 = false;

					//取消框框
					pt1 = Point(0 , 0);
					pt2 = Point(0 , 0);

					i++;
					jointString(i);
					image = imread(oss_input.str());
					if (image.empty())
					{
						if (i > n)
						{
							i = n-1;
						}
						flag2 = true;
					}
					break;
				case 112:	//按p键，上一张图片
					flag2 = false;

					//取消框框
					pt1 = Point(0 , 0);
					pt2 = Point(0 , 0);

					i--;
					jointString(i);
					image = imread(oss_input.str());
					if (image.empty())
					{
						if (i < m)
						{
							i = m+1;
						}
						flag2 = true;
					}
					break;
				case 113:	//按q键，退出
					flag2 = false;
					flag1 = false;
					break;
				default:
					flag2 = false;
					break;
			}
		}

	}

	return 0;
}
