#include <videofilter.h>
#include "rgbframehelper.h"
#include <QDebug>

QVideoFilterRunnable *VideoFilter::createFilterRunnable()
{
    return new FilterRunnable(this);    
}

int VideoFilter::gaussianBlurSize() const
{
    return m_gaussianBlurSize;
}

void VideoFilter::setGaussianBlurSize(int gaussianBlurSize)
{
    m_gaussianBlurSize = gaussianBlurSize %2 == 1 ? gaussianBlurSize : m_gaussianBlurSize;
}

double VideoFilter::gaussianBlurCoef() const
{    
    return m_gaussianBlurCoef;
}

void VideoFilter::setGaussianBlurCoef(double gaussianBlurCoef)
{    
    m_gaussianBlurCoef = gaussianBlurCoef;
}

double VideoFilter::cannyThreshold() const
{      
    return m_cannyThreshold;
}

void VideoFilter::setCannyThreshold(double cannyThreshold)
{    
    m_cannyThreshold = cannyThreshold;
}

int VideoFilter::cannyKernelSize() const
{   
    return m_cannyKernelSize;
}

void VideoFilter::setCannyKernelSize(int cannyKernelSize)
{    
    m_cannyKernelSize = cannyKernelSize%2 == 1 ? cannyKernelSize : m_cannyKernelSize;
}


FilterRunnable::FilterRunnable(VideoFilter *filter) :
    m_filter(filter)
{        
    texture = new QOpenGLTexture( QOpenGLTexture::Target2D );
    texture->setMinificationFilter( QOpenGLTexture::Nearest );
    texture->setMagnificationFilter( QOpenGLTexture::Linear );
    texture->setFormat( QOpenGLTexture::RGBA8_UNorm );
}

FilterRunnable::~FilterRunnable()
{
    qDebug()<<"{destructor FilterRunnable}";

}

QVideoFrame FilterRunnable::run( QVideoFrame *input,
                                 const QVideoSurfaceFormat &surfaceFormat,
                                 QVideoFilterRunnable::RunFlags flags )
{

//    int gaussianBlurSize = m_filter->gaussianBlurSize();
//    double gaussianBlurCoef = m_filter->gaussianBlurCoef();
//    int cannyKernelSize = m_filter->cannyKernelSize();
//    double cannyThreshold = m_filter->cannyThreshold();

    // La valores por defecto son estos.
    int gaussianBlurSize = 7;
    double gaussianBlurCoef = 1.5f;
    int cannyKernelSize = 3;
    double cannyThreshold = 0;

//    gaussianBlurSize 7 gaussianBlurCoef 1.5 cannyKernelSize 3 cannyThreshold 0
//    gaussianBlurSize 7 gaussianBlurCoef 1.5 cannyKernelSize 3 cannyThreshold 0
//    gaussianBlurSize 7 gaussianBlurCoef 1.5 cannyKernelSize 3 cannyThreshold 0
//    gaussianBlurSize 7 gaussianBlurCoef 1.5 cannyKernelSize 3 cannyThreshold 0
//    gaussianBlurSize 7 gaussianBlurCoef 1.5 cannyKernelSize 3 cannyThreshold 0

    if ( ! input->isValid() )
        return *input;

    if (input->handleType() == QAbstractVideoBuffer::NoHandle) {
        input->map(QAbstractVideoBuffer::ReadOnly);
        cv::Mat mat( input->height(), input->width(), CV_8UC1, input->bits() );
        Point pt1;
        pt1.x = 0;
        pt1.y = 0;
        Point pt2;
        pt2.x = 200;
        pt2.y = 200;
        cv::line(mat, pt1, pt2, cv::Scalar(0,255,0), 20);
        QVector<QRgb> colorTable;
        for (int i = 0; i < 256; i++)
        {
            colorTable.push_back(qRgb(i, i, i));
        }        
        const uchar *qImageBuffer = (const uchar*) mat.data;        
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);    
        input->unmap();
        QVideoFrame outputFrame = QVideoFrame(img);    
        return outputFrame;
    } else {
        input->map(QAbstractVideoBuffer::ReadOnly);
        QImage image = imageWrapper(*input);
        cv::Mat mat(image.width(),image.height(),CV_8UC3,image.bits(), image.bytesPerLine());
        Point pt1;
        pt1.x = 0;
        pt1.y = 0;
        Point pt2;
        pt2.x = 200;
        pt2.y = 200;
        cv::line(mat, pt1, pt2, cv::Scalar(0,255,0), 20);
        input->unmap();
        texture->setData(image);
        texture->bind();
        return frameFromTexture(texture->textureId(),input->size(),input->pixelFormat());
    }

}

void FilterRunnable::deleteColorComponentFromYUV( QVideoFrame *input )
{
    // Assign 0 to Us and Vs
    int firstU = input->width() * input->height();
    int lastV = input->width() * input->height() + 2 * input->width() * input->height() / 4;
    uchar* inputBits = input->bits();

    for ( int i = firstU ; i < lastV ; i++ )
        inputBits[i] = 127;    
}

// Metodo extraido de https://github.com/alpqr/qt-opencv-demo/blob/master/opencvhelper.cpp
cv::Mat FilterRunnable::yuvFrameToMat8( QVideoFrame * frame)
{    

    Q_ASSERT(frame->handleType() == QAbstractVideoBuffer::NoHandle && frame->isReadable());
    Q_ASSERT(frame->pixelFormat() == QVideoFrame::Format_YUV420P || frame->pixelFormat() == QVideoFrame::Format_NV12);

    cv::Mat tmp(frame->height() + frame->height() / 2, frame->width(), CV_8UC1, (uchar *) frame->bits());
    cv::Mat result(frame->height(), frame->width(), CV_8UC3);
    cvtColor(tmp, result, frame->pixelFormat() == QVideoFrame::Format_YUV420P ? CV_YUV2BGR_YV12 : CV_YUV2BGR_NV12);
    return result;
}
