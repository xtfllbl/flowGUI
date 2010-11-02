#ifndef QJDFLOWLIST_H
#define QJDFLOWLIST_H

#include <QListWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
class qjdFlowList : public QListWidget
{
    Q_OBJECT
public:
    explicit qjdFlowList(QWidget *parent = 0);

signals:
    void sigDroped(QString);
    void sigPressIndex(QString);

    void sigNewItemAdd();
    void sigItemChangePos();
    void sigJobXMLfileName(QString);

public slots:
    void deleteFlow();
    void addFlow(QString);
    void creatJobXML();  // 需要人为的创建
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:

    QPoint startPos;
    QMenu *menu;
    QAction *actDelete;

};

#endif // QJDFLOWLIST_H
