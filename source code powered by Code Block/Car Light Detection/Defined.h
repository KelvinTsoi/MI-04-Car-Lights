#ifndef _DEFINED_H_
#define _DEFINED_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <vector>

#include<opencv.hpp>
#include<highgui.hpp>
#include<imgproc.hpp>

#define SOFTWARE_VERSION "1.0.01"

#define DEBUG_FILE "/tmp/DebugData.log"
#define DEBUG_BUFFER_MAX 4096

#define PRINT_DATALOG(format, ...) \
{\
    char buffer[DEBUG_BUFFER_MAX+1]={0};\
    snprintf( buffer, DEBUG_BUFFER_MAX \
         , ""format"", ##__VA_ARGS__);\
    FILE* fd = fopen(DEBUG_FILE, "a");\
    if ( fd != NULL ) \
    {\
        fwrite( buffer, strlen(buffer), 1, fd );\
        fflush( fd );\
        fclose( fd );\
    }\
}\

#define DEBUG_PRINT(format, ...) \
{\
    printf("[%s %s %d] "format"", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
}\

using namespace cv;
using namespace std;

#endif
