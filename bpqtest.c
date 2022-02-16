#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "BoundedPriorityQueue.h" 

int main(){

	BoundedPriorityQueue *bpq = bpqCreate(10);

	bpqInsert(bpq, 16, 1);
	bpqInsert(bpq, 14, 2);
	bpqInsert(bpq, 10, 3);
	bpqInsert(bpq, 8, 4);
	bpqInsert(bpq, 7, 5);
	bpqInsert(bpq, 3, 7);
	bpqInsert(bpq, 2, 8);
	bpqInsert(bpq, 4, 9);
	bpqInsert(bpq, 1, 10);
	bpqInsert(bpq, 9, 6); // OK échange bien avec la clé 7
						  // car c'est son parent et sa clé est + petite
	dispBpq(bpq);
	printf("\n--------------------------\n\n");

	bpqReplaceMaximum(bpq, 11, 6); // OK remplace bien 16 par 11 et
								   // reconstruit la file
	dispBpq(bpq);
	printf("\n--------------------------\n\n");

	size_t *items = bpqGetItems(bpq);
	for(size_t i = 0; i < bpqSize(bpq); i++){
		printf("Item %lu : value = %lu\n", i, items[i]);
	} // OK renvoie ce qui faut
	printf("\n--------------------------\n\n");

	printf("Capacity of the bpq = %lu\n", bpqCapacity(bpq)); // OK renvoie ce qui faut

	printf("\n--------------------------\n");

	bpqFree(bpq);

}
