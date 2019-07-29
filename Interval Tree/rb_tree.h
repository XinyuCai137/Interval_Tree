#include <iostream>    
#define BLACK 1
#define RED 0 

//This code is the initial rb_tree version to accomplish interval tree, so it contains bugs and has not been debugged.
//interval_tree has been debugged and can work well.

using namespace std;

struct Node
{
	int value;
	bool color;
	Node *parent, *left, *right;

	Node() : value(0), color(RED), left(NULL), right(NULL), parent(NULL) { }
	Node(int x) : value(x), color(RED), left(NULL), right(NULL), parent(NULL) { }

	Node* grandparent()
	{
		if (parent == NULL)
			return NULL;
		return parent->parent;
	}
	Node* uncle()
	{
		if (grandparent() == NULL)
			return NULL;
		if (parent == grandparent()->left)
			return grandparent()->right;
		else
			return grandparent()->left;
	}
	Node* sibling()
	{
		if (this == parent->left)
			return parent->right;
		else
			return parent->left;
	}
};

class rb_tree
{

private:

	void rotate_right(Node *p);
	void rotate_left(Node *p);
	void print_inorder(Node *p);

	void insert_data(Node *T, int data);
	void insert_fixup(Node *p);

	void transplant(Node *u, Node * v);
	bool delete_data(Node *p);
	void delete_fixup(Node *p);
	Node* tree_minimum(Node *p);

	void delete_tree(Node *p);

public:
	rb_tree();
	~rb_tree();
	void print_inorder();
	void insert_data(int data);
	bool delete_data(int data);
	Node* search_data(int data, bool print = false);

private:
	Node * root, *NIL;
};   

