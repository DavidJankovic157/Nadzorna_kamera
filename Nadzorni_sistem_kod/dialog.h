#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include<QtMultimedia>
#include<QtMultimediaWidgets>
#include<wiringPi.h>
#include<QTimer>
#include<softPwm.h>

#define PIR 26
#define SG90 16


QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_pushButton_Start_clicked();

    void on_pushButton_Stop_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_clicked();



    void on_motor_levo_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_0_clicked();

private:
    Ui::Dialog *ui;

    QScopedPointer<QCamera>M_Camera;
    QScopedPointer<QCameraViewfinder>M_Viewfinder;

    void start_Camera();
    void stop_Camera();
    void motion_detected();
    void moveSG90(int pwmValue);
};
#endif // DIALOG_H
