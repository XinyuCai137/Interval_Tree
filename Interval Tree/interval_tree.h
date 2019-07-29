#pragma once
#include <iostream>
#include <vector>
#include <string>
#ifndef RED
#define RED 0
#define BLACK 1
#endif // !RED

using namespace std;

struct Node_
{
	int start;
	int end;
	int num;
	string name;
	
	bool color;
	int max;

	Node_ *parent, *left, *right;

	Node_() : start(0), end(0), max(0),num(0), color(RED), left(NULL), right(NULL), parent(NULL) { }
	Node_(int x, int y, int n, string name) : start(x), end(y), max(y), num(n), name(name), color(RED), left(NULL), right(NULL), parent(NULL) { }

	Node_* grandparent()
	{
		if (parent == NULL)
			return NULL;
		return parent->parent;
	}
	Node_* uncle()
	{
		if (grandparent() == NULL)
			return NULL;
		if (parent == grandparent()->left)
			return grandparent()->right;
		else
			return grandparent()->left;
	}
	Node_* sibling()
	{
		if (this == parent->left)
			return parent->right;
		else
			return parent->left;
	}
};

class interval_tree
{

private:

	int max(int a, int b, int c);
	void rotate_right(Node_ *p);
	void rotate_left(Node_ *p);
	void print_inorder(Node_ *p);

	void insert_data(Node_ *T, int start, int end, int n, string name);
	void insert_fixup(Node_ *p);

	vector<Node_*> search_data(Node_ *p, int start, int end, bool print = false);

	void transplant(Node_ *u, Node_ * v);
	bool delete_data(Node_ *p);
	void delete_fixup(Node_ *p);
	Node_* tree_minimum(Node_ *p);

	void delete_tree(Node_ *p);

public:
	interval_tree();
	~interval_tree();
	void print_inorder();
	void print_layer();
	void insert_data(int start, int end, int n, string name);
	bool delete_data(int start, int end);
	void search_data(int start, int end);

private:
	Node_ * root, *NIL;
};