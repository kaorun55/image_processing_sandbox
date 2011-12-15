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
#define HIST_LEN    256

// 閾値処理
void threshold( const cv::Ptr< IplImage >& in, cv::Ptr< IplImage >& out, int thresh )
{
    for ( int y = 0; y < in->width; ++y ) {
        for ( int x = 0; x < in->height; ++x ) {
            int index = (y * in->width) + x;
            out->imageData[index] = (in->imageData[index] <= thresh) ? HIGH : LOW;
        }
    }
}

// ヒストグラムを求める
void histgram( const cv::Ptr< IplImage >& in, long hist[HIST_LEN] )
{
    memset( hist, 0, sizeof(hist) );

    for ( int i = 0; i < (in->height * in->width); ++i ) {
        ++hist[(unsigned char)in->imageData[i]];
    }
}

// ヒストグラムを数値とグラフで印字する
void hist_print( const long hist[HIST_LEN] )
{
    double max = 0;
    double p = X_SIZE * Y_SIZE;

    for ( int i = 0; i < HIST_LEN; ++i ) {
        if ( hist[i] > max ) {
            max = hist[i];
        }
    }

    for ( int i = 0; i < HIST_LEN; ++i ) {
        double q = hist[i] / p * 100.0;
        printf( "%3d:%5.1f%%|", i, q );

        int k = (int)(hist[i] / max * 60.0);
        for ( int j = 0; j < k; ++j ) {
            printf( "*" );
        }
        printf( "\n" );
    }
}

// ヒストグラムを画像化する
void hist_image( long hist[HIST_LEN], cv::Ptr< IplImage >& out )
{
    memset( out->imageData, LOW, out->imageSize );

    int max = 0;
    for ( int i = 0; i < HIST_LEN; ++i ) {
        if ( hist[i] > max ) {
            max = hist[i];
        }
    }

    int range = out->height - 5;
    for ( int x = 0; x < out->width; ++x ) {
        long n = (long)(hist[x] / (double)max * range);
        for ( int y = 0; y < n; ++y ) {
            int index = ((range - y) * out->width) + x;
            out->imageData[index] = HIGH;
        }
    }

    for ( int i = 0; i < 4; ++i ) {
        int x = 64 * i;
        if ( x >= out->width ) {
            x = out->width - 1;
        }

        for ( int y = range; y < out->height; ++y ) {
            int index = (y * out->width) + x;
            out->imageData[index] = HIGH;
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

        ::cvShowImage( "cv", image );
        while ( 1 ) {
            int key = ::cvWaitKey( 10 );
            if ( (key == 'q') || (key == 13) ) {
                break;
            }
            // もとにもどす
            else if ( key == 'u' ) {
                ::cvShowImage( "cv", image );
            }
            // 閾値処理
            else if ( key == 't' ) {
                ::threshold( image, show, 16 );
                ::cvShowImage( "cv", show );
            }
            // ヒストグラムの表示
            else if ( key == 'h' ) {
                long hist[HIST_LEN] = { 0 };
                ::histgram( image, hist );
                ::hist_print( hist );

                ::hist_image( hist, show );
                ::cvShowImage( "cv", show );
            }
        }
    }
    catch ( std::exception& ex ) {
        std::cout << ex.what() << std::endl;
    }
}
