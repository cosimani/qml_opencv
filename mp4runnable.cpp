#include "mp4runnable.h"

#include <QDebug>
#include <QOpenGLExtraFunctions>
#include <QMatrix4x4>

#include "texturebuffer.h"

#include "scene.h"


Mp4Runnable::Mp4Runnable( Mp4Filter * filter ) : m_filter(filter)
{
    texture = new QOpenGLTexture( QOpenGLTexture::Target2D );
    texture->setMinificationFilter( QOpenGLTexture::Nearest );
    texture->setMagnificationFilter( QOpenGLTexture::Linear );
    texture->setFormat( QOpenGLTexture::RGBA8_UNorm );

}

Mp4Runnable::~Mp4Runnable()
{
    qDebug()<<"{destructor Mp4Runnable}";

}

QVideoFrame Mp4Runnable::run( QVideoFrame *input,
                              const QVideoSurfaceFormat &surfaceFormat,
                              QVideoFilterRunnable::RunFlags flags )
{
    Q_UNUSED(surfaceFormat);
    Q_UNUSED(flags);

    qDebug() << "input->handleType();" << input->handleType();

    if ( ! input->isValid() )
        return *input;



    if ( input->handleType() == QAbstractVideoBuffer::GLTextureHandle )  {

        using namespace cv;
        input->map(QAbstractVideoBuffer::ReadOnly);

        QImage image = this->wrapper( *input );

//        image = image.scaled( 320, 240 );

        image = image.convertToFormat( QImage::Format_RGB888 );

        Mat mat( image.height(), image.width(), CV_8UC3, image.bits(), image.bytesPerLine() );


        input->unmap();

        texture->setData( image );
        texture->bind();

        Scene::getInstancia()->getBackend()->setImage( image );

        return QVideoFrame(new TextureBuffer( texture->textureId() ), input->size(), input->pixelFormat() );

    }
    else  {
        return *input;
    }
}

QImage Mp4Runnable::wrapper( const QVideoFrame & input )
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
