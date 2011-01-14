#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QString moduleName;
    QString moduleVersion;

private slots:
    void on_lineModule_textEdited(QString );
    void on_btnAddProperty_clicked();
    void slotSetModuleName();
};

#endif // MAINWINDOW_H
