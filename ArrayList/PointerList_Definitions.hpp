//////////////////////////////////////////////////
template <typename Ty>
PointerListElement<Ty>::PointerListElement(Ty* ptr)
{
	setElementPointer(ptr);
}

template <typename Ty>
PointerListElement<Ty>::PointerListElement(const PointerListElement<Ty>&) noexcept
{
	this->element_in_ptrlist = nullptr;
}

template <typename Ty>
PointerListElement<Ty>::PointerListElement(PointerListElement<Ty>&& other) noexcept
{
	removefromPointerList();
	if (other.element_in_ptrlist)
	{
		*other.element_in_ptrlist = other.element;
		this->element_in_ptrlist = other.element_in_ptrlist;
		other.element_in_ptrlist = nullptr;
	}
}

template <typename Ty>
PointerListElement<Ty>& PointerListElement<Ty>::operator=(const PointerListElement<Ty>&) noexcept
{
	this->element_in_ptrlist = nullptr;
	return *this;
}

template <typename Ty>
PointerListElement<Ty>& PointerListElement<Ty>::operator=(PointerListElement<Ty>&& other) noexcept
{
	removefromPointerList();
	if (other.element_in_ptrlist)
	{
		*other.element_in_ptrlist = other.element;
		this->element_in_ptrlist = other.element_in_ptrlist;
		other.element_in_ptrlist = nullptr;
	}
	return *this;
}

template <typename Ty>
PointerListElement<Ty>::~PointerListElement()
{
	removefromPointerList();
}

template <typename Ty>
void PointerListElement<Ty>::removefromPointerList()
{
	if (element_in_ptrlist)
	{
		*element_in_ptrlist = nullptr;
		element_in_ptrlist = nullptr;
	}
}

template <typename Ty>
bool PointerListElement<Ty>::IsInList() const
{
	if (element_in_ptrlist)
		return true;
	return false;
}

template <typename Ty>
void PointerListElement<Ty>::setElementPointer(Ty* ptr)
{
	this->element = ptr;
}

//////////////////////////////////////////////////

template <PointerListType Ty>
PointerList<Ty>::PointerList(const PointerList&) noexcept
{
	this->pointer_list.clear();
}

template <PointerListType Ty>
PointerList<Ty>::PointerList(PointerList&& other) noexcept
{
	this->pointer_list = other.pointer_list;
	other.pointer_list.clear();
}

template <PointerListType Ty>
PointerList<Ty>& PointerList<Ty>::operator=(const PointerList<Ty>&) noexcept
{
	this->pointer_list.clear();
	return *this;
}

template <PointerListType Ty>
PointerList<Ty>& PointerList<Ty>::operator=(PointerList&& other) noexcept
{
	this->pointer_list = other.pointer_list;
	other.pointer_list.clear();
	return *this;
}

template <PointerListType Ty>
PointerList<Ty>::~PointerList()
{
	for (Ty* ptr : pointer_list)
	{
		if (ptr)
		{
			ptr->element_in_ptrlist = nullptr;
		}
	}
}

template <PointerListType Ty>
bool PointerList<Ty>::addElement(Ty& element)
{
	if (element.IsInList())
		return false; // element is already in a list and thus can't be added to another.
	pointer_list.emplace_back(&element);
	element.element_in_ptrlist = &pointer_list.back();
	return true;
}

template <PointerListType Ty>
bool PointerList<Ty>::removeElement(Ty& element)
{
	if (!ElementInList(element))
		return false; // element not in list
	for (typename std::list<Ty*>::iterator iter = pointer_list.begin(); iter != pointer_list.end(); iter++)
	{
		if ((*iter) == &element)
		{
			pointer_list.erase(iter);
			element.element_in_ptrlist = nullptr;
			break;
		}
	}
	return true;
}

template <PointerListType Ty>
bool PointerList<Ty>::ElementInList(Ty& element)
{
	for (typename std::list<Ty*>::iterator iter = pointer_list.begin(); iter != pointer_list.end(); iter++)
	{
		if ((*iter) == &element)
		{
			return true;
		}
	}
	return false;
}

template<PointerListType Ty>
std::list<Ty*> PointerList<Ty>::get()
{
	return pointer_list;
}

template<PointerListType Ty>
std::list<Ty*>::iterator PointerList<Ty>::begin()
{
	return pointer_list.begin();
}

template<PointerListType Ty>
std::list<Ty*>::iterator PointerList<Ty>::end()
{
	return pointer_list.end();
}

template<PointerListType Ty>
size_t PointerList<Ty>::size()
{
	return pointer_list.size();
}

template <PointerListType Ty>
void PointerList<Ty>::verify()
{
	for (typename std::list<Ty*>::iterator iter = pointer_list.begin(); iter != pointer_list.end();)
	{
		if (*iter)
		{
			iter++;
		}
		else
		{
			iter = pointer_list.erase(iter);
		}
	}
}

//////////////////////////////////////////////////