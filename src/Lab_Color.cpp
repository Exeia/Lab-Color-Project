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
Lab_Color::Lab_Color(Mat i1, Mat i2)
{
    im1 = i1;
    im2 = i2;
}
Lab_Color::~Lab_Color()
{
    cvReleaseImage(&img1);
    cvReleaseImage(&img2);
}

bool Lab_Color::Init()
{
    int size1 = img1->height *img1->width;
    int size2 = img2->height *img2->width;
    if (size1 > size2 ) {
        //resize
        /* code */
    }
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
       
      // uchar * map= (uchar*) img2->imageData ;
       for (int x = 0; x < img2->width; x++) {
           map[0] =new_b[y*img2->width +x] ;
           map[x*img2->nChannels+1] =new_a[y*img2->width +x];
           map[x*img2->nChannels+2] =new_L[y*img2->width +x];
 // B
 // G

           //((uchar *)(img2->imageData + y*img2->widthStep))[x*img2->nChannels + 2]=new_L[y*img2->width +x];// R
           //((uchar *)(img2->imageData + y*img2->widthStep))[x*img2->nChannels +1]=new_a[y*img2->width +x];// R
       }
   }
   
   cvCvtColor(img1, img1, CV_Lab2RGB);
   cvCvtColor(img2, img2, CV_Lab2RGB);
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
   Std_dev();
   Transfer();
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
    float total= data->width * data->height;
    for (int y = 0; y < data->height; y++) {
        for (int x = 0; x < data->width; x++) {
             //uchar* Lab = ((uchar*)data->imageData+ data->widthStep*y);
             uchar* Lab = ((uchar*)data->imageData+ data->widthStep*y);
             total_L += (float)Lab[2] ;
             total_a += (float)Lab[1];
             total_b += (float)Lab[0];

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
void Lab_Color::Difference( )
{
    sL = new float[img1->height * img1->width ];
    sa = new float[img1->height * img1->width ];
    sb = new float[img1->height * img1->width ];
    
    for (int y = 0; y < img1->height; y++) {
        for (int x = 0; x < img1->width; x++) {
             uchar* Lab1 = ((uchar*)img1->imageData+ img1->widthStep*y);
             sL[y*img1->width +x] = (float)Lab1[2] - src_L_avg;             
             sa[y*img1->width +x] = (float)Lab1[1] -src_a_avg;             
             sb[y*img1->width +x] = (float)Lab1[0] -src_b_avg;  
             //cout <<"a" <<src_a_avg << endl; 
             //cout <<"b" <<src_b_avg << endl; 
                           
        }


    }
}
void Lab_Color::Sum()
{ 
    
    for (int y = 0; y < img1->height; y++) {
        for (int x = 0; x < img1->width; x++) {
             uchar* Lab1 = ((uchar*)img2->imageData+ img2->widthStep*y);
             s_Lsum += pow((float)Lab1[2],2);
             s_asum += pow((float)Lab1[1],2);
             s_bsum += pow((float)Lab1[0],2);
        }
    }
    for (int y = 0; y < img2->height; y++) {
        for (int x = 0; x < img2->width; x++) {
             uchar* Lab2 = ((uchar*)img2->imageData+ img2->widthStep*y);
             t_Lsum += pow((float)Lab2[2],2);
             t_asum += pow((float)Lab2[1],2);
             t_bsum += pow((float)Lab2[0],2);
        }
    }

}

void Lab_Color::Std_dev()
{
   
    new_L = new float[img1->height * img1->width], new_a = new float[img1->height * img1->width],
    new_b= new float[img1->height * img1->width];
   for (int y = 0; y < img1->height; y++) {
       for (int x = 0; x < img1->width; x++) {
           int c = y*img1->width + x;
           new_L[c] = ((sqrt(t_Lsum/total_tar)/sqrt(s_Lsum/total_src)) * sL[y*img1->width +x]) + tar_L_avg; 
           new_a[c] = ((sqrt(t_asum/total_tar)/sqrt(s_asum/total_src)) * sa[y*img1->width +x]) + tar_a_avg; 
           new_b[c] = ((sqrt(t_bsum/total_tar)/sqrt(s_bsum/total_src)) * sb[y*img1->width +x])+tar_b_avg;

   //       cout << "L " << new_L[y*img1->width + x] << endl; 
       }
   }
}

void Lab_Color::matMean(Mat dat, int num)
{
    float total_L, total_a, total_b;
    float total= dat.rows*dat.cols;
    unsigned char  *input = (unsigned char*) (dat.data);
    for (int y = 0; y < dat.cols; y++) {
        for (int x = 0; x < dat.rows; x++) {
             total_L += (float)input[dat.step*y+x+2] ;
             total_a += (float)input[dat.step*y+x+1];
             total_b += (float)input[dat.step*y+x];

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
void Lab_Color::matSum()
{

    unsigned char  *input = (unsigned char*) (im1.data);
    unsigned char  *input1 = (unsigned char*) (im2.data);
    for (int y = 0; y < im1.cols; y++) {
        for (int x = 0; x < im1.rows; x++) {
             s_Lsum += pow((float)input[im1.step*y+x+2],2);
             s_asum += pow((float)input[im1.step*y+x+1],2);
             s_bsum += pow((float)input[im1.step*y+x],2);
        }
    }
    for (int y = 0; y < im2.cols; y++) {
        for (int x = 0; x < im2.rows; x++) {
             t_Lsum += pow((float)input1[im2.step*y+x+2],2);
             t_asum += pow((float)input1[im2.step*y+x+1],2);
             t_bsum += pow((float)input1[im2.step*y+x],2);
        }
    }
}
void Lab_Color::matStd_dev()
{

    new_L = new float[im1.rows * im1.cols], new_a = new float[im1.rows * im1.cols],
    new_b= new float[im1.cols * im1.rows];
   for (int y = 0; y < im1.cols; y++) {
       for (int x = 0; x < im1.rows; x++) {
           int c = y*img1->width + x;
           new_L[c] = ((sqrt(t_Lsum/total_tar)/sqrt(s_Lsum/total_src)) * sL[y*img1->width +x]) + tar_L_avg; 
           new_a[c] = ((sqrt(t_asum/total_tar)/sqrt(s_asum/total_src)) * sa[y*img1->width +x]) + tar_a_avg; 
           new_b[c] = ((sqrt(t_bsum/total_tar)/sqrt(s_bsum/total_src)) * sb[y*img1->width +x])+tar_b_avg;

   //       cout << "L " << new_L[y*img1->width + x] << endl; 
       }
   }
}


void Lab_Color::matTransfer()
{

}
