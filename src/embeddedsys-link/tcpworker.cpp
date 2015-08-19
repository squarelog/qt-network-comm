#include "tcpworker.h"

#include "mainwindow.h"

#include <QHash>
//#include "windows.h"

#include <telemetrydata.h>


extern void mydebug(QString funcinfo, QString str);

//tcpworker::tcpworker(QTcpServer *tcp_server, QObject *parent) :
tcpworker::tcpworker(QTcpSocket *tcp_socket, QObject *parent) :
    QObject(parent)
{

    //tcpSock_ = tcp_server->nextPendingConnection();
//    connect(this, SIGNAL(client_connected()), this,
  //          SLOT(send_telemetry_data()), Qt::DirectConnection);
    tcpSock_ = tcp_socket;
    connect(tcpSock_, SIGNAL(readyRead()), this, SLOT(start_read()));
    connect(tcpSock_, SIGNAL(readChannelFinished()), this, SLOT(connection_broken()));
    connect(tcpSock_, SIGNAL(aboutToClose()), this, SLOT(worker_exit()));

    emit client_connected();  

    tcpSock_->write("Server acknowledged client connection");
    quit_=false;

}


void tcpworker::send_telemetry_data()
{    
    QMutexLocker locker(&telemetryData::mutex_);
    QString telemetry_mesg = telemetryData::telemetry_;

    if(telemetry_mesg.isEmpty())
    {
        qDebug() << "Nothing to send to client.";
        return;
    }
    QString mesg = "writing message: " + telemetry_mesg + " to client";
    mydebug(Q_FUNC_INFO, mesg);
    tcpSock_->write(telemetry_mesg.toStdString().c_str());
    tcpSock_->flush();
}

void tcpworker::worker_exit()
{
    mydebug(Q_FUNC_INFO, "quitting.");
    quit_=true;
}

void tcpworker::connection_broken()
{
    mydebug(Q_FUNC_INFO, "client connection broken.");

    if(!quit_)
        tcpSock_->deleteLater();
    emit client_closed_connection();    
}


void tcpworker::start_read()
{

    /////////////////////////////////////////////
    QByteArray qstrbytes = tcpSock_->readAll();

    QString mesg = "received message from client: " + qstrbytes;
    mydebug(Q_FUNC_INFO, mesg);

    MainWindow::qCommandData_ = qstrbytes;

    emit client_issued_command();
}
