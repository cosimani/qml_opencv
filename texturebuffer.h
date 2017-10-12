#ifndef TEXTUREBUFFER_H
#define TEXTUREBUFFER_H

#include <QOpenGLTexture>
#include <QAbstractVideoBuffer>

#include <QVariant>

class TextureBuffer : public QAbstractVideoBuffer
{
public:
    TextureBuffer( uint id );
    MapMode mapMode() const;
    uchar *map( MapMode, int *, int * );
    void unmap();
    QVariant handle() const;

private:
    GLuint m_id;
};

#endif // TEXTUREBUFFER_H
