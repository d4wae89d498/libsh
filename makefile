all:
	g++ -g -fsanitize=address conflict_decrypt.cpp -std=c++20 -Iinclude -o conflict_decrypt

test:
	./conflict_decrypt decr data.sah data-decr.sah