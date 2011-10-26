#ifndef QJDTEXTBROWSER_H
#define QJDTEXTBROWSER_H

#include <QMainWindow>
#include <QtGui>
#include "qjdhighlighter.h"

namespace Ui {
    class QJDTextBrowser;
}

class QJDTextBrowser : public QMainWindow
{
    Q_OBJECT

public:
    explicit QJDTextBrowser(QWidget *parent = 0);
    ~QJDTextBrowser();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QJDTextBrowser *ui;

    QJDHighLighter *highLighter;
    QLineEdit *find_lineEdit;
    bool isUntitled;//是否是无标题
    QString curFile;//当前文件的标题
    QString currentFileName;
    QTimer *timer;

    void initConnect();//初始化信号与槽的连接
//    void IsSave();//提示是否保存文件
    bool loadFile(const QString&fileName);//加载文件
    void setCurrentFile(const QString&fileName);//设置当前的文件
//    bool saveFile(const QString& fileName);//保存文件

    void resizeEvent(QResizeEvent *);
    QImage _image;

private slots:
    void doCursorChanged();//
//    void doNew();//新建文件信号的槽
    void doOpen();
    void doClose();//关闭
//    void doSave();//保存
//    void doSaveAs();//另存为
    void doQuit();//退出
//    void doUndo();//撤销
//    void doCut();//剪切
    void doCopy();
//    void doPaste();
    void doAll();
//    void doSetFont();
//    void doSetColor();
//    void doAbout();
//    void doFind();
//    void show_findText();
public slots:
    void openLog(QString fileName);
private slots:
    void refreashFile();
};

#endif // QJDTEXTBROWSER_H
