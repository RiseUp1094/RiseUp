#include <ctype.h>
#include <iostream>
#include <stdlib.h>
#include <thread>
#include <chrono>


using namespace std;


int const NUM_OF_THREADS = 2;
int const SIZE = 8000;


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
// Вставка узла
__attribute__((transaction_unsafe))
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
		//	printf("Out of Space\n");
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
			//printf("Элемет существует\n");
		}
	}
	int m, n, d;
	m = bsheight(p->left);
	n = bsheight(p->right);
	d = max(m, n);
	p->height = d + 1;
}
// Нахождение минимума
nodeptr bstree::findmin(nodeptr p)
{
	__transaction_atomic{
	if (p == NULL)
	{
		//printf("В дереве нет элементов\n");
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
}
// Нахождение максимума
nodeptr bstree::findmax(nodeptr p)
{
	__transaction_atomic{
	if (p == NULL)
	{
		//printf("В дереве нет элементов\n") ;
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
}
// Поиск элемента

void bstree::find(int x, nodeptr &p)
{
	if (p == NULL)
	{
		//printf("Простите, но такого элемента нет\n");
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
			//	printf("Элемент, который вы искали есть в дереве!\n");
			}
		}
	}
}
// Копирование 
void bstree::copy(nodeptr &p, nodeptr &p1)
{
	makeempty(p1);
	p1 = nodecopy(p);
}

// Опустошение дерева
void bstree::makeempty(nodeptr &p)
{
	__transaction_atomic{
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
}

// Копирование узлов
nodeptr bstree::nodecopy(nodeptr &p)
{
	__transaction_atomic{
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
}
// Удаление узла
__attribute__((transaction_unsafe))
void bstree::del(int x, nodeptr &p)
{

	nodeptr d;
	if (p == NULL)
	{
		//printf("Простите, но такого элемента нет\n");
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
		//printf("Элемент удален\n");
	}
	else if (p->left == NULL)
	{
		d = p;
		free(d);
		p = p->right;
		//printf("Элемент удален\n");;
	}
	else if (p->right == NULL)
	{
		d = p;
		p = p->left;
		free(d);
		//printf("Элемент удален\n");;
	}
	else
	{
		p->element = deletemin(p->right);
	}
}

__attribute__((transaction_unsafe))
int bstree::deletemin(nodeptr &p)
{
	int c;
	//printf("Выбрано удаление минимального значения\n");
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
__attribute__((transaction_unsafe))
void bstree::preorder(nodeptr p)
{

	if (p != NULL)
	{
		//printf("%d\t",p->element);
		preorder(p->left);
		preorder(p->right);
	}
}

// Inorder Printing
__attribute__((transaction_unsafe))
void bstree::inorder(nodeptr p)
{
	if (p != NULL)
	{
		inorder(p->left);
		//printf("%d\t",p->element);
		inorder(p->right);
	}
}

// PostOrder Printing
__attribute__((transaction_unsafe))
void bstree::postorder(nodeptr p)
{

	if (p != NULL)
	{
		postorder(p->left);
		postorder(p->right);
		//printf("%d\t",p->element);
	}
}

__attribute__((transaction_unsafe))
int bstree::max(int value1, int value2)
{
	return ((value1 > value2) ? value1 : value2);
}

int bstree::bsheight(nodeptr p)
{
	__transaction_atomic{
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
}

//Малое левое вращение 
__attribute__((transaction_unsafe))
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

//Малое правое вращение
__attribute__((transaction_unsafe))
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
// Большое левое вращение
__attribute__((transaction_unsafe))
nodeptr bstree::drl(nodeptr &p1)
{
	p1->left = srr(p1->left);
	
	return srl(p1);

}

// Большое правое вращение
__attribute__((transaction_unsafe))
nodeptr bstree::drr(nodeptr &p1)
{
	p1->right = srl(p1->right);	
	return srr(p1);

}

int bstree::nonodes(nodeptr p)
{
	__transaction_atomic{
	int count = 0;
	if (p != NULL)
	{
		nonodes(p->left);
		nonodes(p->right);
		count++;
	}
	return count;
}
}
__attribute__((transaction_unsafe))
void ThreadFunction(bstree &bst, int num, nodeptr root)
{
	int part = SIZE / NUM_OF_THREADS;
	int op;
	unsigned int seed = (unsigned int)(time(NULL)*num);
	for (int i = num*part; i < (num*part + part); ++i) {
		op = rand() % 100;
		if (op < 35) {
			
			bst.find(rand() % part*num + part*(num + 1), root);
			
		}
		else if (op > 65) {
			
			bst.insert(rand() % part*num + part*(num + 1), root);
			
		}
		else {
			
			bst.del(rand() % part*num + part*(num + 1), root);
			
		}
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");
	//clrscr();
	nodeptr root, root1, min, max;//,flag;
								  //int a, choice, findele, delele, num;
	bstree bst;

	//system("clear");
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
