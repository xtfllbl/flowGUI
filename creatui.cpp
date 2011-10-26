#include "creatui.h"
#include <QTime>
#include "src/qjdcheckbox.h"
#include "src/qjdcombobox.h"
#include "src/qjdlineedit.h"
#include "src/qjdradiobutton.h"
#include "src/qjdspinbox.h"
#include "src/qjdgroupbox.h"
#include "src/qjdfilecombobox.h"
#include <QDir>

/// readxml->creatui->creatjoblist
creatUI::creatUI(QWidget *parent) :
        QWidget(parent)
{
    ///  需要通过解析xml文件来解析出这些东西
    finalLayout=new QVBoxLayout();
    widgetList.clear();

    job=new writeJob();    // 写入job，需要创建写xml步骤
    /// 如何判断flow的名称和

}

creatUI::~creatUI()
{}

void creatUI::getModuleName(const QString name)
{
    MODULE=name;
    QString a;
    MODULEID=setRandString(a);
    /// 需要此时就加上id
    job->setModuleName(MODULE,MODULEID);
}

QString creatUI::setRandString(QString & randString)
{
    int max = 15;
    QString tmp = QString("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWZYZ");
    QString str = QString();
    QTime t;
    t= QTime::currentTime();
    qsrand(t.msec()+t.second()*qrand());
    for(int i=0;i<max;i++)
    {
        int ir = qrand()%tmp.length();
        str[i] = tmp.at(ir);
    }
    randString = str;
    return randString;
}

void creatUI::nodeData(const QString property, const QString desc, const QString datatype,
                       const QString min, const QString max, const QString displaytype,const QString displaytext,
                       const QString displayvalue, const QStringList optiontext, const QStringList optionvalue,
                       const QString hidevalue, const QString hidetype)
{
    if(hidevalue.isEmpty())
    {
        creatNormalWidget(property,desc,datatype,min,max,displaytype,displaytext,displayvalue,
                          optiontext,optionvalue);
    }
    /// 辨别隐藏控件的种类
    if(!hidevalue.isEmpty() && hidetype=="checkbox")
    {
        creatHideCheckWidget(property,desc,datatype,min,max,displaytype,displaytext,displayvalue,
                             optiontext,optionvalue,hidevalue);
    }
    if(!hidevalue.isEmpty() && hidetype=="radiobutton")
    {
        creatHideRadioWidget(property,desc,datatype,min,max,displaytype,displaytext,displayvalue,
                             optiontext,optionvalue,hidevalue);
    }

}

void creatUI::creatNormalWidget(const QString property, const QString desc, const QString datatype,
                           const QString min, const QString max, const QString displaytype, const QString /*displaytext*/,
                           const QString displayvalue,
                           const QStringList optiontext, const QStringList optionvalue)
{
    QHBoxLayout *normalLayout=new QHBoxLayout();
    qDebug()<<"\ncreatUI:: creatNormalWidget";
    QLabel *proLabel=new QLabel;

    /// 带id
    QJDLineEdit *lineEdit=new QJDLineEdit;
    QJDCheckBox *checkBox=new QJDCheckBox;
    QJDComboBox *comboBox=new QJDComboBox;
    QJDFileComboBox *fileComboBox=new QJDFileComboBox;
    QJDSpinBox *spinBox=new QJDSpinBox;
    QJDGroupBox *groupBox=new QJDGroupBox;

    QString id;
    QList<QString> radioIDList;

    if(!property.isEmpty())   // 名称，作为label
    {
        proLabel->setText(property);
        proLabel->setFixedWidth(150);
        proLabel->setFrameShape(QFrame::Box);
        proLabel->setFrameShadow(QFrame::Sunken);
        normalLayout->addWidget(proLabel);
    }
    if(!desc.isEmpty())  //描述，tooltip
    {
        proLabel->setToolTip(desc);
    }

    /// 单独做一个范围函数
    setWidgetRange(displaytype,min,max);

    /// --------------------- 在此，需要和writejob联系起来，动态更改xml------------------ ///
    if(!displaytype.isEmpty())    // 创建控件的类型
    {
        /// ------------------------------------lineEdit-------------------------------//
        if(displaytype=="lineedit")
        {
            lineEdit->setMinimumWidth(200);
            lineEdit->setText(displayvalue);
//            qDebug()<<"lineedit id::"<<lineEdit->id();
            connect(lineEdit,SIGNAL(sigLineEditChanged(QString,QString)),job,SLOT(lineEditChanged(QString,QString)));
            id=lineEdit->id();
            normalLayout->addWidget(lineEdit);
            widgetList.append(lineEdit);
        }

        /// ------------------------------------checkBox-------------------------------//
        if(displaytype=="checkbox")
        {
            // 可能不需要之前的那个Label
            checkBox->setMinimumWidth(200);
            if(displayvalue=="checked")
            {
                checkBox->setChecked(true);
            }
//            qDebug()<<"checkbox id::"<<checkBox->id();
            connect(checkBox,SIGNAL(sigStateChanged(QString,QString)),job,SLOT(checkBoxChanged(QString,QString)));
            id=checkBox->id();
            normalLayout->addWidget(checkBox);
            widgetList.append(checkBox);
        }

        /// ------------------------------------combobox-------------------------------//
        if(displaytype=="combobox")
        {
//            qDebug()<<optionvalue<<optiontext;
            comboBox->setMinimumWidth(200);
            for(int i=0;i<optionvalue.size();i++)
            {
                comboBox->addItem(optiontext[i]);
            }
//            qDebug()<<"combobox id::"<<comboBox->id();
            connect(comboBox,SIGNAL(sigIndexChanged(QString,QString)),job,SLOT(comboBoxChanged(QString,QString)));
            id=comboBox->id();
            normalLayout->addWidget(comboBox);
            widgetList.append(comboBox);
        }
        /// ------------------------------------filecombobox-------------------------------//
        if(displaytype=="filecombobox")
        {
//            qDebug()<<optionvalue<<optiontext;  // /home/xtf     path
            fileComboBox->setMinimumWidth(200);
            //遍历optionvalue，然后添加到comboBox当中去
            QDir dir;
            dir.setPath(optionvalue.at(0));
            dir.setFilter(QDir::Files | QDir::NoSymLinks);
//            dir.setSorting(QDir::Size | QDir::Reversed);

            QFileInfoList list = dir.entryInfoList();
            fileComboBox->addItem("Please Choose");
            for (int i = 0; i < list.size(); ++i)
            {
                QFileInfo fileInfo = list.at(i);
//                qDebug()<< qPrintable(QString("%1 %2").arg(fileInfo.size(), 10)
//                                      .arg(fileInfo.fileName()));
                fileComboBox->addItem(fileInfo.fileName());
            }

//            qDebug()<<"fileComboBox id::"<<fileComboBox->id();
            connect(fileComboBox,SIGNAL(sigIndexChanged(QString,QString)),job,SLOT(comboBoxChanged(QString,QString)));
            id=fileComboBox->id();
            normalLayout->addWidget(fileComboBox);
            widgetList.append(fileComboBox);
        }

        /// ------------------------------------spinbox-------------------------------//
        if(displaytype=="spinbox")
        {
            spinBox->setMinimumWidth(200);
            spinBox->setValue(displayvalue.toInt());
//            qDebug()<<"spinBox id::"<<spinBox->id();
            connect(spinBox,SIGNAL(sigValueChanged(QString,QString)),job,SLOT(spinBoxChanged(QString,QString)));
            id=spinBox->id();
            normalLayout->addWidget(spinBox);
            widgetList.append(lineEdit);
        }

        /// ------------------------------------radiobutton-------------------------------//
        /// 问题最多，最后处理
        if(displaytype=="radiobutton")
        {
            // 这可是有列表和选中与否的，还要设好buttongroup等
            QGridLayout *radioLayout=new QGridLayout();
            QButtonGroup *group=new QButtonGroup();
            for(int i=0;i<optiontext.size();i++)
            {
                radio=new QJDRadioButton();  // 因为需要很多，所以一个是不够的
                radio->setOtherID(groupBox->id());
                radioIDList.append(radio->id());
//                qDebug()<<"radio id::"<<radio->id()<<groupBox->id();
                radio->setText(optiontext.at(i));

                if(optionvalue[i]=="checked")
                {
                    radio->setChecked(true);
                }

                /// 应该还要加上groupBox的id,另外增加
                connect(radio,SIGNAL(sigRadioChecked(QString,QString,QString)),
                        job,SLOT(radioButtonChanged(QString,QString,QString)));
                group->addButton(radio);
                /// 合理分配布局
                int a=0,b=1;
                if(i%2==0)
                {
                    radioLayout->addWidget(radio,i-a,0);
                    a++;
                }
                if(i%2==1)
                {
                    radioLayout->addWidget(radio,i-b,1);
                    b++;
                }

                widgetList.append(radio);
            }
            groupBox->setLayout(radioLayout);
            normalLayout->addWidget(groupBox);
        }
    }

    finalLayout->addLayout(normalLayout);
    /// -------------------------------------  write to job -------------------------------------///
    job->addNormalWidget(property,desc,datatype,min,max,displaytype,displayvalue,optiontext,optionvalue,
                         id,groupBox->id(),radioIDList);
}


/// 下一步着重写这个部分,需要重新布局
void creatUI::creatHideCheckWidget(const QString property, const QString desc, const QString datatype,
                          const QString min, const QString max, const QString displaytype, const QString displaytext,
                          const QString displayvalue,
                          const QStringList optiontext, const QStringList optionvalue, QString hidevalue)
{
    /// --------------可以有check和radio两种隐藏形式------------///
    // 形式多样，有待考虑
//    qDebug()<<"\n creatHideCheckWidget"<<displaytext;
    /// 使用groupbox统一考虑
    QLabel *proLabel=new QLabel;
    QJDLineEdit *lineEdit=new QJDLineEdit;
    QJDGroupBox *groupBox=new QJDGroupBox;
    groupBox->setCheckable(true);
    QJDCheckBox *checkBox=new QJDCheckBox;
    QString boxID=groupBox->id();
    QString lineID=lineEdit->id();

    QHBoxLayout *groupLayout=new QHBoxLayout;
    if(!hidevalue.isEmpty())
    {
        QString checkBoxText;
        groupBox->setTitle(property);
        checkBoxText.append(property);   /// 怎么名字和lable的一样呢？应该添加hidelabeltext
        checkBox->setText(checkBoxText);

        proLabel->setText(property);
        groupLayout->addWidget(proLabel);

        if(hidevalue=="NO")
        {
            groupBox->setChecked(true);
        }
        if(hidevalue=="YES")
        {
            groupBox->setChecked(false);
        }
        /// 关联groupbox和xml之间的联系
        connect(groupBox,SIGNAL(sigGroupChecked(bool,QString)),job,SLOT(groupBoxChanged(bool,QString)));
    }
    if(!displaytype.isEmpty())    // 创建控件的类型
    {
        if(displaytype=="lineedit")
        {
            // 这也是有默认值的
            lineEdit->setText(displayvalue);
            lineEdit->setBoxID(groupBox->id());
            groupLayout->addWidget(lineEdit);
            widgetList.append(lineEdit);
            /// 关联lineedit的xml修改，这个还需要知道boxID才能进入和修改
            connect(lineEdit,SIGNAL(sigLineEditChanged(QString,QString,QString)),
                    job,SLOT(lineEditChanged(QString,QString,QString)));
        }       
    }
    groupBox->setLayout(groupLayout);
    finalLayout->addWidget(groupBox);
    /// -------------------------------------  write to job -------------------------------------///
    job->addHideWidget(property,desc,datatype,min,max,displaytype,displayvalue,
                       optiontext,optionvalue,hidevalue,boxID,lineID);
}

void creatUI::creatHideRadioWidget(const QString property, const QString desc, const QString datatype,
                                   const QString min, const QString max, const QString displaytype, const QString displaytext,
                                   const QString displayvalue,
                                   const QStringList optiontext, const QStringList optionvalue, QString showvalue)
{
//    qDebug()<<"\n creatHideRadioWidget:: 目前暂时不考虑";
    /// 如何设计radio部分的隐藏
//    qDebug()<<property<<desc<<datatype<<min<<max<<displaytype<<displaytext<<displayvalue
//            <<optiontext<<optionvalue<<showvalue;
//    <property name="testshow3">
//            <desc>this is a hide example</desc>
//            <datatype>string</datatype>
//            <displaytype name="lineedit">lineditshow</displaytype>
//            <hide type="radiobutton">YES</hide>
//            <>
//    </property>
}

void creatUI::setWidgetRange(QString /*displaytype*/, QString /*min*/, QString /*max*/)
{
    /// 那个控件如何传过来？
//    if(!datatype.isEmpty())  //允许输入的数据类型, 这个如何控制？regexp?
//    {
//        // －－－－－－－－－－－目前仅针对了lineEdit,其他仍然需要改进－－－－－－－－///
//        if(datatype=="string")
//        {
//            /// 数字，字母，符号都需要
//        }
//        if(datatype=="int")
//        {
//            QValidator* validator = new QIntValidator();  // this set int is enough
//            lineEdit->setValidator( validator );

//            if(!min.isNull() && !max.isNull())   // 如果是int，还要设置范围,float也要有取值范围
//            {
//                QValidator* validator = new QIntValidator( min.toInt(), max.toInt(), this );
//                lineEdit->setValidator( validator );
//            }
//        }
//        if(datatype=="float")
//        {
//            QValidator *validator = new QDoubleValidator(this);
//            lineEdit->setValidator( validator );

//            if(!min.isNull() && !max.isNull())   // float也有取值范围,无法使用正则表达式解决
//            {
//                QValidator* validator = new QDoubleValidator(min.toInt(), max.toInt(), 4, this );  // 小数点后面4位
//                lineEdit->setValidator( validator );
//            }
//        }
//    }

}
void creatUI::finishCreatUI()
{    
    /// 结束前添加spacer
    QSpacerItem *verticalSpacer = new QSpacerItem(148, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    finalLayout->addItem(verticalSpacer);
    // 底部会有一大片空白如何是好？
    // 可以固定高度，变成scrollarea

    setLayout(finalLayout);
}

void creatUI::resizeEvent(QResizeEvent *)
{
//    qDebug()<<"creatui ::"<<this->size();
}

void creatUI::setJobFileName(const QString fileName)
{
    job->setJobFileName(fileName);
}

int creatUI::indexOfStackWidget()
{
    return index;
}

void creatUI::setIndex(int temp)
{
    index=temp;
}

void creatUI::deleteJob()
{
    job->deleteThisJob();
}

//void creatUI::upJob()
//{
//    qDebug()<<"upJob";
//    job->upThisJob();
//}

//void creatUI::downJob()
//{
//    qDebug()<<"downJob";
//    job->downThisJob();
//}
void creatUI::turnOnJob()
{
    qDebug()<<"turnOnJob";
    job->turnOnThisJob();
}
void creatUI::turnOffJob()
{
    qDebug()<<"turnOffJob";
    job->turnOffThisJob();
}

void creatUI::resetJob(int upValue)
{
    qDebug()<<"resetJob";  /// 其实就是upJob,只不过up多次而已
    job->resetThisJob(upValue);
}

void creatUI::dragJob(int index,int allRow,QString upOrDown)
{
    qDebug()<<"2.dragJob"<<index<<allRow<<upOrDown;
    job->dragThisJob(index,allRow,upOrDown);
}
