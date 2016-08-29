#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <iostream>
using namespace std;
using namespace cv;
#define REP(i, n)   for(int i=0;i<(int)(n);++i)
#define LOOP(i,b,n)   for(int i=(b);i<(int)(n);++i)

// Function Headers
Rect detectAndDisplay(Mat frame);
string face_cascade_name = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;
int filenumber; // Number of file to be saved
string filename;
Rect call_face_detect(Mat frame)
{
	// Load the cascade
	if (!face_cascade.load(face_cascade_name))
	{
		printf("--(!)Error loading\n");
		return Rect();
	};
	if (!frame.empty())
		return detectAndDisplay(frame);
	else
		printf(" --(!) No captured frame -- Break!");
	return Rect();
}

Rect detectAndDisplay(Mat frame)
{
	std::vector<Rect> faces;
	Mat frame_gray;
	Mat crop;
	Mat res;
	Mat gray;
	string text;
	stringstream sstm;
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);
	// Detect faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
	cv::Rect roi_b;
	cv::Rect roi_c;
	size_t ic = 0; // ic is index of current element
	int ac = 0; // ac is area of current element
	size_t ib = 0; // ib is index of biggest element
	int ab = 0; // ab is area of biggest element
	for (ic = 0; ic < faces.size(); ic++) // Iterate through all current elements (detected faces)
	{
		roi_c.x = faces[ic].x;
		roi_c.y = faces[ic].y;
		roi_c.width = (faces[ic].width);
		roi_c.height = (faces[ic].height);
		ac = roi_c.width * roi_c.height; // Get the area of current element (detected face)
		roi_b.x = faces[ib].x;
		roi_b.y = faces[ib].y;
		roi_b.width = (faces[ib].width);
		roi_b.height = (faces[ib].height);
		ab = roi_b.width * roi_b.height; // Get the area of biggest element, at beginning it is same as "current" element
		if (ac > ab)
		{
			ib = ic;
			roi_b.x = faces[ib].x;
			roi_b.y = faces[ib].y;
			roi_b.width = (faces[ib].width);
			roi_b.height = (faces[ib].height);
		}
		crop = frame(roi_b);
		resize(crop, res, Size(128, 128), 0, 0, INTER_LINEAR); // This will be needed later while saving images
		cvtColor(crop, gray, CV_BGR2GRAY); // Convert cropped image to Grayscale
		//faces[ic].width += 110;
		faces[ic].width += faces[ic].width*0.40;
		Point pt1(faces[ic].x, faces[ic].y); // Display detected faces on main window - live stream from camera
		Point pt2((faces[ic].x + faces[ic].height), (faces[ic].y + faces[ic].width));
		rectangle(frame, pt1, pt2, Scalar(0, 255, 0), 2, 8, 0);
		imshow("Face detected or not",frame);
		cout << faces.size();
		return faces[ic];
	}
	printf("there is no clear face\n");
	return Rect();
}
int crop(Mat shirt)
{
	int count = 0;
	REP(i, shirt.rows)
	{
		REP(j, shirt.cols)
		{
			if (shirt.at<uchar>(i, j) == 0)goto l1;
		}
		count++;
	}
l1:return count;
}
Mat imageCapture()
{
	//Capture image from the webcam
	
	VideoCapture stream1(0);   //0 is the id of video device.0 if you have only one camera.
 
	if (!stream1.isOpened()) 
	{ //check if video device has been initialised
	cout << "cannot open camera";
	}
 
	//unconditional loop
	L2:while (true) 
	{
	Mat cameraFrame;
	stream1.read(cameraFrame);
	imshow("cam", cameraFrame);
	if (waitKey(30) >= 0)
	{
		imwrite("im.jpg",cameraFrame);
		goto L1;
	}
	   }
	L1:
	{
		Mat im=imread("im.jpg");imshow("Captured image",im);
		waitKey(5000);
		int c;
		cout<<"\nAre you happy with the captured image?\n1.Yes\n2.No:";
		cin>>c;
		if(c==1) return im;
		else if(c==2)goto L2;
		else cout<<"Wrong choice";

	}
}
void catalog()
{
	waitKey(60);
	cout<<"**************CATALOG**************\n";
	Mat cat=imread("catalog.jpg");resize(cat,cat, Size(800,800));imshow("Catalog",cat);

}
Mat list(int item)
{
	switch(item)
	{
	case 1:return imread("t1.jpg");
	case 2:return imread("t2.jpg");
	case 3:return imread("t3.jpg");
	case 4:return imread("t4.jpg");
	case 5:return imread("t5.jpg");
	case 6:return imread("t6.jpg");
	case 7:return imread("t7.jpg");
	case 8:return imread("t8.jpg");
	case 9:return imread("t9.jpg");
	default:cout<<"\nInvalid option";
	}
}

// Function detectAndDisplay
int main(int argc, char** argv)
{
	Mat read_img,read_img1,shirt;int item;
	catalog();
	waitKey(60);
	read_img1 =imageCapture();
	L4:cout<<"\nEnter your choice:";
	cin>>item;
	shirt=list(item);
	//read_img = imread("C:\\Users\\Sugosh\\Documents\\Visual Studio 2013\\Projects\\FirstOpenCV\\FirstOpenCV\\VFR_Datasets\\b18.jpg");
	resize(read_img1, read_img, Size(800,800));
	Mat can=read_img.clone();
	GaussianBlur(can, can, Size(7, 7), 0, 0);
	imshow("After smoothing", can);
	cvtColor(can, can, COLOR_RGB2GRAY);
	imshow("After supressing", can);
	Canny(read_img, can, 90, 90 * 3, 3);
	imshow("Canny", can);
	
	Rect face = call_face_detect(read_img);
	resize(shirt, shirt, Size(face.width*2.5,face.height*5));
	//resize(shirt, shirt, Size(150,200));
	Mat im_gray, shirt_bw;
	cvtColor(shirt, im_gray, CV_RGB2GRAY);
	threshold(im_gray, shirt_bw, 200, 255, THRESH_BINARY);
	int face_mid_x = face.x + (face.height / 2);
	int face_mid_y = face.y + face.width;
	int row_disp = crop(shirt_bw);
	int shirt_width = shirt_bw.cols, shirt_height = shirt_bw.rows;
	imshow("..", shirt_bw);
	try
	{
	for (int i = 0; i < shirt_bw.rows; i++)
	{
		for (int j = 0; j < shirt_bw.cols; j++)
		{
			int x = shirt_bw.at<uchar>(i, j);
			if (x == 0)
			{
				Vec3b colors = shirt.at<Vec3b>(i, j);
				int m = face_mid_y-row_disp + i;
				int n = face_mid_x-(shirt_width/2) + j;
				if(m>read_img.rows||n>read_img.cols)
				{
					cout<<"\nInvalid input image";
					break;
				}
				read_img.at<Vec3b>(m, n) = colors;
			}
		}
	}
	}
	catch(Exception e)
	{cout<<"fail";}
	cout << endl <<face.width<<" "<<face.height<<"--"<< face.width * face.height << endl;
	imshow("Final", read_img);
	waitKey(0);
	int shir;
	cout<<"\nDo you want to try on another shirt?1.Yes\n2.No:";
	cin>>shir;
	if(shir==1)
	{
		destroyAllWindows();
		catalog();
		waitKey(60);
		goto L4;
	}
	else if(shir==2)return 0;
	//int c = cvWaitKey(0);
	
}
