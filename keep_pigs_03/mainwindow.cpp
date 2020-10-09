#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "keep_pig.h"
#include "pig_and_sty.h"
#include <QPushButton>
#include <QDebug>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFile>
#include <QString>
#include <QFileDialog>
#include <QDataStream>
#include <QLabel>
#include <QtCharts>
#include <QTextBrowser>
#include <QScrollBar>
#include<stdlib.h>
#include<time.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>

QT_CHARTS_USE_NAMESPACE

using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    c1.setParent(this);
    c2.setParent(this);
    c3.setParent(this);
    c1.resize(500,300);
    c1.move(0,50);
    c2.resize(500,300);
    c2.move(500,50);
    c3.resize(400,300);
    c3.move(1000,50);

    QAction *load_f=ui->load_file->addAction("从文件读取");
    QAction *save_f=ui->save->addAction("保存至文件");

    QString str_info="总猪数：";
    ui->stys_info->setText(str_info + QString::number(kp.N));

    ui->textBrowser->addScrollBarWidget(ui->verticalScrollBar,Qt::AlignRight);

    ui->label_1->setText("猪圈编号(0-99):");
    ui->label_2->setText("猪序号(0-n):");
    ui->label_3->setText("猪圈编号(0-99):");
    ui->label_sick->setText("猪瘟潜伏天数(>0):");
    ui->label_dead->setText("发病致死天数(>0):");

    load_file();

    connect(load_f,&QAction::triggered,
            [=]()
    {
        load_file();
    });
    connect(save_f,&QAction::triggered,
            [=]()
    {
        QFile file;
        //QFileDialog f_dialog(w);
        QString path=QFileDialog::getSaveFileName(this,"save","../","TXT(*.txt)");
        if(path.isEmpty()==0)
        {
            file.setFileName(path);
        }
        bool isOK = file.open(QIODevice::WriteOnly);
        if(true == isOK)
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            stream<<kp.N<<" "<<kp.Day<<" "<<kp.Money<<"\n";
            for(int i=0;i<100;i++)
            {
                for(int j=0;j<kp.stys[i].pig_count;j++)
                {
                    stream<<i<<" "<<j<<" "<<kp.stys[i].pigs[j].type<<" "<<kp.stys[i].pigs[j].weight<<" "<<kp.stys[i].pigs[j].day<<"\n";
                }
            }
            file.close();
            qDebug()<<"数据保存成功！";
            ui->textBrowser->insertPlainText(QString("------数据保存成功------")+QString("\n"));
        }
    });
    connect(ui->search_info,&QPushButton::pressed,
            [=]()
    {
        if(ui->textEdit->toPlainText().toInt()+1)
        {
            if(ui->textEdit_2->toPlainText().toInt()+1)
            {
                int a=ui->textEdit->toPlainText().toInt(),b=ui->textEdit_2->toPlainText().toInt();
                //qDebug()<<a<<" "<<b;
                if(a>=0&&a<=99&&b>=0&&b<kp.stys[a].pig_count)
                {
                    QString typetext;
                    switch(kp.stys[a].pigs[b].type)
                    {
                    case 0:typetext="黑猪";break;
                    case 1:typetext="小花猪";break;
                    case 2:typetext="大花白猪";break;
                    }
                    QString tmptext=QString("-----以下是查询结果-----")+"\n"+"品种:"+typetext+"\n"+"猪圈编号:"+QString::number(kp.stys[a].pigs[b].sty_id)
                                                                   +"\n"+"饲养天数:"+QString::number(kp.stys[a].pigs[b].day)+"天"
                                                                   +"\n"+"体重:"+QString::number(kp.stys[a].pigs[b].weight)+"kg"+"\n"+"\n";
                    ui->textBrowser->insertPlainText(tmptext);
                }
                else
                    ui->textBrowser->insertPlainText(QString("-----查询失败，请输入正确的编号和序号-----\n"));
            }

        }
    });
    connect(ui->search_sty_2,&QPushButton::pressed,
            [=]()
    {
        if(ui->textEdit_3->toPlainText().toInt()+1)
        {
            int a=ui->textEdit_3->toPlainText().toInt();
            //qDebug()<<a;
            if(a>=0&&a<=99)
            {
                QString tmptext=QString("-----以下是查询结果-----")+"\n"+"黑猪:"+QString::number(kp.stys[a].b_count)+"头"+
                        "\n"+"小花猪:"+QString::number(kp.stys[a].sw_count)+"头"+
                        "\n"+"大花白猪:"+QString::number(kp.stys[a].bw_count)+"头"+"\n"+"\n";
                ui->textBrowser->insertPlainText(tmptext);
            }

            else
                ui->textBrowser->insertPlainText("-----查询失败，请输入正确的编号和序号-----\n");
        }

    });
    connect(ui->exit_1,&QAction::triggered,
            [=]()
    {
        this->close();
    });
    connect(ui->save_exit,&QAction::triggered,
            [=]()
    {
        QFile file;
        //QFileDialog f_dialog(w);
        QString path=QFileDialog::getSaveFileName(this,"save","../","TXT(*.txt)");
        if(path.isEmpty()==0)
        {
            file.setFileName(path);
        }
        bool isOK = file.open(QIODevice::WriteOnly);
        if(true == isOK)
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            stream<<kp.N<<"\n";
            for(int i=0;i<100;i++)
            {
                for(int j=0;j<kp.stys[i].pig_count;j++)
                {
                    stream<<i<<" "<<j<<" "<<kp.stys[i].pigs[j].type<<" "<<kp.stys[i].pigs[j].weight<<" "<<kp.stys[i].pigs[j].day<<"\n";
                }
            }
            file.close();
            qDebug()<<"数据保存成功！";
        }        
        this->close();
    });
    connect(ui->grow1,&QPushButton::pressed,
    [=]()
    {
        int tmp1=kp.N;
        long long int tmp2=kp.Money;
        kp.grow(1);
        qDebug()<<"-----成长1天-----";
        ui->textBrowser->insertPlainText(QString("------成长1天------")+QString("\n"));
        if(kp.N<tmp1)
            ui->textBrowser->insertPlainText(QString("本次出售:")+QString::number(tmp1-kp.N)+QString("头\n")+QString("本次收入:")+QString::number(kp.Money-tmp2)+QString("元\n"));
        ui->stys_info->setText("总猪数:" + QString::number(kp.N)+"\n"+
                               "总天数:" + QString::number(kp.Day)+"\n"+
                               "累计收入:" + QString::number(kp.Money));
        change_chart1();
        change_chart23();
    });
    connect(ui->grow10,&QPushButton::pressed,
    [=]()
    {
        int tmp1=kp.N;
        long long int tmp2=kp.Money;
        kp.grow(10);
        qDebug()<<"-----成长10天-----";
        ui->textBrowser->insertPlainText(QString("------成长10天------")+QString("\n"));
        if(kp.N<tmp1)
            ui->textBrowser->insertPlainText(QString("本次出售:")+QString::number(tmp1-kp.N)+QString("头\n")+QString("本次收入:")+QString::number(kp.Money-tmp2)+QString("元\n"));
        ui->stys_info->setText("总猪数:" + QString::number(kp.N)+"\n"+
                               "总天数:" + QString::number(kp.Day)+"\n"+
                               "累计收入:" + QString::number(kp.Money));
        change_chart1();
        change_chart23();
    });
    connect(ui->grow30,&QPushButton::pressed,
    [=]()
    {
        int tmp1=kp.N;
        long long int tmp2=kp.Money;
        kp.grow(30);
        qDebug()<<"-----成长30天-----";
        ui->textBrowser->insertPlainText(QString("------成长30天------")+QString("\n"));
        if(kp.N<tmp1)
            ui->textBrowser->insertPlainText(QString("本次出售:")+QString::number(tmp1-kp.N)+QString("头\n")+QString("本次收入:")+QString::number(kp.Money-tmp2)+QString("元\n"));
        ui->stys_info->setText("总猪数:" + QString::number(kp.N)+"\n"+
                               "总天数:" + QString::number(kp.Day)+"\n"+
                               "累计收入:" + QString::number(kp.Money));
        change_chart1();
        change_chart23();
    });
    connect(ui->grow90,&QPushButton::pressed,
    [=]()
    {
        int tmp1=kp.N;
        long long int tmp2=kp.Money;
        kp.grow(90);
        qDebug()<<"-----成长90天-----";
        ui->textBrowser->insertPlainText(QString("------成长90天------")+QString("\n"));
        if(kp.N<tmp1)
            ui->textBrowser->insertPlainText(QString("本次出售:")+QString::number(tmp1-kp.N)+QString("头\n")+QString("本次收入:")+QString::number(kp.Money-tmp2)+QString("元\n"));
        ui->stys_info->setText("总猪数:" + QString::number(kp.N)+"\n"+
                               "总天数:" + QString::number(kp.Day)+"\n"+
                               "累计收入:" + QString::number(kp.Money));
        change_chart1();
        change_chart23();
    });
    connect(ui->reset_all,&QPushButton::pressed,
            [=]()
    {
        kp.reset();
        ui->stys_info->setText("总猪数:" + QString::number(kp.N)+"\n"+
                               "总天数:" + QString::number(kp.Day)+"\n"+
                               "累计收入:" + QString::number(kp.Money));
        change_chart1();
        change_chart23();
        qDebug()<<"-----数据已重置 请购入或读取文件-----";
        ui->textBrowser->insertPlainText(QString("-----数据已重置 请购入或读取文件-----")+QString("\n"));
    });
    connect(ui->add_by_rand,&QPushButton::pressed,
            [=]()
    {
        int tmp=kp.N;
        long long int tmp2=kp.Money;
        kp.add_pig(100);
        ui->stys_info->setText("总猪数:" + QString::number(kp.N)+"\n"+
                               "总天数:" + QString::number(kp.Day)+"\n"+
                               "累计收入:" + QString::number(kp.Money));
        change_chart1();
        change_chart23();
        QFile file;
        file.setFileName("../sold_data.txt");
        bool isOK = file.open(QIODevice::Append);
        if(true == isOK)
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            stream<<"day"<<kp.Day<<":\n";
            stream<<"buy:"<<kp.N-tmp<<"pigs\n";
            stream<<"pay:"<<tmp2-kp.Money<<"RMB\n";
            file.close();
        }
        else
            qDebug()<<"fail";
        qDebug()<<"-----随机购入100头猪-----";
        ui->textBrowser->insertPlainText(QString("-----随机购入100头猪-----\n")+QString("支出:")+QString::number(tmp2-kp.Money)+QString("元\n"));
    });
    connect(ui->plague_no,&QPushButton::pressed,
            [=]()
    {
        if(kp.N==0)
            ui->textBrowser->insertPlainText(QString("当前没有猪,无法进行模拟\n"));
        else
        {
            if(ui->textEdit_4->toPlainText().toInt()&&ui->textEdit_5->toPlainText().toInt())
            {

                ui->textBrowser->insertPlainText(QString("--------------------------------------------\n-----------------模拟猪瘟开始---------------\n--------------------------------------------\n"));
                plague(ui->textEdit_4->toPlainText().toInt(),ui->textEdit_5->toPlainText().toInt());
                ui->textBrowser->insertPlainText(QString("--------------------------------------------\n"));
            }
        }
    });
    change_chart1();
    change_chart23();
}

MainWindow::~MainWindow()
{
    QFile file;
    file.setFileName("../auto_save.txt");
    bool isOK = file.open(QIODevice::WriteOnly);
    if(true == isOK)
    {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        stream<<kp.N<<" "<<kp.Day<<" "<<kp.Money<<"\n";
        for(int i=0;i<100;i++)
        {
            for(int j=0;j<kp.stys[i].pig_count;j++)
            {
                stream<<i<<" "<<j<<" "<<kp.stys[i].pigs[j].type<<" "<<kp.stys[i].pigs[j].weight<<" "<<kp.stys[i].pigs[j].day<<"\n";
            }
        }
        file.close();
        qDebug()<<"数据保存成功！";
        ui->textBrowser->insertPlainText(QString("------数据保存成功------")+QString("\n"));
    }
    delete ui;
}

void MainWindow::change_chart1()
{
    QBarSet *set0 = new QBarSet("数量");

    *set0 << kp.b_n<< kp.sw_n<< kp.bw_n;

    QBarSeries *series = new QBarSeries();
    series->append(set0);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("猪品种分布图");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    categories << "黑猪" << "小花猪" << "大花白猪";
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    c1.setCentralWidget(chartView);
    c1.show();
}

void MainWindow::change_chart23()
{
    QBarSet *set0 = new QBarSet("数量");
    QBarSet *set1 = new QBarSet("数量");

    int x1=0,x2=0,x3=0,x4=0,x5=0,x6=0;
    int d1=0,d2=0,d3=0,d4=0,d5=0;

    for(int i=0;i<100;i++)
    {
        for(int j=0;j<kp.stys[i].pig_count;j++)
        {
            if(kp.stys[i].pigs[j].weight>=20.0&&kp.stys[i].pigs[j].weight<35.0)
                x1++;
            else if(kp.stys[i].pigs[j].weight>=35.0&&kp.stys[i].pigs[j].weight<45.0)
                x2++;
            else if(kp.stys[i].pigs[j].weight>=45.0&&kp.stys[i].pigs[j].weight<55.0)
                x3++;
            else if(kp.stys[i].pigs[j].weight>=55.0&&kp.stys[i].pigs[j].weight<65.0)
                x4++;
            else if(kp.stys[i].pigs[j].weight>=65.0&&kp.stys[i].pigs[j].weight<=75.0)
                x5++;
            else if(kp.stys[i].pigs[j].weight>75.0)
                x6++;
            if(kp.stys[i].pigs[j].day>=0&&kp.stys[i].pigs[j].day<30)
                d1++;
            else if(kp.stys[i].pigs[j].day>=30&&kp.stys[i].pigs[j].day<60)
                d2++;
            else if(kp.stys[i].pigs[j].day>=60&&kp.stys[i].pigs[j].day<90)
                d3++;
            else if(kp.stys[i].pigs[j].day>=90&&kp.stys[i].pigs[j].day<180)
                d4++;
            else if(kp.stys[i].pigs[j].day>=180)
                d5++;
        }
    }
    *set0<<x1<<x2<<x3<<x4<<x5<<x6;
    *set1<<d1<<d2<<d3<<d4<<d5;

    QBarSeries *series = new QBarSeries();
    QBarSeries *series2 = new QBarSeries();
    series->append(set0);
    series2->append(set1);

    QChart *chart = new QChart();
    QChart *chart2 = new QChart();
    chart->addSeries(series);
    chart->setTitle("猪体重分布图");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart2->addSeries(series2);
    chart2->setTitle("猪饲养时间分布图");
    chart2->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    QStringList categories2;
    categories << "20-35" << "35-45" << "45-55"<<"55-65"<<"65-75"<<">75";
    categories2<<"0-30"<<"30-60"<<"60-90"<<"90-180"<<">180";
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);
    QBarCategoryAxis *axis2 = new QBarCategoryAxis();
    axis2->append(categories2);
    chart2->createDefaultAxes();
    chart2->setAxisX(axis2, series2);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart2->legend()->setVisible(true);
    chart2->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    QChartView *chartView2 = new QChartView(chart2);
    chartView2->setRenderHint(QPainter::Antialiasing);

    c2.setCentralWidget(chartView);
    c2.show();
    c3.setCentralWidget(chartView2);
    c3.show();
}

void MainWindow::load_file()
{
    QFile file;
    //QFileDialog f_dialog(w);
    QString path=QFileDialog::getOpenFileName(this,"open","../","TXT(*.txt)");
    if(path.isEmpty()==0)
    {
        file.setFileName(path);
    }
    bool isOK = file.open(QIODevice::ReadOnly);
    if(true == isOK)
    {
        kp.reset();
        int n,t_days,t_money,sid,pid,tt,dd;
        double ww;
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        stream>>n>>t_days>>t_money;
        kp.Day=t_days,kp.Money=t_money;
        for(int k=1;k<=n;k++)
        {
            Pig newpig;
            stream>>sid>>pid>>tt>>ww>>dd;
            newpig.sty_id=sid;
            newpig.day=dd;
            newpig.weight=ww;
            newpig.type=tt;
            switch(tt)
            {
            case 0:newpig.price=15;kp.b_n++;break;
            case 1:newpig.price=7;kp.sw_n++;break;
            case 2:newpig.price=6;kp.bw_n++;break;
            default:break;
            }
            if(kp.stys[sid].pig_count==0)
            {
                switch(tt)
                {
                case 0:kp.bsty_n++;kp.stys[sid].is_black=1;kp.empty_n--;break;
                case 1:
                case 2:kp.wsty_n++;kp.stys[sid].is_black=0;kp.empty_n--;break;
                default:break;
                }
            }
            kp.stys[sid].addpig(newpig);
            kp.N++;
            //qDebug()<<sid<<pid<<tt<<ww<<dd;
        }
        //qDebug()<<n;
        file.close();
        ui->textBrowser->insertPlainText(QString("------读取文件成功------")+QString("\n"));
        ui->stys_info->setText("总猪数:" + QString::number(kp.N)+"\n"+
                               "总天数:" + QString::number(kp.Day)+"\n"+
                               "累计收入:" + QString::number(kp.Money));
        QFile file2;
        file2.setFileName("../sold_data.txt");
        bool isOK = file2.open(QIODevice::WriteOnly);
        if(true == isOK)
        {
            QTextStream stream(&file2);
            stream<<path<<"\n";
            file.close();
        }
        else
            qDebug()<<"fail";
        change_chart1();
        change_chart23();
    }
}

void MainWindow::plague(int si_day, int de_day)
{
    kp.infected_n=0;
    kp.sick_n=0;
    kp.dead_n=0;
    for(int m=0;m<100;m++)
    {
        kp.stys[m].is_infected=0;
        for(int n=0;n<kp.stys[m].pig_count;n++)
        {
            kp.stys[m].pigs[n].is_infected=0;
            kp.stys[m].pigs[n].is_sick=0;
            kp.stys[m].pigs[n].infect_day=0;
            kp.stys[m].pigs[n].sick_day=0;
        }
    }
    int i,j,left,right,days=0,n_may=0;
    while(1)
    {
        i=rand()%100,j=rand()%10;
        if(j<kp.stys[i].pig_count)
        {
            kp.stys[i].is_infected=1;
            kp.stys[i].pigs[j].is_infected=1;
            kp.infected_n=1;
            ui->textBrowser->insertPlainText(QString::number(i)+QString("号猪圈的")+QString::number(j)+QString("号猪被随机感染\n"));
            break;
        }
    }
    left=i,right=i;
    while(kp.stys[left].pig_count!=0&&left!=0)
    {
        left--;
    }
    while(kp.stys[right].pig_count!=0&&right!=99)
    {
        right++;
    }
    if(left==0&&kp.stys[left].pig_count==0)
        left++;
    if(right==99&&kp.stys[right].pig_count==0)
        right--;
    for(int x=left;x<=right;x++)
        n_may+=kp.stys[x].pig_count;
    ui->textBrowser->insertPlainText(QString("分析可得,如果不采取措施,当前有")+QString::number(n_may)+QString("头猪可能被感染(空猪圈具有隔离作用)\n"));
    while(kp.dead_n!=n_may&&days<1000)
    {
        days++;
        for(int m=left;m<=right;m++)
        {
            for(int n=0;n<kp.stys[m].pig_count;n++)
            {
                if(kp.stys[m].pigs[n].is_infected==0)
                {
                    if(kp.stys[m].is_infected==1)
                    {
                        int tmp=rand()%2;
                        switch(tmp)
                        {
                        case 0:break;
                        case 1:kp.stys[m].pigs[n].is_infected=1;kp.infected_n++;break;
                        default:break;
                        }
                    }
                    else if((m-1>=0&&kp.stys[m-1].is_infected==1)||(m+1<=99&&kp.stys[m+1].is_infected==1))
                    {
                        int tmp=rand()%100;
                        if(tmp<15)
                        {
                            kp.stys[m].is_infected=1;
                            kp.stys[m].pigs[n].is_infected=1;
                            kp.infected_n++;
                        }
                    }
                    else
                        ;
                }
                else
                {
                    kp.stys[m].pigs[n].infect_day++;
                    if(kp.stys[m].pigs[n].is_sick==1)
                    {
                        kp.stys[m].pigs[n].sick_day++;
                        if(kp.stys[m].pigs[n].sick_day==de_day)
                        {
                            kp.dead_n++;kp.sick_n--;
                        }
                    }
                    else
                    {
                        if(kp.stys[m].pigs[n].infect_day==si_day)
                        {
                            kp.stys[m].pigs[n].is_sick=1;
                            kp.sick_n++;kp.infected_n--;
                        }
                    }
                }
            }
        }
        ui->textBrowser->insertPlainText(QString("经过")+QString::number(days)+QString("天,目前有")+QString::number(kp.N-kp.dead_n)+QString("头猪存活,")+QString::number(kp.infected_n)+QString("头猪被感染,")+QString::number(kp.sick_n)+QString("头猪发病,")+QString::number(kp.dead_n)+QString("头猪死亡\n"));
    }
    ui->textBrowser->insertPlainText(QString("猪瘟模拟结束,经过")+QString::number(days)+QString("天当前所有可被感染的猪将会死亡\n"));
    int tmp1=i-2*si_day,tmp2=i+2*si_day;
    if(tmp1<left)
        tmp1=left;
    if(tmp2>right)
        tmp2=right;
    ui->textBrowser->insertPlainText(QString("如果采取措施,需要将")+QString::number(tmp1)+QString("-")+QString::number(tmp2)+QString("号猪圈的猪进行集中处理,可将风险最小化\n"));
}








