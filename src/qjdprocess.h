#ifndef QJDPROCESS_H
#define QJDPROCESS_H

#include <QProcess>

class QJDProcess : public QProcess
{
    Q_OBJECT
public:
    explicit QJDProcess(QObject *parent = 0);
private:
    int _pid;
signals:

    void sigFinished(int pid, int exitCode, QProcess::ExitStatus exitStatus);
public slots:
    void savePID();
    int getPID();
private slots:
    void _emitSigFinished(int,QProcess::ExitStatus);
};

#endif // QJDPROCESS_H
