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
        total_src = total; 
    }
    else
    {
        tar_L_avg = total_L/total;
        tar_a_avg = total_a/total;
        tar_b_avg = total_b/total;
        total_tar = total;
    } 

}
void Lab_Color::Difference(IplImage* data, bool isSrc)
{
   if(isSrc)
   {
        
    for (int y = 0; y < data->height; y++) {
        for (int x = 0; x < data->width; x++) {
             char* Lab = ((char*)data->imageData+ data->widthStep*y);
             sL[y*data->width +x] = (float)Lab[2] -src_L_avg;             
             sa[y*data->width +x] = (float)Lab[1] -src_a_avg;             
             sb[y*data->width +x] = (float)Lab[0] -src_b_avg;             
        }
    }  
   }
   else 
   {
    for (int y = 0; y < data->height; y++) {
        for (int x = 0; x < data->width; x++) {
             char* Lab = ((char*)data->imageData+ data->widthStep*y);
             tL[y*data->width +x] = (float)Lab[2] -tar_L_avg;             
             ta[y*data->width +x] = (float)Lab[1] -tar_a_avg;             
             tb[y*data->width +x] = (float)Lab[0] -tar_b_avg;             
        }
    }  
   }

}
void Lab_Color::Sum()
{ 
    for (int i = 0; i < sizeof(sL); i++) {
      t_Lsum +=  pow(tL[i]-tar_L_avg,2);
      t_asum +=  pow(ta[i]-tar_a_avg,2);
      t_bsum +=  pow(tb[i]-tar_b_avg,2);
    }
    for (int i = 0; i < sizeof(tL); i++) {
      s_Lsum +=  pow(sL[i]-src_L_avg,2);
      s_asum +=  pow(sa[i]-src_a_avg,2);
      s_bsum +=  pow(sb[i]-src_b_avg,2);
    }
    

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
