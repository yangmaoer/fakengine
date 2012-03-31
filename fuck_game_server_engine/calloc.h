#pragma once

extern "C" void * sys_alloc(size_t size)
{
	return malloc(size);
}

extern "C" void sys_free(void * p)
{
	FASSERT(p);
	free(p);
}
