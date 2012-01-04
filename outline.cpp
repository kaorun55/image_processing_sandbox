#include "image_processing.h"

unsigned char get_pixel( const cv::Ptr< IplImage >& in, int y, int x )
{
    return in->imageData[y * in->widthStep + x];
}

void set_pixel( cv::Ptr< IplImage >& out, int y, int x, unsigned char pixel )
{
    out->imageData[y * out->widthStep + x] = pixel;
}


// 一次微分による輪郭抽出
// 画像の一次微分
void gradient( const cv::Ptr< IplImage >& in, cv::Ptr< IplImage >& out, double amp, int cx[9], int cy[9] )
{
    int d[9];
    int i, j, dat;
    double xx, yy;

    for ( i = 1; i < Y_SIZE - 1; ++i ) {
        for ( j = 1; j < X_SIZE - 1; ++j ) {
            d[0] = get_pixel( in, i - 1, j - 1 );
            d[1] = get_pixel( in, i - 1, j );
            d[2] = get_pixel( in, i - 1, j + 1 );
            d[3] = get_pixel( in, i,     j - 1 );
            d[4] = get_pixel( in, i,     j );
            d[5] = get_pixel( in, i,     j + 1 );
            d[6] = get_pixel( in, i + 1, j - 1 );
            d[7] = get_pixel( in, i + 1, j );
            d[8] = get_pixel( in, i + 1, j + 1 );

            xx = 0.0;
            for ( int k = 0; k < 9; ++k ) {
                xx += cx[k] * d[k];
            }

            yy = 0.0;
            for ( int k = 0; k < 9; ++k ) {
                yy += cy[k] * d[k];
            }

            dat = (int)(amp * sqrt( xx * xx + yy * yy ));
            set_pixel( out, i, j, (dat > 255) ? 255 : dat );
        }
    }
}

// 画像の一次微分(通常の差分)
int cx_difference[9] = {
    0, 0, 0, 
    0,-1, 1, 
    0, 0, 0, 
};

int cy_difference[9] = {
    0, 0, 0, 
    0,-1, 0, 
    0, 1, 0, 
};

void gradient_difference( const cv::Ptr< IplImage >& in, cv::Ptr< IplImage >& out, double amp )
{
    gradient( in, out, amp, cx_difference, cy_difference );
}

// 画像の一次微分(Robertsオペレーター)
int cx_roberts[9] = {
    0, 0, 0, 
    0,-1, 0, 
    0, 0, 1, 
};

int cy_roberts[9] = {
    0, 0, 0, 
    0, 0,-1, 
    0, 1, 0, 
};

void gradient_roberts( const cv::Ptr< IplImage >& in, cv::Ptr< IplImage >& out, double amp )
{
    gradient( in, out, amp, cx_roberts, cy_roberts );
}

// 画像の一次微分(Sobelオペレーター)
int cx_sobel[9] = {
   -1, 0, 1, 
   -2, 0, 2, 
   -1, 0, 1, 
};

int cy_sobel[9] = {
   -1,-2,-1, 
    0, 0, 0, 
    1, 2, 1, 
};

void gradient_sobel( const cv::Ptr< IplImage >& in, cv::Ptr< IplImage >& out, double amp )
{
    gradient( in, out, amp, cx_sobel, cy_sobel );
}



// Prewittの方法による輪郭抽出
void prewitt( const cv::Ptr< IplImage >& in, cv::Ptr< IplImage >& out, double amp )
{
    int d0, d1, d2, d3, d4, d5, d6, d7, d8;
    int i, j, k, max, dat;
    int m[8];
    double zz;

    for ( i = 1; i < Y_SIZE - 1; ++i ) {
        for ( j = 1; j < X_SIZE - 1; ++j ) {
            d0 = get_pixel( in, i - 1, j - 1 );
            d1 = get_pixel( in, i - 1, j );
            d2 = get_pixel( in, i - 1, j + 1 );
            d3 = get_pixel( in, i,     j - 1 );
            d4 = get_pixel( in, i,     j );
            d5 = get_pixel( in, i,     j + 1 );
            d6 = get_pixel( in, i + 1, j - 1 );
            d7 = get_pixel( in, i + 1, j );
            d8 = get_pixel( in, i + 1, j + 1 );

            m[0] =  d0 + d1 + d2 + d3 -2*d4 + d5 - d6 - d7 - d8;
            m[1] =  d0 + d1 + d2 + d3 -2*d4 - d5 + d6 - d7 - d8;
            m[2] =  d0 + d1 - d2 + d3 -2*d4 - d5 + d6 + d7 - d8;
            m[3] =  d0 - d1 - d2 + d3 -2*d4 - d5 + d6 + d7 + d8;
            m[4] = -d0 - d1 - d2 + d3 -2*d4 + d5 + d6 + d7 + d8;
            m[5] = -d0 - d1 + d2 - d3 -2*d4 + d5 + d6 + d7 + d8;
            m[6] = -d0 + d1 + d2 - d3 -2*d4 + d5 - d6 + d7 + d8;
            m[7] =  d0 + d1 + d2 - d3 -2*d4 + d5 - d6 - d7 + d8;

            max = 0;

            for ( k = 0; k < 9; ++k ) {
                if ( max < m[k] ) {
                    max = m[k];
                }
            }

            dat = amp * max;
            set_pixel( out, i, j, (dat > 255) ? 255 : dat );
        }
    }
}

// 連結数(8近傍連結)を調べる
int ncon( int p[9] )
{
    int i, i1, i2;
    int q[9];
    int n = 0;

    for ( i = 0; i < 9; ++i ) {
        q[i] = ((p[i] == 1) || (p[i] == -1)) ? 0 : 1;
    }

    for ( i = 1; i < 9; i += 2 ){
        i1 = i + 1;
        i2 = i + 2;
        if ( i2 == 9 ) {
            i2 = 1;
        }

        n = n + q[i] - (q[i] * q[i1] * q[i2]);
    }

    return n;
}


// 2値画像の細線化
// なんか出力が違う気がする...
// なんかあってるポイ
#define TMP 128

void thinning( const cv::Ptr< IplImage >& in, cv::Ptr< IplImage >& out )
{
    int flg = 1;
    int i, j, k, n;
    int p[9];   // 図形:1, 背景:0, 背景候補:-1

    ::cvCopy( in, out );

    while ( flg != 0 ) {
        flg = 0;

        for ( i = 1; i < Y_SIZE - 1; ++i ) {
            for ( j = 1; j < X_SIZE - 1; ++j ) {
                p[0] = get_pixel( out, i    , j );
                p[1] = get_pixel( out, i    , j + 1 );
                p[2] = get_pixel( out, i - 1, j + 1 );
                p[3] = get_pixel( out, i - 1, j );
                p[4] = get_pixel( out, i - 1, j - 1 );
                p[5] = get_pixel( out, i,     j - 1 );
                p[6] = get_pixel( out, i + 1, j - 1 );
                p[7] = get_pixel( out, i + 1, j );
                p[8] = get_pixel( out, i + 1, j + 1 );

                for ( k = 0; k < 9; ++k ) {
                    if (      p[k] == HIGH ) p[k] =  1;
                    else if ( p[k] == LOW )  p[k] =  0;
                    else                     p[k] = -1;
                }

                // 条件1：図形の一部である
                if ( p[0] != 1 )
                    continue;

                // 条件2：境界要素である(4近傍のうち、1個以上が背景)
                if ( (p[1] * p[3] * p[5] * p[7]) != 0 )
                    continue;

                // 条件3：端点を保存する(8近傍のうち、2個以上が図形)
                n = 0;
                for ( k = 1; k < 9; ++k ) {
                    if ( p[k] != 0 ) {
                        n++;
                    }
                }

                if ( n < 2 )
                    continue;

                // 条件4：孤立点を保存する(8近傍のうち、1個以上が図形)
                n = 0;
                for ( k = 1; k < 9; ++k ) {
                    if ( p[k] == 1 ) {
                        n++;
                    }
                }

                if ( n < 1 )
                    continue;

                // 条件5：連結性を保存する(8連結数が1である)
                if ( ncon( p ) != 1 )
                    continue;

                // 条件6：線幅が2のとき片方だけ除去する(8近傍すべてにおいて、-1でないか、
                // -1のときこの値を0とした場合の8連結数が1である)
                n = 0;
                for ( k = 1; k < 9; ++k ) {
                    if ( p[k] != -1 ) {
                        n++;
                    }
                    else if ( p[k] == -1 ) {
                        p[k] = 0;
                        if ( ncon( p ) == 1 ) {
                            n++;
                        }

                        p[k] = -1;
                    }
                }

                if ( n < 8 )
                    continue;

                // 条件1～6をすべて満たす場合が削除対象
                ::set_pixel( out, i, j, TMP );
                flg++;
            }
        }

        for ( i = 1; i < Y_SIZE - 1; ++i ) {
            for ( j = 1; j < X_SIZE - 1; ++j ) {
                if ( get_pixel( out, i, j ) == TMP ) {
                    set_pixel( out, i, j, LOW );
                }
            }
        }
    }
}



