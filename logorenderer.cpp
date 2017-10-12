/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "logorenderer.h"
#include <QPainter>
#include <QPaintEngine>
#include <math.h>

#include <QOpenGLShader>

#include "scene.h"

LogoRenderer::LogoRenderer()
{
    vImageBuffer = new QVector< QOpenGLBuffer * >();
    vImageTexture = new QVector< QOpenGLTexture * >();
}

LogoRenderer::~LogoRenderer()
{
}

void LogoRenderer::initialize()
{
    initializeOpenGLFunctions();

    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

    this->loadTextures();

    const char *vsrcUBP =
        "attribute highp vec4 vertex;\n"
        "attribute mediump vec4 texCoord;\n"
        "varying mediump vec4 texc;\n"

        "uniform mediump mat4 matrix;\n"

        "void main()  {\n"
            "gl_Position = matrix * vertex;\n"

            "texc = texCoord;\n"
        "}\n";

    const char *fsrcUBP =
        "uniform sampler2D texture;\n"
        "varying mediump vec4 texc;\n"

        "void main()  {\n"
            "gl_FragColor = texture2D( texture, texc.st );\n"
        "}\n";

    program1.addShaderFromSourceCode(QOpenGLShader::Vertex, vsrcUBP);
    program1.addShaderFromSourceCode(QOpenGLShader::Fragment, fsrcUBP);
    program1.link();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );


    geometries = new GeometryEngine;

}

void LogoRenderer::render()
{
    glDepthMask(true);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    program1.bind();


    // Si no detecta marcadores, entonces la matriz es matriz identidad
    if ( ! Scene::getInstancia()->getBackend()->getMatriz().isIdentity() )  {

        QMatrix4x4 matrix = Scene::getInstancia()->getBackend()->getMatriz();
        matrix.scale( 0.045 );

        program1.setUniformValue( "matrix", matrix );

        vImageTexture->at( 3 )->bind();
        vImageBuffer->at( 3 )->bind();

//        geometries->drawSheetGeometry( &program1 );
        geometries->drawCubeGeometry( &program1 );
//        geometries->drawHojaGeometry( &program1 );

    }

    program1.release();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

}

/*
 * Carga todas las texturas y imagenes en las carpeta 'Textures' y las almacena una por una en el vector
 * 'vImageTextures'. En vImageBuffer almacena las coordenadas (vertices y indices)
 * correspondientes a cada una de ellas
*/
void LogoRenderer::loadTextures()
{
//    QDir directory( "../Textures" );

//    QStringList fileFilter;
//    fileFilter << "*.jpg" << "*.png" << "*.bmp" << "*.gif";
//    QStringList imageFiles = directory.entryList( fileFilter );

    QStringList imageFiles;
    imageFiles << ":/images/side1.png" << ":/images/side2.png" << ":/images/side3.png" << ":/images/messi.png";

    qDebug() << imageFiles;

    for ( int i = 0; i < imageFiles.size(); i++ )
    {
        QOpenGLBuffer * imageBuffer = new QOpenGLBuffer( QOpenGLBuffer::VertexBuffer );

        imageBuffer->create();

        static const int coords[1][4][3] = { { { +1, -1, 0 }, { -1, -1, 0 }, { -1, 0, 0 }, { +1, 0, 0 } } };

        QVector<GLfloat> vertData;
        for (int i = 0; i < 1; ++i) {
            for (int j = 0; j < 4; ++j) {
                // vertex position
                vertData.append( coords[i][j][0]);
                vertData.append( coords[i][j][1]);
                vertData.append( coords[ i ][ j ][ 2 ]);

                // texture coordinate
                if ( j==0 )  {  vertData.append(1);  vertData.append(0);  }
                if ( j==1 )  {  vertData.append(0);  vertData.append(0);  }
                if ( j==2 )  {  vertData.append(0);  vertData.append(1);  }
                if ( j==3 )  {  vertData.append(1);  vertData.append(1);  }
            }
        }

        if ( ! imageBuffer->bind() )
            qDebug() << "False / vbo2 bind()";

        imageBuffer->allocate( vertData.constData(), vertData.count() * sizeof( GLfloat ) );


//        QOpenGLTexture * imageTexture = new QOpenGLTexture( QImage( "../Textures/" + imageFiles.at( i ) ) );
        QOpenGLTexture * imageTexture = new QOpenGLTexture( QImage( imageFiles.at( i ) ) );
        imageTexture->setMinificationFilter( QOpenGLTexture::Nearest );
        imageTexture->setMagnificationFilter( QOpenGLTexture::Linear );

        vImageBuffer->append( imageBuffer );
        vImageTexture->append( imageTexture );

    }
}


