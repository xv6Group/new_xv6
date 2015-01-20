struct Context
{
	unsigned short *addr;
	int width;
	int height;
};

void init_context(struct Context* context_ptr, int width, int height);
void free_context(struct Context* context_ptr);
