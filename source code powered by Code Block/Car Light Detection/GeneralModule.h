#ifndef _GENERALMODULE_H_
#define _GENERALMODULE_H_

#include "Defined.h"

class GeneralModule
{
	public:
		static GeneralModule* Instance();
		Mat LoadImage(const char* path);
		int ShowImage(const Mat& img) const;
        void StartCounting();
		void StopCounting();
	protected:
		GeneralModule();
	private:
		struct timeval tv;
		unsigned long long originalMilliSecond;
		unsigned long long terminalMilliSecond;
		static GeneralModule* _instance;
};

#endif
