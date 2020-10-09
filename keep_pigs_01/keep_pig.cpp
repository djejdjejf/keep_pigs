//keep_pig.cpp

#include"keep_pig.h"
#include"pig_and_sty.h"
#include"mainwindow.h"
#include "ui_mainwindow.h"
#include<vector>
#include<stdlib.h>
#include<time.h>
#include<QDebug>
#include<stdio.h>
#include<QFile>
#include<QString>
#include<QFileDialog>
#include<QDataStream>

keep_pig::keep_pig()
{
    N=0,b_n=0,sw_n=0,bw_n=0,bsty_n=0,wsty_n=0,empty_n=100,Money=0,Day=0,infected_n=0,sick_n=0,dead_n=0;
}

keep_pig::~keep_pig()
{

}

void keep_pig::start(int mode)
{
    if(mode==0)
    {
        for(int i=0;i<100;i++)
        {
            stys[i].id=i;
            stys[i].is_infected=0;
        }
        //add_pig(300,300,300);
        //add_pig(300);
    }    
}

void keep_pig::reset()
{
    N=0,b_n=0,sw_n=0,bw_n=0,bsty_n=0,wsty_n=0,empty_n=100,Money=0,Day=0,infected_n=0,sick_n=0,dead_n=0;
    Sty newstys[100];
    for(int i=0;i<100;i++)
    {
        stys[i]=newstys[i];
        stys[i].id=i;
        stys[i].is_infected=0;
        stys[i].is_black=-1;
    }

}

void keep_pig::show_all()
{
    for(int i=0;i<100;i++)
    {
        if(stys[i].pig_count==0)
            continue;
        stys[i].show_all();
    }
    qDebug()<<N<<" "<<b_n<<" "<<sw_n<<" "<<bw_n<<" "<<bsty_n<<" "<<wsty_n<<" "<<empty_n;
}

void keep_pig::add_pig(int n)
{
    if(n==0)
        return;
    srand((unsigned)time(NULL));
    for(int i=1;i<=n;i++)
    {
        if(N==1000)
        {
            qDebug()<<"已经不能再购入";
            break;
        }
        int t,tmp;
        Pig newpig;
        newpig.type=(rand()%3);
        t=newpig.type;
        newpig.day=0;
        newpig.weight=((rand()%301+200)/10.0);
        newpig.is_infected=0;
        newpig.is_sick=0;
        newpig.infect_day=0;
        newpig.sick_day=0;
        switch (t)
        {
            case 0:newpig.price=15;break;
            case 1:newpig.price=7;break;
            case 2:newpig.price=6;break;
            default:break;
        }
        if(t==0)
        {
            tmp=((b_n+1)/(bsty_n+empty_n))+1;
            for(int j=0;j<100;j++)
            {
                if(stys[j].is_black!=0&&stys[j].pig_count<tmp&&stys[j].pig_count<10)
                {
                    newpig.sty_id=j;
                    stys[j].addpig(newpig);
                    Money-=newpig.price*newpig.weight*2;
                    if(stys[j].is_black==-1)
                    {
                        stys[j].is_black=1;
                        bsty_n++;
                        empty_n--;
                    }
                    N++;
                    b_n++;
                    break;
                }
            }
        }
        else
        {
            tmp=((sw_n+bw_n+1)/(wsty_n+empty_n))+1;
            for(int j=0;j<100;j++)
            {
                if(stys[j].is_black!=1&&stys[j].pig_count<tmp&&stys[j].pig_count<10)
                {
                    newpig.sty_id=j;
                    stys[j].addpig(newpig);
                    Money-=newpig.price*newpig.weight*2;
                    if(stys[j].is_black==-1)
                    {
                        stys[j].is_black=0;
                        wsty_n++;
                        empty_n--;
                    }
                    N++;
                    switch(t)
                    {
                        case 1:sw_n++;break;
                        case 2:bw_n++;break;
                        default:break;
                    }
                    break;
                }
            }
        }
    }
}

void keep_pig::add_pig(int b_n1,int sw_n1,int bw_n1)
{        
    int b=b_n1,sw=sw_n1,bw=bw_n1;
    int k=3,t,tmp;
    for(int i=1;i<=b_n1+sw_n1+bw_n1;i++)
    {
        if(N==1000)
        {
            qDebug()<<"已经不能再购入";
            break;
        }
        Pig newpig;
        newpig.is_infected=0;
        newpig.is_sick=0;
        newpig.infect_day=0;
        newpig.sick_day=0;
        for(k;k>0;k++)
        {
            if(k%3==0&&b>0)
            {
                newpig.type=0;
                b--;
                k++;
                break;
            }
            if(k%3==1&&sw>0)
            {
                newpig.type=1;
                sw--;
                k++;
                break;
            }
            if(k%3==2&&bw>0)
            {
                newpig.type=2;
                bw--;
                k++;
                break;
            }
        }
        t=newpig.type;
        //qDebug()<<t;
        newpig.day=0;
        newpig.weight=((rand()%301+200)/10.0);
        switch (t)
        {
            case 0:newpig.price=15;break;
            case 1:newpig.price=7;break;
            case 2:newpig.price=8;break;
            default:break;
        }
        if(t==0)
        {
            tmp=((b_n+1)/(bsty_n+empty_n))+1;
            for(int j=0;j<100;j++)
            {
                if(stys[j].is_black!=0&&stys[j].pig_count<tmp&&stys[j].pig_count<10)
                {
                    newpig.sty_id=j;
                    stys[j].addpig(newpig);
                    Money-=newpig.price*newpig.weight*2;
                    if(stys[j].is_black==-1)
                    {
                        stys[j].is_black=1;
                        bsty_n++;
                        empty_n--;
                    }
                    N++,b_n++;
                    break;
                }
            }
        }
        else
        {
            tmp=((sw_n+bw_n+1)/(wsty_n+empty_n))+1;
            for(int j=0;j<100;j++)
            {
                if(stys[j].is_black!=1&&stys[j].pig_count<tmp&&stys[j].pig_count<10)
                {
                    newpig.sty_id=j;
                    stys[j].addpig(newpig);
                    Money-=newpig.price*newpig.weight*2;
                    if(stys[j].is_black==-1)
                    {
                        stys[j].is_black=0;
                        wsty_n++;
                        empty_n--;
                    }
                    N++;
                    switch(t)
                    {
                        case 1:sw_n++;break;
                        case 2:bw_n++;break;
                        default:break;
                    }
                    break;
                }
            }
        }
    }
}

void keep_pig::grow(int day_n)
{
    for(int i=1;i<=day_n;i++)
    {        
        for(int j=0;j<100;j++)
            stys[j].grow_sty();
        Day++;
        if(Day!=0&&Day%90==0)
            sold_pig();
    }
}

void keep_pig::sold_pig()
{
    int tmp1=N,tmp2=Money;
    for(int i=0;i<100;i++)
    {
        if(stys[i].pig_count==0)
            continue;
        stys[i].soldpig_sty(b_n,sw_n,bw_n,Money,N);
        if(stys[i].pig_count==0)
        {
            switch(stys[i].is_black)
            {
            case 0:wsty_n--;empty_n++;stys[i].is_black=-1;break;
            case 1:bsty_n--;empty_n++;stys[i].is_black=-1;break;
            }
        }
    }
    QFile file;
    file.setFileName("../sold_data.txt");
    bool isOK = file.open(QIODevice::Append);
    if(true == isOK)
    {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        stream<<"day"<<Day<<":\n";
        stream<<"sell:"<<tmp1-N<<"pigs\n";
        stream<<"earn:"<<Money-tmp2<<"RMB\n";
        stream<<"accumulsted income:"<<Money<<"RMB\n";
        file.close();
    }
    else
        qDebug()<<"fail";
    qDebug()<<"本次出售:"<<tmp1-N<<"头";
    qDebug()<<"本次收入:"<<Money-tmp2<<"元";
    qDebug()<<"累计收入:"<<Money<<"元";
}

void keep_pig::serch(int sty_id,int pig_id)
{
    qDebug()<<"查找结果:";
    if(sty_id<0||stys[sty_id].is_black==-1||sty_id>99||pig_id<0||pig_id>=stys[sty_id].pig_count)
    {
        qDebug()<<"查找无效";
        return;
    }
    stys[sty_id].pigs[pig_id].show();
}

void keep_pig::save_sold_data()
{
    QFile file;
    file.setFileName("sold_data");
    bool isOK = file.open(QIODevice::Append);
    if(true == isOK)
    {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        file.close();
    }
}

/*void keep_pig::load_data(MainWindow *w)
{
    QFile file;
    //QFileDialog f_dialog(w);
    QString path=QFileDialog::getOpenFileName(w,"open","../","TXT(*.txt)");
    if(path.isEmpty()==0)
    {
        file.setFileName(path);
    }
    bool isOK = file.open(QIODevice::ReadOnly);
    if(true == isOK)
    {
        int n,sid,pid,tt,dd;
        double ww;
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        stream>>n;
        for(int k=1;k<=n;k++)
        {
            stream>>sid>>pid>>tt>>ww>>dd;
            qDebug()<<sid<<pid<<tt<<ww<<dd;
        }
        qDebug()<<n;
        file.close();
    }
}*/


