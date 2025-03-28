#ifndef UTIL_TREE_HPP
#define UTIL_TREE_HPP

#include <memory>
#include <concepts>
#include <vector>
#include <stdexcept>

// work in progress
// to do:
// - const_iterator
// - proper erasure

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
		
		class bad_tree_node : public std::exception 
		{
		public:
			bad_tree_node(const char* _Message) : std::exception(_Message) {}
		};

	private:
		static std::shared_ptr<node> _lock(std::weak_ptr<node>& _ptr)
		{
			if (auto _res = _ptr.lock()) { return _res; }
			else { throw bad_tree_node("tried to access deleted tree node"); }
		}
		static const std::shared_ptr<node> _const_lock(const std::weak_ptr<node>& _ptr)
		{
			if (auto _res = _ptr.lock()) { return _res; }
			else { throw bad_tree_node("tried to access deleted tree node"); }
		}
	public:

		class iterator
		{
			std::weak_ptr<node> _ptr;
		public:
			explicit iterator(std::shared_ptr<node> _ptr)
				: _ptr(_ptr) { }

			iterator& operator++() { _ptr = _lock(_ptr)->_next; return *this; }

			iterator& operator+=(std::size_t _steps)
			{
				for (std::size_t i = 0; i < _steps; i++)
				{
					this->operator++();
				}
				return *this;
			}

			iterator& operator--() { _ptr = _lock(_ptr)->_prev; return *this; }

			iterator& operator-=(std::size_t _steps)
			{
				for (std::size_t i = 0; i < _steps; i++)
				{
					this->operator--();
				}
				return *this;
			}

			node& operator*() { return *_lock(_ptr); }

			std::shared_ptr<node> operator->() { return _lock(_ptr); }

			friend bool operator==(const iterator& i, const iterator& j) 
			{
				const std::shared_ptr<node> _i = _const_lock(i._ptr); const std::shared_ptr<node> _j = _const_lock(j._ptr);
				return _i == _j ? true : false;
			}
			friend bool operator!=(const iterator& i, const iterator& j) 
			{
				const std::shared_ptr<node> _i = _const_lock(i._ptr); const std::shared_ptr<node> _j = _const_lock(j._ptr);
				return _i == _j ? false : true;
			}
		};

		friend bool operator==(const iterator& i, const iterator& j);
		friend bool operator!=(const iterator& i, const iterator& j);

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
			std::shared_ptr<node> _next = nullptr;
			std::weak_ptr<node> _prev;
			std::weak_ptr<node> _parent;
			std::vector<std::weak_ptr<node>> _child_nodes;
			std::size_t _depth = 0U;
			value_type _value;

			friend tree;
			friend iterator;

			node(std::size_t _depth, const value_type& _val)
				: _depth(_depth), _value(_val) { }
			node(std::shared_ptr<node> _parent, std::size_t _depth, const value_type& _val)
				: _parent(_parent), _depth(_depth), _value(_val) { }

		public:
			node& operator=(const value_type& _val) { this->_value = _val; return *this; }
			value_type value() const { return _value; }
			value_type& get() { return _value; }
			std::size_t depth() { return _depth; }
			bool has_parent() { return _parent.expired() ? false : true; }
			node& parent()
			{
				return *_lock(_parent);
			}
			std::vector<iterator> children()
			{
				std::vector<iterator> _res;
				for (auto& _ptr : _child_nodes) { _res.emplace_back(iterator(_lock(_ptr))); }
				return _res;
			}

			iterator emplace_front(const value_type& _val)
			{
				std::shared_ptr<node> _new_node = std::shared_ptr<node>(new node(this->shared_from_this(), _depth + 1U, _val));
				_insert_node(_new_node, this->shared_from_this(), _next);
				_child_nodes.emplace_back(_new_node);
				return iterator(_new_node);
			}

			iterator emplace_back(const value_type& _val)
			{
				std::shared_ptr<node> _new_node = std::shared_ptr<node>(new node(this->shared_from_this(), _depth + 1U , _val));
				if (_child_nodes.size()) 
				{ 
					std::shared_ptr<node> _last_child = _lock(_child_nodes.back());
					_insert_node(_new_node, _last_child, _last_child->_next); 
				}
				else { _insert_node(_new_node, this->shared_from_this(), _next); }
				_child_nodes.emplace_back(_new_node);
				return iterator(_new_node);
			}

			void erase_front()
			{

			}

			void erase_back()
			{

			}
		};

		private:
			// utility to rewire node head/tail on insertion
			static void _insert_node(std::shared_ptr<node> _new, std::shared_ptr<node> _prev, std::shared_ptr<node> _next)
			{
				if (!_new || !_prev || !_next) { throw std::runtime_error("insertion failed because a node was nullptr"); }
				
				_new->_next = _next; // this needs to happen before _prev->_next is altered, otherwise all consecutive nodes get deallocated bc shared/weak ptrs
				_new->_prev = _prev;

				_next->_prev = _new;
				_prev->_next = _new;
			}

			void _insert_first(std::shared_ptr<node> _new)
			{
				if (!_new) { throw std::runtime_error("first insertion failed because new node was nullptr"); }

				_new->_next = _back;
				_new->_prev = _back;
				_back->_prev = _new;

				_front = _new;
			}

		public:

		tree()
		{
			_front = _back = std::shared_ptr<node>(new node(0U, value_type())); // _back always points to the invalid .end() node
		}

		/*
		* node manipulaton
		*/

		iterator emplace_front(const value_type& _val)
		{
			std::shared_ptr<node> _new_node = std::shared_ptr<node>(new node(0U, _val));
			if (_back == _front) { _insert_first(_new_node); }
			else { _insert_node(_new_node, _back, _front); }
			return iterator(_new_node);
		}

		iterator emplace_back(const value_type& _val)
		{
			std::shared_ptr<node> _new_node = std::shared_ptr<node>(new node(0U, _val));
			if (_back == _front) { _insert_first(_new_node); }
			else { _insert_node(_new_node, _lock(_back->_prev), _back); }
			return iterator(_new_node);
		}

		void erase_front()
		{

		}

		void erase_back()
		{

		}

		void erase(iterator& _where)
		{

		}

		/*
		* search functions
		*/

		std::vector<iterator> find(const value_type& _val)
		{
			std::vector<iterator> _res;
			for (iterator _iter = begin(); _iter != end(); ++_iter)
			{
				if (_iter->_value == _val) { _res.emplace_back(_iter); }
			}
			return _res;
		}

		iterator find_first_of(const value_type& _val)
		{
			for (iterator _iter = begin(); _iter != end(); ++_iter)
			{
				if (_iter->_value == _val) { return _iter; }
			}
			throw std::runtime_error("could not find any node of given value");
		}

		std::shared_ptr<node> _front = nullptr;
		std::shared_ptr<node> _back = nullptr;
	};
}

#endif 