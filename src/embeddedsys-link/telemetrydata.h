#ifndef TELEMETRYDATA_H
#define TELEMETRYDATA_H

#include <QMutex>
#include <QMutexLocker>

class telemetryData
{
public:
    telemetryData();
    static QString telemetry_;
    static QMutex mutex_;

};

#endif // TELEMETRYDATA_H
