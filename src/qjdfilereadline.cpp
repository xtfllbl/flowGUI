#include "qjdfilereadline.h"
#include <QLayout>
#include <QFileDialog>
#include <QTime>
#include <QDebug>

QJDFileReadLine::QJDFileReadLine(QWidget *parent) :
    QWidget(parent)
{    
    QString a;
    _id=setRandString(a);

    lineEdit=new QLineEdit;
    fileButton=new QToolButton;
    fileButton->setText("...");
    fileButton->setIcon(QIcon(":/images/filesave.png"));
    connect(fileButton,SIGNAL(pressed()),this,SLOT(fileButtonPressed()));

    fileButton->setMaximumHeight(lineEdit->height());

//    qDebug()<<"height::"<<fileButton->height()<<lineEdit->height();
    QHBoxLayout *layout=new QHBoxLayout;
    layout->addWidget(lineEdit);
    layout->addWidget(fileButton);
    this->setLayout(layout);

    connect(lineEdit,SIGNAL(editingFinished()),this,SLOT(emitEditFinished()));
}

void  QJDFileReadLine::fileButtonPressed()
{
    QString _a=QFileDialog::getOpenFileName(this, tr("Choose File"), "/home", tr("Any Files (*)"));
    if(_a!="")
    {
        fileName=_a;
        lineEdit->setText(fileName);
    }
}

QString QJDFileReadLine::setRandString(QString & randString)
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

QString QJDFileReadLine::id()
{
    return _id;
}

void QJDFileReadLine::emitEditFinished()
{
    emit sigLineEditChanged(lineEdit->text(),this->id());
}

void QJDFileReadLine::setText(QString text)
{
    lineEdit->setText(text);
}
