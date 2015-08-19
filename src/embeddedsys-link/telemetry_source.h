#ifndef TELEMETRY_SOURCE_H
#define TELEMETRY_SOURCE_H

#include <QObject>

class telemetry_source : public QObject
{
    Q_OBJECT
public:
    explicit telemetry_source(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // TELEMETRY_SOURCE_H
