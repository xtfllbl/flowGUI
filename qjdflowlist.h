#ifndef QJDFLOWLIST_H
#define QJDFLOWLIST_H

#include <QListWidget>
#include <QWidget>
//#include <QDragEnterEvent>
//#include <QDropEvent>
class QJDFlowList : public QListWidget
{
    Q_OBJECT
public:
    explicit QJDFlowList(QWidget *parent = 0);

    QHash<QListWidgetItem *, int> hashItem;  // item , stack index
    int index;
signals:
//    void sigPressIndex(QString);

    void sigNewItemAdd();
    void sigItemChangePos();
    void sigJobXMLfileName(QString);
    void sigAddFlowWidget(QString,QString,int);

    void sigDeleteFlow(int);
//    void sigUpFlow(int stackIndex);  // 需要前项和后项么？不需要，往上挪就是了
//    void sigDownFlow(int stackIndex);
    void sigChangeStackWidgetIndex(int);

    void sigTurnOn(int);
    void sigTurnOff(int);

    void sigDrag(int stackIndex,int dragIndex,int allRow);  //插到原有这个位置的控件之前就可以了
public slots:
    void addFlow(QString,QString);
    void creatJobXML();  // 需要人为的创建

//    void upFlow();
//    void downFlow();
//    void delFlow();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

    QMenu *menu;
    QAction *actTurn;
    QAction *actDel;
//    QAction *actTer;
    void showContextMenu(QPoint );
private slots:
    void itemChangeSlot(int row);

    void actTurnSlot();
    void actDelSlot();

private:
    QList<QListWidgetItem *> itemList;

    /// 拖拽
//    void startDrag();
    QPoint startPos;
    int mousePressIndex;
    QListWidgetItem *dragItem;
    bool hasMousePressed;

    void insertHashValue(int,QListWidgetItem *);
    void resetHashValue(int,int,QListWidgetItem *);

};

#endif // QJDFLOWLIST_H
