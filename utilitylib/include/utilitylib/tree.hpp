#ifndef UTIL_TREE_CONTAINER_HPP
#define UTIL_TREE_CONTAINER_HPP

#include <list>
#include <stdexcept>

// WORK IN PROGRESS

namespace util
{
	template <typename _Ty>
	class tree
	{
	public:
		using value_type = _Ty;

		tree() = default;
		tree(const tree&) = default;
		tree& operator=(const tree&) = default;
		/*
		* tree node that holds one instance of 'value_type'
		* seriously consider using std::list instead of vector because of iterator invalidation
		* and reallocation when children get added/removed
		* 
		* also the use cases of child[0] are very limited because you probably rarely want to get
		* a node by its index
		*/
		class node
		{
			value_type _value;
			node* _parent = nullptr;
			std::list<node> _child_nodes;
		public:
			node() = default;
			node(const value_type& _val) : _value(_val) {};
			node(const node&) = default;
			node& operator=(const node&) = default;

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

			node& emplace_child(const value_type& _val)
			{
				return _child_nodes.emplace_back(node(_val));
			}

		};
		/*
		* specialization of the node class that cannot have a parent
		*/
		class root_node
		{
			value_type _value;
			std::list<node> _child_nodes;
		public:
			root_node& operator=(const value_type& _val)
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

			node& emplace_child(const value_type& _val)
			{
				return _child_nodes.emplace_back(node(_val));
			}
		};

		class iterator
		{
			friend tree<value_type>;

			node* _node;
		
			iterator(node& _valid_node)
				: _node(&_valid_node)
			{

			}
		public:
			node& get()
			{
				return *_node;
			}
			/*
			* should iterate over the tree from branch to branch jumping back to the last parent and then incrementing by one if branch ends
			*/
			void operator++()
			{

			}

			void operator--()
			{

			}

			node& operator*()
			{
				return *_node;
			}

			node* operator->()
			{
				return _node;
			}

		};

		root_node& root()
		{
			return _root;
		}

		size_t size() const
		{
			return _size;
		}

		/*
		* retreives all nodes which hold the value '_val'
		* 
		* can be costly as the entire tree is iterated through
		*/
		std::vector<node> search_for(const value_type& _val)
		{

		}

	private:
		root_node _root;
		size_t _size; // total number of nodes
	};

}

#endif