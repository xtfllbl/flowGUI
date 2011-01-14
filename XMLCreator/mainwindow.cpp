#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <addproperty.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    moduleName.clear();
    moduleVersion.clear();

    connect(ui->lineModule,SIGNAL(textEdited(QString)),this,SLOT(slotSetModuleName()));
    connect(ui->lineVersion,SIGNAL(textEdited(QString)),this,SLOT(slotSetModuleName()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnAddProperty_clicked()
{
    AddProperty *add=new AddProperty();

    add->show();
}

void MainWindow::slotSetModuleName()
{
    moduleName=ui->lineModule->text();
    moduleVersion=ui->lineVersion->text();
}


void MainWindow::on_lineModule_textEdited(QString a )
{
    qDebug()<<"on_lineModule_textEdited"<<a;
}
