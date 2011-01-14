#ifndef CREATUI_H
#define CREATUI_H
#include <QtGui>
#include "writejob.h"
#include "src/qjdradiobutton.h"

class creatUI: public QWidget
{
    Q_OBJECT

public:
    creatUI(QWidget *parent = 0);
    ~creatUI();

    void setUiLayout();

    QList<QWidget *> widgetList;

    int index;

private:
    writeJob *job;

    QVBoxLayout *finalLayout;
    QJDRadioButton *radio;
//    QScrollArea *scrollArea;
    int colNum;
    QString MODULE;
    QString MODULEID;

    QString setRandString(QString & randString);

    void resizeEvent(QResizeEvent *);
//    QString setRandString(QString & randString);

public slots:
    void getModuleName(QString name);
    void nodeData(const QString property,const QString desc,const QString datatype,
                  const QString min,const QString max,
                  const QString displaytype,const QString displaytext,const QString displayvalue,
                  const QStringList optiontext,const QStringList optionvalue,QString hidevalue,QString hidetype);
    void creatNormalWidget(const QString property,const QString desc,const QString datatype,
                      const QString min,const QString max,const QString displaytype,const QString displaytext,
                      const QString displayvalue,
                      const QStringList optiontext,const QStringList optionvalue);
    void creatHideCheckWidget(const QString property,const QString desc,const QString datatype,
                     const QString min,const QString max,const QString displaytype,const QString displaytext,
                     const QString displayvalue,
                     const QStringList optiontext,const QStringList optionvalue,QString showvalue);
    void creatHideRadioWidget(const QString property,const QString desc,const QString datatype,
                              const QString min,const QString max,const QString displaytype,const QString displaytext,
                              const QString displayvalue,
                              const QStringList optiontext,const QStringList optionvalue,QString showvalue);
    void finishCreatUI();
    void setJobFileName(QString);

    int indexOfStackWidget();
    void setIndex(int temp);

    void deleteJob();
//    void upJob();
//    void downJob();
    void turnOnJob();
    void turnOffJob();
    void dragJob(int,int,QString);
//    QString getLineEditText(QString text, QString id);
};

#endif // CREATUI_H







