#ifndef QJDSTACKWIDGET_H
#define QJDSTACKWIDGET_H

#include <QWidget>
#include <QStackedWidget>
#include <QHash>
#include "readxmlmodule.h"
#include "creatui.h"
//#include "writejob.h"

class QJDStackWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit QJDStackWidget(QWidget *parent = 0);
//    QHash<QString, int> hashIndex; //没有使用
    QHash<creatUI *,int> hashCreat;  //将creat和index联系起来，找的到，还能删除？？？
    int stackIndex;

signals:

public slots:
//    void handleMousePress(QString);  //这个index和实际的stack有出入哦,如何换算呢？

    void addFlowWidget(QString,QString);
    void getJobXMLfileName(const QString);
    void delFlowWidget(int);
//    void moveUpFlowWidget(int);
//    void moveDownFlowWidget(int);

    void turnOnWidget(int);
    void turnOffWidget(int);

    void dragWidget(int,int,int);

private:
    QString jobFileName;

    readXMLModule *xmlModule;
    creatUI *creat;

    void creatWidget(QString, QString);
};

#endif // QJDSTACKWIDGET_H
