#include "image_processing.h"

// 閾値処理
void threshold( const cv::Ptr< IplImage >& in, cv::Ptr< IplImage >& out, int thresh, int type /*= 2*/ )
{
    for ( int y = 0; y < in->width; ++y ) {
        for ( int x = 0; x < in->height; ++x ) {
            int index = (y * in->width) + x;
            if ( type == 2 ) {
                out->imageData[index] = (in->imageData[index] <= thresh) ? HIGH : LOW;
            }
            else {
                out->imageData[index] = (in->imageData[index] >= thresh) ? HIGH : LOW;
            }
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

// ヒストグラムを平滑化(へいかつか)する
void hist_smooth( long hist_in[HIST_LEN], long hist_out[HIST_LEN] )
{
    for ( int i = 0; i < HIST_LEN; ++i ) {
        int sum = 0;
        for ( int j = -2; j <= 2; ++j ) {
            int index = i + j;
            if ( index < 0 ) 
                index = 0;

            if ( index > 255 )
                index = 255;

            sum += hist_in[index];
        }

        hist_out[i] = (long)((double)sum / 5.0 + 0.5);
    }
}


// モード法による閾値処理
int calc_mode_thresh( long hist[HIST_LEN] )
{
    long max = 0;
    int i = 0;
    for ( i = 0; i < HIST_LEN; ++i ) {
        if ( max <= hist[i] ) {
            max = hist[i];
        }
        else {
            break;
        }
    }

    long min = max;
    int j = 0;
    for ( j = i; j < HIST_LEN; ++j ) {
        if ( min >= hist[j] ) {
            min = hist[j];
        }
        else {
            break;
        }
    }

    return j - 1;
}

void thresh_mode( cv::Ptr< IplImage >& in, cv::Ptr< IplImage >& out, int smt, int type )
{
    long hist1[HIST_LEN] = { 0 };
    long hist2[HIST_LEN] = { 0 };
    ::histgram( in, hist1 );
    for ( int i = 0; i < smt; ++i ) {
        memcpy( hist2, hist1, sizeof(hist2) );

        ::hist_smooth( hist2, hist1 );
    }

    int thresh = ::calc_mode_thresh( hist1 );

    threshold( in, out, thresh, type );
}


// 判別分析法による閾値処理
int calc_discrim_thresh( long hist[HIST_LEN], double disparoty )
{
    int i, k;
    double n0, n1, n2, m0, m1, m2;
    double v[HIST_LEN], vmax, v0;

    n0 = 0.0;
    m0 = 0.0;
    for ( i = 0; i < HIST_LEN; ++i ) {
        n0 += hist[i];
        m0 += i * hist[i];
    }

    m0 = (n0 == 0.0) ? 0.0 : (m0 / n0);

    v0 = 0.0;
    for ( i = 0; i < HIST_LEN; ++i ) {
        v0 += (hist[i] * (i - m0) * (i - m0)) / n0;
    }

    for ( k = 0; k < HIST_LEN; ++k ) {
        n1 = 0.0;
        m1 = 0.0;
        for ( i = 0; i < k; ++i ) {
            n1 += hist[i];
            m1 += i * hist[i];
        }

        m1 = (n1 == 0.0) ? 0.0 : (m1 / n1);

        n2 = 0.0;
        m2 = 0.0;
        for ( i = 0; i < HIST_LEN; ++i ) {
            n2 += hist[i];
            m2 += i * hist[i];
        }

        m2 = (n2 == 0.0) ? 0.0 : (m2 / n2);

        v[k] = ((n1 * (m1 - m0) * (m1 - m0)) + (n2 * (m2 - m0) * (m2 - m0))) / n0;
    }

    vmax = 0.0;
    for ( i = 0; i < HIST_LEN; ++i ) {
        if ( vmax <= v[i] ) {
            vmax = v[i];
            k = i;
        }
    }

    if ( v0 == 0 ) {
        return 0;
    }

    if ( (vmax / v0) >= disparoty ) {
        return k;
    }

    return 0;
}


void thresh_discrim( cv::Ptr< IplImage >& in, cv::Ptr< IplImage >& out, int type )
{
    long hist[HIST_LEN] = { 0 };
    ::histgram( in, hist );

    int thresh = ::calc_discrim_thresh( hist, 0.0 );

    threshold( in, out, thresh, type );
}
