#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "Defined.h"
#include "GeneralModule.h"

class Utility
{
	public:
		static Utility* Instance();
		double EuclideanDistance(const Point pt1, const Point pt2);
		double AverageGreyDegree(const Mat& grayImg);
		double StandardDeviation(const double dataList[], const int sizeOfList);
		int NearestNeighborhoodPoint(const Point pointSet[], const int pointSetSize, const Point homePoint);
	protected:
		Utility();
	private:
		static Utility* _instance;
};

#endif
