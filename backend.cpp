#include "backend.h"

BackEnd::BackEnd(QObject *parent) :
    QObject(parent)
{
    
}

BackEnd::~BackEnd()
{
}

QString BackEnd::userName()
{
    return m_userName;
}

void BackEnd::setUserName(const QString &userName)
{
    if (userName == m_userName)
        return;

    m_userName = userName;
    emit userNameChanged( userName );
}

QMatrix4x4 BackEnd::getMatriz() const
{
    return m_matriz;
}

void BackEnd::setMatriz(const QMatrix4x4 &value)
{
    m_matriz = value;

    emit matrizChanged( m_matriz );
}

QImage BackEnd::getImage() const
{
    return m_image;
}

void BackEnd::setImage(const QImage &value)
{
    m_image = value;

    emit imageChanged( m_image );
}

Detector BackEnd::getDetector() const
{
    return detector;
}

void BackEnd::setDetector(const Detector &value)
{
    detector = value;

    emit detectorChanged( detector );
}


