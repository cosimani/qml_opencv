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

void Scene::pausarVideo()
{
    QQuickItem * itemRoot = ui->quickWidget->rootObject();

//    qDebug() << "antes" << itemRoot->property( "reproduciendo" );

    itemRoot->setProperty( "reproduciendo", false );

//    qDebug() << "despues" << itemRoot->property( "reproduciendo" );


//    qDebug() << itemRoot->childItems().at( 2 )->childItems();
//    QQuickItem * rectangle = itemRoot->childItems().at( 0 );
//    QQuickItem * mouseArea = itemRoot->childItems().at( 1 );
//    QQuickItem * videoOutput = itemRoot->childItems().at( 2 );
}

void Scene::reanudarVideo()
{
    QQuickItem * itemRoot = ui->quickWidget->rootObject();

//    qDebug() << "antes" << itemRoot->property( "reproduciendo" );

    itemRoot->setProperty( "reproduciendo", true );

//    qDebug() << "despues" << itemRoot->property( "reproduciendo" );


//    qDebug() << itemRoot->childItems().at( 2 )->childItems();
//    QQuickItem * rectangle = itemRoot->childItems().at( 0 );
//    QQuickItem * mouseArea = itemRoot->childItems().at( 1 );
//    QQuickItem * videoOutput = itemRoot->childItems().at( 2 );
}


void Scene::showEvent( QShowEvent * )
{
    qDebug() << "showEvent";
    this->configurarWidget();
}

void Scene::resizeEvent(QResizeEvent *e)
{
    qDebug() << "resizeEvent" << e->type();
    this->configurarWidget();

//    resizeEvent 449 425
//    showEvent 449 425
//    resizeEvent 1440 2672
}

void Scene::configurarWidget()
{
    qDebug() << "Dimensiones Scene" << this->width() << this->height();

    ui->quickWidget->resize( this->width(), this->height() );
    ui->quickWidget->move( 0, 0 );

    qDebug() << "Dimensiones quickWidget" << ui->quickWidget->width() << ui->quickWidget->height();

//    QQmlEngine * engine = ui->quickWidget->rootContext()->engine();

//    QQuickItem * itemRoot = ui->quickWidget->rootObject();
//    QQuickItem * rectangle = itemRoot->childItems().at( 0 );
//    QQuickItem * mouseArea = itemRoot->childItems().at( 1 );
//    QQuickItem * videoOutput = itemRoot->childItems().at( 2 );

//    qDebug() << "rectangle" << rectangle;
//    qDebug() << "mouseArea" << mouseArea;
//    qDebug() << "videoOutput" << videoOutput;

//    qDebug() << "Listado rectangle" << rectangle->childItems();
//    qDebug() << "Listado mouseArea" << mouseArea->childItems();
//    qDebug() << "Listado videoOutput" << videoOutput->childItems();

//    QQuickItem * renderer = videoOutput->childItems().at( 0 );
//    qDebug() << "renderer" << renderer;

//    qDebug() << "Listado renderer" << renderer->childItems();


//    qDebug() << "childItems" << itemRoot->childItems();

//    for ( int i=0 ; i<itemRoot->childItems().size() ; i++ )  {
//        qDebug() << itemRoot->childItems().at( i )
//                 << itemRoot->childItems().at( i )->x()
//                 << itemRoot->childItems().at( i )->y()
//                 << itemRoot->childItems().at( i )->width()
//                 << itemRoot->childItems().at( i )->height();
//    }


//    QQuickRectangle(0xe28b3400, parent=0xe28b2ce0, geometry=0,0 1440x2672) 0 0 1440 2672
//    QQuickMouseArea(0xe28b3460, parent=0xe28b2ce0, geometry=0,0 1440x2672) 0 0 1440 2672
//    QDeclarativeVideoOutput(0xd4494300, parent=0xe28b2ce0, geometry=0,0 1440x2672) 0 0 1440 2672





//    QQmlApplicationEngine

//    QQmlComponent component( engine );
//    QObject *object = component.create();

//    qDebug() << "Property value:" << QQmlProperty::read( object, "item.videoOutput.renderer.width" ).toInt();


//    qDebug() << "children" << engine->children();
//    qDebug() << "children" << engine->rootContext()->children();



//    QObject *rootObject = engine->rootObjects().first();

//    QObject *qmlObject_renderer = rootObject->findChild<QObject*>("renderer");
//    QObject *qmlObject_videoOutput = rootObject->findChild<QObject*>("videoOutput");
//    QObject *qmlObject_item = rootObject->findChild<QObject*>("item");
//    QObject *qmlObject_rectangle = rootObject->findChild<QObject*>("rectangle");
//    QObject *qmlObject_mouseArea = rootObject->findChild<QObject*>("mouseArea");
//    QObject *qmlObject_camera = rootObject->findChild<QObject*>("camera");
//    QObject *qmlObject_videoFilter = rootObject->findChild<QObject*>("videoFilter");

//    qDebug() << qmlObject_renderer->property("visible");


}

