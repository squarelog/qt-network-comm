#ifndef TCPWORKER_H
#define TCPWORKER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>
#include <QThread>

class tcpworker : public QObject
{
    Q_OBJECT
public:
    //explicit tcpworker(QTcpServer *tcp_server, QObject *parent = 0);
    explicit tcpworker(QTcpSocket *tcp_socket, QObject *parent = 0);
    //~tcpworker();
signals:
    void client_closed_connection();

public slots:
    void start_read();
    void connection_broken();
    void send_telemetry_data();
    void worker_exit();

signals:
    //void error(QString err);
    void client_connected();
    void client_issued_command();

private:
    QTcpSocket *tcpSock_;
    bool quit_;
};

#endif // TCPWORKER_H
