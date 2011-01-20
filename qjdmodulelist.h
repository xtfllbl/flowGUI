#ifndef QJDLIST_H
#define QJDLIST_H

#include <QTreeWidget>
#include "readxmlindex.h"

// 可以在当中加入箭头widget之类，比单个控件组合方便
// 这个难道要实现拖拽？其实也不难实现

class QJDModuleList : public QTreeWidget
{
    Q_OBJECT
public:
    explicit QJDModuleList(QWidget *parent = 0);

    readXMLIndex *indexXML;
signals:
    void sigAddFlow(QString);
    void sigAddFlowList(QString,QString);
protected:
private:
    void performDrag();
    void analysisIndexXML();
    void addModule();
    QPoint startPos;
    QFile fileXMLIndex;
private slots:
    void handleItemDoubleClickedEvent(QTreeWidgetItem *,int col);

};

#endif // QJDLIST_H
