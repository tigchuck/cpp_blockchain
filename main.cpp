#include <iostream>
#include <vector>
#include <ctime>
#include "backend/blockchain/block.h"
#include "backend/blockchain/blockchain.h"
#include "backend/utils/crypto_hash.h"

int main() {
  Blockchain blockchain;
  blockchain.extend(7);
  blockchain.extend(3);
  cout << blockchain << endl;
  return 0;
}