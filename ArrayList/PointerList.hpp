//////////////////////////////////////////////////
#ifndef POINTERLIST_H
#define POINTERLIST_H
//////////////////////////////////////////////////
#include <list>
#include <concepts>
#include <type_traits>
//////////////////////////////////////////////////
/*
* PointerList to manage pointers to polymorphic types. intends to make runtime-type-polymorphism easier to use.
* 
* if you need list of pointers to many polymorphic types
* and you need to ensure memory safety (regarding copy, move and destruction operations) around that type and the
* integrity of the list, the 'PointerList<Type>' and 'PointerListElement<Type>' can manage this for you.
* effectively, this entangles an object on the stack with a pointer in a list.
* 
* when a 'PointerListElement<Type>' gets moved or destroyed, the corresponding list which
* that the element was part of gets notified of the new memory location or destruction
* 
* in such a case, the list CAN hold nullptrs. always check before using pointers of a PointerList
* or call 'PointerList<Type>::verify()' before. this pulls out any nullptrs in the list.
* 
* this ensures that no corruption is happening due to the element going out of scope or being moved.
* 
* if the list were to go out of scope before its elements, the respective pointers of all of its elements will be set to null.
*
* EXAMPLE:
* 
* struct Base : public PointerListElement<Base> {
*	Base() : PointerListElement<Base>(this) {}
*	Base(const Base&) : PointerListElement<Base>(this) {}
*	Base& operator=(const Base&) { setElementPointer(this); return *this; }	
* 
*   virtual void base();
* }
* 
* struct Derived : public Base {
*	void base() override;
* }
* 
* void func() {
*	PointerList<Base> base_nodes;
*	Derived derived_node;
*	base_nodes.addElement(derived_node); // adding the derived type polymorphically to the list
*	...
* }
* 
*/
//////////////////////////////////////////////////
template <typename Ty> class PointerListElement;
//////////////////////////////////////////////////
template <typename Ty>
concept PointerListType = std::is_base_of<PointerListElement<Ty>, Ty>::value;
//////////////////////////////////////////////////
template <PointerListType Ty> class PointerList;
//////////////////////////////////////////////////
/*
* element usable in a Pointerlist
*/
template <typename Ty>
class PointerListElement
{
protected:
	PointerListElement(Ty*);
public:
	PointerListElement(const PointerListElement<Ty>&) noexcept;
	PointerListElement(PointerListElement<Ty>&&) noexcept;

	PointerListElement<Ty>& operator=(const PointerListElement<Ty>&) noexcept;
	PointerListElement<Ty>& operator=(PointerListElement<Ty>&&) noexcept;

	virtual ~PointerListElement() = 0; // is only virtual to make this a polymorphic type, otherwise there would be no vtable

protected:
	void removefromPointerList();
	bool IsInList() const;
	void setElementPointer(Ty*);
	friend PointerList;
	Ty* element = nullptr;
	Ty** element_in_ptrlist = nullptr;
};
//////////////////////////////////////////////////
/*
* data structure that holds a list of pointers of type 'Ty'.
* automatically deletes nullptrs out of the list with the call of Pointerlist::verify()
* 
* is intended to be used along with PointerListElement, which automatically manages copying, moving
* and destructing operations for this list to prevent undefined behaviour.
* 
* inherit 'public PointerlistElement<YourType>' to make your type usable in the Pointerlist.
* also define a constructor as well as a copy constructor and assigment operator
* that passes 'this' to the PointerListElement() constructor (or uses PointerListElement::setElementPointer(this)).
* 
* does not allow duplicates in the list (measured with memory location).
* one object can only be part of one list (cannot be present in many lists).
*/
template<PointerListType Ty> class PointerList
{
public:
	PointerList() = default;

	PointerList(const PointerList<Ty>&) noexcept;
	PointerList(PointerList<Ty>&&) noexcept;

	PointerList<Ty>& operator=(const PointerList<Ty>&) noexcept;
	PointerList<Ty>& operator=(PointerList<Ty>&&) noexcept;

	~PointerList();
	/*
	* adds element to the list
	* 
	* returns 'false' if element was already in a list.
	* returns 'true' on success.
	*/
	bool addElement(Ty&);
	/*
	* removes element from the list
	*
	* returns 'false' if element was not in the list.
	* returns 'true' on success.
	*/
	bool removeElement(Ty&);
	bool ElementInList(Ty&);

	std::list<Ty*> get();
	std::list<Ty*>::iterator begin();
	std::list<Ty*>::iterator end();
	size_t size();

	void verify();
private:
	std::list<Ty*> pointer_list;
};
//////////////////////////////////////////////////
#include "PointerList_Definitions.hpp"
//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////