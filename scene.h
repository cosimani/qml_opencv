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


#include "backend.h"

using namespace cv;

namespace Ui {
class Scene;
}

class Scene : public QWidget
{
    Q_OBJECT

private:
    static Scene *instancia;
    explicit Scene( QWidget * parent = 0 );

public:
    static Scene *getInstancia();
    ~Scene();

    void send();

    void sendMatrix( QMatrix4x4 matrix );


    BackEnd * backend;

    BackEnd *getBackend() const;
    void setBackend(BackEnd *value);

private:
    Ui::Scene *ui;


};

#endif // SCENE_H
