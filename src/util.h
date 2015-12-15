#pragma once
#ifndef idAEB0C3B2_F010_4C35_BF12C7A3650D5121
#define idAEB0C3B2_F010_4C35_BF12C7A3650D5121

#include <QtWidgets>
#include <opencv2\core.hpp>

QImage MatToQImage(const cv::Mat& mat);

QImage cvMatToQImage( const cv::Mat &inMat );

QPixmap cvMatToQPixmap( const cv::Mat &inMat );

cv::Mat QImageToCvMat( const QImage &inImage, bool inCloneImageData = true );

cv::Mat QPixmapToCvMat( const QPixmap &inPixmap, bool inCloneImageData = true );

#endif // header
