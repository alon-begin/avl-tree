/**************************************************************************************/
/*  AUTHOR: Alon Begin                                                               */
/*  CREATE DATE: 06/01/16                                                             */
/*  LAST MODIFY: 10/01/16      		                                                  */
/*  DESCRIPTION: Recursive AVL Binary Tree				                                  */
/**************************************************************************************/

#include <stdio.h>
#include "rec_bin_tree.h"

int CompareData(const void *data1 , const void* data2);
int HelloNode(const void *data1 , const void* data2);
int main(int argc, char const *argv[])
{
	rec_bin_tree_t *tree = NULL;
	node_t *tree_nodes[8] = {NULL};
	int num[7] = {27,45,34,48,12,9,36};
	int i = 0;
	int *number_data[8] = {NULL};
	size_t number_of_nodes = 0;
	tree = RecBinTreeCreate(CompareData);

	while(i<7)
	{
		RecBinTreeInsert(tree,&num[i]);
		++i;
	}
	number_data[6] = RecBinTreeFind(tree,&num[0]);

	printf("%d\n", *number_data[6] );

	PrintBinTreeRec(tree);
	printf("\n\n");
	PrintTree(tree);
	printf("\n\n");
	RecBinTreeRemove(tree,&num[0]);
	printf("\n\n");
	PrintBinTreeRec(tree);
	printf("\n\n");
	PrintTree(tree);


/*
	tree_nodes[0] = BinTreeInsert(tree,&num[0]);
	
	tree_nodes[7] = BinTreeBegin(tree);
	number_data[6] = BinTreeGetData(tree_nodes[7]);

	printf("%d\n", *number_data[6] );

	number_of_nodes = BinTreeCount(tree);

	printf("abc%lu\n", number_of_nodes );	
	BitreeForeach(tree,HelloNode,NULL);

*/

	RecBinTreeDestroy(tree);
	return 0;
}

int CompareData(const void *data1 , const void* data2)
{
	if (*(int*)data1 > *(int*)data2)
	{
		return 1;
	}
	if (*(int*)data1 < *(int*)data2)
	{
		return -1;
	}
	return 0;
		
}
int HelloNode(const void *data1 , const void* data2)
{
	static int counter;
	printf("Hello Node %d : %d !\n", counter, *(int*)data1);
	
	++counter;

	return 0;
}
