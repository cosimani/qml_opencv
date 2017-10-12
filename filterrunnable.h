#ifndef FILTERRUNNABLE_H
#define FILTERRUNNABLE_H

#include <QAbstractVideoFilter>
#include <QOpenGLTexture>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include "opencv2/videoio.hpp"

#include <aruco/aruco.h>
#include "backend.h"

#include <QDebug>

class VideoFilter;

class FilterRunnable : public QVideoFilterRunnable
{
public:
    FilterRunnable( VideoFilter *filter );
    ~FilterRunnable();

    QVideoFrame run( QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags ) Q_DECL_OVERRIDE;

private:
    VideoFilter * m_filter;
    QOpenGLTexture *texture;
    aruco::MarkerDetector * markerDetector;
    std::vector< aruco::Marker > detectedMarkers;
    aruco::CameraParameters *cameraParameters;

    QImage wrapper(const QVideoFrame &input);

};
#endif // FILTERRUNNABLE_H
