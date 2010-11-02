#ifndef CREATUI_H
#define CREATUI_H
#include <QtGui>
#include "writejob.h"

class creatUI: public QWidget
{
    Q_OBJECT

public:
    creatUI(QWidget *parent = 0);
    ~creatUI();

    void setUiLayout();

    QList<QWidget *> widgetList;

private:
    writeJob *job;

    QVBoxLayout *showLayout;
    QVBoxLayout *vLayout;
    QGridLayout *gLayout;
    QLayout *allLayout;
    QRadioButton *radio;
//    QScrollArea *scrollArea;
    int rowNum;
    int colNum;

    void resizeEvent(QResizeEvent *);

public slots:
    void nodeData(const QString property,const QString desc,const QString datatype,
                  const QString min,const QString max,const QString displaytype,const QString displayvalue,
                  const QStringList optiontext,const QStringList optionvalue,QString visible);
    void creatNormalWidget(const QString property,const QString desc,const QString datatype,
                      const QString min,const QString max,const QString displaytype,const QString displayvalue,
                      const QStringList optiontext,const QStringList optionvalue);
    void creatVisibleWidget(const QString property,const QString desc,const QString datatype,
                     const QString min,const QString max,const QString displaytype,const QString displayvalue,
                     const QStringList optiontext,const QStringList optionvalue,QString showvalue);
    void finishCreatUI();
    void setJobFileName(QString);
};

#endif // CREATUI_H







