#ifndef UTIL_TREE_HPP
#define UTIL_TREE_HPP

#include <memory>
#include <concepts>
#include <vector>

// work in progress
// to do:
// - const_iterator
// - proper erasure
// - maybe some corner-cases/exception handling

namespace util
{
	/*
	* n-ary tree structure to model xml-style trees:
	* - can store arbitrary amount of root-nodes
	* - each node can store arbitrary amount of child-nodes
	* 
	* internally implemented as a doubly-linked-list with extra metadata for trees using STL smart-pointers
	* 
	* @tparam any copyable, copyassignable and default-constructible type is valid
	*/
	template <typename _Ty>
	requires std::copyable<_Ty> and std::is_copy_assignable<_Ty>::value and std::is_default_constructible<_Ty>::value
	class tree
	{
	public:
		using value_type = _Ty;

		class node;

		class iterator
		{
			std::weak_ptr<node> _ptr;
		public:
			iterator(std::shared_ptr<node> _ptr)
				: _ptr(_ptr)
			{

			}

			std::shared_ptr<node> get()
			{
				if (std::shared_ptr<node> _lock = _ptr.lock())
				{
					return _lock;
				}
				else
				{
					throw std::runtime_error("iterator reference invalid");
				}
			}

			iterator& operator++()
			{
				if (std::shared_ptr<node> _lock = _ptr.lock())
				{
					_ptr = _lock->_next;
				}
				else
				{
					throw std::runtime_error("iterator reference invalid");
				}
			}

			iterator& operator-=(std::size_t _steps)
			{
				for (std::size_t i = 0; i < _steps; i++)
				{
					this->operator--();
				}
				return *this;
			}

			iterator& operator+=(std::size_t _steps)
			{
				for (std::size_t i = 0; i < _steps; i++)
				{
					this->operator++();
				}
				return *this;
			}

			iterator& operator--()
			{
				if (std::shared_ptr<node> _lock = _ptr.lock())
				{
					_ptr = _lock->_prev;
				}
				else
				{
					throw std::runtime_error("iterator reference invalid");
				}
			}

			node& operator*()
			{
				if (std::shared_ptr<node> _lock = _ptr.lock())
				{
					return *_lock;
				}
				else
				{
					throw std::runtime_error("iterator reference invalid");
				}
			}

			std::shared_ptr<node> operator->()
			{
				if (std::shared_ptr<node> _lock = _ptr.lock())
				{
					return _lock;
				}
				else
				{
					throw std::runtime_error("iterator reference invalid");
				}
			}

			friend bool operator==(const iterator& i, const iterator& j) 
			{
				std::shared_ptr<node> _i = i._ptr.lock(); std::shared_ptr<node> _j = j._ptr.lock();
				if (_i && _j)
				{
					return _i == _j ? true : false;
				}
				else
				{
					throw std::runtime_error("iterator reference invalid");
				}
			}
			friend bool operator!=(const iterator& i, const iterator& j) 
			{
				std::shared_ptr<node> _i = i._ptr.lock(); std::shared_ptr<node> _j = j._ptr.lock();
				if (_i && _j)
				{
					return _i == _j ? false : true;
				}
				else
				{
					throw std::runtime_error("iterator reference invalid");
				}
			}
		};

		iterator begin()
		{
			return iterator(_front);
		}

		iterator end()
		{
			return iterator(_back);
		}

		class node : public std::enable_shared_from_this<node>
		{
			value_type _value;
			std::size_t _depth = 0U;
			std::size_t _child_num = 0U;
			std::shared_ptr<node> _next = nullptr;
			std::weak_ptr<node> _prev;
			std::weak_ptr<node> _parent;

			friend tree;
			friend iterator;

			node(std::shared_ptr<node> _next, std::shared_ptr<node> _prev, std::size_t _depth, const value_type& _val)
				: _next(_next), _prev(_prev), _depth(_depth), _value(_val) { }
			node(std::shared_ptr<node> _parent, std::shared_ptr<node> _next, std::shared_ptr<node> _prev, std::size_t _depth, const value_type& _val)
				: _parent(_parent), _next(_next), _prev(_prev), _depth(_depth), _value(_val) {
			}

		public:

			value_type value() const { return _value; }
			value_type& get() { return _value; }
			std::size_t depth() { return _depth; }
			bool has_parent() { return _parent.expired() ? false : true; }
			node& parent()
			{
				if (std::shared_ptr<node> _lock = _parent.lock())
				{
					return *_lock;
				}
				else
				{
					throw std::runtime_error("node has no parent");
				}
			}

			iterator emplace_front(const value_type& _val)
			{
				std::shared_ptr<node> _new_node = std::shared_ptr<node>(new node(this->shared_from_this(), _next, this->shared_from_this(), _depth + 1U, _val)); _child_num++;
				_next = _new_node;
				return _new_node;
			}

			iterator emplace_back(const value_type& _val)
			{
				iterator _last_child = this->shared_from_this();
				_last_child += _child_num;
				std::shared_ptr<node> _new_node = std::shared_ptr<node>(new node(this->shared_from_this(), _last_child->_next, _last_child.get(), _depth + 1U, _val)); _child_num++;
				_last_child->_next->_prev = _new_node;
				_last_child->_next = _new_node;
				return _new_node;
			}

			void erase_front()
			{

			}

			void erase_back()
			{
				iterator _last_child = this->shared_from_this();
				_last_child += _child_num;
			}
		};

		tree()
		{
			_front = _back = std::shared_ptr<node>(new node(nullptr, nullptr, 0U, value_type())); // _back always points to the invalid .end() node
		}

		iterator emplace_front(const value_type& _val)
		{
			std::shared_ptr<node> _new_node = std::shared_ptr<node>(new node(_front, nullptr, 0U, _val));
			_front->_prev = _new_node;
			_front = _new_node;
			return _new_node;
		}

		iterator emplace_back(const value_type& _val)
		{
			std::shared_ptr<node> _lock = _back->_prev.lock();
			std::shared_ptr<node> _new_node = std::shared_ptr<node>(new node(_back, _lock, 0U, _val));
			if (_back == _front)
			{
				_front = _new_node;
			}
			else
			{
				if (std::shared_ptr<node> __lock = _back->_prev.lock())
				{
					__lock->_next = _new_node;
				}
				else
				{
					throw std::runtime_error("previous node was expired");
				}
			}
			_back->_prev = _new_node;
			return _new_node;
		}

		void erase_front()
		{

		}

		void erase_back()
		{

		}

		void erase(iterator& _where)
		{
			// works for recursive branch erasure, probably some corner cases with _front or _back not handled
			iterator _next = _where; ++_next;
			while (_next->_depth > _where->_depth) { ++_next; }
			if (std::shared_ptr<node> _lock = _where->_prev.lock())
			{
				_lock->_next = _next.get();
				_next->_prev = _lock;
			}
		}

		std::vector<std::reference_wrapper<node>> find(const value_type& _val)
		{
			std::vector<std::reference_wrapper<node>> _res;
			for (iterator _iter = begin(); _iter != end(); ++_iter)
			{
				if (_iter->_value == _val)
				{
					_res.emplace_back(*_iter.get());
				}
			}
			return _res;
		}

		node& find_first_of(const value_type& _val)
		{
			for (iterator _iter = begin(); _iter != end(); ++_iter)
			{
				if (_iter->_value == _val)
				{
					return *_iter.get();
				}
			}
			throw std::runtime_error("could not find any node of given value");
		}

		std::shared_ptr<node> _front = nullptr;
		std::shared_ptr<node> _back = nullptr;
	};
}

#endif 