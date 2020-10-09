//pig_and_sty.h
//type=0
//type=1
//type=2

#ifndef PIG_AND_STY_H
#define PIG_AND_STY_H

#include<iostream>
#include<iomanip> 
#include<vector>
using namespace std;

struct Pig
{
		int type;
		double weight;
		int sty_id;
		int day;
		int price;
        bool is_infected;
        bool is_sick;
        int infect_day,sick_day;
		void show();
};

class Sty
{
	private:

	public:
        int id;
        int pig_count=0,b_count=0,sw_count=0,bw_count=0;
        vector<Pig> pigs;
        int is_black=-1;
        bool is_infected;
        Sty();
		Sty(int id);
		~Sty();
		
		void addpig(Pig &newpig);
		void show_all();
		void show_type_count();
		void show_one(int id);
        void grow_sty();
        void soldpig_sty(int &,int &,int &,long long int &,int &);
};

#endif






















