#include <iostream>

#include "image_processing.h"

void main()
{
    try { 
        cv::Ptr< IplImage > image = ::cvLoadImage( "lena.bmp", CV_LOAD_IMAGE_GRAYSCALE );
        if ( image == 0 ) {
            throw std::runtime_error("error : cvLoadImage");
        }

        // �\������摜�f�[�^�̍쐬
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
            // ���Ƃɂ��ǂ�
            else if ( key == 'u' ) {
                ::cvShowImage( "cv", image );
            }
            // 臒l����
            else if ( key == 't' ) {
                ::threshold( image, show, 16 );
                ::cvShowImage( "cv", show );
            }
            // �q�X�g�O�����̕\��
            else if ( key == 'h' ) {
                long hist[HIST_LEN] = { 0 };
                ::histgram( image, hist );
                ::hist_print( hist );

                ::hist_image( hist, show );
                ::cvShowImage( "cv", show );
            }
            // �q�X�g�O�����̕\��(������)
            else if ( key == 's' ) {
                long hist[HIST_LEN] = { 0 };
                long hist_out[HIST_LEN] = { 0 };
                ::histgram( image, hist );

                ::hist_smooth( hist, hist_out );
                ::hist_image( hist_out, show );
                ::cvShowImage( "cv", show );
            }
            // ���[�h�@�ɂ��臒l����
            else if ( key == 'm' ) {
                ::thresh_mode( image, show, 3, 1 );
                ::cvShowImage( "cv", show );
            }
            // ���ʕ��͖@�ɂ��臒l����
            else if ( key == 'd' ) {
                ::thresh_discrim( image, show, 2 );
                ::cvShowImage( "cv", show );
            }
            // ���I臒l�@
            else if ( key == 'y' ) {
                ::thresh_dynamic( image, show, 1 );
                ::cvShowImage( "cv", show );
            }
        }
    }
    catch ( std::exception& ex ) {
        std::cout << ex.what() << std::endl;
    }
}
