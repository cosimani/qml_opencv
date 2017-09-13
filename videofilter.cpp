#include <videofilter.h>
#include "rgbframehelper.h"
#include <QDebug>
#include <QResource>
#include <QFile>
#include<QDir>

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
    m_filter(filter), markerDetector( new MarkerDetector ),
cameraParameters( new CameraParameters )
{        
    texture = new QOpenGLTexture( QOpenGLTexture::Target2D );
    texture->setMinificationFilter( QOpenGLTexture::Nearest );
    texture->setMagnificationFilter( QOpenGLTexture::Linear );
    texture->setFormat( QOpenGLTexture::RGBA8_UNorm );

    #define CAMERA_PARAMETERS_FILE_RESOURCE ":/CameraParameters.yml"
    #define CAMERA_PARAMETERS_FILE_LOCAL "./CameraParameters.yml"

    QResource yml( CAMERA_PARAMETERS_FILE_RESOURCE );

    QFile ymlFileResource(yml.absoluteFilePath());

    if (!ymlFileResource.open(QIODevice::ReadOnly | QIODevice::Text))  {
        qDebug() << "No se pudo iniciar camara 2 / Problema con parametros de la camara";
    }

    QTextStream in(&ymlFileResource);
    QString content = in.readAll();

        // Creo un archivo nuevo para almacenarlo
    QFile ymlFileLocal(CAMERA_PARAMETERS_FILE_LOCAL);
    if (!ymlFileLocal.open(QIODevice::WriteOnly | QIODevice::Text))  {
        qDebug() << "No se pudo iniciar camara / Problema con parametros de la camara";
    }

    QTextStream out(&ymlFileLocal);
    out << content;

    ymlFileLocal.close();

    cameraParameters->readFromXMLFile( CAMERA_PARAMETERS_FILE_LOCAL );


    if ( ! cameraParameters->isValid() )  {
        qDebug() << "Error con YML / No es valido. La App se cerrara";
    }

}

FilterRunnable::~FilterRunnable()
{
    qDebug()<<"{destructor FilterRunnable}";

}

QVideoFrame FilterRunnable::run( QVideoFrame *input,
                                 const QVideoSurfaceFormat &surfaceFormat,
                                 QVideoFilterRunnable::RunFlags flags )
{
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
        cameraParameters->resize( mat.size() );
        markerDetector->detect( mat, detectedMarkers, *cameraParameters, 0.08f );

        for( unsigned int i = 0; i < detectedMarkers.size(); i++ )
            detectedMarkers.at( i ).draw( mat, Scalar( 255, 0, 255 ), 1 );
        const uchar *qImageBuffer = (const uchar*) mat.data;
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        input->unmap();
        QVideoFrame outputFrame = QVideoFrame(img);
        return outputFrame;
    } else {
        using namespace cv;
        input->map(QAbstractVideoBuffer::ReadOnly);
        QImage image = imageWrapper(*input);
        image = image.scaled(480,640,Qt::IgnoreAspectRatio); // si hago esto no detecta markers
        image = image.convertToFormat(QImage::Format_RGB888);
        Mat mat(image.width(),image.height(),CV_8UC3,image.bits(), image.bytesPerLine());
        cameraParameters->resize( mat.size() );
        markerDetector->detect( mat, detectedMarkers, *cameraParameters, 0.08f );
        for( unsigned int i = 0; i < detectedMarkers.size(); i++ )
            detectedMarkers.at( i ).draw( mat, Scalar( 255, 0, 255 ), 1 );
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
