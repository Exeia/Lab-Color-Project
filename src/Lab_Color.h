#include <stdio.h>
#include "cv.h"

using namespace cv;
class Lab_Color {
	private:
        IplImage *img1, *img2,*img_result;
        //total average mean of Lab
        float src_L_avg, src_a_avg, src_b_avg, tar_L_avg, tar_a_avg, tar_b_avg;
        //stored Lab values for source and target image
        float *sL, *sa, *sb, *tL, *ta, *tb;

        void Mean(IplImage *, int );
        void Std_dev();

	public:
        Lab_Color(IplImage* i1, IplImage* i2);
        ~Lab_Color(void);
        bool Init();
        void Convert();
	
};
