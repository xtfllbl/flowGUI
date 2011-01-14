#include "qjdlineedit.h"
#include <QTime>
#include <QDebug>
QJDLineEdit::QJDLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    QString a;
    _id=setRandString(a);
    _boxID.clear();

    connect(this,SIGNAL(editingFinished()),this,SLOT(emitEditFinished()));
}

QString QJDLineEdit::setRandString(QString & randString)
{
    int max = 15;
    QString tmp = QString("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWZYZ");
    QString str = QString();
    QTime t;
    t= QTime::currentTime();
    qsrand(t.msec()+t.second()*qrand());
    for(int i=0;i<max;i++)
    {
        int ir = qrand()%tmp.length();
        str[i] = tmp.at(ir);
    }
    randString = str;
    return randString;
}

QString QJDLineEdit::id()
{
    return _id;
}

void QJDLineEdit::emitEditFinished()
{
    if(_boxID.isEmpty())
    {
        qDebug()<<"普通lineedit改变";
        emit sigLineEditChanged(this->text(),this->id());
    }
    if(!_boxID.isEmpty())
    {
        qDebug()<<"隐藏lineedit改变";
        emit sigLineEditChanged(this->text(),_boxID,this->id());
    }
}

void QJDLineEdit::setBoxID(QString id)
{
    _boxID=id;
}
