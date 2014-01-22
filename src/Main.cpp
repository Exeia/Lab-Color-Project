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
    Lab_Color* lab = new Lab_Color(img1,img2);
    
    if (! img1 ) {
        cout << "error" << endl;
        return -1;
    }
    lab->Init();    
    return 0;
}
