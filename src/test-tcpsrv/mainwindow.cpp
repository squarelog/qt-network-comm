#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tcpworker.h"
#include "time.h"
#include "telemetrydata.h"

QHash <QString, int> MainWindow::telemetry_data_;
//QString MainWindow::qTelemetry_;
QString telemetryData::telemetry_;
QMutex telemetryData::mutex_;
QString MainWindow::qCommandData_;

void mydebug(QString funcinfo, QString str)
{
    qDebug() << funcinfo << "thread ID "
             << (int) QThread::currentThreadId()
             << str;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(tr("SPACE SCRAFT EMULATOR"));


    tcpListener_ = new QTcpServer(this);
    QObject::connect(tcpListener_, SIGNAL(newConnection()),
                     this, SLOT(handle_new_connection()));

    uint iport=7755;

    if(!tcpListener_->listen(QHostAddress::Any, iport)) {
        qDebug() << "Listener failed to start!";
    }
    else {
        qDebug() << "Listener started on port " << iport;
    }


    tcpTelemetryListener_ = new QTcpServer(this);
    QObject::connect(tcpTelemetryListener_ , SIGNAL(newConnection()),
                     this, SLOT(handle_telemetry_interrupts()));

    if(!tcpTelemetryListener_->listen(QHostAddress::Any, 7756)) {
        qDebug() << "Listener failed to start!";
    }
    else {
        qDebug() << "Listener started on port 7756";
    }

    for(int num=0; num < 5; num++)
    {
        QString format = "Format";
        format.append(QString("%1").arg(num));
        telemetry_data_.insert( format, num);
    }

    qDebug()<< "main thread ID" << (int) QThread::currentThreadId();

    num_connections_=0;
}

MainWindow::~MainWindow()
{
    delete tcpListener_;
    delete ui;
}

void MainWindow::handle_telemetry_interrupts()
{
    mydebug(Q_FUNC_INFO, "telemetry source has made a connection");
    tcpTelemetrySocket_ = tcpTelemetryListener_->nextPendingConnection();
    connect(tcpTelemetrySocket_, SIGNAL(readyRead()), this, SLOT(read_telemetry_data()));
    connect(tcpTelemetrySocket_, SIGNAL(disconnected()), this, SLOT(handle_telemetry_disconnect()));
    ui->telemetryFeedStatus->setText("Telemetry Source Status: connected");
}

void MainWindow::handle_telemetry_disconnect()
{
    ui->telemetryFeedStatus->setText("Telemetry Source Status: disconnected");
    tcpTelemetrySocket_->deleteLater();
}

void MainWindow::read_telemetry_data()
{
    QMutexLocker locker(&telemetryData::mutex_);
    QByteArray qstrbytes = tcpTelemetrySocket_->readAll();
    telemetryData::telemetry_ = qstrbytes;

    mydebug(Q_FUNC_INFO, qstrbytes);


    ui->telemetryDataDisplay->setText(qstrbytes);
    emit new_data_interrupt();
}

void MainWindow::display_commands()
{
    ui->commandsIssuedDisplay->setText(qCommandData_);
}


void MainWindow::handle_TE_disconnect()
{
    num_connections_--;

    mydebug(Q_FUNC_INFO, "TEClient disconnected");
    QString status_mesg = "TEClient disconnected.\nConnected clients count = ";
    status_mesg.append(QString::number(num_connections_));

    ui->teStatus->setText(status_mesg);
    //worker_->deleteLater();

}

void MainWindow::handle_new_connection()
{
    mydebug(Q_FUNC_INFO, "TEClient disconnected");

    tcpCommandSocket_ = tcpListener_->nextPendingConnection();

    tcp_thread_ = new QThread;    
    //worker_ = new tcpworker(tcpCommandSocket_);
    worker_ = new tcpworker(tcpCommandSocket_);
    worker_->moveToThread(tcp_thread_);

    //connect(worker_, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    //connect(tcp_thread_, SIGNAL(started()), worker_, SLOT(send_telemetry_data()));
    connect(worker_, SIGNAL(client_closed_connection()), tcp_thread_, SLOT(quit()));
    connect(worker_, SIGNAL(client_closed_connection()), this, SLOT(handle_TE_disconnect()));
    connect(worker_, SIGNAL(client_closed_connection()), worker_, SLOT(deleteLater()));
    connect(tcp_thread_, SIGNAL(finished()), tcp_thread_, SLOT(deleteLater()));
    //connect(this, SIGNAL(destroyed()), worker_, SLOT(worker_exit()));

    connect(this,
            SIGNAL(new_data_interrupt()),
            worker_,
            SLOT(send_telemetry_data())
            );

    connect(worker_,
            SIGNAL(client_issued_command()),
            this,
            SLOT(display_commands())
            );


    tcp_thread_->start();
    num_connections_++;
    QString status_mesg = "TEClient connected.\nConnected clients count = ";
    status_mesg.append(QString::number(num_connections_));
    ui->teStatus->setText(status_mesg);

}
