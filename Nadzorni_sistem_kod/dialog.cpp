#include "dialog.h"
#include "ui_dialog.h"

int currentPosition=4;

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    wiringPiSetup();
    softPwmCreate(SG90,0,200);


    for (const QCameraInfo &info : QCameraInfo::availableCameras())
    {
        qDebug() << "Available camera: " << info.description();
    }

    // Inicijalizacija kamere
    M_Camera.reset(new QCamera(QCameraInfo::defaultCamera()));
    qDebug() << "Default camera: " << QCameraInfo::defaultCamera().description();

    // Kreiranje viewfinder-a
    M_Viewfinder.reset(new QCameraViewfinder(this));

    // Dodavanje viewfinder-a u layout
    ui->verticalLayout->addWidget(M_Viewfinder.data());

    // Povezivanje kamere sa viewfinder-om
    M_Camera->setViewfinder(M_Viewfinder.data());

    // Postavljanje video izlaza
    QCameraViewfinderSettings viewfinderSettings;
    viewfinderSettings.setResolution(640, 480);
    viewfinderSettings.setMinimumFrameRate(15.0);
    viewfinderSettings.setMaximumFrameRate(30.0);
    M_Camera->setViewfinderSettings(viewfinderSettings);

    // Proveri podržane formate
    qDebug() << "Supported viewfinder resolutions:";
    for (const QSize &resolution : M_Camera->supportedViewfinderResolutions())
    {
        qDebug() << resolution;
    }

    // Inicijalizacija QTimer-a
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Dialog::motion_detected);
    timer->start(1000); // Proverava svake 100 ms



}

Dialog::~Dialog()
{
    delete ui;
}
void Dialog::moveSG90(int pwmValue)
{
    softPwmWrite(SG90,pwmValue);
    delay(200);
    softPwmWrite(SG90,0);

}

void Dialog::start_Camera()
{
    qDebug() << "Starting camera...";
    M_Camera->start();
    qDebug() << "Camera state: " << M_Camera->state();
}

void Dialog::stop_Camera()
{
    qDebug() << "Stopping camera...";
    M_Camera->stop();
    qDebug() << "Camera state: " << M_Camera->state();
}

void Dialog::motion_detected()
{
    //QTimer *recordingTimer = new QTimer(this);
    //connect(recordingTimer, &QTimer::timeout, this, &Dialog::stop_Camera);


    if(ui->radioButton_2->isChecked()){
    
		if(digitalRead(PIR)==HIGH){
         if (M_Camera->state() != QCamera::ActiveState) {
                start_Camera();

                QTimer::singleShot(20000, this, &Dialog::stop_Camera);
             }
		}
    }

}


void Dialog::on_pushButton_Start_clicked()
{
    start_Camera();
}

void Dialog::on_pushButton_Stop_clicked()
{
	stop_Camera();
}

void Dialog::on_radioButton_2_clicked()
{
	ui->pushButton_Start->setDisabled(true);
	ui->pushButton_Stop->setDisabled(true);
	stop_Camera();
}
void Dialog::on_radioButton_clicked()
{
	ui->pushButton_Start->setDisabled(false);
	ui->pushButton_Stop->setDisabled(false);
}


void Dialog::on_motor_levo_clicked()
{
	currentPosition+=1;
    if(currentPosition>=25)
		currentPosition=25;
     qDebug()<<"Trenutna pozivija:  " <<currentPosition;
     moveSG90(currentPosition);
}


void Dialog::on_pushButton_2_clicked()
{
    currentPosition-=1;
    if(currentPosition<=4)
		currentPosition=4;
    qDebug()<<"Trenutna pozivija:  " <<currentPosition;
    moveSG90(currentPosition);
}


void Dialog::on_pushButton_0_clicked()
{
    currentPosition=15;
    qDebug()<<"Trenutna pozicija: " <<currentPosition;
    moveSG90(currentPosition);
}

