#include "Lab_Color.h"

#include "cv.h"
#include "highgui.h"
using namespace std;
using namespace cv;
Lab_Color::Lab_Color(IplImage* i1,IplImage* i2 )
{
    img1 = i1;
    img2 = i2;
}

Lab_Color::~Lab_Color()
{
    cvReleaseImage(&img1);
    cvReleaseImage(&img2);
}

bool Lab_Color::Init()
{
    Convert();
    cvNamedWindow("Image1");
    cvNamedWindow("Image2");
    cvShowImage("Image1", img1);
    cvShowImage("Image2", img2);
    cvWaitKey(0);
    return true;
}
//RGB -> XYZ -> LMS -> LMS -> Lab color space or....
//RGB to Lab Color

void Lab_Color::Convert()
{
   CvSize size = cvGetSize(img1);

   img_result = cvCreateImage(size, img1->depth, 3);
   
   cvCvtColor(img1, img1, CV_RGB2Lab);
   
   cvCvtColor(img2, img2, CV_RGB2Lab);
    /*for (int y = 0; y < img->height; y++) {
    for(int x = 0; x < img->width; x++) {
        uchar *blue = ((uchar*)(img->imageData + img->widthStep*y))[x*3];
        uchar *green = ((uchar*)(img->imageData + img->widthStep*y))[x*3+1];
        uchar *red = ((uchar*)(img->imageData + img->widthStep*y))[x*3+2];
        }
    }*/ 

}
//computes the total mean of Lab 
void Lab_Color::Mean(IplImage *data, int num)
{
    //b-> "b", g -> "a", r -> L (assuming for now)
    float total_L, total_a, total_b;
    float total;
    for (int y = 0; y < data->height; y++) {
        for (int x = 0; x < data->width; x++) {
             char* Lab = ((char*)data->imageData+ data->widthStep*y);
             total_L += (float)Lab[2] ;
             total_a += (float)Lab[1];
             total_b += (float)Lab[0];

            total++;
        }
    }  
    if(num == 1)
    {
        src_L_avg = total_L/total;
        src_a_avg = total_a/total;
        src_b_avg = total_b/total;
    }
    else
    {
        tar_L_avg = total_L/total;
        tar_a_avg = total_a/total;
        tar_b_avg = total_b/total;
    } 

}

