#include "Utility.h"

Utility* Utility::_instance = NULL;

Utility::Utility()
{
	;
}

Utility* Utility::Instance()
{
	if (_instance == 0)
	{
		_instance = new Utility();
	}
	return _instance;
}

double Utility::EuclideanDistance(const Point pt1, const Point pt2)
{
    double firstTerm = pow( abs( pt1.x - pt2.x ) * 1.0, 2.0 );
	double lastTerm = pow( abs( pt1.y - pt2.y ) * 1.0, 2.0 );
	return sqrt(firstTerm + lastTerm);
}

double Utility::AverageGreyDegree(const Mat& grayMat)
{
    long total = grayMat.rows * grayMat.cols;
	long sum = 0;

	for( int row = 0; row < grayMat.rows; row++ )
	{
		const uchar *imageData = grayMat.ptr<uchar>(row);
		for( int col = 0; col < grayMat.cols; col++ )
			sum += *imageData++;
	}
	return ( sum * 1.0 / total * 1.0 );
}

double Utility::StandardDeviation(const double dataList[], const int sizeOfList)
{
    double sumUp = 0;
    for(int i = 0; i < sizeOfList; i++)
    {
        sumUp += dataList[i];
    }
    double avg = sumUp / sizeOfList * 1.0;

    double sum;
    for(int i = 0; i < sizeOfList; i++)
    {
        sum += (dataList[i] - avg) * (dataList[i] - avg);
    }
    return sqrt(sum/sizeOfList);
}

int Utility::NearestNeighborhoodPoint(const Point pointSet[], const int pointSetSize, const Point homePoint)
{
    int Neighborhood = 0;
    double currentShortestDistance;

    if(pointSet[0] != homePoint)
        currentShortestDistance = EuclideanDistance(pointSet[0], homePoint);
    else
        currentShortestDistance = EuclideanDistance(pointSet[1], homePoint);

    for(int i = 1; i < pointSetSize; i++)
    {
        if(pointSet[i] == homePoint)
            continue;

        if(EuclideanDistance(pointSet[i], homePoint) < currentShortestDistance)
        {
            Neighborhood = i;
            currentShortestDistance = EuclideanDistance(pointSet[i], homePoint);
        }
    }
    return Neighborhood;
}
