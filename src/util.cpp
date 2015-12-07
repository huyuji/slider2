#include "util.h"
#include <qdebug.h>

QImage MatToQImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS=1
    if(mat.type()==CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    if(mat.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else
    {
        return QImage();
    }
}

QImage cvMatToQImage( const cv::Mat &inMat )
{
    switch ( inMat.type() )
    {
        // 8-bit, 4 channel
        case CV_8UC4:
        {
        QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB32 );
 
        return image;
        }
 
        // 8-bit, 3 channel
        case CV_8UC3:
        {
        QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888 );
 
        return image.rgbSwapped();
        }
 
        // 8-bit, 1 channel
        case CV_8UC1:
        {
        static QVector<QRgb>  sColorTable;
 
        // only create our color table once
        if ( sColorTable.isEmpty() )
        {
            for ( int i = 0; i < 256; ++i )
                sColorTable.push_back( qRgb( i, i, i ) );
        }
 
        QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_Indexed8 );
 
        image.setColorTable( sColorTable );
 
        return image;
        }
 
        default:
        break;
    }
 
    return QImage();
}
 
QPixmap cvMatToQPixmap( const cv::Mat &inMat )
{
    return QPixmap::fromImage( cvMatToQImage( inMat ) );
}

cv::Mat QImageToCvMat( const QImage &inImage, bool inCloneImageData )
{
    switch ( inImage.format() )
    {
        // 8-bit, 4 channel
        case QImage::Format_RGB32:
        {
        cv::Mat  mat( inImage.height(), inImage.width(), CV_8UC4, const_cast<uchar*>(inImage.bits()), inImage.bytesPerLine() );
 
        return (inCloneImageData ? mat.clone() : mat);
        }
 
        // 8-bit, 3 channel
        case QImage::Format_RGB888:
        {
        if ( !inCloneImageData )
            qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning since we use a temporary QImage";
 
        QImage   swapped = inImage.rgbSwapped();
 
        return cv::Mat( swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine() ).clone();
        }
 
        // 8-bit, 1 channel
        case QImage::Format_Indexed8:
        {
        cv::Mat  mat( inImage.height(), inImage.width(), CV_8UC1, const_cast<uchar*>(inImage.bits()), inImage.bytesPerLine() );
 
        return (inCloneImageData ? mat.clone() : mat);
        }
 
        default:
        qWarning() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
        break;
    }
 
    return cv::Mat();
}
 
// If inPixmap exists for the lifetime of the resulting cv::Mat, pass false to inCloneImageData to share inPixmap's data
// with the cv::Mat directly
//    NOTE: Format_RGB888 is an exception since we need to use a local QImage and thus must clone the data regardless
cv::Mat QPixmapToCvMat( const QPixmap &inPixmap, bool inCloneImageData )
{
    return QImageToCvMat( inPixmap.toImage(), inCloneImageData );
}

void DeleteLayout(QLayout* layout)
{
    if(layout)
    {
        QLayoutItem * item;
        QLayout * sublayout;
        QWidget * widget;
        while ((item = layout->takeAt(0))) 
        {
            if ((sublayout = item->layout()) != 0)
            {
                DeleteLayout(sublayout);
            }
            else if ((widget = item->widget()) != 0)
            {
                widget->hide();
                delete widget;
            }
            else 
            {
                delete item;
            }
        }

        delete layout;
    }
}