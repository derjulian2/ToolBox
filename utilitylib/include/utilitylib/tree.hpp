#ifndef UTIL_TREE_CONTAINER_HPP
#define UTIL_TREE_CONTAINER_HPP

#include <list>
#include <stdexcept>

// WORK IN PROGRESS

// a lot of raw pointer usage
// maybe remedy that by using smart pointers??? -> or unnecessary complicated

namespace util
{
	template <typename _Ty>
	class tree
	{
	public:
		using value_type = _Ty;

		tree()
		{
			_root_nodes.emplace_back(node(nullptr, nullptr, value_type())); // emplace a 'null-node' that will mark the end of the tree
		}
		
		class node
		{
			friend tree;

			value_type _value;
			node* _parent = nullptr;
			std::list<node>* _sibling_nodes = nullptr; // ptr to sibling nodes to always know when the end of a node is reached
			std::list<node> _child_nodes;
			std::list<node>::iterator _this; // iterator to the node in the list pointed to by '::_sibling_nodes'

			node(node* _p, std::list<node>* _s, const value_type& _val) 
				: _parent(_p), _sibling_nodes(_s), _value(_val) { }
		public:

			node& operator=(const value_type& _val)
			{
				this->_value = _val;
				return *this;
			}

			value_type value() const
			{
				return _value;
			}

			value_type& get()
			{
				return _value;
			}

			bool has_parent()
			{
				return _parent ? true : false;
			}

			node& get_parent()
			{
				if (_parent) return *_parent;
				throw std::runtime_error("parent was nullptr");
			}

			node& emplace_node_back(const value_type& _val)
			{
				node& res = _child_nodes.emplace_back(node(this, &_child_nodes, _val));
				res._this = std::prev(_child_nodes.end());
				return res;
			}
		};

		node& emplace_node_back(const value_type& _val)
		{
			node& res = *_root_nodes.insert(std::prev(_root_nodes.end(), 1), node(nullptr, &_root_nodes, _val));
			res._this = std::prev(_root_nodes.end(), 2); // for some reason works with example right now, but i dont think the right iterators are being assigned here
			return res;
		}

	private:
		std::list<node> _root_nodes;
	public:

		class iterator
		{
			friend tree;

			node* _ptr = nullptr;
			iterator(node* _p) : _ptr(_p) { }

			static node* _rec_parent_has_siblings(node* _parent_ptr)
			{
				if (_parent_ptr)
				{
					auto test = _parent_ptr->_this;
					if (_parent_ptr->_this != std::prev(_parent_ptr->_sibling_nodes->end()))
					{
						return &*std::next(_parent_ptr->_this);
					}
					else
					{
						if (!_parent_ptr->_parent)
						{
							return &*std::next(_parent_ptr->_this);
						}
						return _rec_parent_has_siblings(_parent_ptr->_parent);
					}
				}
				throw std::runtime_error("bad parent ptr");
			}
		public:
	
			iterator& operator++()
			{
				if (_ptr->_child_nodes.size())
				{
					_ptr = &_ptr->_child_nodes.front();
					return *this;
				}

				if (_ptr->_this != std::prev(_ptr->_sibling_nodes->end()))
				{
					_ptr = &*std::next(_ptr->_this);
					return *this;
				}

				if (_ptr->_parent) // recursively lookup if parent has siblings, if not go up one generation further
				{
					_ptr = _rec_parent_has_siblings(_ptr->_parent);
					return *this;
				}

				throw std::runtime_error("bad tree iterator");
			}

			node& operator*() { return *_ptr; }

			node* operator->() { return _ptr; }

			friend bool operator==(const iterator& i, const iterator& j) { return i._ptr == j._ptr ? true : false; }
			friend bool operator!=(const iterator& i, const iterator& j) { return i._ptr == j._ptr ? false : true; }
		};

		iterator begin()
		{
			return iterator(&_root_nodes.front());
		}

		iterator end()
		{
			return iterator(&_root_nodes.back());
		}
	};

}

#endif