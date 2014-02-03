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
    /*cvNamedWindow("Image1");
    cvNamedWindow("Image2");
    cvShowImage("Image1", img1);
    cvShowImage("Image2", img2);*/
    namedWindow( "Display window 1", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window 1", im1 );                   // Show our image inside it.

    namedWindow( "Display window 2", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window 2", im2 );                   // Show our image inside it.
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
   
  /* cvCvtColor(img1, img1, CV_RGB2Lab);
   
   cvCvtColor(img2, img2, CV_RGB2Lab);
   Mean(img1,1);
   Mean(img2,2);
   Difference();
   Sum();
   Std_dev();
   Transfer();*/
    /*for (int y = 0; y < img->height; y++) {
    for(int x = 0; x < img->width; x++) {
        uchar *blue = ((uchar*)(img->imageData + img->widthStep*y))[x*3];
        uchar *green = ((uchar*)(img->imageData + img->widthStep*y))[x*3+1];
        uchar *red = ((uchar*)(img->imageData + img->widthStep*y))[x*3+2];
        }
    }*/
    cvtColor(im1, im1, CV_RGB2Lab);
    cvtColor(im2, im2, CV_RGB2Lab);
    matMean(im1,1);
    matMean(im2,2);
    matDifference();
    matSum();
    matStd_dev();
    matTransfer();

   

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

void Lab_Color::matStd_dev()
{
   
    new_L2 = new float[im1.rows * im1.cols], new_a2 = new float[im1.rows * im1.cols],
    new_b2= new float[im1.rows * im1.cols];
   for (int y = 0; y < im1.cols; y++) {
       for (int x = 0; x < im1.rows; x++) {
           int c = y*im1.rows + x;
           new_L2[c] = ((sqrt(t_Lsum/total_tar)/sqrt(s_Lsum/total_src)) * sL[y*im1.rows +x]) + tar_L_avg; 
           new_a2[c] = ((sqrt(t_asum/total_tar)/sqrt(s_asum/total_src)) * sa[y*im1.rows +x]) + tar_a_avg; 
           new_b2[c] = ((sqrt(t_bsum/total_tar)/sqrt(s_bsum/total_src)) * sb[y*im1.rows +x])+tar_b_avg;
            
       }
   }
}

void Lab_Color::Std_dev()
{
   
    new_L = new float[img1->width * img1->height], new_a = new float[img1->width * img1->height],
    new_b= new float[img1->height * img1->width];
   for (int y = 0; y < img1->height; y++) {
       for (int x = 0; x < img1->width; x++) {
           int c = y*im1.rows + x;
           new_L[c] = ((sqrt(t_Lsum/total_tar)/sqrt(s_Lsum/total_src)) * sL[y*im1.rows +x]); 
           new_a[c] = ((sqrt(t_asum/total_tar)/sqrt(s_asum/total_src)) * sa[y*im1.rows +x]); 
           new_b[c] = ((sqrt(t_bsum/total_tar)/sqrt(s_bsum/total_src)) * sb[y*im1.rows +x]);

   //       cout << "L " << new_L[y*img1->width + x] << endl; 
       }
   }
}
void Lab_Color::matMean(Mat dat, int num)
{
    float total_L , total_a, total_b;
    float total= (dat.rows*dat.cols)/2;
    uchar  *input = (uchar*)dat.data;
    for (int y = 0; y < dat.cols; y++) {
        CvScalar scale;
        for (int x = 0; x < dat.rows; x++) {
             total_L += dat.at<Vec3b>(x,y)[2] ;
            total_a += dat.at<Vec3b>(x,y)[1];
             total_b += dat.at<Vec3b>(x,y)[0];
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
    for (int y = 0; y < im1.cols/2; y++) {
        for (int x = 0; x < im1.rows/2; x++) {
             s_Lsum += pow(im1.at<Vec3b>(x,y)[2],2);
             s_asum += pow(im1.at<Vec3b>(x,y)[1],2);
             s_bsum += pow(im1.at<Vec3b>(x,y)[0],2);
        }
    }
    for (int y = 0; y < im2.cols/2; y++) {
        for (int x = 0; x < im2.rows/2; x++) {
            /* t_Lsum += pow((float)input1[im2.step*y+x+2],2);
             t_asum += pow((float)input1[im2.step*y+x+1],2);
             t_bsum += pow((float)input1[im2.step*y+x],2);*/

             t_Lsum += pow(im2.at<Vec3b>(x,y)[2],2);
             t_asum += pow(im2.at<Vec3b>(x,y)[1],2);
             t_bsum += pow(im2.at<Vec3b>(x,y)[0],2);
        }
    }
}
void Lab_Color::matDifference()
{
    sL = new float[im1.rows * im1.cols ];
    sa = new float[im1.rows * im1.cols ];
    sb = new float[im1.rows * im1.cols ];
    cout << "row "<<im1.rows << endl; 
    cout << "col "<<im1.cols << endl; 
    for (int y = 0; y < im1.cols; y++) {
        for (int x = 0; x < im1.rows; x++) {
             //cout <<"a" <<src_a_avg << endl; 
             //cout <<"b" <<src_b_avg << endl; 
             /*sL[y*im1.rows+x]= (float)input[im1.step*y+x+2] - src_L_avg;
             sa[y*im1.rows+x]= (float)input[im1.step*y+x+1] - src_a_avg;
             sb[y*im1.rows+x]= (float)input[im1.step*y+x] - src_b_avg; 
               */
            sL[y*im1.rows+x] = im1.at<Vec3b>(x,y)[2] - src_L_avg;
            sa[y*im1.rows+x] = im1.at<Vec3b>(x,y)[1] - src_a_avg;
            sb[y*im1.rows+x] = im1.at<Vec3b>(x,y)[0] - src_b_avg;

        }


    }
}

void Lab_Color::matTransfer()
{
    unsigned char  *input = (unsigned char*) (im2.data);
   for (int y  = 0; y < im2.cols; y++) {
       
      // uchar * map= (uchar*) img2->imageData ;
       for (int x = 0; x < im2.rows; x++) {
           /*input[im2.step*y+x] =new_b2[y*im2.rows +x] ;
           input[im2.step*y+x+1] =new_a2[y*im2.rows +x];
           input[im2.step*y+x+2] =new_L2[y*im2.rows +x];*/
 // B
 // G
          im2.at<Vec3b>(x,y)[0] = new_b2[y*im2.rows +x] ;
           im2.at<Vec3b>(x,y)[1] = new_a2[y*im2.rows +x] ; 
           im2.at<Vec3b>(x,y)[2] = new_L2[y*im2.rows +x] ;
           //((uchar *)(img2->imageData + y*img2->widthStep))[x*img2->nChannels + 2]=new_L[y*img2->width +x];// R
           //((uchar *)(img2->imageData + y*img2->widthStep))[x*img2->nChannels +1]=new_a[y*img2->width +x];// R
       }
   }
   
   cvtColor(im1, im1, CV_Lab2RGB);
   cvtColor(im2, im2, CV_Lab2RGB);
}
