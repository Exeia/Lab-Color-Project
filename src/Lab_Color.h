#include <stdio.h>
#include "cv.h"

class Lab_Color {
	private:
        void RGB_to_XYZ();
        void XYZ_to_LMS();
        void LMS_to_LAB();
        void LAB_to_LMS();
        void LMS_to_XYZ();
        void XYZ_to_RGB();
	public:
        Lab_Color(void);
        ~Lab_Color(void);
        bool Init();
        void Convert();
	
}
