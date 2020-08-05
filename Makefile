stor: wordbrain.c
	gcc wordbrain.c -o wordSolver -O3 -DSTOR

liten: wordbrain.c
	gcc wordbrain.c -o wordsolver -O3 -DLITEN
	
eng: wordbrain.c
	gcc wordbrain.c -o wordsolverENG -O3 -DENG