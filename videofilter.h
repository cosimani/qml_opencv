#ifndef VIDEOFILTER
#define VIDEOFILTER

#include <QAbstractVideoFilter>
#include <QOpenGLTexture>

#include <QDebug>

class VideoFilter : public QAbstractVideoFilter
{
    Q_OBJECT

public:
    QVideoFilterRunnable * createFilterRunnable() Q_DECL_OVERRIDE;

};

#endif // VIDEOFILTER

