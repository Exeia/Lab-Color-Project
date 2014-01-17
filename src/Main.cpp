#include <iostream>
#include "cv.h"
#include "highgui.h"
using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    IplImage * img1 = cvLoadImage(argv[1]) ;
    //IplImage * img2 = cvLoadImage(argv[2]) ;
    if (! img1 ) {
        cout << "error" << endl;
        return -1;
    }
        
    cvNamedWindow("Display Window");
    cvShowImage("Display Window", img1);
    waitKey(0);
    cvReleaseImage(&img1);
    return 0;
}
