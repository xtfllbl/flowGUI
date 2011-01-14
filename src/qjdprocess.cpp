#include "qjdprocess.h"

QJDProcess::QJDProcess(QObject *parent) :
    QProcess(parent)
{
    connect(this,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(_emitSigFinished(int,QProcess::ExitStatus)));
}

void QJDProcess::_emitSigFinished(int exit,QProcess::ExitStatus exitStatus)
{
    emit sigFinished(getPID(),exit,exitStatus);
}

void QJDProcess::savePID()
{
    _pid=this->pid();
}

int QJDProcess::getPID()
{
    return _pid;
}
