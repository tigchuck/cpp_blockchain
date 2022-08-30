#include <iostream>
#include <cassert>
#include "../utils/crypto_hash.h"
using namespace std;

void test_crypto_hash() {
    assert(sha256("abc").length() == 66);
}