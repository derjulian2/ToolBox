//////////////////////////////////////////////////
#ifndef POLYMORPHICLIST_H
#define POLYMORPHICLIST_H
//////////////////////////////////////////////////
#include <type_traits>
#include <concepts>
#include <vector>
#include <memory>
//////////////////////////////////////////////////
/*
* data structure to handle polymorphic types
* via standard-library smart-pointers
* 
* always copies the passed object into the heap
* using std::make_shared<_Ty>(obj)
* 
* copies over the pointers on copy operations,
* meaning that two lists can essentially hold
* pointers to the same objects. 
*/
//////////////////////////////////////////////////
template <typename Ty>
class PolymorphicList
{
public:
	PolymorphicList() = default;
	PolymorphicList(const PolymorphicList<Ty>&) noexcept;
	PolymorphicList(PolymorphicList<Ty>&&) noexcept;
	PolymorphicList<Ty>& operator=(const PolymorphicList&) noexcept;
	PolymorphicList<Ty>& operator=(PolymorphicList&&) noexcept;

	template <std::derived_from<Ty> _Ty>
	void push_back(_Ty&);

	typedef std::vector<std::shared_ptr<Ty>>::iterator iterator;

	iterator begin();
	iterator end();
	std::shared_ptr<Ty>& at(uint64_t);
	std::shared_ptr<Ty>& operator[](uint64_t);
	std::shared_ptr<Ty>& front();
	std::shared_ptr<Ty>& back();
	iterator erase(iterator);
	void clear();
	size_t size() const;

private:
	std::vector<std::shared_ptr<Ty>> internal_vec;
};

//////////////////////////////////////////////////
#include "PolymorphicList_Definitions.hpp"
//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////