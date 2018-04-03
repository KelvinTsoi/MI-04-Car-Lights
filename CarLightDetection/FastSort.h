#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "GeneralModule.h"

#ifndef _FASTSORT_H_
#define _FASTSORT_H_

class FastSort
{
	public:
		static FastSort* Instance();
		int Process(double Array[], int ArraySize);
	protected:
		FastSort();
	private:
	    int Partition(double Array[], int low, int high);
        void Sort(double Array[], int low, int high);
        void Swap(double& left, double& right);
		static FastSort* _instance;
};

#endif
