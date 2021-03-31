#pragma once
#include "hyperscan/hs.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
// 从hyperscan测试用例里面整理能充分说明用法的例子
// HS_MODE_BLOCK 和 HS_MODE_NOSTREAM两个宏是等价的
// HS_MODE_STREAM
// HS_MODE_VECTORED

struct MatchRecord {
	MatchRecord(unsigned long long t, int i)
		: to(t)
		, id(i) {}
	bool operator==(const MatchRecord &o) const {
		return to == o.to && id == o.id;
	}
	unsigned long long to;
	int id;
};

std::ostream &operator<<(std::ostream &o, const MatchRecord &m);

struct CallBackContext {
	bool halt = false;
	std::vector<MatchRecord> matches;

	void clear() {
		halt = false;
		matches.clear();
	}
};

static int dummy_cb(unsigned int id, unsigned long long from, unsigned long long to, unsigned int flags, void *context) {
	if (from)
	{
		return 1;
	}
	
	int& length = *(int*)context;
	length = to;
	return 0;
}

static int alloc_called = 0;

static void *my_alloc(size_t s) {
	++alloc_called;
	return malloc(s);
}
	
static void my_free(void *p) {
	free(p);
	--alloc_called;
}

static int record_cb(unsigned id, unsigned long long from, unsigned long long to, unsigned int flag, void *ctxt) {
	CallBackContext *c = (CallBackContext *)ctxt;

	c->matches.emplace_back(to, id);

	return (int)c->halt;
}