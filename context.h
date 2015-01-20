#ifndef CONTEXT_H
#define CONTEXT_H

typedef struct Context
{
	unsigned short *addr;
	int width;
	int height;
} Context;

//context 函数接口
void init_context(struct Context* context_ptr, int width, int height);
void free_context(struct Context* context_ptr);

#endif