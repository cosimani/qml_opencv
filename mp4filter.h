#ifndef MP4FILTER_H
#define MP4FILTER_H

#include <QAbstractVideoFilter>
#include <QOpenGLTexture>

class Mp4Filter : public QAbstractVideoFilter
{
    Q_OBJECT

public:
    QVideoFilterRunnable * createFilterRunnable() Q_DECL_OVERRIDE;

};

#endif // MP4FILTER_H
