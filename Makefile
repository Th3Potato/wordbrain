all: wordbrain.c
	gcc wordbrain.c trie.c -o wordsolver -O3