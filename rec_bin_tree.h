/**************************************************************************************/
/*  AUTHOR: Alon Begin                                                               */
/*  CREATE DATE: 06/01/16                                                             */
/*  LAST MODIFY: 10/01/16      		                                                  */
/*  DESCRIPTION: Recursive AVL Binary Tree				                                  */
/**************************************************************************************/




#ifndef REC_BIN_TREE_H_
#define REC_BIN_TREE_H_

#include <stddef.h> /*For size_t*/

typedef struct rec_bin_tree rec_bin_tree_t;

typedef struct avl_node node_t;
struct avl_node
{
	node_t *left;
	node_t *right;
	void   *data;
	int balance_factor;
};


struct rec_bin_tree
{
	node_t dummy_to_root;
	int (*cmp_func)(const void *data1, const void *data2);
};

rec_bin_tree_t *RecBinTreeCreate(int (*cmp_func)(const void *data1, const void *data2));

void RecBinTreeDestroy(rec_bin_tree_t *tree);

int RecBinTreeRemove(rec_bin_tree_t *tree, void *data);

int RecBinTreeInsert(rec_bin_tree_t *tree, void *data);

size_t RecBinTreeHeight(rec_bin_tree_t *tree);

size_t RecBinTreeSize(rec_bin_tree_t *tree);

/*O(1)*/
int RecBinTreeIsEmpty(rec_bin_tree_t *tree);

void *RecBinTreeFind(rec_bin_tree_t *tree, void *data);

/*int RecBinTreeMultiFind(rec_bin_tree_t *tree, void *data, int (*comp_func)(void *data1, void *data2), dlist* dlist);*/

/*Returns the first failure value of do_func if it failed, and 0 otherwise*/
int RecBinTreeForEach(rec_bin_tree_t *tree, int (*do_func)(void *data, void *args), void *args);

void *NodeData(node_t *node);

void PrintBinTreeRec(rec_bin_tree_t *tree);

void PrintTree(rec_bin_tree_t *tree);

#endif /* REC_BIN_TREE_H_ */
