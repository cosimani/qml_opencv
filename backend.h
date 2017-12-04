#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
#include <QMatrix4x4>
#include <QImage>
#include <QVector>
#include <QPair>

class Detector  {
public:
    Detector() : idMarcador( -1 ) { }

    QMatrix4x4 getMatrix() const  { return matrix; }
    void setMatrix(const QMatrix4x4 &value)  { matrix = value; }

    int getIdMarcador() const  { return idMarcador; }
    void setIdMarcador(int value)  { idMarcador = value; }

    void append( QPair< int, QMatrix4x4 > datosMarcador )  { vector.append( datosMarcador ); }
    QPair< int, QMatrix4x4 > at( int i )  { return vector.at( i ); }
    int size()  { return vector.size(); }

    QVector<QPair<int, QMatrix4x4> > getVector() const  { return vector; }
    void setVector(const QVector<QPair<int, QMatrix4x4> > &value)  { vector = value; }

private:
    QMatrix4x4 matrix;
    int idMarcador;

    // El par contiene idMarcador y la matrix par el render
    QVector< QPair < int, QMatrix4x4 > > vector;
};

class BackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QMatrix4x4 matriz MEMBER m_matriz READ getMatriz WRITE setMatriz NOTIFY matrizChanged)
    Q_PROPERTY(QImage image MEMBER m_image READ getImage WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(Detector det MEMBER detector READ getDetector WRITE setDetector NOTIFY detectorChanged)

public:
    explicit BackEnd( QObject *parent = nullptr );
    ~BackEnd();

    QString userName();
    void setUserName(const QString &userName);

    QMatrix4x4 getMatriz() const;
    void setMatriz(const QMatrix4x4 &value);

    QImage getImage() const;
    void setImage(const QImage &value);

    Detector getDetector() const;
    void setDetector(const Detector &value);

signals:
    void userNameChanged( QString );
    void matrizChanged( QMatrix4x4 );
    void imageChanged( QImage );
    void detectorChanged( Detector );

private:
    QString m_userName;
    QMatrix4x4 m_matriz;
    QImage m_image;
    Detector detector;
};

#endif // BACKEND_H
