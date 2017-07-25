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

#include <videofilter.h>



Scene::Scene(QWidget *parent) : QWidget(parent),
                                ui(new Ui::Scene),
                                manager( new QNetworkAccessManager(this) ),
                                placeTimer( new QTimer(this) )
{
    ui->setupUi(this);

#define CAMERA_PARAMETERS_FILE_RESOURCE ":/CameraParameters.yml"
#define CAMERA_PARAMETERS_FILE_LOCAL "./CameraParameters.yml"

    QResource yml( CAMERA_PARAMETERS_FILE_RESOURCE );

    QFile ymlFileResource(yml.absoluteFilePath());

    if (!ymlFileResource.open(QIODevice::ReadOnly | QIODevice::Text))  {
        qDebug() << "No se pudo iniciar camara 2 / Problema con parametros de la camara";
    }

    QTextStream in(&ymlFileResource);
    QString content = in.readAll();

        // Creo un archivo nuevo para almacenarlo
    QFile ymlFileLocal(CAMERA_PARAMETERS_FILE_LOCAL);
    if (!ymlFileLocal.open(QIODevice::WriteOnly | QIODevice::Text))  {
        qDebug() << "No se pudo iniciar camara / Problema con parametros de la camara";
    }

    QTextStream out(&ymlFileLocal);
    out << content;

    ymlFileLocal.close();

//    ui->quickWidget->setSource(QUrl("qrc:///qmlcamera.qml"));
    ui->quickWidget->setSource(QUrl("qrc:///CameraProcessing.qml"));

    QObject *rect = dynamic_cast<QObject*>(ui->quickWidget->rootObject());

//    qmlRegisterType<VideoFilter>( "com.calibration.opencv", 1, 0, "VideoFilter" );

//    QQmlApplicationEngine engine;

//    engine.load( QUrl( QStringLiteral("qrc:/main.qml")));


//    QObject::connect(rect, SIGNAL(signal_imageProcessing(QString)), this, SLOT(slot_imageProcessing(QString)));
//    QObject::connect(rect, SIGNAL(signal_imagePath(QString)), this, SLOT(slot_imagePath(QString)));


    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_uploadFinished(QNetworkReply*)));

    connect( placeTimer, SIGNAL( timeout() ), SLOT( showPlaceAgain() ) );

}

Scene::~Scene()
{
    this->apagarCamara();
    delete ui;
}

void Scene::apagarCamara()
{
//    QObject *object = dynamic_cast<QObject*>(ui->quickWidget->rootObject());

//    QVariant returnedValue;
//    QVariant msg = "Hello from C++";
//    QMetaObject::invokeMethod(object, "cerrarCamara",
//            Q_RETURN_ARG(QVariant, returnedValue),
//            Q_ARG(QVariant, msg));

//    qDebug() << "QML function returned:" << returnedValue.toString();

}

void Scene::encenderCamara()
{
//    QObject *object = dynamic_cast<QObject*>(ui->quickWidget->rootObject());

//    QVariant returnedValue;
//    QVariant msg = "Hello from C++";
//    QMetaObject::invokeMethod(object, "abrirCamara",
//            Q_RETURN_ARG(QVariant, returnedValue),
//            Q_ARG(QVariant, msg));

//    qDebug() << "QML function returned:" << returnedValue.toString();


}

void Scene::slot_imageProcessing( const QString& path )  {

    qDebug() << "slot_imageProcessing";


}

void Scene::slot_imagePath(const QString &path)
{
    qDebug() << " Se borro la imagen slot_imagePath";

}

void Scene::slot_uploadFinished(QNetworkReply *reply)
{
    QByteArray ba = reply->readAll();
    qDebug() << ba;

    if ( ! ba.contains("200") )  {
        emit mensaje( "No se subió la foto, intente de nuevo!!" );
        placeTimer->start( 4000 );
    }
    else  {
        emit mensaje( "La foto se tomó con éxito!!!" );
        placeTimer->start( 4000 );
    }

}

void Scene::showPlaceAgain()
{
    placeTimer->stop();
    emit mensaje( "Logre fotos de tarjetas personales" );
}



