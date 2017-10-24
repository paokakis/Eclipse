#include "Resource.hpp"
#include <sys/stat.h>
#include <fstream>
#include <SOIL.h>

Resource::Resource(const char* pPath) :
	mPath(pPath), mfStream()
{}

status Resource::open() 
{
	mfStream.open(mPath, std::ifstream::ate | std::ifstream::binary);

	if (!mfStream.is_open())
	{
		return STATUS_KO;
	}

	return STATUS_OK;
}

void Resource::close() 
{
	if (mfStream.is_open())
	{
		mfStream.close();
	}
}

status Resource::read(void* pBuffer, size_t pCount) 
{
	mfStream.read((char*)pBuffer, pCount);

	return STATUS_OK;

	//int32_t readCount = AAsset_read(mAsset, pBuffer, pCount);
	//return (readCount == pCount) ? STATUS_OK : STATUS_KO;
}

ResourceDescriptor Resource::descriptor() 
{
	ResourceDescriptor lDescriptor = { -1, 0, 0 };
	if (mfStream.is_open())
	{
		lDescriptor.mLength = mfStream.tellg();
		lDescriptor.mStart = 0;
	}

	return lDescriptor;
}

off_t Resource::getLength() 
{
	return mfStream.tellg();
}

bool Resource::operator==(const Resource& pOther) 
{
	return !strcmp(mPath, pOther.mPath);
}