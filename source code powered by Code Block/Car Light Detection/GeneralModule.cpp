#include "GeneralModule.h"

GeneralModule* GeneralModule::_instance = NULL;

GeneralModule::GeneralModule()
{
	;
}

GeneralModule* GeneralModule::Instance()
{
	if (_instance == 0)
	{
		_instance = new GeneralModule();
	}
	return _instance;
}

Mat GeneralModule::LoadImage(const char* path)
{
    Mat img = imread(path, CV_LOAD_IMAGE_UNCHANGED);
    return img;
}

int GeneralModule::ShowImage(const Mat& img) const
{
    if (img.empty())
    {
        printf("Load source image failed!\r\n");
        return 1;
    }
    namedWindow("Debug", CV_WINDOW_AUTOSIZE);
    imshow("Debug", img);
    waitKey(0);
    destroyWindow("Debug");
    return 0;
}

void GeneralModule::StartCounting()
{
	gettimeofday(&tv,NULL);
	originalMilliSecond = (unsigned long long)tv.tv_sec*1000 + tv.tv_usec/1000;
}

void GeneralModule::StopCounting()
{
	gettimeofday(&tv,NULL);
	terminalMilliSecond = (unsigned long long)tv.tv_sec*1000 + tv.tv_usec/1000;
}
