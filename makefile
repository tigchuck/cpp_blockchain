all: main

main: main.o block.o blockchain.o crypto_hash.o
	g++ --std=c++20 -o main main.o block.o blockchain.o crypto_hash.o

main.o: main.cpp
	g++ --std=c++20 -c main.cpp

block.o: backend/blockchain/block.cpp backend/blockchain/block.h
	g++ --std=c++20 -c backend/blockchain/block.cpp

blockchain.o: backend/blockchain/blockchain.cpp backend/blockchain/blockchain.h
	g++ --std=c++20 -c backend/blockchain/blockchain.cpp

crypto_hash.o: backend/utils/crypto_hash.cpp backend/utils/crypto_hash.h
	g++ --std=c++20 -c backend/utils/crypto_hash.cpp


