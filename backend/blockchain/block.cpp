#include <iostream>
#include <ctime>
#include <string>
#include "block.h"
#include "../utils/crypto_hash.h"
using namespace std;

ostream& operator << (ostream& o, const Block& obj) {
	o << "Block( time: " << obj.timestamp << ", last_hash: " << obj.last_hash << ", hash: " << obj.hash << ", data: " << obj.data << " )";
	return o;
}

Block Block::genesis() {
	time_t t = time(NULL);
	string last_hash = sha256("Genesis Hash");
	int data = 0;
	string hash = crypto_hash(t, data, last_hash);
	return Block(t, last_hash, hash, data); 
}

Block Block::mine(const Block& last, int data) { 
	time_t t = time(NULL);
	string last_hash = last.hash;
	string hash = crypto_hash(t, data, last_hash);
	return Block(t, last_hash, hash, data);
} 