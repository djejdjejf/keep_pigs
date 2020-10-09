//pig_and_sty.cpp

#include"keep_pig.h"
#include"pig_and_sty.h"
#include<QDebug>
#include<vector>
#include<stdlib.h>
#include<time.h>
using namespace std;

void Pig::show()
{
	switch(type)
	{
        case 0:qDebug()<<"   种类: "<<"黑猪";break;
        case 1:qDebug()<<"   种类: "<<"小花猪";break;
        case 2:qDebug()<<"   种类: "<<"大花白猪";break;
        default:qDebug()<<"   种类: "<<"未知";break;
	}
    qDebug()<<"   体重: "<<weight<<"千克";
    qDebug()<<"猪圈编号: "<<sty_id;
    qDebug()<<"饲养时间: "<<day<<"天";
    qDebug()<<"   价格: "<<price<<"元/斤"<<"\n";
}

Sty::Sty()
{
    this->id=-1;
    this->is_black=-1;
    this->pigs.reserve(10);
}

Sty::Sty(int id)
{
	this->id=id;
    this->pigs.reserve(10);
    pig_count=0,b_count=0,sw_count=0,bw_count=0;
    is_black=-1;
}

Sty::~Sty()
{

}

void Sty::addpig(Pig &newpig)
{
    if(pig_count==10)
        return;
	pigs.push_back(newpig);
	pig_count++;
    //if(pig_count==1&&pigs[0].type==0)
    //	is_black=1;
	switch(newpig.type)
	{
		case 0:b_count++;break;
		case 1:sw_count++;break;
		case 2:bw_count++;break;
        default:qDebug()<<"error"<<"\n";break;
	}
}

void Sty::show_all()
{
	if(pig_count==0)
	{
        qDebug()<<this->id<<"号猪圈为空";
		return;
	}
    qDebug()<<this->id<<"号猪圈信息如下:"<<b_count<<" "<<sw_count<<" "<<bw_count;
	for(int i=0;i<pig_count;i++) 
		pigs[i].show();
}

void Sty::show_type_count()
{
    if(pig_count==0)
    {
        qDebug()<<this->id<<"号猪圈为空";
        return;
    }
    qDebug()<<"猪圈编号: "<<id;
    qDebug()<<"   黑猪: "<<b_count<<"头";
    qDebug()<<" 小花猪: "<<sw_count<<"头";
    qDebug()<<"大花白猪: "<<bw_count<<"头";
}

void Sty::show_one(int id)
{
	if(id>=pig_count)
	{
        qDebug()<<"not found"<<"\n";
		return;
	}
    pigs[id].show();
}

void Sty::grow_sty()
{
    if(this->pig_count==0)
        return;
    for(int i=0;i<pig_count;i++)
    {
        double r=(rand()%13*0.1);
        pigs[i].weight+=r;
        pigs[i].day++;
    }
}

void Sty::soldpig_sty(int &a,int &b,int &c,long long int &d,int &e)
{
	if(pig_count==0)
        return;
    for(int i=0;i<pig_count;i++)
	{
        if(pigs[i].weight*2.0>150.0||pigs[i].day>365)
		{			
			pig_count--;
            e--;
            switch(pigs[i].type)
			{
                case 0:b_count--;a--;break;
                case 1:sw_count--;b--;break;
                case 2:bw_count--;c--;break;
                default:qDebug()<<"error"<<"\n";break;
			}
            d+=(pigs[i].price*pigs[i].weight*2);
            pigs.erase(pigs.begin()+i);
            i--;
		}
	}
    return;
}



