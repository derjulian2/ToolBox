//////////////////////////////////////////////////
#ifndef POLYMORPHICLIST_H
#define POLYMORPHICLIST_H
//////////////////////////////////////////////////
#include <type_traits>
#include <concepts>
#include <vector>
#include <memory>
#include <stdexcept>
//////////////////////////////////////////////////
/*
* 
* std::vector wrapper to handle polymorphic types
* via standard-library smart-pointers (with added 
* constraints and copying features)
* 
* always copies the passed object into the heap
* using 'std::make_unique<_Ty>(obj)'
* 
* to ensure your type is polymorphically copyable,
* you need to define a clone() function for
* the PolymorphicList to call later.
* 
* expand the predefined macro using your types to
* easily add it in. it needs to be placed in your 
* polymorphic classes at public access specifier.
* 
* this clone() needs to be overriden in every derived class.
* use the corresponding macro for this purpose.
* 
*/
//////////////////////////////////////////////////

#define POLYMORPHIC_CLONE_BASE(base)						\
virtual std::unique_ptr<base> clone() const = 0;

//////////////////////////////////////////////////

#define POLYMORPHIC_CLONE_DERIVED(base, derived)			\
virtual inline std::unique_ptr<base> clone() const override	\
{															\
	return std::make_unique<derived>(*this);				\
}

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
	void push_back(const _Ty&);

	typedef std::vector<std::unique_ptr<Ty>>::iterator iterator;

	iterator begin();
	iterator end();
	Ty& at(uint64_t);
	Ty& operator[](uint64_t);
	Ty& front();
	Ty& back();
	iterator erase(iterator);
	void clear();
	size_t size() const;

private:
	std::vector<std::unique_ptr<Ty>> internal_vec;
};

//////////////////////////////////////////////////
#include "PolymorphicList_Definitions.hpp"
//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////
