#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QDebug>

#include <videofilter.h>

#include <scene.h>
#include "fboinsgrenderer.h"

//#include "backend.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType< VideoFilter >( "com.camera.vayra", 1, 0, "VideoFilter" );
    qmlRegisterType< FboInSGRenderer >( "SceneGraphRendering", 1, 0, "Renderer" );

    Scene::getInstancia()->show();

    return app.exec();
}

