#include "texturebuffer.h"

TextureBuffer::TextureBuffer(uint id) : QAbstractVideoBuffer( GLTextureHandle ),
                                        m_id( id )
{

}

QAbstractVideoBuffer::MapMode TextureBuffer::mapMode() const  {
    return NotMapped;
}

uchar *TextureBuffer::map( QAbstractVideoBuffer::MapMode, int *, int * )  {
    return 0;
}

void TextureBuffer::unmap()  {

}

QVariant TextureBuffer::handle() const  {
    return QVariant::fromValue< uint >( m_id );
}
