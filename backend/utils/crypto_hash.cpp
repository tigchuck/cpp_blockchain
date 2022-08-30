#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <array>
#include <bitset>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "crypto_hash.h"
using namespace std;

vector< bitset<32> > pad_bits(string s) {
	const short int N = 512, BYTE = 8, BYTES = 64, WORDS = 16;
	vector< bitset <32> > bits;
	bitset<32> word = {0};
	long int i = 0;					        // NEED CHECK i < (2^32)/8
	while (i < s.size()) {				    // convert string to bits
		word = word << BYTE;
		word |= s[i++];
		if (i % 4 == 0) { bits.push_back(word); word.reset(); } // bytes per word
	}

	word = word << BYTE;	                // adding initial padding starting with 10000000
	word |= 128;

	int j = BYTES - ((i + 1) % BYTES);		// bytes remaining to get to 64
	if (j < 8) { 					        // message interferes with length
		word = word << ((j % 4) * BYTE); 	// bytes per word
		bits.push_back(word); 
		word.reset(); 
		if (j >= 4)				            // need 1 extra word of padding
			bits.push_back(word);			
		for (int k = 0; k < WORDS - 2; ++k)	// fill padding
			bits.push_back(word);
	} else {
		word = word << ((j % 4) * BYTE);	// pad last word of message
		bits.push_back(word);
		word.reset();
		for (int k = 0; k < (j - 8) / 4; ++k) 	
			bits.push_back(word);
	}
	long int x = pow(2, 32);
	word |= (i * BYTE) / x;
	bits.push_back(word);
	word.reset();
	word |= (i * BYTE) % x;
	bits.push_back(word);
	return bits;
}

bitset<32> rotr(bitset<32> bits, int r) {
    bitset<32> x = bits >> r;
    bitset<32> y = bits << (32 - r);
    return x | y;
}

bitset<32> sigma0(bitset<32> bits) { return (rotr(bits, 7) ^ rotr(bits, 18) ^ (bits >> 3)); }

bitset<32> sigma1(bitset<32> bits) { return (rotr(bits, 17) ^ rotr(bits, 19) ^ (bits >> 10)); }

bitset<32> SIGMA0(bitset<32> bits) { return (rotr(bits, 2) ^ rotr(bits, 13) ^ rotr(bits, 22)); }

bitset<32> SIGMA1(bitset<32> bits) { return (rotr(bits, 6) ^ rotr(bits, 11) ^ rotr(bits, 25)); }

bitset<32> choice(bitset<32> x, bitset<32> y, bitset<32> z) { return ((x & y) | (~x & z)); }

bitset<32> majority(bitset<32> x, bitset<32> y, bitset<32> z) { return ((x & y) | (x & z) | (y & z)); }

bitset<32> operator + (const bitset<32>& a, const bitset<32> b) { return bitset<32> (a.to_ulong() + b.to_ulong()); }

array<bitset<32>, 64> constants() {
    array<long, 64> cbrt_primes = { 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };
    array<bitset<32>, 64> k;
    for (int i = 0; i < 64; ++i)
        k[i] = bitset<32> (cbrt_primes[i]);
    return k;
}

array<bitset<32>, 8> hash_constants() {
    long int x = pow(2, 32);
    array<double, 8> sqrt_primes = { sqrt((2.0)) - 1, sqrt(3.0) - 1, sqrt(5.0) - 2, sqrt(7.0) - 2, sqrt(11.0) - 3, sqrt(13.0) - 3, sqrt(17.0) - 4, sqrt(19.0) - 4 };
    array<bitset<32>, 8> h;
    for (int i = 0; i < 8; ++i)
        h[i] = bitset<32> (static_cast<unsigned int> (sqrt_primes[i] * x));
    return h;
}

array<bitset<32>, 64> message_schedule(vector< bitset<32> > bits, int start, int end) {
    array<bitset<32>, 64> ms;
    for (int i = start; i < end; ++i)
        ms[i - start] = bits[i];
    for (int i = 16; i < 64; ++i)
        ms[i] = sigma1(ms[i -  2]) + ms[i - 7] + sigma0(ms[i - 15]) + ms[i - 16];
    return ms; 
}

array<bitset<32>, 8> compress(array<bitset<32>, 64> ms, array<bitset<32>, 64> k, array<bitset<32>, 8> h) {
    array<bitset<32>, 8> c = h;
    bitset<32> t1, t2;
    for (int i = 0; i < 64; ++i) {
        t1 = SIGMA1(c[4]) + choice(c[4], c[5], c[6]) + c[7] + ms[i] + k[i];
        t2 = SIGMA0(c[0]) + majority(c[0], c[1], c[2]);
        for (int j = 7; j > 0; --j)
            c[j] = c[j - 1];
        c[0] = t1 + t2;
        c[4] = c[4] + t1;
    }
    for (int i = 0; i < 8; ++i)
        c[i] = c[i] + h[i];
    return c;
}

array<bitset<32>, 8> build_hash(vector< bitset<32> > bits) {
    int i = 0;
    const array<bitset<32>, 64> k = constants();
    array<bitset<32>, 64> ms;
    array<bitset<32>, 8> h = hash_constants();
    while (i < bits.size()) {
        ms = message_schedule(bits, i, i + 16);
        h = compress(ms, k, h);
        i += 16;
    }
    return h;
}

string sha256(string s) {
    vector< bitset<32> > bits = pad_bits(s);
    array<bitset<32>, 8> h = build_hash(bits);
    string sha256_hash = "0x", temp;
    for (int i = 0; i < 8; ++i) {
        ostringstream stream;   // redeclare so it doesn't keep building on previous hex values
        stream << hex << h[i].to_ulong();
        temp = stream.str();
        int precision = 8 - temp.size();
        temp = string(precision, '0').append(temp); // account for leading 0's
        sha256_hash += temp;
    }
    
    return sha256_hash;
}

string crypto_hash(time_t t, int i, string last_hash) {
    string timestamp = to_string(static_cast<long int> (t));
    string data = to_string(i);
    string c = timestamp + data + last_hash;
    return sha256(c);
}