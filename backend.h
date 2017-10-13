#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
#include <QMatrix4x4>

class BackEnd : public QObject
{
    Q_OBJECT

public:
    explicit BackEnd( QObject *parent = nullptr );
    ~BackEnd();

    QString userName();
    void setUserName(const QString &userName);

    QMatrix4x4 getMatriz() const;
    void setMatriz(const QMatrix4x4 &value);


signals:
    void userNameChanged( QString );

    void matrizChanged( QMatrix4x4 );

private:
    QString m_userName;
    QMatrix4x4 m_matriz;



};

#endif // BACKEND_H
