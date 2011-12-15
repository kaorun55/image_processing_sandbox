#include <iostream>
#include <opencv2\opencv.hpp>

#define X_SIZE  256
#define Y_SIZE  256
#define X_EXP   8
#define Y_EXP   8
#define HIGH    255
#define LOW     0
#define OFFSET  128
#define TEXT_BUF_LEN    10000

void threshold( const cv::Ptr< IplImage >& in, cv::Ptr< IplImage >& out, int thresh )
{
    for ( int y = 0; y < in->width; ++y ) {
        for ( int x = 0; x < in->height; ++x ) {
            int index = (y * in->width) + x;
            out->imageData[index] = (in->imageData[index] <= thresh) ? HIGH : LOW;
        }
    }
}

void main()
{
    try { 
        cv::Ptr< IplImage > image = ::cvLoadImage( "lena.bmp", CV_LOAD_IMAGE_GRAYSCALE );
        if ( image == 0 ) {
            throw std::runtime_error("error : cvLoadImage");
        }

        // 表示する画像データの作成
        cv::Ptr< IplImage > show = ::cvCreateImage( cvSize(image->width, image->height), image->depth, image->nChannels );
        if ( show == 0 ) {
            throw std::runtime_error("error : cvCreateImage");
        }

        ::threshold( image, show, 16 );

        ::cvShowImage( "cv", show );
        ::cvWaitKey();
    }
    catch ( std::exception& ex ) {
        std::cout << ex.what() << std::endl;
    }
}
