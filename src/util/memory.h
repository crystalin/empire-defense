#ifndef __MEMORY_H__
#define __MEMORY_H__

#ifdef CHECK_MEMORY

void EDMODEL_API AddTrack(int addr,  int asize,  const char *fname, int lnum);
void EDMODEL_API RemoveTrack(int addr);

inline void * __cdecl operator new(unsigned int size,
									 const char *file, int line) {
	void *ptr = (void *)malloc(size);
	AddTrack((int)ptr, size, file, line);
	return(ptr);
};

inline void * __cdecl operator new[](unsigned int size,
									 const char *file, int line) {
	void *ptr = (void *)malloc(size);
	AddTrack((int)ptr, size, file, line);
	return(ptr);
};

inline void __cdecl operator delete(void *p, const char *file, int line) {
	RemoveTrack((int)p);
	free(p);
};

#undef delete
inline void __cdecl operator delete(void *p) {
	RemoveTrack((int)p);
	free(p);
};

inline void __cdecl operator delete[](void *p) {
	RemoveTrack((int)p);
	free(p);
};

inline void __cdecl operator delete[](void *p, const char *file, int line) {
	RemoveTrack((int)p);
	free(p);
};

void EDMODEL_API DumpUnfreed();

#define new new(__FILE__, __LINE__)

#endif



#endif