#include <stdio.h>
#include "cv.h"

using namespace cv;
class Lab_Color {
	private:
        IplImage *img1, *img2, *img_result;
        void to_XYZ(uchar*, uchar*, uchar*);

	public:
        Lab_Color(IplImage* i1, IplImage* i2);
        ~Lab_Color(void);
        bool Init();
        void Convert();
	
};
