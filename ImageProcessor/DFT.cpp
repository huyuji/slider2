#include "opencv2/imgproc/imgproc.hpp"

#include "DFT.h"
#include "Const.h"

using namespace ImageProcessor;
using namespace cv;

namespace
{
    template<class T> void fftShift(T *out, const T* in, size_t nx, size_t ny)
    {
        size_t xoffset = nx/2;
        size_t yoffset = ny/2;
        for(size_t x = 0; x < nx; x++)
        {
            for(size_t y = 0; y < ny; y++)
            {
                size_t r = (x + xoffset) % nx;
                size_t c = (y + yoffset) % ny;
                out[r * ny + c] = in[x * ny + y];
            }
        }
    }

    template<class T> void ifftShift(T *out, const T* in, size_t nx, size_t ny)
    {
        size_t xoffset = nx/2;
        size_t yoffset = ny/2;
        for(size_t x = 0; x < nx; x++)
        {
            for(size_t y = 0; y < ny; y++)
            {
                size_t r = (x - xoffset + nx) % nx;
                size_t c = (y - yoffset + ny) % ny;
                out[r * ny + c] = in[x * ny + y];
            }
        }
    }

    template<class Tfrom, class Tto> Mat convert(const Mat& in, int Totype)
    {
        Mat out = cv::Mat::zeros(in.rows, in.cols, Totype);
        for(int r = 0; r < in.rows; ++r)
        {
            for(int c = 0; c < in.cols; ++c)
            {
                out.at<Tto>(r,c) = (Tto) in.at<Tfrom>( r,c );
            }
        }

        return out;
    }
}

DFT::DFT(const boost::property_tree::ptree& params)
    : m_kmask(params.get<double>(Const::kmask)),
      m_rmax(params.get<double>(Const::rmax)),
      m_rmin(params.get<double>(Const::rmin))
{
    denormalizeParams();
}

void DFT::denormalizeParams()
{

}

cv::Mat DFT::operator()(const cv::Mat& image)
{
    unsigned int rows = image.rows;
    unsigned int cols = image.cols;

    Mat floatMat = convert<unsigned char, float>(image, CV_32FC1);

    Mat complexMat1;
    dft(floatMat, complexMat1, DFT_COMPLEX_OUTPUT, floatMat.rows);

    Mat complexMat2 = Mat::ones(rows, cols, CV_32FC2);
    fftShift((Vec2f*)complexMat2.data, (Vec2f*)complexMat1.data, rows, cols);

    applyMask(complexMat2, getMask(rows, cols));

    ifftShift((Vec2f*)complexMat1.data, (Vec2f*)complexMat2.data, rows, cols);

    idft(complexMat1, complexMat2);

    Mat mats[2];
    split(complexMat2, mats);

    magnitude(mats[0], mats[1], floatMat);

    Mat output;
    cv::normalize(floatMat, output, 0, 255, NORM_MINMAX, CV_8UC1);

    return output;
}

Mat DFT::getMask(unsigned int rows, unsigned int cols)
{
    Mat mask = Mat::ones(rows, cols, CV_32FC2);
    for(unsigned int i = 0; i < rows; i++)
    {
        for(unsigned int j = 0; j < cols; j++)
        {
            (mask.at<Vec2f>(i, j))[0] = m_rmax;
            (mask.at<Vec2f>(i, j))[1] = m_rmax;
        }
    }

    double nmask = std::pow((double)m_kmask, (double)3) / (m_rmax - m_rmin);
    int crow = rows/2;
    int ccol = cols/2;
    for(int i = -1 * m_kmask; i < m_kmask + 1; i++)
    {
        for(int j = -1 * m_kmask; j < m_kmask + 1; j++)
        {
            double ir = std::sqrt((double)(i * i + j * j));
            if(ir <= m_kmask)
            {
                float value = -1 * std::pow((-1 * std::abs(ir) + m_kmask), (double)3) / nmask + m_rmax;
                (mask.at<Vec2f>(crow + i, ccol + j))[0] = value;
                (mask.at<Vec2f>(crow + i, ccol + j))[1] = value;
            }
        }
    }

    return mask;
}

void DFT::applyMask(Mat& mat, const Mat& mask)
{
    for(int i = 0; i < mat.rows; i++)
    {
        for(int j = 0; j < mat.cols; j++)
        {
            Vec2f& temp = mat.at<Vec2f>(i, j);
            temp[0] *= (mask.at<Vec2f>(i, j))[0];
            temp[1] *= (mask.at<Vec2f>(i, j))[1];
        }
    }
}
