#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QMainWindow>
#include <QSerialPort>

namespace Ui {
class Controller;
}

class Controller : public QMainWindow
{
    Q_OBJECT

public:
    explicit Controller(QWidget *parent = 0);
    ~Controller();

private:
    Ui::Controller *ui;
    QSerialPort *controller;
};

#endif // CONTROLLER_H
