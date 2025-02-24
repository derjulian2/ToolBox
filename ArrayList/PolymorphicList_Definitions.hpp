//////////////////////////////////////////////////

template <typename Ty>
PolymorphicList<Ty>::PolymorphicList(const PolymorphicList<Ty>& other) noexcept
{
	this->internal_vec = other.internal_vec;
}
template <typename Ty>
PolymorphicList<Ty>::PolymorphicList(PolymorphicList<Ty>&& other) noexcept
{
	this->internal_vec = other.internal_vec;
	other.internal_vec.clear();
}
template <typename Ty>
PolymorphicList<Ty>& PolymorphicList<Ty>::operator=(const PolymorphicList& other) noexcept
{
	this->internal_vec = other.internal_vec;
	return *this;
}
template <typename Ty>
PolymorphicList<Ty>& PolymorphicList<Ty>::operator=(PolymorphicList&& other) noexcept
{
	this->internal_vec = other.internal_vec;
	other.internal_vec.clear();
	return *this;
}

template <typename Ty>
template <std::derived_from<Ty> _Ty>
void PolymorphicList<Ty>::push_back(_Ty& elem)
{
	internal_vec.emplace_back(std::make_shared<_Ty>(elem));
}

template <typename Ty>
PolymorphicList<Ty>::iterator PolymorphicList<Ty>::begin()
{
	return internal_vec.begin();
}

template <typename Ty>
PolymorphicList<Ty>::iterator PolymorphicList<Ty>::end()
{
	return internal_vec.end();
}

template <typename Ty>
std::shared_ptr<Ty>& PolymorphicList<Ty>::at(uint64_t index)
{
	if (index >= size())
	{
		throw std::runtime_error("PolymorphicList: get() index out of range");
	}
	else
	{
		return internal_vec[index];
	}
}

template <typename Ty>
std::shared_ptr<Ty>& PolymorphicList<Ty>::operator[](uint64_t index)
{
	return at(index);
}

template <typename Ty>
std::shared_ptr<Ty>& PolymorphicList<Ty>::front()
{
	if (size())
	{
		return at(0);
	}
}

template <typename Ty>
std::shared_ptr<Ty>& PolymorphicList<Ty>::back()
{
	if (size())
	{
		return at(size() - 1);
	}
}

template <typename Ty>
PolymorphicList<Ty>::iterator PolymorphicList<Ty>::erase(PolymorphicList<Ty>::iterator where)
{
	return internal_vec.erase(where);
}

template <typename Ty>
void PolymorphicList<Ty>::clear()
{
	internal_vec.clear();
}

template <typename Ty>
size_t PolymorphicList<Ty>::size() const
{
	return internal_vec.size();
}

//////////////////////////////////////////////////