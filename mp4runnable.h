#ifndef MP4RUNNABLE_H
#define MP4RUNNABLE_H

#include <QAbstractVideoFilter>
#include <QOpenGLTexture>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include "opencv2/videoio.hpp"

#include <QDebug>

class Mp4Filter;

class Mp4Runnable : public QVideoFilterRunnable
{
public:
    Mp4Runnable( Mp4Filter *filter );
    ~Mp4Runnable();

    QVideoFrame run( QVideoFrame *input,
                     const QVideoSurfaceFormat &surfaceFormat,
                     RunFlags flags ) Q_DECL_OVERRIDE;

private:
    Mp4Filter * m_filter;
    QOpenGLTexture *texture;

    QImage wrapper(const QVideoFrame &input);

};

#endif // MP4RUNNABLE_H
