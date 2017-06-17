#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include <conio.h>
#include <mutex>
#include <thread>
#include <chrono>

using namespace std;

mutex mut;

int const NUM_OF_THREADS = 8;
int const SIZE = 5000;


struct node
{
	int element;
	node *left;
	node *right;
	int height;
};

typedef struct node *nodeptr;

class bstree
{
public:
	void insert(int, nodeptr &);
	void del(int, nodeptr &);
	int deletemin(nodeptr &);
	void find(int, nodeptr &);
	nodeptr findmin(nodeptr);
	nodeptr findmax(nodeptr);
	void makeempty(nodeptr &);
	void copy(nodeptr &, nodeptr &);
	nodeptr nodecopy(nodeptr &);
	void preorder(nodeptr);
	void inorder(nodeptr);
	void postorder(nodeptr);
	int bsheight(nodeptr);
	nodeptr srl(nodeptr &);
	nodeptr drl(nodeptr &);
	nodeptr srr(nodeptr &);
	nodeptr drr(nodeptr &);
	int max(int, int);
	int nonodes(nodeptr);
};
// Âñòàâêà óçëà
void bstree::insert(int x, nodeptr &p)
{
	
	if (p == NULL)
	{
		p = new node;
		p->element = x;
		p->left = NULL;
		p->right = NULL;
		p->height = 0;
		if (p == NULL)
		{
			cout << "Out of Space\n" << endl;
		}
	}
	else
	{
		if (x<p->element)
		{
			insert(x, p->left);
			if ((bsheight(p->left) - bsheight(p->right)) == 2)
			{
				if (x < p->left->element)
				{
					p = srl(p);
				}
				else
				{
					p = drl(p);
				}
			}
		}
		else if (x>p->element)
		{
			insert(x, p->right);
			if ((bsheight(p->right) - bsheight(p->left)) == 2)
			{
				if (x > p->right->element)
				{
					p = srr(p);
				}
				else
				{
					p = drr(p);
				}
			}
		}
		else
		{
			cout << "Ýëåìåò ñóùåñòâóåò\n" << endl;
		}
	}
	int m, n, d;
	m = bsheight(p->left);
	n = bsheight(p->right);
	d = max(m, n);
	p->height = d + 1;
	
}
// Íàõîæäåíèå ìèíèìóìà
nodeptr bstree::findmin(nodeptr p)
{
	
	if (p == NULL)
	{
		cout << "Â äåðåâå íåò ýëåìåíòîâ\n" << endl;
		return p;
	}
	else
	{
		while (p->left != NULL)
		{
			p = p->left;
			//return p;
		}
		return p;
	}
	
}
// Íàõîæäåíèå ìàêñèìóìà
nodeptr bstree::findmax(nodeptr p)
{
	
	if (p == NULL)
	{
		cout << "Â äåðåâå íåò ýëåìåíòîâ\n" << endl;
		return p;
	}
	else
	{
		while (p->right != NULL)
		{
			p = p->right;
			//return p;
		}
		return p;
	}
	
}
// Ïîèñê ýëåìåíòà
void bstree::find(int x, nodeptr &p)
{
	
	if (p == NULL)
	{
		cout << "Ïðîñòèòå, íî òàêîãî ýëåìåíòà íåò\n" << endl;
	}
	else
	{
		if (x < p->element)
		{
			find(x, p->left);
		}
		else
		{
			if (x>p->element)
			{
				find(x, p->right);
			}
			else
			{
				cout << "Ýëåìåíò, êîòîðûé âû èñêàëè åñòü â äåðåâå!\n" << endl;
			}
		}
	}
	
}
// Êîïèðîâàíèå 
void bstree::copy(nodeptr &p, nodeptr &p1)
{
	makeempty(p1);
	p1 = nodecopy(p);
}

// Îïóñòîøåíèå äåðåâà
void bstree::makeempty(nodeptr &p)
{
	nodeptr d;
	if (p != NULL)
	{
		makeempty(p->left);
		makeempty(p->right);
		d = p;
		free(d);
		p = NULL;
	}
}

// Êîïèðîâàíèå óçëîâ
nodeptr bstree::nodecopy(nodeptr &p)
{
	
	nodeptr temp;
	if (p == NULL)
	{
		return p;
	}
	else
	{
		temp = new node;
		temp->element = p->element;
		temp->left = nodecopy(p->left);
		temp->right = nodecopy(p->right);
		return temp;
	}
	
}
// Óäàëåíèå óçëà
void bstree::del(int x, nodeptr &p)
{
	nodeptr d;
	if (p == NULL)
	{
		cout << "Ïðîñòèòå, íî òàêîãî ýëåìåíòà íåò\n" << endl;
	}
	else if (x < p->element)
	{
		del(x, p->left);
	}
	else if (x > p->element)
	{
		del(x, p->right);
	}
	else if ((p->left == NULL) && (p->right == NULL))
	{
		d = p;
		free(d);
		p = NULL;
		cout << "Ýëåìåíò óäàëåí\n" << endl;
	}
	else if (p->left == NULL)
	{
		d = p;
		free(d);
		p = p->right;
		cout << "Ýëåìåíò óäàëåí\n" << endl;
	}
	else if (p->right == NULL)
	{
		d = p;
		p = p->left;
		free(d);
		cout << "Ýëåìåíò óäàëåí\n" << endl;
	}
	else
	{
		p->element = deletemin(p->right);
	}
}

int bstree::deletemin(nodeptr &p)
{
	
	int c;
	cout << "Âûáðàíî óäàëåíèå ìèíèìàëüíîãî çíà÷åíèÿ\n" << endl;
	if (p->left == NULL)
	{
		c = p->element;
		p = p->right;
		return c;
	}
	else
	{
		c = deletemin(p->left);
		return c;
	}
	
}

//Preorder Printing
void bstree::preorder(nodeptr p)
{
	
	if (p != NULL)
	{
		cout << p->element << "\t";
		preorder(p->left);
		preorder(p->right);
	}
	
}

// Inorder Printing
void bstree::inorder(nodeptr p)
{
	
	if (p != NULL)
	{
		inorder(p->left);
		cout << p->element << "\t";
		inorder(p->right);
	}
	
}

// PostOrder Printing
void bstree::postorder(nodeptr p)
{
	
	if (p != NULL)
	{
		postorder(p->left);
		postorder(p->right);
		cout << p->element << "\t";
	}
	
}

int bstree::max(int value1, int value2)
{
	return ((value1 > value2) ? value1 : value2);
}

int bstree::bsheight(nodeptr p)
{
	int t;
	if (p == NULL)
	{
		return -1;
	}
	else
	{
		t = p->height;
		return t;
	}
}

//Ìàëîå ëåâîå âðàùåíèå 
nodeptr bstree::srl(nodeptr &p1)
{
	
	nodeptr p2;
	p2 = p1->left;
	p1->left = p2->right;
	p2->right = p1;
	p1->height = max(bsheight(p1->left), bsheight(p1->right)) + 1;
	p2->height = max(bsheight(p2->left), p1->height) + 1;
	
	return p2;

}

//Ìàëîå ïðàâîå âðàùåíèå
nodeptr bstree::srr(nodeptr &p1)
{
	
	nodeptr p2;
	p2 = p1->right;
	p1->right = p2->left;
	p2->left = p1;
	p1->height = max(bsheight(p1->left), bsheight(p1->right)) + 1;
	p2->height = max(p1->height, bsheight(p2->right)) + 1;
	
	return p2;
}
// Áîëüøîå ëåâîå âðàùåíèå
nodeptr bstree::drl(nodeptr &p1)
{
	
	p1->left = srr(p1->left);
	
	return srl(p1);

}

// Áîëüøîå ïðàâîå âðàùåíèå
nodeptr bstree::drr(nodeptr &p1)
{
	
	p1->right = srl(p1->right);
	
	return srr(p1);

}

int bstree::nonodes(nodeptr p)
{
	int count = 0;
	if (p != NULL)
	{
		nonodes(p->left);
		nonodes(p->right);
		count++;
	}
	return count;
}

void ThreadFunction(bstree &bst, int num, nodeptr root)
{
	int part = SIZE / NUM_OF_THREADS;
	int op;
	for (int i = num*part; i < (num*part + part); ++i) {
		op = rand() % 100;
		if (op < 35) {
			mut.lock();
			bst.find(rand() % part*num + part*(num + 1), root);
			mut.unlock();
		}
		else if (op > 65) {
			mut.lock();
			bst.insert(rand() % part*num + part*(num + 1), root);
			mut.unlock();
		}
		else {
			mut.lock();
			bst.del(rand() % part*num + part*(num + 1), root);
			mut.unlock();
		}
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");
	nodeptr root, root1, min, max;
    bstree bst;
	root = NULL;
	root1 = NULL;
	for (int i = 0; i < SIZE ; ++i) {
		bst.insert(i, root);
	}
	std::thread thr[NUM_OF_THREADS];
	auto begin = chrono::high_resolution_clock::now();
	for (int i = 0; i<NUM_OF_THREADS; ++i) {
		thr[i] = std::thread(ThreadFunction, std::ref(bst), i, root);
	}
	for (int i = 0; i<NUM_OF_THREADS; ++i) {
		thr[i].join();
	}
	auto end = chrono::high_resolution_clock::now();
	cout << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "ms" << endl;
	system("pause");
	return 0;
}
