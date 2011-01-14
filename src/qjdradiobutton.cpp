#include "qjdradiobutton.h"
#include <QTime>
#include <QDebug>

QJDRadioButton::QJDRadioButton(QWidget *parent) :
    QRadioButton(parent)
{
    QString a;
    _id=setRandString(a);

    /// 没有相应的信号,无法发送id之类的。。。,但是有clicked之类的
    connect(this,SIGNAL(clicked(bool)),this,SLOT(emitStateChanged(bool)));

}

QString QJDRadioButton::setRandString(QString & randString)
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

QString QJDRadioButton::id()
{
    return _id;
}

void QJDRadioButton::emitStateChanged(bool checked)
{
    if(checked==true)
    {
        qDebug()<<"checked"<<this->id();
        emit sigRadioChecked("checked",this->id(),otherID);   /// 如何获得groupbox的信号呢？
    }
}

void QJDRadioButton::setOtherID(QString id)
{
    otherID=id;
}
