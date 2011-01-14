#include "qjdgroupbox.h"
#include <QTime>
QJDGroupBox::QJDGroupBox(QWidget *parent) :
    QGroupBox(parent)
{
    QString a;
    _id=setRandString(a);

    connect(this,SIGNAL(clicked(bool)),this,SLOT(emitGroupChecked(bool)));
}

QString QJDGroupBox::setRandString(QString & randString)
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

QString QJDGroupBox::id()
{
    return _id;
}

void QJDGroupBox::emitGroupChecked(bool checked)
{
    emit sigGroupChecked(checked,this->id());
}


//void QJDGroupBox::setChecked(bool checked)
//{
//    Q_D(QJDGroupBox);
//    if (d->checkable && b != d->checked)
//    {
//        update();
//        d->checked = b;
//        d->_q_setChildrenEnabled(b);
//        emit toggled(b);
//    }
//}

//void QJDGroupBox::_q_setChildrenEnabled(bool b)
//{
//    Q_Q(QJDGroupBox);
//    QObjectList childList = q->children();
//    for (int i = 0; i < childList.size(); ++i) {
//        QObject *o = childList.at(i);
//        if (o->isWidgetType()) {
//            QWidget *w = static_cast<QWidget *>(o);
//            if (b) {
//                if (!w->testAttribute(Qt::WA_ForceDisabled))
//                    w->setEnabled(true);
//            } else {
//                if (w->isEnabled()) {
//                    w->setEnabled(false);
//                    w->setAttribute(Qt::WA_ForceDisabled, false);
//                }
//            }
//        }
//    }
//}
