#ifndef QJDLIST_H
#define QJDLIST_H

#include <QTableWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include "readxmlindex.h"

// 可以在当中加入箭头widget之类，比单个控件组合方便
// 这个难道要实现拖拽？其实也不难实现

class qjdModuleList : public QTableWidget
{
    Q_OBJECT
public:
    explicit qjdModuleList(QWidget *parent = 0);

    readXMLIndex *indexXML;
signals:
    void sigAddFlow(QString);
    void sigAddFlowWidget(QString,QString);
protected:
    void mousePressEvent(QMouseEvent *event);
//    void mouseDoubleClickEvent(QMouseEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);
//    void dragEnterEvent(QDragEnterEvent *event);
//    void dragMoveEvent(QDragMoveEvent *event);
//    void dropEvent(QDropEvent *event);

private:
    void performDrag();
    void analysisIndexXML();
    void addModule();
    QPoint startPos;
    QFile fileXMLIndex;
private slots:
    void handleItemDoubleClickedEvent(QTableWidgetItem *);

};

#endif // QJDLIST_H
