#ifndef QJDSTACKWIDGET_H
#define QJDSTACKWIDGET_H

#include <QWidget>
#include <QStackedWidget>
#include <QHash>
#include "readxmlmodule.h"
#include "creatui.h"
//#include "writejob.h"

class qjdStackWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit qjdStackWidget(QWidget *parent = 0);
    QHash<QString, int> hashIndex;

signals:

public slots:
    void handleMousePress(QString);  //这个index和实际的stack有出入哦,如何换算呢？

    void addFlowWidget(QString,QString);
    void getJobXMLfileName(const QString);

private:
    QString jobFileName;

    readXMLModule *xmlModule;
    creatUI *creat;

    void creatWidget(QString, QString);
};

#endif // QJDSTACKWIDGET_H
