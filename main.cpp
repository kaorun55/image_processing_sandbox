#include <iostream>

#include "image_processing.h"

void main()
{
    try { 
        cv::Ptr< IplImage > gray = ::cvLoadImage( "lena.bmp", CV_LOAD_IMAGE_GRAYSCALE );
        if ( gray == 0 ) {
            throw std::runtime_error("error : cvLoadImage");
        }

        // �\������摜�f�[�^�̍쐬
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
            // ���Ƃɂ��ǂ�
            else if ( key == 'u' ) {
                ::cvShowImage( "cv", gray );
            }
            // 臒l����
            else if ( key == 't' ) {
                ::threshold( gray, show, 16 );
                ::cvShowImage( "cv", show );
            }
            // �q�X�g�O�����̕\��
            else if ( key == 'h' ) {
                long hist[HIST_LEN] = { 0 };
                ::histgram( gray, hist );
                ::hist_print( hist );

                ::hist_image( hist, show );
                ::cvShowImage( "cv", show );
            }
            // �q�X�g�O�����̕\��(������)
            else if ( key == 's' ) {
                long hist[HIST_LEN] = { 0 };
                long hist_out[HIST_LEN] = { 0 };
                ::histgram( gray, hist );

                ::hist_smooth( hist, hist_out );
                ::hist_image( hist_out, show );
                ::cvShowImage( "cv", show );
            }
            // ���[�h�@�ɂ��臒l����
            else if ( key == 'm' ) {
                ::thresh_mode( gray, show, 3, 1 );
                ::cvShowImage( "cv", show );
            }
            // ���ʕ��͖@�ɂ��臒l����
            else if ( key == 'd' ) {
                ::thresh_discrim( gray, show, 2 );
                ::cvShowImage( "cv", show );
            }
            // ���I臒l�@
            else if ( key == 'y' ) {
                ::thresh_dynamic( gray, show, 1 );
                ::cvShowImage( "cv", show );
            }
            // �摜�̈ꎟ����(�ʏ�̍���)
            else if ( key == 'f' ) {
                ::gradient_difference( gray, show, 1 );
                ::cvShowImage( "cv", show );
            }
            // �摜�̈ꎟ����(Roberts�I�y���[�^�[)
            else if ( key == 'r' ) {
                ::gradient_roberts( gray, show, 1 );
                ::cvShowImage( "cv", show );
            }
            // �摜�̈ꎟ����(Sobel�I�y���[�^�[)
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
