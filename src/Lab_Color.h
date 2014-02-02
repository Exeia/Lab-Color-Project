#include <stdio.h>
#include "cv.h"

using namespace cv;
class Lab_Color {
	private:
        IplImage *img1, *img2,*img_result;
        Mat im1, im2;
        
        //total average mean of Lab
        float src_L_avg, src_a_avg, src_b_avg, tar_L_avg, tar_a_avg, tar_b_avg;
        //stored Lab values for source and target image
        float *sL, *sa, *sb, *tL, *ta, *tb;
        float *new_L, *new_a, *new_b;
        float total_tar, total_src;
        float t_Lsum, t_asum,t_bsum,s_Lsum,s_asum,s_bsum;
        void Mean(IplImage *, int );
        void Sum(); 
        void Std_dev();
        void Transfer();

        //the current lab - the average
        void Difference();
        
        
        void matMean(Mat, int );
        void matSum(); 
        void matStd_dev();
        void matTransfer();

        //the current lab - the average
        void matDifference();
	public:
        Lab_Color(IplImage* i1, IplImage* i2);
        Lab_Color(Mat i1, Mat i2);
        ~Lab_Color(void);
        bool Init();
        void Convert();
	
};
