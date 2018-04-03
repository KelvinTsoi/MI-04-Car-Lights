#include "Defined.h"
#include "GeneralModule.h"
#include "Utility.h"
#include "Pretreatment.h"

void PrintHelp(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Invalid Parameter. Option:\r\n");
        printf("--version\r\n");
        exit(0);
    }
}

void PrintVersionInfo(int argc, char** argv)
{
    char DTChar[100] = {0};
    if (argc == 2)
    {
        strcpy(DTChar, argv[1]);
        if (!strcasecmp(DTChar, "--version"))
        {
            char date[32] = __DATE__;
            struct tm t;
            memset(&t, 0, sizeof (t));
            strptime(date, "%b %d %Y", &t);
            t.tm_mon += 1;
            printf("\r\n"
                   "Application Name: Car Light Detection\r\n"
                   "Application Version: %s\r\n"
                   "Compile Date: %04d-%02d-%2d %s\r\n"
                   "\r\n",
                   SOFTWARE_VERSION, t.tm_year + 1900, t.tm_mon, t.tm_mday, __TIME__);
            exit(0);
        }
    }
}

int alpha_slider = 0;
int alpha_slider_max = 255;
Mat AdjustImgSrc;

 void on_ParameterSelect(int position,void *param)
 {
     Mat AdjustImgDst(AdjustImgSrc.rows, AdjustImgSrc.cols, CV_8UC1);
     Pretreatment::Instance()->Binaryzation(AdjustImgSrc, AdjustImgDst, (const int)alpha_slider);
     imshow("Slider", AdjustImgDst);
 }

int main(int argc, char** argv)
{
    PrintHelp(argc, argv);
    PrintVersionInfo(argc, argv);
    Mat srcMat = GeneralModule::Instance()->LoadImage(argv[1]);
    GeneralModule::Instance()->ShowImage(srcMat);

    Mat grayMat(srcMat.rows, srcMat.cols, CV_8UC1);
    Pretreatment::Instance()->Graying(srcMat, grayMat);
    GeneralModule::Instance()->ShowImage(grayMat);

    Mat blurMat(srcMat.rows, srcMat.cols, CV_8UC1);
    Pretreatment::Instance()->Bluring(grayMat, blurMat);
    GeneralModule::Instance()->ShowImage(blurMat);

    #if 0
    AdjustImgSrc = blurMat;
    namedWindow("Slider", 1);
    createTrackbar("Threshold Control", "Slider", &alpha_slider, alpha_slider_max, on_ParameterSelect);
    on_ParameterSelect(alpha_slider, 0);
    waitKey(0);
    #endif

    int liminal = 0;
    Pretreatment::Instance()->OTSU(blurMat, liminal);

    double grayscaleAverage = Utility::Instance()->AverageGreyDegree(blurMat);

    Mat binaryMat(srcMat.rows, srcMat.cols, CV_8UC1);
    Pretreatment::Instance()->Binaryzation(blurMat, binaryMat, liminal + grayscaleAverage * 2.0);
    GeneralModule::Instance()->ShowImage(binaryMat);

    Mat morphologyMat(srcMat.rows, srcMat.cols, CV_8UC1);
    Pretreatment::Instance()->Reconstruction(binaryMat, morphologyMat);
    GeneralModule::Instance()->ShowImage(morphologyMat);

    Mat opaqueMat(srcMat.rows, srcMat.cols, CV_8UC1);
    Pretreatment::Instance()->RemoveReflective(morphologyMat, opaqueMat);
    GeneralModule::Instance()->ShowImage(opaqueMat);

    Mat dstMat(srcMat.rows, srcMat.cols, CV_8UC1);
    Pretreatment::Instance()->Marker(opaqueMat, srcMat);
    GeneralModule::Instance()->ShowImage(srcMat);

    return 0;
}
