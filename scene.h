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

#include <QShowEvent>


#include "backend.h"

using namespace cv;

namespace Ui {
class Scene;
}

class Scene : public QWidget
{
    Q_OBJECT

private:
    Ui::Scene *ui;

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

    void pausarVideo();

    void reanudarVideo();
private:
    void configurarWidget();

protected:
    void showEvent( QShowEvent * );
    void resizeEvent( QResizeEvent * e );


};

#endif // SCENE_H
