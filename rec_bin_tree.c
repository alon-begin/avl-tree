
/**************************************************************************************/
/*  AUTHOR: Alon Begin                                                               */
/*  CREATE DATE: 06/01/16                                                             */
/*  LAST MODIFY: 10/01/16      		                                                  */
/*  DESCRIPTION: Recursive AVL Binary Tree				                                  */
/**************************************************************************************/






#include <stdio.h> /*NULL*/
#include <stdlib.h> /*malloc ,free*/
#include <string.h>
#include <assert.h>
#include "rec_bin_tree.h"
#define AVL_LFT_HEAVY 1
#define AVL_RGT_HEAVY -1
#define AVL_BALANCED 0

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

static int FuncFor_RecBinTreeInsert(rec_bin_tree_t *tree, node_t *node, void *data);

/*In case of insert func = 1, for remove func = -1 */
static int BalanceUpdate(node_t *node, node_t *parent,int (*cmp_func)(const void *data1, const void *data2), void *data,int func);


static int FuncFor_RecBinTreeRemove(rec_bin_tree_t *tree, node_t *node, void *data);
static void RotateLeft(node_t **node);
static void RotateRight(node_t **node);
static int MakeNodeToInsert(node_t **node_to_insert, void *data);

static void Func_ForRecBinTreeDestroy(node_t *root);

/*-----------------------------------------------------------------------------*/
static node_t *ParentOfNodeToRemove(rec_bin_tree_t *tree, node_t *node, void *data);

static node_t *MinInRightSubTree(node_t *node);
static void SwapNodes(node_t *node1, node_t *node2);
static int SideOfCurrentChild(rec_bin_tree_t *tree, node_t *parent, node_t *child);
static int FuncFor_RecBinTreeRemove(rec_bin_tree_t *tree, node_t *node, void *data);

static void *Func_forRecBinTreeFind(rec_bin_tree_t *tree, node_t *node, void *data);

static void PrintBinTreeRecAux(node_t *node);

static int RecBinTreeForEachInOrder(node_t *node, int (*do_func)(void *data, void *args), void *args);

static int RecBinTreeForEachPostOrder(node_t *node, int (*do_func)(void *data, void *args), void *args);
static int RecBinTreeForEachPreOrder(node_t *node, int (*do_func)(void *data, void *args), void *args);

/****************************************************************************************/
rec_bin_tree_t *RecBinTreeCreate(int (*cmp_func)(const void *data1, const void *data2))
{
	rec_bin_tree_t *tree = NULL;
	
	node_t dummy_to_root = {NULL};

	if( !(tree = (rec_bin_tree_t*)malloc(sizeof(rec_bin_tree_t)))) 
	{
		return NULL;
	}

	dummy_to_root.left = NULL;
	dummy_to_root.right = NULL;
	dummy_to_root.data = NULL;

	tree->cmp_func = cmp_func;
	
	tree->dummy_to_root = dummy_to_root;

	return tree;
}
/***************************************************************************************/
void RecBinTreeDestroy(rec_bin_tree_t *tree)
{
	Func_ForRecBinTreeDestroy(tree->dummy_to_root.left);
	free(tree);
}
static void Func_ForRecBinTreeDestroy(node_t *root)
{
	if(!root)
	{
		return;
	}
	Func_ForRecBinTreeDestroy(root->left);
	Func_ForRecBinTreeDestroy(root->right);

	free(root);
	return;
}
/***************************************************************************************/
/***************************************************************************************/
int RecBinTreeInsert(rec_bin_tree_t *tree, void *data)
{
	node_t *node_to_insert = NULL;
	if(!RecBinTreeIsEmpty(tree))
	{

		return FuncFor_RecBinTreeInsert(tree, tree->dummy_to_root.left, data);
	}
	if(!MakeNodeToInsert(&node_to_insert, data))
	{
		tree->dummy_to_root.left = node_to_insert;
		tree->dummy_to_root.left->balance_factor = AVL_BALANCED;
		return 0;
	}
	return 1;
}


static int FuncFor_RecBinTreeInsert(rec_bin_tree_t *tree, node_t *node, void *data)
{
	node_t *node_to_insert = NULL;
	node_t *parent_node = NULL;
	if(tree->cmp_func(node->data, data) > 0)
	{
		if(!node->left)
		{
			if(!MakeNodeToInsert(&node_to_insert, data))
			{
				node->left = node_to_insert;
				BalanceUpdate(tree->dummy_to_root.left, &tree->dummy_to_root, tree->cmp_func, data,1);
				return 0;
			}
			return 1;
		}
		return FuncFor_RecBinTreeInsert(tree, node->left, data);
	}
	if(!node->right)
	{
		if(!MakeNodeToInsert(&node_to_insert, data))
		{
			node->right = node_to_insert;
			BalanceUpdate(tree->dummy_to_root.left, &tree->dummy_to_root, tree->cmp_func, data,1);
			return 0;
		}
		return 1;
	}
	return FuncFor_RecBinTreeInsert(tree, node->right, data);
}



static int MakeNodeToInsert(node_t **node_to_insert, void *data)
{
	*node_to_insert = (node_t*)malloc(sizeof(node_t));
	if(!*node_to_insert)
	{
		return 1;
	}
	(*node_to_insert)->left = NULL;
	(*node_to_insert)->right = NULL;
	(*node_to_insert)->data = data;
	(*node_to_insert)->balance_factor = AVL_BALANCED;
	return 0;
}
/****************************************************************************************/

static size_t TreeHeight(node_t *node)
{
	/*return node ? (1+max(TreeHeight(node->left),TreeHeight(node->right))) , 0 ;*/
	if (!node)
	{
		return 0;
	}
	return (1+max(TreeHeight(node->left),TreeHeight(node->right)));
}

size_t RecBinTreeHeight(rec_bin_tree_t *tree)
{
	return (TreeHeight(tree->dummy_to_root.left));
}

int RecBinTreeIsEmpty(rec_bin_tree_t *tree)
{
	if (tree->dummy_to_root.left)
	{
		return 0;
	}
	return 1;
}
/***************************************************************************************/
int RecBinTreeRemove(rec_bin_tree_t *tree, void *data)
{
	node_t *parent = ParentOfNodeToRemove(tree, &tree->dummy_to_root, data);
	node_t *child = NULL;
	int flag = 0;

	if(parent->left)
	{
		if(tree->cmp_func(parent->left->data, data) == 0)
			{
				flag = 1;
				child = parent->left;
			}
	}
	if(flag == 0)
	{
		child = parent->right;
	}
	return FuncFor_RecBinTreeRemove(tree, child, data);
}
/*-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*/

static int FuncFor_RecBinTreeRemove(rec_bin_tree_t *tree, node_t *node, void *data)
{
	node_t *next = NULL;
	node_t *parent = NULL;
	int side = 0;

	if(node->left && node->right) /* Two Childern */
	{
		next = MinInRightSubTree(node->right);
		SwapNodes(next, node);
		node = next;
	}
	parent = ParentOfNodeToRemove(tree, &tree->dummy_to_root, node->data);
	side = SideOfCurrentChild(tree, parent, node);
	if(node->left) /* Only Left Child */
	{
		if (side == 0)
		{
			parent->left = node->left;
			free(node);
		}
		else
		{
			parent->right = node->left;
			free(node);
		}
		BalanceUpdate(tree->dummy_to_root.left, &tree->dummy_to_root, tree->cmp_func, parent->data,-1);
	}
	else if (node->right) /* Only Right Child */
	{
		if(side == 0)
		{
			parent->left = node->right;
			free(node);
		}
		else
		{
			parent->right = node->right; 

			free(node);
		}
		BalanceUpdate(tree->dummy_to_root.left, &tree->dummy_to_root, tree->cmp_func, parent->data,-1);
	}
	else /* No Children */
	{
		if(side == 0)
		{
			parent->left = NULL;

			free(node);
		}
		else
		{
			parent->right = NULL;
			free(node);
		}
		BalanceUpdate(tree->dummy_to_root.left, &tree->dummy_to_root, tree->cmp_func, parent->data,-1);
	}
	return 1;
}

/*-----------------------------------------------------------------------------*/
static int SideOfCurrentChild(rec_bin_tree_t *tree, node_t *parent, node_t *child)
{
	if(parent->left)
	{
		if(tree->cmp_func(parent->left->data, child->data) == 0)
		return 0;
	}
	return 1;
}
/*-----------------------------------------------------------------------------*/
static void SwapNodes(node_t *node1, node_t *node2)
{
	void *tmp = node1->data;

	node1->data = node2->data;
	node2->data = tmp;
	return;
}
/*-----------------------------------------------------------------------------*/

static node_t *MinInRightSubTree(node_t *node)
{
	if(!node->left)
	{
		return node;
	}
	return(MinInRightSubTree(node->left));
}
/*-----------------------------------------------------------------------------*/
static node_t *ParentOfNodeToRemove(rec_bin_tree_t *tree, node_t *node, void *data)
{
	node_t *ret = NULL;
	if(!node)
	{
		return NULL;
	}
	if(node->left)
	{
		if(!tree->cmp_func(node->left->data, data))
		{
			return node;
		}
		ret = ParentOfNodeToRemove(tree, node->left, data);
	}
	if(node->right && !ret) /*in case the ret didn't find nothing in the left side*/
	{
		if(!tree->cmp_func(node->right->data, data))
		{
			return node;
		}
		ret = ParentOfNodeToRemove(tree, node->right, data);
	}
	return ret;
}
void *RecBinTreeFind(rec_bin_tree_t *tree, void *data)
{
	return Func_forRecBinTreeFind(tree, tree->dummy_to_root.left, data);
}

static void *Func_forRecBinTreeFind(rec_bin_tree_t *tree, node_t *node, void *data)
{
	if(!node)
	{
		return NULL;
	}
	if (!tree->cmp_func(node->data, data))
	{
		return node->data;
	}
	if(tree->cmp_func(node->data, data) > 0)
	{
		if(!node->left)
		{
			return NULL;
		}
		return Func_forRecBinTreeFind(tree, node->left, data);
	}
	if(!node->right)
	{
		return NULL;
	}
	return Func_forRecBinTreeFind(tree, node->right, data);
}

/****************************************************************************************/
int RecBinTreeForEach(rec_bin_tree_t *tree, int (*do_func)(void *data, void *args), void *args)
{
	return RecBinTreeForEachPreOrder(tree->dummy_to_root.left, do_func, args);
}

static int RecBinTreeForEachPreOrder(node_t *node, int (*do_func)(void *data, void *args), void *args)
{
	if(!node)
	{
		return 0;
	}
	if(!do_func(node->data, args))
	{
		if(!RecBinTreeForEachPreOrder(node->left, do_func, args) &&
		   !RecBinTreeForEachPreOrder(node->right, do_func, args))
		{
			return 0;
		}

	}
	return 1;
}

static int RecBinTreeForEachPostOrder(node_t *node, int (*do_func)(void *data, void *args), void *args)
{
	if(!node)
	{
		return 0;
	}
	
	if(!RecBinTreeForEachPostOrder(node->left, do_func, args) &&
		   !RecBinTreeForEachPostOrder(node->right, do_func, args))
	{
		if(!do_func(node->data, args))
		{
			return 0;
		}
	}
	return 1;
}

static int RecBinTreeForEachInOrder(node_t *node, int (*do_func)(void *data, void *args), void *args)
{
	if(!node)
	{
		return 0;
	}
	if(!RecBinTreeForEachInOrder(node->left, do_func, args))
	{
		if(!do_func(node->data, args) &&
		   !RecBinTreeForEachInOrder(node->right, do_func, args))
		{
			return 0;
		}
	}
	return 1;
}

/****************************************************************************************/
/****************************************************************************************/
void PrintBinTreeRec(rec_bin_tree_t *tree)
{
	PrintBinTreeRecAux(tree->dummy_to_root.left);
	printf("\n");
}
static void PrintBinTreeRecAux(node_t *node)
{
	if(!node)
	{
		return;
	}
	PrintBinTreeRecAux(node->left);
	printf("%d ", *(int*)(node->data));
	PrintBinTreeRecAux(node->right);
	return;
}
void TreePadding ( char ch, int n )
{
  int i;
  for ( i = 0; i < n; i++ )
    putchar ( ch );
}
void PrintNode ( node_t *root, int level )
{
  if ( root == NULL ) {	
    TreePadding ( '\t', level );
    puts ( "~" );
  }
  else {
    PrintNode ( root->right, level + 1 );
    TreePadding ( '\t', level );
    printf ( "%d\n", *(int*)root->data );
    PrintNode ( root->left, level + 1 );
  }
}

void PrintTree(rec_bin_tree_t *tree)
{
    PrintNode(tree->dummy_to_root.left, 0);
}
/******************************************************************************/

static void ConnectParent2NextNode(node_t *parent, node_t *node, node_t *next_node)
{
	if (parent->left == node)
	{
		parent->left = next_node;
	}
	else
	{
		parent->right = next_node;
	}
}

/******************************************************************************/

static void RotLeft(node_t *node, node_t *parent)
{
	node_t *tmp = NULL;
	assert(node);
	assert(parent);

	ConnectParent2NextNode(parent, node, node->right);
	tmp = node->right;
	node->right = tmp->left;
	tmp->left = node;
}

/******************************************************************************/

static void RotRight(node_t *node, node_t *parent)
{
	node_t *tmp = NULL;
	assert(node);
	assert(parent);

	ConnectParent2NextNode(parent, node, node->left);
	tmp = node->left;
	node->left = tmp->right;
	tmp->right = node;
}

/******************************************************************************/

static void BalanceTree(node_t *node, node_t *parent)
{
	if (2 == node->balance_factor)
	{

		if (-1 == node->left->balance_factor)
		{
			RotLeft(node->left, node);
		}
		node->left->balance_factor = 0;
		RotRight(node, parent);
	}
	else if (-2 == node->balance_factor)
	{
		if (1 == node->right->balance_factor)
		{
			RotRight(node->right, node);
		}
		node->right->balance_factor = 0;
		RotLeft(node, parent);
	}
	node->balance_factor = 0;
}

/******************************************************************************/
/*In case of insert func = 1, for remove func = -1 */
static int BalanceUpdate(node_t *node, node_t *parent,int (*cmp_func)(const void *data1, const void *data2), void  *data,int func)
{
	int res = 0;
	if (0 == cmp_func(data,node->data))
	{
		return (0);
	}


	if (cmp_func(data, node->data) > 0)
	{
		res = BalanceUpdate(node->right, node, cmp_func, data,func);
		if (!res)
		{
			node->balance_factor -= 1*func;;
		}
	}
	else
	{
		res = BalanceUpdate(node->left, node, cmp_func, data,func);
		if (!res)
		{
			node->balance_factor += 1*func; ;
		}
	}

	if (!res && (node->balance_factor == 2 || node->balance_factor == -2))
	{
		BalanceTree(node, parent);
		return (1);
	}
	return (res);
}

/***********************************************************************************/
/*
int IsFullTree(node_t *node)
{
	if (!node->left && !node->right)
	{
		return 1;
	}
	IsFullTree(node->left);
	IsFullTree(node->right);
	if (node->left && node->right)
	{
		return 1;
	}
	return 0;
}
int IsFullTree(node_t *node)
{
	if (!node)
	{
		return 1;
	}
	
	if ((!node->left && node->right)||(node->left && !node->right))
	{
		return 0;
	}
	return IsFullTree(node->left)&&	IsFullTree(node->right);
}
*/
