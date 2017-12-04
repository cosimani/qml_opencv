#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QDebug>

#include "videofilter.h"
#include "mp4filter.h"


#include <scene.h>
#include "fboinsgrenderer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType< VideoFilter >( "com.vayra.camera", 1, 0, "VideoFilter" );
    qmlRegisterType< Mp4Filter >( "com.vayra.mp4", 1, 0, "Mp4Filter" );
    qmlRegisterType< FboInSGRenderer >( "com.vayra.rendering", 1, 0, "Renderer" );

    Scene::getInstancia()->show();

    return app.exec();
}

