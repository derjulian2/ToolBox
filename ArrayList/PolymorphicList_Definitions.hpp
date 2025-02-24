//////////////////////////////////////////////////

template <Cloneable Ty>
PolymorphicList<Ty>::PolymorphicList(const PolymorphicList<Ty>& other) noexcept
{
	for (const std::unique_ptr<Ty>& ptr : other.internal_vec)
	{
		this->internal_vec.emplace_back(ptr->clone());
	}
}
template <Cloneable Ty>
PolymorphicList<Ty>::PolymorphicList(PolymorphicList<Ty>&& other) noexcept
{
	this->internal_vec = other.internal_vec;
	other.internal_vec.clear();
}
template <Cloneable Ty>
PolymorphicList<Ty>& PolymorphicList<Ty>::operator=(const PolymorphicList& other) noexcept
{
	for (const std::unique_ptr<Ty>& ptr : other.internal_vec)
	{
		this->internal_vec.emplace_back(ptr->clone());
	}
	return *this;
}
template <Cloneable Ty>
PolymorphicList<Ty>& PolymorphicList<Ty>::operator=(PolymorphicList&& other) noexcept
{
	this->internal_vec = other.internal_vec;
	other.internal_vec.clear();
	return *this;
}

template <Cloneable Ty>
template <CloneableAndDerived<Ty> _Ty>
void PolymorphicList<Ty>::push_back(const _Ty& elem)
{
	internal_vec.emplace_back(std::make_unique<_Ty>(elem));
}

template <Cloneable Ty>
PolymorphicList<Ty>::iterator PolymorphicList<Ty>::begin()
{
	return internal_vec.begin();
}

template <Cloneable Ty>
PolymorphicList<Ty>::iterator PolymorphicList<Ty>::end()
{
	return internal_vec.end();
}

template <Cloneable Ty>
Ty& PolymorphicList<Ty>::at(uint64_t index)
{
	if (index >= size())
	{
		throw std::runtime_error("PolymorphicList: get() index out of range");
	}
	else
	{
		return *internal_vec[index];
	}
}

template <Cloneable Ty>
Ty& PolymorphicList<Ty>::operator[](uint64_t index)
{
	return at(index);
}

template <Cloneable Ty>
Ty& PolymorphicList<Ty>::front()
{
	if (size())
	{
		return at(0);
	}
}

template <Cloneable Ty>
Ty& PolymorphicList<Ty>::back()
{
	if (size())
	{
		return at(size() - 1);
	}
}

template <Cloneable Ty>
PolymorphicList<Ty>::iterator PolymorphicList<Ty>::erase(PolymorphicList<Ty>::iterator where)
{
	return internal_vec.erase(where);
}

template <Cloneable Ty>
void PolymorphicList<Ty>::clear()
{
	internal_vec.clear();
}

template <Cloneable Ty>
size_t PolymorphicList<Ty>::size() const
{
	return internal_vec.size();
}

//////////////////////////////////////////////////