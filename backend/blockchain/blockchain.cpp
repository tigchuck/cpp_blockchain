#include <iostream>
#include <vector>
#include <string>
#include "block.h"
#include "blockchain.h"
using namespace std;

void Blockchain::extend(int data) {
	Block last = chain.back();
	chain.push_back(Block::mine(last, data)); 
}

ostream& operator << (ostream& o, const Blockchain& obj) {
	o << "Blockchain [" << endl;
	for (auto it = begin(obj.chain); it != end(obj.chain); ++it)
		o << "\t" << *it << " -> " << endl;
	o << "]" << endl;
	return o;
}