#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <time.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    qsrand(time(NULL));
    int randomValue = qrand();

    client_ID_ = QString::number(randomValue);

    ui->setupUi(this);
    QString title = "CONSOLE CLIENT " + client_ID_;
    setWindowTitle(title);

    QRect button_geo1(0,0,15,15);
    QPushButton *connect_button = new QPushButton("&Connect", this);
    connect_button->setGeometry(button_geo1);
    ui->mainToolBar->addWidget(connect_button);
    connect( connect_button, SIGNAL(clicked()), this, SLOT(connect_to_SCE()) );

    QRect button_geo2(30,0,15,15);
    QPushButton *send_button = new QPushButton("&Send message", this);
    send_button->setGeometry(button_geo2);
    ui->mainToolBar->addWidget(send_button);
    connect( send_button, SIGNAL(clicked()), this, SLOT(send_message()) );

    QRect button_geo3(0,60,15,15);
    QPushButton *disconnect_button = new QPushButton("&Disconnect", this);
    disconnect_button->setGeometry(button_geo3);
    ui->mainToolBar->addWidget(disconnect_button );
    connect( disconnect_button , SIGNAL(clicked()), this, SLOT(disconnect_from_SCE()) );

    connect( this, SIGNAL(telemetry_data_updated()), this, SLOT(update_display()));


    connect(ui->ipaddressInput, SIGNAL(textEdited(QString)), this, SLOT(update_IPaddress(QString)));

    mesg_count_ = 0;
    connected_ = false;
    tcpsocket_ = NULL;
    telemetry_data_ = "No data yet";

}

void MainWindow::update_IPaddress(QString ipaddress)
{
    ipaddress_ = ipaddress;
}

void MainWindow::update_display()
{
    ui->telemetryData->setText(telemetry_data_);
}

void MainWindow::disconnect_from_SCE()
{
    tcpsocket_->disconnectFromHost();
    connected_ = false;
    delete tcpsocket_;
    tcpsocket_ = NULL;
    ui->connectionStatus->setText("Status: disconnected from SCE");
}

void MainWindow::SCE_disconnected()
{
    connected_ = false;
    tcpsocket_->deleteLater();
    tcpsocket_ = NULL;
    ui->connectionStatus->setText("Status: SCE disconnected");
}

bool MainWindow::connect_to_SCE()
{
    if(connected_) {
        return true;
    }

    QHostAddress addr = QHostAddress::LocalHost;
    if(!ipaddress_.isEmpty())
        addr.setAddress(ipaddress_);

    //Set the default destination port
    quint32 iport = 7755;

    tcpsocket_ = new QTcpSocket();

    tcpsocket_->connectToHost(addr, iport );
    if(tcpsocket_->waitForConnected(30000))
    {
        connected_ = true;
        connect(tcpsocket_, SIGNAL(readyRead()), this, SLOT(start_read()));
        connect(tcpsocket_, SIGNAL(disconnected()), this, SLOT(SCE_disconnected()));
        ui->connectionStatus->setText("Status: connected to SCE");
    }


    return true;
}

void MainWindow::send_message()
{
    if( !connected_) {
        qDebug() << "not connected to host";
        return;
    }
    mesg_ = "client " + client_ID_ + " message ";
    mesg_.append(QString("%1").arg(mesg_count_));
    qDebug() << "size of message " << mesg_.size() << " " << mesg_.length();
    qDebug() << "message: " << mesg_;

    int numbytes_written =  tcpsocket_->write( mesg_.toStdString().c_str(), mesg_.length());
    qDebug() << "wrote message " << mesg_count_ << " to sce. "
             << numbytes_written << " bytes written.";
    if( tcpsocket_->waitForBytesWritten())
    {
        mesg_count_++;
    }
}

void MainWindow::start_read()
{
    /////////////////////////////////////////////
    QByteArray qstrbytes = tcpsocket_->readAll();

    qDebug() << "recieved from SCE: " << qstrbytes;
    qDebug() << "===============================";

    telemetry_data_ = qstrbytes;

    emit telemetry_data_updated();
}

MainWindow::~MainWindow()
{
    disconnect(tcpsocket_, SIGNAL(disconnected()), this, SLOT(SCE_disconnected()));
    qDebug() << "Disconnecting from SCE.";
    delete ui;
    tcpsocket_->deleteLater();
}
