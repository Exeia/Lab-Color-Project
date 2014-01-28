#include "Lab_Color.h"
#include <math.h>
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
void Lab_Color::Transfer()
{
    
   CvSize size = cvGetSize(img1);

   img_result = cvCreateImage(size, img1->depth, 3);

   for (int y  = 0; y < img2->height; y++) {
       uchar * map= (uchar*) (img2->imageData +y * img2->widthStep);
       
       for (int x = 0; x < img2->width; x++) {
           map[3*x] = new_b[y*img2->width +x];
           map[3*x+1] =new_a[y*img2->width +x] ;
           map[3*x+2] =new_L[y*img2->width +x];
       }
   }
}
void Lab_Color::Convert()
{
   
   cvCvtColor(img1, img1, CV_RGB2Lab);
   
   cvCvtColor(img2, img2, CV_RGB2Lab);
   Mean(img1,1);
   Mean(img2,2);
   Difference();
  // Difference(img2, false);
  Sum();
   //Std_dev();
   //Transfer();
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
             uchar* Lab = ((uchar*)data->imageData+ data->widthStep*y);
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
        total_src = total;
       cout << "Source: " << endl; 
   cout <<"L" <<src_L_avg << endl; 
   cout <<"a" <<src_a_avg << endl; 
   cout <<"b" <<src_b_avg << endl; 
    }
    else
    {
        tar_L_avg = total_L/total;
        tar_a_avg = total_a/total;
        tar_b_avg = total_b/total;
        total_tar = total;
        cout << "Source: " << endl; 
        cout <<"L" <<src_L_avg << endl; 
        cout <<"a" <<src_a_avg << endl; 
        cout <<"b" <<src_b_avg << endl; 
    } 

}
void Lab_Color::Difference( )
{
    sL = new float[img1->height * img1->width ];
    sa = new float[img1->height * img1->width ];
    sb = new float[img1->height * img1->width ];
    cout <<"Difference: src" << endl;
    
    for (int y = 0; y < img1->height; y++) {
        for (int x = 0; x < img1->width; x++) {
             uchar* Lab1 = ((uchar*)img1->imageData+ img1->widthStep*y);
             sL[y*img1->width +x] = (float)Lab1[2] - src_L_avg;             
             sa[y*img1->width +x] = (float)Lab1[1] -src_a_avg;             
             sb[y*img1->width +x] = (float)Lab1[0] -src_b_avg;  
             cout << (float)Lab1[2] <<" "<< sL[y*img1->width +x] << endl;             
             //cout <<"a" <<src_a_avg << endl; 
             //cout <<"b" <<src_b_avg << endl; 
                           
        }


    }
}
void Lab_Color::Sum()
{ 
    

}

void Lab_Color::Std_dev()
{
   for (int y = 0; y < img1->height; y++) {
       for (int x = 0; x < img1->width; x++) {
           new_L[y*img1->width + x] = (sqrt(t_Lsum/total_tar)/sqrt(s_Lsum/total_src)) * sL[y*img1->width +x]; 
           new_a[y*img1->width + x] = (sqrt(t_asum/total_tar)/sqrt(s_asum/total_src)) * sa[y*img1->width +x]; 
           new_b[y*img1->width + x] = (sqrt(t_bsum/total_tar)/sqrt(s_bsum/total_src)) * sb[y*img1->width +x]; 
       }
   }
}
