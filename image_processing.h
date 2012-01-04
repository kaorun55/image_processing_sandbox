#pragma once

#include <opencv2\opencv.hpp>

#define X_SIZE  256
#define Y_SIZE  256
#define X_EXP   8
#define Y_EXP   8
#define HIGH    255
#define LOW     0
#define OFFSET  128
#define TEXT_BUF_LEN    10000
#define HIST_LEN    256

// 臒l
void threshold( const cv::Ptr< IplImage >& in, cv::Ptr< IplImage >& out, int thresh, int type = 2 );
void histgram( const cv::Ptr< IplImage >& in, long hist[HIST_LEN] );
void hist_print( const long hist[HIST_LEN] );
void hist_image( long hist[HIST_LEN], cv::Ptr< IplImage >& out );
void hist_smooth( long hist_in[HIST_LEN], long hist_out[HIST_LEN] );

void thresh_mode( cv::Ptr< IplImage >& in, cv::Ptr< IplImage >& out, int smt, int type );
void thresh_discrim( cv::Ptr< IplImage >& in, cv::Ptr< IplImage >& out, int type );
void thresh_dynamic( cv::Ptr< IplImage >& in, cv::Ptr< IplImage >& out, int type );

// �֊s
void gradient_difference( const cv::Ptr< IplImage >& in, cv::Ptr< IplImage >& out, double amp );
void gradient_roberts( const cv::Ptr< IplImage >& in, cv::Ptr< IplImage >& out, double amp );
void gradient_sobel( const cv::Ptr< IplImage >& in, cv::Ptr< IplImage >& out, double amp );
