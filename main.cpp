#include <iostream>

#include "image_processing.h"

void main()
{
    try { 
        cv::Ptr< IplImage > gray = ::cvLoadImage( "lena.bmp", CV_LOAD_IMAGE_GRAYSCALE );
        if ( gray == 0 ) {
            throw std::runtime_error("error : cvLoadImage");
        }

        // 表示する画像データの作成
        cv::Ptr< IplImage > show = ::cvCreateImage( cvSize(gray->width, gray->height), gray->depth, gray->nChannels );
        if ( show == 0 ) {
            throw std::runtime_error("error : cvCreateImage");
        }

        ::cvShowImage( "cv", gray );
        while ( 1 ) {
            int key = ::cvWaitKey( 10 );
            if ( (key == 'q') || (key == 13) ) {
                break;
            }
            // もとにもどす
            else if ( key == 'u' ) {
                ::cvShowImage( "cv", gray );
            }
            // 閾値処理
            else if ( key == 't' ) {
                ::threshold( gray, show, 16 );
                ::cvShowImage( "cv", show );
            }
            // ヒストグラムの表示
            else if ( key == 'h' ) {
                long hist[HIST_LEN] = { 0 };
                ::histgram( gray, hist );
                ::hist_print( hist );

                ::hist_image( hist, show );
                ::cvShowImage( "cv", show );
            }
            // ヒストグラムの表示(平滑化)
            else if ( key == 's' ) {
                long hist[HIST_LEN] = { 0 };
                long hist_out[HIST_LEN] = { 0 };
                ::histgram( gray, hist );

                ::hist_smooth( hist, hist_out );
                ::hist_image( hist_out, show );
                ::cvShowImage( "cv", show );
            }
            // モード法による閾値処理
            else if ( key == 'm' ) {
                ::thresh_mode( gray, show, 3, 1 );
                ::cvShowImage( "cv", show );
            }
            // 判別分析法による閾値処理
            else if ( key == 'd' ) {
                ::thresh_discrim( gray, show, 2 );
                ::cvShowImage( "cv", show );
            }
            // 動的閾値法
            else if ( key == 'y' ) {
                ::thresh_dynamic( gray, show, 1 );
                ::cvShowImage( "cv", show );
            }
            // 画像の一次微分(通常の差分)
            else if ( key == 'f' ) {
                ::gradient_difference( gray, show, 1 );
                ::cvShowImage( "cv", show );
            }
            // 画像の一次微分(Robertsオペレーター)
            else if ( key == 'r' ) {
                ::gradient_roberts( gray, show, 1 );
                ::cvShowImage( "cv", show );
            }
            // 画像の一次微分(Sobelオペレーター)
            else if ( key == 'o' ) {
                ::gradient_sobel( gray, show, 1 );
                ::cvShowImage( "cv", show );
            }
        }
    }
    catch ( std::exception& ex ) {
        std::cout << ex.what() << std::endl;
    }
}
