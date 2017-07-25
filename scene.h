#ifndef SCENE_H
#define SCENE_H

#include <QWidget>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>

#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QTimer>

using namespace cv;

namespace Ui {
class Scene;
}

class Scene : public QWidget
{
    Q_OBJECT

public:
    explicit Scene(QWidget *parent = 0);
    ~Scene();

    void apagarCamara();
    void encenderCamara();

private:
    Ui::Scene *ui;

    QNetworkAccessManager *manager;
    QTimer * placeTimer;



public slots:
    void slot_imageProcessing( const QString& image );
    void slot_imagePath( const QString& path );

    void slot_uploadFinished(QNetworkReply *reply);
    void showPlaceAgain();

signals:
    void newMarkerDetected( int markerId );
    void mensaje( QString mensaje );

};

#endif // SCENE_H
