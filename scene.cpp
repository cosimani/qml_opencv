#include "scene.h"
#include "ui_scene.h"
#include <QQmlContext>
#include <QQuickItem>
#include <QDebug>

#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickImageProvider>
#include <QDebug>
#include <QDir>
#include <QFileInfo>

#include <QResource>
#include <QMessageBox>
#include <QBuffer>
#include <QDateTime>

#include <QQmlApplicationEngine>
#include <QtQml>
#include <QDebug>

#include <QQmlContext>



Scene * Scene::instancia = NULL;



Scene::Scene(QWidget *parent) : QWidget(parent),
                                ui(new Ui::Scene),
                                backend( new BackEnd )
{
    ui->setupUi(this);

    ui->quickWidget->setSource(QUrl("qrc:///CameraProcessing.qml"));

    ui->quickWidget->rootContext()->setContextProperty( "Back", backend );

}



Scene * Scene::getInstancia()
{
    if( instancia == NULL )
    {
        instancia = new Scene();
    }
    return instancia;
}

Scene::~Scene()
{
    if( instancia != NULL )
    {
        delete instancia;
    }

    delete ui;
}

void Scene::send()
{
    backend->setUserName("aerrrr");
}

void Scene::sendMatrix(QMatrix4x4 matrix)
{
    backend->setMatriz( matrix );
}



BackEnd *Scene::getBackend() const
{
    return backend;
}

void Scene::setBackend(BackEnd *value)
{
    backend = value;
}


