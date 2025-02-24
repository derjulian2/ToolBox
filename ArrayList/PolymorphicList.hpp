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
* container structure to handle polymorphic types
* via standard-library smart-pointers
* 
* always copies the passed object into the heap
* using 'std::make_shared<_Ty>(obj)'
* 
* to make your type is polymorphically copyable,
* you need to define a clone() function for
* the PolymorphicList to call later.
* 
* expand the predefined macro using your types to
* easily add it in. it needs to be placed in your 
* polymorphic classes at public access specifier.
* 
* this clone() needs to be overriden in every derived class.
* use the corresponding macro for this purpose.
*/
//////////////////////////////////////////////////
#define POLYMORPHIC_CLONE_BASE(base)						\
virtual inline std::shared_ptr<base> clone() const = 0;
//////////////////////////////////////////////////
#define POLYMORPHIC_CLONE_DERIVED(base, derived)			\
virtual inline std::shared_ptr<base> clone() const override	\
{															\
	return std::make_shared<derived>(*this);				\
}
//////////////////////////////////////////////////

template <typename Ty>
concept Cloneable = requires(Ty t)
{
	{ t.clone() };
	{ std::is_polymorphic<Ty>::value };
};

template <typename Ty, typename _Ty>
concept CloneableAndDerived = Cloneable<Ty> && std::derived_from<Ty, _Ty>;

//////////////////////////////////////////////////

template <Cloneable Ty>
class PolymorphicList
{
public:
	PolymorphicList() = default;
	PolymorphicList(const PolymorphicList<Ty>&) noexcept;
	PolymorphicList(PolymorphicList<Ty>&&) noexcept;
	PolymorphicList<Ty>& operator=(const PolymorphicList&) noexcept;
	PolymorphicList<Ty>& operator=(PolymorphicList&&) noexcept;

	template <CloneableAndDerived<Ty> _Ty>
	void push_back(const _Ty&);

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