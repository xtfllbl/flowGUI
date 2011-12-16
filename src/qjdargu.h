#ifndef QJDARGU_H
#define QJDARGU_H

#include <QObject>
#include <QApplication>

class QJDArgu : public QObject
{
    Q_OBJECT
public:
    explicit QJDArgu(QObject *parent = 0);

    QString FlowProcess;
    int HistoryLogRefreshInterval;

    QString flowGuiPath;
signals:

public slots:
};

#endif // QJDARGU_H
