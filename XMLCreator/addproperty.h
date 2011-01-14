#ifndef ADDPROPERTY_H
#define ADDPROPERTY_H

#include <QWidget>
#include "ui_addproperty.h"
#include <QFile>
#include <QtXml>

namespace Ui {
    class AddProperty;
}

class AddProperty : public QWidget
{
    Q_OBJECT

public:
    explicit AddProperty(QWidget *parent = 0);
    ~AddProperty();
    Ui::AddProperty *ui;

    setModule(const QString name,const QString version);

private:
    bool initJob();
    void writeHead(QIODevice *file);
    bool readJob();

    QString jobFileName;
    QString moduleName;
    QString moduleID;
    bool isFileNameSet;
    QFile fileJob;
    QFile fileRead;
    QFile fileWrite;
    QFile fileDelete;
    QDomDocument doc;
    QDomElement moduleEle;

    QString errorStr;
    int errorLine;
    int errorColumn;
    bool alreadyReadJob;

private slots:
    void on_btnAdd_clicked();
};

#endif // ADDPROPERTY_H
