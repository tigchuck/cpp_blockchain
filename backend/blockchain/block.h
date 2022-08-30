#ifndef BLOCK_H
#define BLOCK_H
#include <iostream>
#include <ctime>
#include <string>
using namespace std;

class Block {
	private:
		time_t timestamp;
		string last_hash, hash;
		int data;  // can make template or something
	public:
		Block(time_t timestamp, string last_hash, string hash, int data) : timestamp(timestamp), last_hash(last_hash), hash(hash), data(data) {}
		static Block genesis();
		static Block mine(const Block&, int);
		friend ostream& operator << (ostream&, const Block&);
};


#endif