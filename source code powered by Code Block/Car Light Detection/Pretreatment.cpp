#include "Pretreatment.h"

Pretreatment* Pretreatment::_instance = NULL;

Pretreatment::Pretreatment()
{
	;
}

Pretreatment* Pretreatment::Instance()
{
	if (_instance == 0)
	{
		_instance = new Pretreatment();
	}
	return _instance;
}

int Pretreatment::Graying(const Mat& srcMat, Mat& grayMat)
{
    int rValue, gValue, bValue;	    // RGB Values

	int grayValue;					// Gray Value

	// Graying Loop
	for ( int row = 0; row < srcMat.rows; row++ )
	{
		uchar * imageData = grayMat.ptr<uchar>(row);
		for( int col = 0; col < srcMat.cols; col++ )
		{
			bValue = srcMat.at<Vec3b>(row,col)[0];
			gValue = srcMat.at<Vec3b>(row,col)[1];
			rValue = srcMat.at<Vec3b>(row,col)[2];

			grayValue = min( min(rValue,gValue), bValue );

			imageData[col] = grayValue;
		}
	}
	return 0;
}

int Pretreatment::OTSU(const Mat &grayMat, int &liminal)
{
    int retVal = 0;

	long size = grayMat.rows * grayMat.cols;

	// Check the mat is empty or not.
	if (grayMat.empty())
	{
        printf("The Matrix is Empty!\r\n");
		retVal = -1;
		return (retVal);
	}

	// Accept only char type matrices.
	CV_Assert(grayMat.depth() != sizeof(uchar));

	// Histogram.
	float histogram[256] = {0};

	// Find the Max Value of the image.
	for( int row = 0; row < grayMat.rows; row++ )
	{
		const uchar *imageData = grayMat.ptr<uchar>(row);

		for( int col = 0; col < grayMat.cols; col++ )
		{
			histogram[ int( *imageData++ ) ]++;
		}
	}

	// The sum of the frontground gray values.
	long sum0 = 0;

	// The sum of the background gray values.
	long sum1 = 0;

	// The numbers of frontground.
	long cnt0 = 0;

	// The numbers of background.
	long cnt1 = 0;

	// The proportion of the frontground.
	double w0 = 0;

	// The proportion of the background.
	double w1 = 0;

	// The average gray level in the frontground.
	double u0 = 0;

	// The average gray level in the background.
	double u1 = 0;

	// OTSU.
	double variance = 0;

	//double u = 0;

	double maxVariance = 0;

	for( int i = 1; i < 256; i++)
	{
		sum0 = 0;
		sum1 = 0;
		cnt0 = 0;
		cnt1 = 0;
		w0 = 0;
		w1 = 0;

		for( int j = 0; j < i; j++)
		{
			cnt0 += static_cast<long>(histogram[j]);
			sum0 += static_cast<long>(j * histogram[j]);
		}

		u0 = (double)sum0 / cnt0;
		w0 = (double)cnt0 / size;

		for(int j = i ; j <= 255; j++)
		{
			cnt1 +=static_cast<long>(histogram[j]);
			sum1 += static_cast<long>(j * histogram[j]);
		}

		u1 = (double)sum1 / cnt1;

		w1 = 1 - w0;					// (double)cnt1 / size;

		//u = u0 * w0 + u1 * w1;			// Image average values

		variance =  w0 * w1 *  (u0 - u1) * (u0 - u1);

		if(variance > maxVariance)
		{
			maxVariance = variance;
			liminal = i;
		}
	}
    DEBUG_PRINT("liminal = %d\r\n", liminal);
	return 0;
}

int Pretreatment::Binaryzation(const Mat &grayMat, Mat &binaryMat, const int liminal)
{
    threshold(grayMat, binaryMat, liminal, 255.0, THRESH_BINARY);
    return 0;
}

int Pretreatment::Bluring(const Mat &grayMat, Mat &blurMat)
{
    GaussianBlur(grayMat, blurMat, Size(15,15), 0, 0 );
    return 0;
}

int Pretreatment::Filling(Mat &binaryMat)
{
    Mat tempMat;
	binaryMat.copyTo(tempMat);
	floodFill(binaryMat, Point(0, 0), Scalar(255));
	binaryMat = ~binaryMat;
	binaryMat = tempMat | binaryMat;
	return 0;
}

int Pretreatment::Reconstruction(const Mat &binaryMat, Mat &morphologyMat)
{
	Mat element = getStructuringElement( MORPH_RECT, Size(9,9) );
	morphologyEx( binaryMat, morphologyMat,	MORPH_OPEN, element, Point(-1,-1), 1 );
	return 0;
}

int Pretreatment::RemoveReflective(const Mat &morphologyMat, Mat &opaqueMat)
{
    vector< vector<Point> > contours;
	findContours(morphologyMat, contours, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	double *SumOfArea = new double[contours.size()];    // Contours' area.
    Moments *moment = new Moments[contours.size()];     // Contours' moments.
    Point *central  = new Point[contours.size()];       // Contours' central point.

	for(unsigned int i = 0; i < contours.size(); i++)
    {
        SumOfArea[i] = contourArea(contours[i], false);
        moment[i] = moments(contours[i], false);
		central[i] = Point(static_cast<int>(moment[i].m10 / moment[i].m00), static_cast<int>( moment[i].m01 / moment[i].m00));
    }

    double stv = Utility::Instance()->StandardDeviation(SumOfArea, contours.size());

    int Neighborhood;
    double AreaDiff, centralDiff;
    double DistancePenalty;
    Mat tempMat(morphologyMat.rows, morphologyMat.cols, CV_8UC1, Scalar(255));
    for(unsigned int i = 0; i < contours.size(); i++)
    {
        Neighborhood = Utility::Instance()->NearestNeighborhoodPoint(central, contours.size(), central[i]);
        printf("Neighborhood %d\r\n", Neighborhood);
        AreaDiff = SumOfArea[i] - SumOfArea[Neighborhood];
        centralDiff = Utility::Instance()->EuclideanDistance(central[i], central[Neighborhood]);

        if( (abs(AreaDiff)-stv) < (SumOfArea[i] * ((abs(central[i].y-central[Neighborhood].y)/centralDiff)+1)))
        {
            if(centralDiff/DistancePenalty < 2.5 || i == 0)
            {
                drawContours(tempMat, contours, i, Scalar(0), 1 );
                DistancePenalty = centralDiff;
            }
        }
    }

	floodFill(tempMat, Point(0, 0), Scalar(0));
	tempMat.copyTo(opaqueMat);
	return 0;
}

int Pretreatment::Marker(const Mat &opaqueMat, Mat &dstMat)
{
    vector< vector<Point> > contours;
	findContours(opaqueMat, contours, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    Moments *moment = new Moments[contours.size()];     // Contours' moments.
    Point *central  = new Point[contours.size()];       // Contours' central point.

    for(unsigned int i = 0; i < contours.size(); i++)
    {
        moment[i] = moments(contours[i], false);
		central[i] = Point(static_cast<int>(moment[i].m10 / moment[i].m00), static_cast<int>( moment[i].m01 / moment[i].m00));
    }

    for( unsigned i = 0; i < contours.size(); i++ )
	{
	    line(
			dstMat,
			Point(central[i].x, central[i].y - 20),
			Point(central[i].x, central[i].y + 20),
			Scalar( 0, 0, 255 ),
			1,
			8
		);

		line(
			dstMat,
			Point( central[i].x - 20, central[i].y ),
			Point( central[i].x + 20, central[i].y ),
			Scalar( 0, 0, 255 ),
			1,
			8
		);
    }
    return 0;
}
