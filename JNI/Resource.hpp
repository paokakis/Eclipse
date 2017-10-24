#ifndef _PACKT_RESOURCE_HPP_
#define _PACKT_RESOURCE_HPP_

#include "../Types.hpp"
#include <fstream>

struct ResourceDescriptor {
	int32_t mDescriptor;
	off_t mStart;
	off_t mLength;
};

class Resource 
{
public:
	Resource(const char* pPath);
	const char* getPath() { return mPath; };

	status open();
	void close();
	status read(void* pBuffer, size_t pCount);

	ResourceDescriptor descriptor();
	off_t getLength();

	bool operator==(const Resource& pOther);
private:
	const char* mPath;
	std::ifstream mfStream;
};

#endif