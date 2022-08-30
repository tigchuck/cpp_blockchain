#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H
#include <iostream>
#include <vector>
#include "block.h"
using namespace std;

class Blockchain {
	private:
		vector<Block> chain;
	public:
		Blockchain() : chain({ Block::genesis() }) {}
		void extend(int);
		friend ostream& operator << (ostream&, const Blockchain& obj);
};


#endif