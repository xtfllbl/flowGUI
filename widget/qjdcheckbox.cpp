#include "qjdcheckbox.h"
#include <QTime>

QJDCheckBox::QJDCheckBox(QWidget *parent) :
    QCheckBox(parent)
{
    QString a;
    _id=setRandString(a);

    connect(this,SIGNAL(stateChanged(int)),this,SLOT(emitStateChanged(int)));

}

QString QJDCheckBox::setRandString(QString & randString)
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

QString QJDCheckBox::id()
{
    return _id;
}

void QJDCheckBox::emitStateChanged(int state)
{
    if(state==0)
    {
        //unckecked
        emit sigStateChanged("unchecked",this->id());
    }
    if(state==2)
    {
        //checked
        emit sigStateChanged("checked",this->id());
    }
}
