#include "interval_tree.h"
//============================================================================ 
//constructor & destructor
//============================================================================ 
interval_tree::interval_tree()
{
	NIL = new Node_();
	NIL->color = BLACK;
	root = NULL;
}

interval_tree::~interval_tree()
{
	if (root)
		delete_tree(root);
	delete NIL;
}


//============================================================================ 
//private
//============================================================================ 
int interval_tree::max(int a, int b, int c)
{
	return (a > b ? (a > c ? a : c) : (b > c ? b : c));
}


void interval_tree::rotate_left(Node_ *p)
{
	if (p->right == NIL)
		return;

	Node_ *t = p->right;
	p->max = max(p->end, p->left->max, t->left->max);
	t->max = max(t->end, t->right->max, p->max);
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
	if (p == root)
		root = t;
}

void interval_tree::rotate_right(Node_ *p)
{
	if (p->left == NIL)
		return;

	Node_ *t = p->left;
	p->max = max(p->end, p->right->max, t->right->max);
	t->max = max(t->end, t->left->max, p->max);
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
	if (p == root)
		root = t;
}

void interval_tree::print_inorder(Node_ *p)
{
	if (!p || p == NIL)
		return;
	if (p->left)
		print_inorder(p->left);

	cout << p->name << "(" << p->start << "," << p->end << ")" << " ";

	if (p->right)
		print_inorder(p->right);
}


//Insert
void interval_tree::insert_data(Node_ *T, int start, int end, int n, string name)
{
	Node_ *p = new Node_(start, end, n, name);
	Node_ *x = T, *y = NIL;
	while (x != NIL)
	{
		y = x;
		if (p->max > x->max)
			x->max = p->max;
		if (start < x->start)
			x = x->left;
		else
			x = x->right;
	}
	p->parent = y;
	if (y == NIL)
		root = p;
	else if (p->start < y->start)
		y->left = p;
	else
		y->right = p;
	p->left = NIL;
	p->right = NIL;

	insert_fixup(p);
}

void interval_tree::insert_fixup(Node_ *p)
{
	while (p->parent && p->grandparent() && p->parent->color == RED && p->grandparent() != NULL)
	{
		Node_ *y = p->uncle();
		if (p->parent == p->grandparent()->left)
		{
			if (y->color == RED)
			{
				y->color = BLACK;
				p->parent->color = BLACK;
				p->grandparent()->color = RED;
				p = p->grandparent();
			}
			else if (p == p->parent->right && p->uncle() && p->uncle()->color == BLACK)
			{
				p = p->parent;
				rotate_left(p);
			}
			if (p->uncle() && p->uncle()->color == BLACK)
			{
				p->parent->color = BLACK;
				p->grandparent()->color = RED;
				rotate_right(p->grandparent());
			}
		}
		else
		{
			if (y->color == RED)
			{
				y->color = BLACK;
				p->parent->color = BLACK;
				p->grandparent()->color = RED;
				p = p->grandparent();
			}
			else if (p == p->parent->left && p->uncle() && p->uncle()->color == BLACK)
			{
				p = p->parent;
				rotate_right(p);
			}
			if (p->uncle() && p->uncle()->color == BLACK)
			{
				p->parent->color = BLACK;
				p->grandparent()->color = RED;
				rotate_left(p->grandparent());
			}
		}
	}
	root->color = BLACK;
}


//Search
vector<Node_*> interval_tree::search_data(Node_ *p, int start, int end, bool print)
{
	vector<Node_*> v;
	vector<Node_*>::iterator itr;
	if (p != NIL && start <= p->max)
	{
		if (start <= p->end && end >= p->start)
			v.push_back(p);

		vector<Node_*> v_l;
		if (start <= p->left->max)
		{
			v_l = search_data(p->left, start, end);
			itr = v_l.begin();
			for (; itr != v_l.end(); itr++)
				v.push_back(*itr);
		}

		vector<Node_*> v_r;
		if (start > p->left->max || v_l.begin() != v_l.end())
		{
			v_r = search_data(p->right, start, end);
			itr = v_r.begin();
			for (; itr != v_r.end(); itr++)
				v.push_back(*itr);
		}
	}
	if (print)
		for (itr = v.begin(); itr != v.end(); itr++)
		{
			cout << (*itr)->name << "(" << (*itr)->start << "," << (*itr)->end << ")" << " ";
		}
	return v;
}

//Delete
void interval_tree::transplant(Node_ *u, Node_ * v)
{
	if (u->parent == NULL)
		root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;
	if (v != NIL)
		v->parent = u->parent;
}

bool interval_tree::delete_data(Node_ *p)
{
	Node_ *y = p, *x = NIL;
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

Node_* interval_tree::tree_minimum(Node_ *p)
{
	while (p->left != NIL)
		p = p->left;
	return p;
}

void interval_tree::delete_fixup(Node_ *p)
{
	while (p != root && p->color == BLACK)
	{
		Node_ *w = p->sibling();
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
void interval_tree::delete_tree(Node_ *p)
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
void interval_tree::print_inorder()
{
	if (root == NULL)
		return;
	print_inorder(root);
	cout << endl;
	cout << "Finish!\n\n";
}

void interval_tree::print_layer()
{
	if (root == NULL)
		return;
	vector<Node_*> v;
	Node_ *p = root;
	v.push_back(p);
	vector<Node_*>::iterator itr = v.begin();
	for (int i = 1; itr != v.end(); i++)
	{
		p = *itr;
		cout << p->name << "(" << p->start << "," << p->end << ")" << " ";
		if (p->left != NIL)
			v.push_back(p->left);
		if (p->right != NIL)
			v.push_back(p->right);
		itr = v.begin() + i;
	}
	cout << endl;
	cout << "Finish!\n\n";
}

void interval_tree::insert_data(int start ,int end, int n, string name)
{
	if (root == NULL)
	{
		root = new Node_(start, end, n, name);
		root->color == BLACK;
		root->left = root->right = NIL;
	}
	else
		insert_data(root, start, end, n, name);
}

bool interval_tree::delete_data(int start, int end)
{
	vector<Node_*> v = search_data(root, start, end);
	vector<Node_*>::iterator itr = v.begin();
	for (; itr != v.end(); itr++)
		delete_data(*itr);
	return true;
}

void interval_tree::search_data(int start, int end)
{
	search_data(root, start, end, true);
	cout << endl;
}