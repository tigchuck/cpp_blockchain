#ifndef CRYPTO_HASH_H
#define CRYPTO_HASH_H
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <bitset>
#include <ctime>
using namespace std;

vector< bitset<32> > pad_bits(string);

bitset<32> rotr(bitset<32>, int);

bitset<32> sigma0(bitset<32>);

bitset<32> sigma1(bitset<32>);

bitset<32> SIGMA0(bitset<32>);

bitset<32> SIGMA1(bitset<32>);

bitset<32> choice(bitset<32>, bitset<32>, bitset<32>);

bitset<32> majority(bitset<32>, bitset<32>, bitset<32>);

bitset<32> operator + (const bitset<32>&, const bitset<32>);

array<bitset<32>, 64> constants();

array<bitset<32>, 8> hash_constants();

array<bitset<32>, 64> message_schedule(vector< bitset<32> >, int, int);

array<bitset<32>, 8> compress(array<bitset<32>, 64>, array<bitset<32>, 64>, array<bitset<32>, 8>);

array<bitset<32>, 8> build_hash(vector< bitset<32> >);

string sha256(string);string crypto_hash(time_t, int, string);


#endif