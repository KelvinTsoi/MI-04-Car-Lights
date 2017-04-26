#ifndef _PRETREATMENT_H_
#define _PRETREATMENT_H_

#include "Defined.h"
#include "Utility.h"
#include "FastSort.h"

class Pretreatment
{
	public:
        int Graying(const Mat& srcMat, Mat& grayMat);
        int OTSU(const Mat &grayMat, int &liminal);
        int Bluring(const Mat &grayMat, Mat &blurMat);
        int Binaryzation(const Mat &grayMat, Mat &binaryMat, const int liminal);
        int Filling(Mat &binaryMat);
        int Reconstruction(const Mat &binaryMat, Mat &morphologyMat);
        int RemoveReflective(const Mat &morphologyMat, Mat &opaqueMat);
        int Marker(const Mat &opaqueMat, Mat &dstMat);
        static Pretreatment* Instance();
	protected:
		Pretreatment();

	private:
		static Pretreatment* _instance;
};

#endif
