#include "creatui.h"

/// readxml->creatui->creatjoblist
creatUI::creatUI(QWidget *parent) :
        QWidget(parent)
{
    ///  需要通过解析xml文件来解析出这些东西
//    vLayout=new QVBoxLayout(this);  // 不能在这里new
    gLayout=new QGridLayout();
    gLayout->setSizeConstraint(QLayout::SetFixedSize);
    widgetList.clear();
    rowNum=0;

    job=new writeJob();    // 写入job，需要创建写xml步骤
    /// 如何判断flow的名称和
}

creatUI::~creatUI()
{}

void creatUI::nodeData(const QString property, const QString desc, const QString datatype,
                       const QString min, const QString max, const QString displaytype,
                       const QString displayvalue, const QStringList optiontext, const QStringList optionvalue,
                       const QString visible)
{
    if(visible.isEmpty())
    {
        // 解析创建
        creatNormalWidget(property,desc,datatype,min,max,displaytype,displayvalue,optiontext,optionvalue);
        // 重新写成xml。为何不简单复制,但是传递进来的只有参数而已
        job->addNormalWidget(property,desc,datatype,min,max,displaytype,displayvalue,optiontext,optionvalue);
        /// 应当在创建的过程中同步链接控件，可以同步更改属性
    }
    if(!visible.isEmpty())
    {
        creatVisibleWidget(property,desc,datatype,min,max,displaytype,displayvalue,optiontext,optionvalue,visible);
        job->addVisibleWidget(property,desc,datatype,min,max,displaytype,displayvalue,optiontext,optionvalue,visible);
    }
}

void creatUI::creatNormalWidget(const QString property, const QString desc, const QString datatype,
                           const QString min, const QString max, const QString displaytype, const QString displayvalue,
                           const QStringList optiontext, const QStringList optionvalue)
{
    /// 处理一组数据，创建一个控件，并且添加到layout当中去
    QLabel *proLabel=new QLabel;    //一组控件，一个说明label
    QLineEdit *lineEdit=new QLineEdit;
    QGroupBox *groupBox=new QGroupBox;

    if(!property.isEmpty())   // 名称，作为label
    {
        proLabel->setText(property);
        gLayout->addWidget(proLabel,rowNum,0);
    }
    if(!desc.isEmpty())  //描述，tooltip
    {
        proLabel->setToolTip(desc);
    }
    if(!datatype.isEmpty())  //允许输入的数据类型, 这个如何控制？regexp?
    {
        //lineedit
        if(datatype=="string")
        {
            /// 数字，字母，符号都需要
        }
        if(datatype=="int")
        {
            QValidator* validator = new QIntValidator();  // this set int is enough
            lineEdit->setValidator( validator );

            if(!min.isNull() && !max.isNull())   // 如果是int，还要设置范围,float也要有取值范围
            {
                QValidator* validator = new QIntValidator( min.toInt(), max.toInt(), this );
                lineEdit->setValidator( validator );
            }
        }
        if(datatype=="float")
        {
            QValidator *validator = new QDoubleValidator(this);
            lineEdit->setValidator( validator );

            if(!min.isNull() && !max.isNull())   // float也有取值范围,无法使用正则表达式解决
            {
                QValidator* validator = new QDoubleValidator(min.toInt(), max.toInt(), 4, this );  // 小数点后面4位
                lineEdit->setValidator( validator );
            }
        }
    }
    if(!displaytype.isEmpty())    // 创建控件的类型
    {
        if(displaytype=="lineedit")
        {
            // 这也是有默认值的
            lineEdit->setText(displayvalue);

            gLayout->addWidget(lineEdit,rowNum,1);
            rowNum++;
            widgetList.append(lineEdit);
        }
        if(displaytype=="radiobutton")
        {
            // 这可是有列表和选中与否的，还要设好buttongroup等
            QGridLayout *radioLayout=new QGridLayout();
            QButtonGroup *group=new QButtonGroup();
            for(int i=0;i<optiontext.size();i++)
            {
                radio=new QRadioButton();
                radio->setText(optiontext.at(i));

                QString value=optionvalue.at(i);
                radio->setChecked(value.toInt());

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
            gLayout->addWidget(groupBox,rowNum,1);
            rowNum++;
        }
//        if(displaytype="spinbox")
//        {}
    }

}

void creatUI::creatVisibleWidget(const QString property, const QString desc, const QString datatype,
                          const QString min, const QString max, const QString displaytype, const QString displayvalue,
                          const QStringList optiontext, const QStringList optionvalue, QString visible)
{
    QLabel *proLabel=new QLabel;
    QLineEdit *lineEdit=new QLineEdit;
    QGroupBox *groupBox=new QGroupBox;
    QCheckBox *checkBox=new QCheckBox;

    if(!visible.isEmpty())
    {
        QString checkBoxText;
//        checkBoxText<<"Show "<<property;
        checkBoxText.append("Show ");
        checkBoxText.append(property);
        checkBox->setText(checkBoxText);
        gLayout->addWidget(checkBox,rowNum,0);
        rowNum++;

        proLabel->setText(property);
        gLayout->addWidget(proLabel,rowNum,0);

        if(visible=="NO")
        {
            checkBox->setChecked(false);
        }
        if(visible=="YES")
        {
            checkBox->setChecked(true);
        }
        connect(checkBox,SIGNAL(clicked(bool)),proLabel,SLOT(setVisible(bool)));
    }
    if(!desc.isEmpty())
    {
        proLabel->setToolTip(desc);
    }
    if(!datatype.isEmpty())  //允许输入的数据类型, 这个如何控制？regexp?
    {
        //lineedit
        if(datatype=="string")
        {
            /// 数字，字母，符号都需要
        }
        if(datatype=="int")
        {
            QValidator* validator = new QIntValidator();  // this set int is enough
            lineEdit->setValidator( validator );

            if(!min.isNull() && !max.isNull())   // 如果是int，还要设置范围,float也要有取值范围
            {
                QValidator* validator = new QIntValidator( min.toInt(), max.toInt(), this );
                lineEdit->setValidator( validator );
            }
        }
        if(datatype=="float")
        {
            QValidator *validator = new QDoubleValidator(this);
            lineEdit->setValidator( validator );

            if(!min.isNull() && !max.isNull())   // float也有取值范围,无法使用正则表达式解决
            {
                QValidator* validator = new QDoubleValidator(min.toInt(), max.toInt(), 4, this );  // 小数点后面4位
                lineEdit->setValidator( validator );
            }
        }
    }
    if(!displaytype.isEmpty())    // 创建控件的类型
    {
        if(displaytype=="lineedit")
        {
            // 这也是有默认值的
            lineEdit->setText(displayvalue);

            gLayout->addWidget(lineEdit,rowNum,1);
            rowNum++;
            widgetList.append(lineEdit);

            connect(checkBox,SIGNAL(clicked(bool)),lineEdit,SLOT(setVisible(bool)));
        }
        if(displaytype=="radiobutton")
        {
            // 这可是有列表和选中与否的，还要设好buttongroup等
            QGridLayout *radioLayout=new QGridLayout();
            QButtonGroup *group=new QButtonGroup();
            for(int i=0;i<optiontext.size();i++)
            {
                radio=new QRadioButton();
                radio->setText(optiontext.at(i));

                QString value=optionvalue.at(i);
                radio->setChecked(value.toInt());

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
                connect(checkBox,SIGNAL(clicked(bool)),radio,SLOT(setVisible(bool)));
            }
            groupBox->setLayout(radioLayout);
            connect(checkBox,SIGNAL(clicked(bool)),groupBox,SLOT(setVisible(bool)));

            gLayout->addWidget(groupBox,rowNum,1);
            rowNum++;
        }
    }

}

void creatUI::finishCreatUI()
{    
    setLayout(gLayout);  
}


void creatUI::resizeEvent(QResizeEvent *)
{
    qDebug()<<"creatui ::"<<this->size();
}

void creatUI::setJobFileName(const QString fileName)
{
    job->setJobFileName(fileName);
}
