#ifndef SHAREDPTR_HPP
#define SHAREDPTR_HPP

#include <cassert>
#include <algorithm>

template <class T>
class SharedPtr
{
public:
	SharedPtr() : d(nullptr) {}

	SharedPtr(const SharedPtr &p) : d(p.d) 
	{
		if (d) ++d->ref;
	}

	SharedPtr(SharedPtr &&p) : d(p.d) 
	{
		p.d = nullptr;
	}

	SharedPtr(T *ptr) : d(nullptr) 
	{
		reset(ptr);
	}

	~SharedPtr() 
	{
		reset();
	}

	SharedPtr& operator = (const SharedPtr &p) 
	{
		reset();
		d = p.d;
		if (d) ++d->ref;
		return *this;
	}

	SharedPtr& operator = (SharedPtr && p) 
	{
		reset();
		d = p.d;
		p.d = nullptr;
		return *this;
	}

	T* operator * () 
	{
		assert(d);
		return d->ptr;
	}

	const T* operator * () const 
	{
		assert(d);
		return d->ptr;
	}

	T* operator -> () 
	{
		assert(d);
		return d->ptr;
	}

	const T* operator -> () const 
	{
		assert(d);
		return d->ptr;
	}

	operator bool () const 
	{
		return d != nullptr;
	}

	void reset() 
	{
		if (d) {
			if (--d->ref == 0) {
				delete d->ptr;
				delete d;
			}
			d = nullptr;
		}
	}

	void reset(T *ptr) 
	{
		reset();
		if (ptr != nullptr)
			d = new SharedData {ptr, 1};
	}

	T* get() 
	{
		return d ? d->ptr : nullptr;
	}

	const T* get() const 
	{
		return d ? d->ptr : nullptr;
	}
	
	void swap(SharedPtr &p) 
	{
		swap(d, p.d);
	}

private:
	struct SharedData 
	{
		T *ptr;
		int ref;
	};

	SharedData *d;
};

#endif // SHAREDPTR_HPP
