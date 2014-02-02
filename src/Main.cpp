#include <iostream>
#include "cv.h"
#include "highgui.h"
#include "Lab_Color.h"
using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    IplImage * img1 = cvLoadImage(argv[1]) ;
    IplImage * img2 = cvLoadImage(argv[2]) ;
    Mat image1 = imread(argv[1],  CV_LOAD_IMAGE_COLOR); 
    Mat image2 = imread(argv[2],  CV_LOAD_IMAGE_COLOR); 
    Lab_Color* lab = new Lab_Color(img1,img2);
    Lab_Color* lab1 = new Lab_Color(image1, image2); 
    if (! img1 ) {
        cout << "error" << endl;
        return -1;
    }
    lab->Init();    
    return 0;
}
