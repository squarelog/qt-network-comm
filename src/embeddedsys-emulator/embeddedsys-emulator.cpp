#include "embeddedsys-emulator.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <time.h>

#include "windows.h"
#include "windows.h"


EmbeddedSysEmu::EmbeddedSysEmu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EmbeddedSysEmu)
{

    qsrand(time(NULL));
    int randomValue = qrand();

    client_ID_ = QString::number(randomValue);

    ui->setupUi(this);
    QString title = "EMBEDDED EMULATOR ";
    setWindowTitle(title);

    QRect button_geo1(0,0,15,15);
    QPushButton *connect_button = new QPushButton("&Connect", this);
    connect_button->setGeometry(button_geo1);
    ui->mainToolBar->addWidget(connect_button);
    connect( connect_button, SIGNAL(clicked()), this, SLOT(connect_to_SCE()) );

    QRect button_geo2(30,0,15,15);
    QPushButton *send_button = new QPushButton("&Send telemetry data", this);
    send_button->setGeometry(button_geo2);
    ui->mainToolBar->addWidget(send_button);
    connect( send_button, SIGNAL(clicked()), this, SLOT(send_data()) );

    QRect button_geo3(0,60,15,15);
    QPushButton *disconnect_button = new QPushButton("&Disconnect", this);
    disconnect_button->setGeometry(button_geo3);
    ui->mainToolBar->addWidget(disconnect_button );
    connect( disconnect_button , SIGNAL(clicked()), this, SLOT(disconnect_from_SCE()) );

    mesg_count_ = 0;
    connected_ = false;
    tcpsocket_ = NULL;


    scheduler_ = new QTimer(this);
    scheduler_->setInterval(10000);

    connect(scheduler_, SIGNAL(timeout()), this, SLOT(send_data()));
    scheduler_->start();

    frequency_= 4000;
    ui->outputFrequency->setText(QString("%1").arg(frequency_));
    connect(ui->setFrequency, SIGNAL(clicked()), this, SLOT(set_frequency()));
    connect(ui->outputFrequency, SIGNAL(textEdited(QString)), this, SLOT(update_frequency(QString)));
    connect(ui->outputFrequency, SIGNAL(textChanged(QString)), this, SLOT(update_frequency(QString)));

    connect(ui->setIPaddress, SIGNAL(clicked()), this, SLOT(set_IPaddress()));
    connect(ui->ipaddress, SIGNAL(textEdited(QString)), this, SLOT(update_IPaddress(QString)));
    connect(ui->ipaddress, SIGNAL(textChanged(QString)), this, SLOT(update_IPaddress(QString)));


}



void EmbeddedSysEmu::update_IPaddress(QString ipaddress)
{
    ipaddress_ = ipaddress;
}

void EmbeddedSysEmu::set_IPaddress()
{

}



void EmbeddedSysEmu::update_frequency(QString frequency)
{
    frequency_ = frequency.toInt();
}

void EmbeddedSysEmu::set_frequency()
{
    scheduler_->setInterval(frequency_);

}

void EmbeddedSysEmu::disconnect_from_SCE()
{
    tcpsocket_->disconnectFromHost();
    connected_ = false;
    delete tcpsocket_;
    tcpsocket_ = NULL;
}

bool EmbeddedSysEmu::connect_to_SCE()
{
    QHostAddress addr = QHostAddress::LocalHost;
    if(!ipaddress_.isEmpty())
        addr.setAddress(ipaddress_);
    //Set the default destination port
    quint32 iport = 7756;

    tcpsocket_ = new QTcpSocket();

    tcpsocket_->connectToHost(addr, iport );
    if(tcpsocket_->waitForConnected(30000))
    {
        connected_ = true;
        connect(tcpsocket_, SIGNAL(readyRead()), this, SLOT(start_read()));
    }


    return true;
}


void EmbeddedSysEmu::send_data()
{
    if( !connected_)
    {
        qDebug() << "not connected to host";
        return;
    }
    mesg_ = "Telemetry client sent: Telemetry data ";
    mesg_.append(QString("%1").arg(mesg_count_));
    qDebug() << "size of message " << mesg_.size() << " " << mesg_.length();
    qDebug() << "message: " << mesg_;

    int numbytes_written =  tcpsocket_->write( mesg_.toStdString().c_str(), mesg_.length());
    tcpsocket_->flush();
    ui->telemetryDisplay->setText(mesg_);
    qDebug() << "wrote message " << mesg_count_ << " to sce. "
                 << numbytes_written << " bytes written.";
    //if( tcpsocket_->waitForBytesWritten())
    {
        if(mesg_count_ > 7000)
            mesg_count_ = 0;
        else
            mesg_count_++;
    }
}

void EmbeddedSysEmu::start_read()
{
    /////////////////////////////////////////////
    QByteArray qstrbytes = tcpsocket_->readAll();

    qDebug() << "recieved from SCE: " << qstrbytes;
    qDebug() << "===============================";

}

EmbeddedSysEmu::~EmbeddedSysEmu()
{
    qDebug() << "Disconnecting from SCE.";
    delete ui;
    delete tcpsocket_;
}
