#include "qjdspinbox.h"
#include <QTime>

QJDSpinBox::QJDSpinBox(QWidget *parent) :
    QSpinBox(parent)
{
    QString a;
    _id=setRandString(a);
    connect(this,SIGNAL(valueChanged(QString)),this,SLOT(emitValueChanged(QString)));

    setRange(-100000,100000);  // 可在设置创建时再做改动
}

QString QJDSpinBox::setRandString(QString & randString)
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

QString QJDSpinBox::id()
{
    return _id;
}

void QJDSpinBox::emitValueChanged(QString value)
{
    emit sigValueChanged(value,this->id());
}
