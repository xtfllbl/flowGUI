#include "qjdfilesaveline.h"
#include <QLayout>
#include <QFileDialog>
#include <QTime>

QJDFileSaveLine::QJDFileSaveLine(QWidget *parent) :
    QWidget(parent)
{
    QString a;
    _id=setRandString(a);

    lineEdit=new QLineEdit;
    fileButton=new QToolButton;
    fileButton->setText("...");
    fileButton->setIcon(QIcon(":/images/filesave.png"));
    connect(fileButton,SIGNAL(pressed()),this,SLOT(fileButtonPressed()));

    QHBoxLayout *layout=new QHBoxLayout;
    layout->addWidget(lineEdit);
    layout->addWidget(fileButton);
    this->setLayout(layout);

    connect(lineEdit,SIGNAL(editingFinished()),this,SLOT(emitEditFinished()));
}

void  QJDFileSaveLine::fileButtonPressed()
{
    QString _a=QFileDialog::getSaveFileName(this, tr("Save To File "), "/home", tr("Any Files (*)"));
    if(_a!="")
    {
        fileName=_a;
        lineEdit->setText(fileName);
    }
}

QString QJDFileSaveLine::setRandString(QString & randString)
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

QString QJDFileSaveLine::id()
{
    return _id;
}

void QJDFileSaveLine::emitEditFinished()
{
    emit sigLineEditChanged(lineEdit->text(),this->id());
}

void QJDFileSaveLine::setText(QString text)
{
    lineEdit->setText(text);
}

