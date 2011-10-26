#include "qjdfilecombobox.h"
#include <QTime>

QJDFileComboBox::QJDFileComboBox(QWidget *parent) :
    QComboBox(parent)
{
    QString a;
    _id=setRandString(a);

    connect(this,SIGNAL(currentIndexChanged(QString)),this,SLOT(emitIndexChanged(QString)));
}

QString QJDFileComboBox::setRandString(QString & randString)
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

QString QJDFileComboBox::id()
{
    return _id;
}

void QJDFileComboBox::emitIndexChanged(QString comboText)
{
    emit sigIndexChanged(comboText, this->id());
}
