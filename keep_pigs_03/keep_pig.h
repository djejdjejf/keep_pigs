//keep_pig.h

#ifndef KEEP_PIG_H
#define KEEP_PIG_H

#include"pig_and_sty.h"
#include<iostream>
#include<vector>
#include<stdlib.h>
#include<time.h>

class keep_pig
{
	public:
        int N,b_n,sw_n,bw_n,bsty_n,wsty_n,empty_n,Day,infected_n,sick_n,dead_n;
        long long int Money;
        Sty stys[100];

		keep_pig();
		~keep_pig();
		
		void start(int mode=0);
        //mode=0 rand
        //mode=1 file
        //mode=2 input
        void reset();
        void show_all();
		void add_pig(int n);
		void add_pig(int b_n,int sw_n,int bw_n);
        void grow(int day_n);
        void sold_pig();
        void serch(int sty_id,int pig_id);

        void save_sold_data();
        //void load_data(MainWindow *w);
};

#endif
