all: windowTest.c
	gcc wordbrain.c trie.c windowTest.c -o wordsolver -O3