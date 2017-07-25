#ifndef REGISTRO_H
#define REGISTRO_H

#include <QWidget>

namespace Ui {
class Registro;
}

class Registro : public QWidget
{
    Q_OBJECT

public:
    explicit Registro(QWidget *parent = 0);
    ~Registro();

    void configurarWidgets();

protected:
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *);
    void resizeEvent(QResizeEvent *);

    void mousePressEvent(QMouseEvent *e);

private:
    Ui::Registro *ui;

};

#endif // REGISTRO_H
