#include "filterrunnable.h"

#include <QDebug>
#include <QResource>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QOpenGLExtraFunctions>
#include <QMatrix4x4>

#include "texturebuffer.h"

#include "scene.h"

FilterRunnable::FilterRunnable( VideoFilter * filter ) : m_filter(filter),
                                                         markerDetector( new aruco::MarkerDetector ),
                                                         cameraParameters( new aruco::CameraParameters )
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


/**
 * @brief FilterRunnable::run
 * @param input Para Desktop el pixelFormat = Format_YUV420P
 *              Para Samsung S8 pixelFormat = Format_BGR32
 *
 *        input Para Desktop el input->handleType() = NoHandle
 *              Para Samsung S8 input->handleType() = GLTextureHandle
 *
 *        input Para Desktop el input->width() height() bytesPerLine() = 640 480 0
 *              Para Samsung S8 input->width() height() bytesPerLine() = 1440 1080 5760 -> FrontalFace
 *              Para Samsung S8 input->width() height() bytesPerLine() = 1440 1080 5760 -> BackFace
 *              ( Raro que la camara frontal y la trasera sean con la misma resolucion, bueno, eso dio )
 *              Para Samsung S8 si dividimos 5760 / 1440 = 4 . Entonces, 4 canales, por eso es Format_BGR32
 *                  Este formato QVideoFrame::Format_BGR32 es using a 32-bit BGR format (0xBBGGRRff).
 *
 * @param surfaceFormat Para Desktop los valores de QVideoSurfaceFormat son:
 *              pixelFormat = Format_YUV420P  //  pixelAspectRatio = QSize(1, 1)
 *        surfaceFormat Para Android Samsung S8 los valores de QVideoSurfaceFormat son:
 *              pixelFormat = Format_BGR32  //  pixelAspectRatio = QSize(1, 1)
 *
 * @param flags Para Desktop: flags = QFlags(0x1)
 * @return
 */
QVideoFrame FilterRunnable::run( QVideoFrame *input,
                                 const QVideoSurfaceFormat &surfaceFormat,
                                 QVideoFilterRunnable::RunFlags flags )
{
    Q_UNUSED(surfaceFormat);
    Q_UNUSED(flags);

    if ( ! input->isValid() )
        return *input;

    if ( input->handleType() == QAbstractVideoBuffer::GLTextureHandle )  {

        using namespace cv;
        input->map(QAbstractVideoBuffer::ReadOnly);

        QImage image = this->wrapper( *input );

//        image = image.scaled( 640, 480 );

        image = image.convertToFormat( QImage::Format_RGB888 );

        Mat mat( image.height(), image.width(), CV_8UC3, image.bits(), image.bytesPerLine() );

//        flip( mat, mat, 0 );  // eje x
        flip( mat, mat, 1 );  // eje y
        flip( mat, mat, -1 );  // ambos

        cameraParameters->resize( mat.size() );
        markerDetector->detect( mat, detectedMarkers, *cameraParameters, 0.08f );

        flip( mat, mat, 0 );
//        flip( mat, mat, 1 );
//        flip( mat, mat, -1 );


        Detector detector;

        if( detectedMarkers.size() > 0 )  {

            double projectionMatrix[16];
            cameraParameters->glGetProjectionMatrix( cv::Size2i( 640, 480 ),
                                                     cv::Size2i( 640, 480 ),
                                                     projectionMatrix, 0.05, 10 );            

            for( unsigned int i = 0; i < detectedMarkers.size(); i++ )  {

                double modelview_matrix[ 16 ];
                detectedMarkers.operator []( i ).glGetModelViewMatrix( modelview_matrix );

                QMatrix4x4 projection;
                QMatrix4x4 modelView;

                projection( 0, 0 ) = projectionMatrix[ 0 ];
                projection( 1, 0 ) = projectionMatrix[ 1 ];
                projection( 2, 0 ) = projectionMatrix[ 2 ];
                projection( 3, 0 ) = projectionMatrix[ 3 ];
                projection( 0, 1 ) = projectionMatrix[ 4 ];
                projection( 1, 1 ) = projectionMatrix[ 5 ];
                projection( 2, 1 ) = projectionMatrix[ 6 ];
                projection( 3, 1 ) = projectionMatrix[ 7 ];
                projection( 0, 2 ) = projectionMatrix[ 8 ];
                projection( 1, 2 ) = projectionMatrix[ 9 ];
                projection( 2, 2 ) = projectionMatrix[ 10 ];
                projection( 3, 2 ) = projectionMatrix[ 11 ];
                projection( 0, 3 ) = projectionMatrix[ 12 ];
                projection( 1, 3 ) = projectionMatrix[ 13 ];
                projection( 2, 3 ) = projectionMatrix[ 14 ];
                projection( 3, 3 ) = projectionMatrix[ 15 ];

                modelView( 0, 0 ) = modelview_matrix[ 0 ];
                modelView( 1, 0 ) = modelview_matrix[ 1 ];
                modelView( 2, 0 ) = modelview_matrix[ 2 ];
                modelView( 3, 0 ) = modelview_matrix[ 3 ];
                modelView( 0, 1 ) = modelview_matrix[ 4 ];
                modelView( 1, 1 ) = modelview_matrix[ 5 ];
                modelView( 2, 1 ) = modelview_matrix[ 6 ];
                modelView( 3, 1 ) = modelview_matrix[ 7 ];
                modelView( 0, 2 ) = modelview_matrix[ 8 ];
                modelView( 1, 2 ) = modelview_matrix[ 9 ];
                modelView( 2, 2 ) = modelview_matrix[ 10 ];
                modelView( 3, 2 ) = modelview_matrix[ 11 ];
                modelView( 0, 3 ) = modelview_matrix[ 12 ];
                modelView( 1, 3 ) = modelview_matrix[ 13 ];
                modelView( 2, 3 ) = modelview_matrix[ 14 ];
                modelView( 3, 3 ) = modelview_matrix[ 15 ];

//                projection.rotate(180, 0,0,1);

                modelView.rotate( 270, 0, 0, 1 );

                QPair< int, QMatrix4x4 > datosMarcador;
                datosMarcador.first = detectedMarkers.at( i ).id;
                datosMarcador.second = projection * modelView;
                detector.append( datosMarcador );

//                Scene::getInstancia()->getBackend()->setMatriz( projection * modelView );

//                detectedMarkers.at( i ).draw( mat, Scalar( 255, 0, 255 ), 1 );

            }
        }

        // Setea los datos de todos los marcadores detectados en este frame
        // Si no se detectaron marcadores, entonces Detector tendra un size de cero
        Scene::getInstancia()->getBackend()->setDetector( detector );

        input->unmap();

        texture->setData( image );
        texture->bind();

        return QVideoFrame(new TextureBuffer( texture->textureId() ), input->size(), input->pixelFormat() );

//        frameFromTexture( texture->textureId(), input->size(), input->pixelFormat() );
    }
    else  {
        return *input;
    }
}

QImage FilterRunnable::wrapper( const QVideoFrame & input )
{
    // Slow and inefficient path.
    // Ideally what's on the GPU should remain on the GPU, instead of readbacks like this.
    // Ver alguna opcion en: https://github.com/alpqr/qt-opencv-demo/blob/master/filter.cpp
    QImage image( input.width(), input.height(), QImage::Format_RGBA8888 );


    GLuint textureId = input.handle().toUInt();

    QOpenGLContext *ctx = QOpenGLContext::currentContext();
    QOpenGLFunctions *f = ctx->functions();

    f->glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    GLuint fbo;
    f->glGenFramebuffers( 1, &fbo );

    GLuint prevFbo;
    f->glGetIntegerv( GL_FRAMEBUFFER_BINDING, (GLint *) &prevFbo );

    f->glBindFramebuffer( GL_FRAMEBUFFER, fbo );

    f->glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0 );
    f->glReadPixels( 0, 0, input.width(), input.height(), GL_RGBA, GL_UNSIGNED_BYTE, image.bits() );


    f->glBindFramebuffer( GL_FRAMEBUFFER, prevFbo );

    return image;
}
