#include "rb_tree.h"
//This code is the initial rb_tree version to accomplish interval tree, so it contains bugs and has not been debugged.
//interval_tree has been debugged and can work well.

//============================================================================ 
//constructor & destructor
//============================================================================ 
rb_tree::rb_tree()
{
	NIL = new Node();
	NIL->color = BLACK;
	root = NULL;
}

rb_tree::~rb_tree()
{
	if (root)
		delete_tree(root);
	delete NIL;
}


//============================================================================ 
//private
//============================================================================ 
void rb_tree::rotate_left(Node *p)
{
	if (p->right == NIL)
		return;

	Node *t = p->right;
	p->right = t->left;
	t->parent = p->parent;
	if (t->left != NIL)
		t->left->parent = p;
	if (p->parent != NULL)
	{
		if (p->parent->right == p)
			p->parent->right = t;
		else
			p->parent->left = t;
	}
	t->left = p;
	p->parent = t;
}

void rb_tree::rotate_right(Node *p)
{
	if (p->right == NIL)
		return;

	Node *t = p->left;
	p->left = t->right;
	t->parent = p->parent;
	if (t->right != NIL)
		t->right->parent = p;
	if (p->parent != NULL)
	{
		if (p->parent->right == p)
			p->parent->right = t;
		else
			p->parent->left = t;
	}
	t->right = p;
	p->parent = t;
}

void rb_tree::print_inorder(Node *p)
{
	if (!p || p == NULL)
		return;
	if (p->left)
		print_inorder(p->left);

	cout << p->value << " ";

	if (p->right)
		print_inorder(p->right);
}


//Insert
void rb_tree::insert_data(Node *T, int data)
{
	Node *p = new Node(data);
	Node *x = T, *y = NIL;
	while (x != NIL)
	{
		y = x;
		if (data < x->value)
			x = x->left;
		else
			x = x->right;
	}
	p->parent = y;
	if (y == NIL)
		root = p;
	else if (p->value < y->value)
		y->left = p;
	else
		y->right = p;
	p->left = NIL;
	p->right = NIL;

	insert_fixup(p);
}

void rb_tree::insert_fixup(Node *p)
{
	while (p->parent->color == RED)
	{
		Node *y = p->uncle();
		if (p->parent == p->grandparent()->left)
		{
			if (y->color == RED)
			{
				y->color = BLACK;
				p->right->color = BLACK;
				p->grandparent()->color = RED;
				p = p->grandparent();
			}
			else if (p = p->parent->right)
			{
				p = p->parent;
				rotate_left(p);
			}
			p->parent->color = BLACK;
			p->grandparent()->color =RED;
			rotate_right(p->grandparent());
		}
		else
		{
			if (y->color == RED)
			{
				y->color = BLACK;
				p->left->color = BLACK;
				p->grandparent()->color = RED;
				p = p->grandparent();
			}
			else if (p = p->parent->left)
			{
				p = p->parent;
				rotate_right(p);
			}
			p->parent->color = BLACK;
			p->grandparent()->color = RED;
			rotate_left(p->grandparent());
		}
	}
	root->color = BLACK;
}

//Delete
void rb_tree::transplant(Node *u, Node * v)
{
	if (u->parent == NIL)
		root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;
	if (v != NIL)
		v->parent = u->parent;
}

bool rb_tree::delete_data(Node *p)
{
	Node *y = p, *x = NIL;
	bool y_color = y->color;
	if (p->left == NIL)
	{
		x = p->right;
		transplant(p, p->right);
	}
	else if (p->right == NIL)
	{
		x = p->left;
		transplant(p, p->left);
	}
	else
	{
		y = tree_minimum(p->right);
		y_color = y->color;
		x = y->right;
		if (y->parent == p)
			x->parent = y;
		else
		{
			transplant(y, y->right);
			y->right = p->right;
			y->right->parent = y;
		}
		transplant(p, y);
		y->left = p->left;
		y->left->parent = y;
		y->color = p->color;
	}
	if (y_color == BLACK)
		delete_fixup(x);
	return true;
}

Node* rb_tree::tree_minimum(Node *p)
{
	while (p->left != NIL)
		p = p->left;
	return p;
}

void rb_tree::delete_fixup(Node *p)
{
	while (p != root && p->color == BLACK)
	{
		Node *w = p->sibling();
		if (p == p->parent->left)
		{
			if (w->color == RED)
			{
				w->color = BLACK;
				p->parent->color = RED;
				rotate_left(p->parent);
				w = p->parent->right;
			}
			if (w->left->color == BLACK && w->right->color == BLACK)
			{
				w->color = RED;
				p = p->parent;
			}
			else if (w->right->color == BLACK)
			{
				w->left->color = BLACK;
				w->color = RED;
				rotate_right(w);
				w = p->parent;
			}
			w->color = p->parent->color;
			p->parent->color = BLACK;
			w->right->color = BLACK;
			rotate_left(p->parent);
			p = root;
		}
		else
		{
			if (w->color == RED)
			{
				w->color = BLACK;
				p->parent->color = RED;
				rotate_right(p->parent);
				w = p->parent->left;
			}
			if (w->right->color == BLACK && w->left->color == BLACK)
			{
				w->color = RED;
				p = p->parent;
			}
			else if (w->left->color == BLACK)
			{
				w->right->color = BLACK;
				w->color = RED;
				rotate_left(w);
				w = p->parent;
			}
			w->color = p->parent->color;
			p->parent->color = BLACK;
			w->left->color = BLACK;
			rotate_right(p->parent);
			p = root;
		}
	}
	p->color = BLACK;
}

//Delete tree
void rb_tree::delete_tree(Node *p)
{
	if (!p || p == NIL)
		return;
	delete_tree(p->left);
	delete_tree(p->right);
	delete p;
}


//============================================================================ 
//public
//============================================================================ 
void rb_tree::print_inorder()
{
	if (root == NULL)
		return;
	print_inorder(root);
	cout << endl;
}

void rb_tree::insert_data(int data)
{
	if (root == NULL)
	{
		root = new Node(data);
		root->color == BLACK;
		root->left = root->right = NIL;
	}
	else
		insert_data(root, data);
}

bool rb_tree::delete_data(int data)
{
	Node *p = search_data(data);
	if (p == NIL)
		return false;
	return delete_data(p);
}

Node* rb_tree::search_data(int data, bool print)
{
	Node *p = root;
	while (p != NIL)
	{
		if (data == p->value)
			return p;
		else if (data < p->value)
		{
			if (print)
				cout << "L ";
			p = p->left;
		}
		else
		{
			if (print)
			cout << "R ";
			p = p->right;
		}
	}
	return p;
}