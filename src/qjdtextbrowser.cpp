#include "qjdtextbrowser.h"
#include "ui_qjdtextbrowser.h"
#include <QtGui>
#include <QTimer>

//TODO: 定时刷新   //todo
//TODO: 不得修改  //y
//TODO: 多tab  //n

QJDTextBrowser::QJDTextBrowser(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::QJDTextBrowser)
{
    ui->setupUi(this);

    _image.load(":/images/window.png");
    setAutoFillBackground(true);

    QFile file(":/styles/stylesheet.qss");
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString s = in.readAll();

    this->setStyleSheet(s);

    isUntitled=true;
    curFile=tr("new.txt");   //永远不会有这个时刻
    setWindowTitle(curFile+tr("[*]"));
    initConnect();
    highLighter = new QJDHighLighter(ui->textEdit->document());
    currentFileName.clear();
    timer=new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(refreashFile()));
}

QJDTextBrowser::~QJDTextBrowser()
{
    delete ui;
}

void QJDTextBrowser::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    QPalette pal(palette());
    pal.setBrush(QPalette::Window,
                QBrush(_image.scaled(event->size(), Qt::IgnoreAspectRatio,
                        Qt::SmoothTransformation)));
    setPalette(pal);
}
void QJDTextBrowser::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void QJDTextBrowser::initConnect()
{
    connect(ui->textEdit,SIGNAL(cursorPositionChanged()),this,SLOT(doCursorChanged()));
    connect(ui->action_Open,SIGNAL(triggered()),this,SLOT(doOpen()));
    connect(ui->action_Exit,SIGNAL(triggered()),this,SLOT(doQuit()));
    connect(ui->action_Copy,SIGNAL(triggered()),this,SLOT(doCopy()));
    connect(ui->action_All,SIGNAL(triggered()),this,SLOT(doAll()));
}

/*
 * 函数名称：捕获鼠标改变
 * 函数功能：当光标的位置改变时，更新状态栏页、列信息
 */
void QJDTextBrowser::doCursorChanged()
{
    int pageNum=ui->textEdit->document()->pageCount();//页计数
    const QTextCursor cursor=ui->textEdit->textCursor();//获取当前的可见光标
    int colNum=cursor.columnNumber();//列计数
    int rowNum=ui->textEdit->document()->blockCount();// 行计数
    ui->label_1->setText(tr("Page:%1 Line:%2 Col:%3").arg(pageNum).arg(rowNum).arg(colNum));
}

/*
 * 函数名称：打开操作
 * 函数功能：打开文件，获取文件名和文件内容
 */
void QJDTextBrowser::doOpen()
{
    //获取打开路径的文件名称
    QString fileName=QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())
    {
        if(loadFile(fileName))//加载文件
        {
            ui->label_2->setText(tr("Success"));
        }
    }
    ui->textEdit->setVisible(true);
}

void QJDTextBrowser::openLog(QString fileName)
{
    if(!fileName.isEmpty())
    {
        if(loadFile(fileName))//加载文件
        {
            ui->label_2->setText(tr("Success"));
        }
    }
    ui->textEdit->setVisible(true);
}

void QJDTextBrowser::refreashFile()
{
    QFile file(currentFileName);
    if(!file.open(QFile::ReadOnly|QFile::Text))
    {
        QMessageBox::warning(this,tr("Open"),tr("Can`t open file%1:\n%2").arg(currentFileName).arg(file.errorString()));
    }
    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->textEdit->setPlainText(in.readAll());//读取全部的文件内容
    QApplication::restoreOverrideCursor();
}


/*
 * 函数名称：加载文件
 * 函数功能：加载文件获取文件的内容,  并显示在文本编辑框内
 * 参数列表：fileName：要加载的文件名
 * @ruturn  ：true---成功  < false---失败
 */
bool QJDTextBrowser::loadFile(const QString &fileName)
{
    currentFileName=fileName;
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly|QFile::Text))
    {
        QMessageBox::warning(this,tr("Open"),tr("Can`t open file%1:\n%2")
                             .arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->textEdit->setPlainText(in.readAll());//读取全部的文件内容
    QApplication::restoreOverrideCursor();
    setCurrentFile(fileName);//设置为当前的文本

    return true;
}
/*
 * 函数名称：设置当前的文本
 * 函数功能：设置当前文本编辑器的状态
 * 参数列表：fileName：文件名称
 */
void QJDTextBrowser::setCurrentFile(const QString&fileName)
{
    //获取文件的标准路径
    curFile=QFileInfo(fileName).canonicalFilePath();
    isUntitled=false;
    setWindowTitle(curFile+tr("[*]"));//设置标题为当前的文本路径
    setWindowModified(false);
}

void QJDTextBrowser::doClose()
{
    ui->textEdit->setVisible(false);
    setWindowTitle(tr("QJDTextBrowser"));
}

void QJDTextBrowser::doQuit()
{
    doClose();
    this->close();
}

void QJDTextBrowser::doCopy()
{
    ui->textEdit->copy();
}

void QJDTextBrowser::doAll()
{
    ui->textEdit->selectAll();
}

/*
 * 函数名称：新建操作
 * 函数功能：建立一个新的文本文件
 */
//void QJDTextBrowser::doNew()
//{//如果文件改动，提示保存
//    IsSave();
//    isUntitled=true;
//    curFile=tr("new.txt");
//    setWindowTitle(curFile+tr("[*]"));
//    ui->textEdit->clear();
//    ui->textEdit->setVisible(true);//设置文本可见（必须）
//    setWindowModified(false);//文件名不变
//}
/*
 * 函数名称：是否保存
 * 函数功能：在操作时询问是否需要保存文件
 */
//void QJDTextBrowser::IsSave()
//{
//    if(ui->textEdit->document()->isModified())
//    {
//        int ret= QMessageBox::warning(this,tr("Warning"),tr("File need to be saved, save?")
//                                      ,QMessageBox::Save|QMessageBox::Cancel,QMessageBox::Save);

//        if(ret==QMessageBox::Save)
//        {
//            doSave();
//        }
//    }
//}

/*
 * 函数名称：保存操作
 * 函数功能：保存文本编辑器的内容
 */
//void QJDTextBrowser::doSave()
//{
//    if(isUntitled)
//    {
//        doSaveAs();//另存为操作
//    }
//    else
//    {
//        saveFile(curFile);//保存文件
//    }
//}
/*
 * 函数名称：另保存操作
 * 函数功能：另保存文本编辑器的内容
 */
//void QJDTextBrowser::doSaveAs()
//{
//    QString fileName=
//            QFileDialog::getSaveFileName(this,tr("Save As..."),curFile);
//    if(!fileName.isEmpty())
//    {
//        saveFile(fileName);
//    }
//}
/*
 * 函数名称：保存文件
 * 函数功能：保存文本编辑器的内容
 */

//bool QJDTextBrowser::saveFile(const QString &fileName)
//{
//    QFile file(fileName);
//    if(!file.open(QFile::WriteOnly|QFile::Text))
//    {
//        QMessageBox::warning(this,tr("Save"),tr("Can`t save file%1:\n%2")
//                             .arg(fileName).arg(file.errorString()));
//        return false;
//    }
//    QTextStream out(&file);
//    QApplication::setOverrideCursor(Qt::WaitCursor);
//    out<<ui->textEdit->toPlainText();//保存文本
//    QApplication::restoreOverrideCursor();
//    setCurrentFile(fileName);//设置为当前的文本
//    ui->label_2->setText(tr(" Saved"));
//    return true;
//}
/*
 * 函数名称：退出，剪切，撤销，复制，粘贴和全选
 * 函数功能：退出，剪切，撤销，复制，粘贴和全选文本编辑器的内容
 */
//void QJDTextBrowser::doCut()
//{
//    ui->textEdit->cut();
//}
//void QJDTextBrowser::doUndo()
//{
//    ui->textEdit->undo();
//}
//void QJDTextBrowser::doPaste()
//{
//    ui->textEdit->paste();
//}
//void QJDTextBrowser::doSetFont()
//{
//    bool ok;
//    const QFont& font=QFontDialog::getFont(&ok,
//                                           ui->textEdit->font(),
//                                           this,
//                                           tr("Font"));
//    if(ok)
//    {
//        ui->textEdit->setFont(font);
//    }
//}
//void QJDTextBrowser::doSetColor()
//{
//    QPalette palette=ui->textEdit->palette();
//    const QColor& color=
//            QColorDialog::getColor(palette.color(QPalette::Base),this);
//    if(color.isValid())
//    {
//        palette.setColor(QPalette::Base,color);
//        ui->textEdit->setPalette(palette);
//    }
//}
//void QJDTextBrowser::doAbout()
//{
//    QAction* action=qobject_cast<QAction*>(sender());
//    if(action==ui->action_About)
//        QMessageBox::aboutQt(this,tr("About QT"));

//}
//void QJDTextBrowser::doFind()
//{
//    QDialog *findDlg = new QDialog(this);
//    findDlg->setWindowTitle(tr("Find"));
//    find_lineEdit = new QLineEdit(findDlg);
//    QPushButton *find_Btn = new QPushButton(tr("Find Next"),findDlg);
//    QVBoxLayout* layout = new QVBoxLayout(findDlg);
//    layout->addWidget(find_lineEdit);
//    layout->addWidget(find_Btn);
//    findDlg ->show();
//    //显示对话框
//    connect(find_Btn,SIGNAL(clicked()),this,SLOT(show_findText()));
//    //设置“查找下一个”按钮的单击事件和其槽函数的关联
//}
//void QJDTextBrowser::show_findText()
//{
//    QString findText = find_lineEdit->text();
//    //获取行编辑器中的内容
//    ui->textEdit->find(findText,QTextDocument::FindBackward);
//    if(!ui->textEdit->find(findText,QTextDocument::FindBackward))
//    {
//        QMessageBox::warning(this,tr("Find"),tr("Can`t find %1") .arg(findText));
//    }
//}

