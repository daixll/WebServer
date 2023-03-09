#include "MemoryPool.h"

MemoryPool::MemoryPool(ull size) {
	for (ull i = 1024; i <= size; i *= 2) {
		BlockInfo* t = new BlockInfo();
		t->live = 0;	// 活跃 0	
		t->have = 0;	// 存在 0
		t->p = NULL;	// 指向 空
		Bmap[i] = t;	// 信息块
	}
}
MemoryPool::~MemoryPool() {
	unsigned long long i = 1024;
	while (Bmap.count(i)) {
		Block* q = Bmap[i]->p;
		do {
			q->st = 0;
			q = q->next;
		} while (q != Bmap[i]->p);
		i *= 2;
	}
	fast();
}

ull MemoryPool::to2(ull size) {
	int res = 1024;
	while (res < size)res *= 2;
	return res;
}

Block* MemoryPool::nb(ull size) {
	size = to2(size);
	if (Bmap[size]->have == 0) {		// 未初始化
		Bmap[size]->p = new Block();
		if (Bmap[size]->p == NULL) return NULL;
		Bmap[size]->p->size = size;
		Bmap[size]->p->st = 0;
		Bmap[size]->p->data = new char[size];
		Bmap[size]->p->next = Bmap[size]->p;
		Bmap[size]->have = 1;
		Bmap[size]->live = 0;
	}
	if (Bmap[size]->live == Bmap[size]->have) {
		for (int i = 1; i <= Bmap[size]->have; i++) {
			Block* one = new Block();
			one->size = size;
			one->st = 0;
			one->data = new char[size];
			one->next = Bmap[size]->p->next;
			Bmap[size]->p->next = one;
			Bmap[size]->p = one;	// 可以优化
		}
		Bmap[size]->have *= 2;
	}
	while (Bmap[size]->p->st == 1)
		Bmap[size]->p = Bmap[size]->p->next;
	Bmap[size]->live++;		// 活跃的增加
	Bmap[size]->p->st = 1;	// 标记使用
	return Bmap[size]->p;
}

void MemoryPool::sb(Block* b) {
	b->st = 0;				// 未在使用
	Bmap[b->size]->live--;	// 活跃的减少
}

void MemoryPool::getinfo() {
	unsigned long long i = 1024;
	while (Bmap.count(i)) {
		printf("%20lld%20d%20d\n", i, Bmap[i]->live, Bmap[i]->have);
		i *= 2;
	}
	puts("\n\n");
}

void MemoryPool::fast() {
	unsigned long long i = 1024;
	while(Bmap.count(i)){
		if (Bmap[i]->have == 0) {
			i *= 2;
			continue;
		}
		Block* a = Bmap[i]->p;
		Block* b = a->next;
		do {
			if (b->st == 0) {	// 未使用
				a->next = b->next;
				delete b->data;
				delete b;
				Bmap[i]->have--;
			}
			else {
				a = a->next;
			}
			b = a->next;
		} while (Bmap[i]->have != Bmap[i]->live);

		Bmap[i]->p = a;

		i *= 2;
	}
	getinfo();
	return;
}

void MemoryPool::w2b(const char* file, char* data){
    FILE* fp=NULL;
    fp=fopen(file, "r");
    fread(data, sizeof(char), 1000, fp);
}