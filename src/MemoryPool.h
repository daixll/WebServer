#pragma once
#include <string.h>
#include <iostream>
#include <unordered_map>

typedef unsigned long long ull;

class Block {
public:
	bool	st;		// 是否在使用
	Block* next;	// 下个结点
	ull		size;	// 该块大小
	char* data;		// 数据
};

struct BlockInfo{
	int		live;	// 活跃的
	int		have;	// 存在的
	Block*	p;		// 指向一个空闲的
};

class MemoryPool {
private:
	std::unordered_map<ull, BlockInfo*> Bmap;	// 内存信息一览表
	ull to2(ull size);		// 转换为2的幂
public:
	MemoryPool(ull size);	// 内存池允许的单个文件最大大小
	~MemoryPool();
	Block* nb(ull size);	// 拿 内存块
	void sb(Block*);		// 还 内存块
	void getinfo();			// 内存池信息
	void fast();			// 优化空间

    void w2b(const char* file, char* data); // 将文件中的内容写到内存块中
};