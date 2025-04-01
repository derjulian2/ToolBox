////////////////////////////////////////
/// general utility header-only-library
/// for convenience methods/types in C++
/// 2025 Julian Benzel
////////////////////////////////////////
/// tree datastructure
////////////////////////////////////////
#ifndef UTIL_TREE_HPP
#define UTIL_TREE_HPP
////////////////////////////////////////
#include <memory>
#include <stdexcept>
#include <iterator>
#include <list>
#include <algorithm>
#include <functional>
////////////////////////////////////////
namespace util
{
	////////////////////////////////////////

	template <typename _Ty>
	struct shared_to_ref {
		std::reference_wrapper<_Ty> operator()(const std::shared_ptr<_Ty>& _ptr) {
			if (!_ptr) { throw std::runtime_error("conversion from shared_ptr to ref failed"); }
			return *_ptr;
		}
	};

	////////////////////////////////////////
	/*
	 * C++ implementation of an iterable tree-data-structure
	 *
	 * every node of the tree can hold an arbitrary amount of child-nodes.
	 * internally uses std::list and std::shared_ptr/std::weak_ptr for this.
	 *
	 * this implementation might not be ideal, but i tried my best.
  	 * actually i already have a more performant approach. instead of leaving ownership with the nodes, you store every node in a std::unordered_map
         * and each node stores keys to adjacent nodes (parent, children, siblings)
	 * after some testing i found that this is way faster than this pointer-based approach
	 * 
	 * features:
	 * - tree-concatenation: trees can be merged. you can put nodes of one tree on a node of another
	 * - filter: you can search for nodes whose value satisfies a certain condition
	 * - iterable: with an iterator you can get from one node to the next/previous node (using a depth-first-search algorithm)
	 * 
	 * to-do:
	 * - reverse iterator
	 * - const iterator
	 * - unit testing
	 * 
	 * possible issues:
	 * - tree structure is recursive, a parent-node has ownership and cleanup-responsibility for its children. therefore large trees may lead to stack-overflow.
	 * - for deletion/iteration it is important to know in which container a node resides. for that, a node asks it's parent for an iterator to itself.
	 *   this may be suboptimal as you could maybe store that iterator when using std::list.
	 * - when using smart-pointers there are a lot of promotions from weak_ptr to shared_ptr as well as dereferences to traverse the tree. 
	 *   this may cause performance overhead.
	 */
	////////////////////////////////////////
	template <typename _Ty>
	class tree
	{
	public:
		////////////////////////////////////////
		/// forward and using declarations
		////////////////////////////////////////
		class node;
		class iterator;

		friend node;
		friend iterator;

		using value_type = _Ty;
		using unique_val = std::unique_ptr<value_type>;
		using filter_condition = std::function<bool(const value_type&)>;

		using node_ptr = std::shared_ptr<node>;
		using node_ref = std::reference_wrapper<node>;
		using node_wptr = std::weak_ptr<node>;
		using node_ptr_list = std::list<node_ptr>;

	public:

		////////////////////////////////////////
		/// node class
		////////////////////////////////////////

		class node : public std::enable_shared_from_this<node>
		{
			friend tree;
			friend iterator;
		
			unique_val m_val = nullptr;
			node_wptr m_parent;
			node_ptr_list m_children;
			std::size_t m_depth = NULL;

			void _set_val(const value_type& _val) { if (!m_val) { m_val = std::make_unique<value_type>(_val); } else { *m_val = _val; } }
			void _set_parent(node_ptr _ptr) { if (!_ptr) { throw std::runtime_error("parent node was nullptr"); }  m_parent = _ptr; }
			void _set_depth(std::size_t _d) { m_depth = _d; }
			node_ptr _parent() { return node_ptr(m_parent); }

			bool _is_first_child(node_ptr_list::iterator _iter) {
				node_ptr_list& _parent_list = _parent()->m_children;
				return _iter == _parent_list.begin();
			}

			bool _is_last_child(node_ptr_list::iterator _iter) {
				node_ptr_list& _parent_list = _parent()->m_children;
				return _iter == (_parent_list.size() ? std::prev(_parent_list.end()) : _parent_list.end());
			}

			node_ptr_list::iterator _get_iter() {
				node_ptr_list& _parent_list = _parent()->m_children;
				return std::find(_parent_list.begin(), _parent_list.end(), this->shared_from_this());
			}

			node_ptr_list::iterator _next_sibling_iter() {
				typename node_ptr_list::iterator _iter = _get_iter();
				if (_is_last_child(_iter)) { throw std::runtime_error("no next sibling"); }
				return std::next(_iter);
			}

			node_ptr_list::iterator _prev_sibling_iter() {
				typename node_ptr_list::iterator _iter = _get_iter();
				if (_is_first_child(_iter)) { throw std::runtime_error("no previous sibling"); }
				return std::prev(_iter);
			}

			node_ptr _emplace_back(const value_type& _val) {
				node_ptr _new = m_children.emplace_back(std::make_shared<node>());
				_new->_set_parent(this->shared_from_this());
				_new->_set_val(_val);
				_new->_set_depth(m_depth + 1);
				return _new;
			}

			node_ptr _emplace_front(const value_type& _val) {
				node_ptr _new = m_children.emplace_front(std::make_shared<node>());
				_new->_set_parent(this->shared_from_this());
				_new->_set_val(_val);
				_new->_set_depth(m_depth + 1);
				return _new;
			}

		public:

			////////////////////////////////////////
			/// value and adjacent node access
			////////////////////////////////////////

			/*
			 * @brief convenience value setter
			 * @param _val - the node's new value
			 */
			node& operator=(const value_type& _val) { _set_val(_val); return *this; }
			/*
			 * @returns reference to the value of the node
			 */
			value_type& value() { if (m_val) { return *m_val; } throw std::runtime_error("value of node was nullptr"); }
			/*
			 * @returns distance from the node to the root of the tree
			 */
			std::size_t depth() const { return m_depth; }
			/*
			 * @returns reference to parent-node
			 */
			node& parent() { return *_parent(); }
			/*
			 * @returns if the node has any sub-nodes 
			 */
			bool has_children() const { return static_cast<bool>(m_children.size()); }
			/*
			 * @returns reference to next sibling-node, if there is one 
			 * @throws std::runtime_error - if called on the last sibling
			 */
			node& next_sibling() { 
				return **_next_sibling_iter();
			}
			/*
			 * @returns reference to previous sibling-node, if there is one
			 * @throws std::runtime_error - if called on the first sibling
			 */
			node& prev_sibling() {
				return **_prev_sibling_iter();
			}
			/*
			 * @returns references to child-nodes
			 */
			std::vector<node_ref> children() {
				std::vector<node_ref> _res;
				std::transform(m_children.begin(), m_children.end(), std::back_inserter(_res), shared_to_ref<node>());
				return _res;
			}

			////////////////////////////////////////
			/// modifiers
			////////////////////////////////////////

			/*
			 * @brief create new node at the back of this node's children
			 * @param _val - value of the child node
			 * @returns reference to the new node
			 */
			node& emplace_back(const value_type& _val) {
				return *_emplace_back(_val);
			}
			/*
			 * @brief create new node at the front of this node's children
			 * @param _val - value of the child node
			 * @returns reference to the new node
			 */
			node& emplace_front(const value_type& _val) {
				return *_emplace_front(_val);
			}
			/*
			 * @brief insert a new node as next sibling node
			 * @param _val - the new node's value
			 * @returns reference to the new node
			 */
			node& emplace_next_sibling(const value_type& _val) {
				typename node_ptr_list::iterator _iter =  _parent()->m_children.insert(std::next(_get_iter()), std::make_shared<node>());
				(*_iter)->_set_depth(m_depth);
				(*_iter)->_set_val(_val);
				(*_iter)->_set_parent(_parent());
				return **_iter;
			}
			/*
			 * @brief insert a new node as previous sibling node
			 * @param _val - the new node's value
			 * @returns reference to the new node
			 */
			node& emplace_prev_sibling(const value_type& _val) {
				typename node_ptr_list::iterator _iter = _parent()->m_children.insert(_get_iter(), std::make_shared<node>());
				(*_iter)->_set_depth(m_depth);
				(*_iter)->_set_val(_val);
				(*_iter)->_set_parent(_parent());
				return **_iter;
			}
			/*
			 * @brief concatenates every child-node of the tree's root node to this node
			 */
			void concatenate(tree _tree) {
				_copy_nodes(_tree.m_root, this->shared_from_this());
			}
		};
		
		////////////////////////////////////////
		/// iterators
		////////////////////////////////////////

		class iterator
		{
			friend tree;

			node_wptr m_ptr;
			node_wptr m_root_ptr;

			node_ptr _get_next(node_ptr _ptr) {
				if (_ptr == node_ptr(m_root_ptr)) { return _ptr; }
				typename node_ptr_list::iterator _iter = _ptr->_get_iter();
				if (_ptr->_is_last_child(_iter)) {
					return _get_next(_ptr->_parent());
				}
				else {
					return *std::next(_iter);
				}
			}

			/*node_ptr _get_prev(node_ptr _ptr) {
				if (_ptr == node_ptr(m_root_ptr)) { return _ptr; }
				typename node_ptr_list::iterator _iter = _ptr->_get_iter();
				if (_ptr->_is_first_child(_iter)) {
					return _get_prev(_ptr->_parent());
				}
				else {
					return *std::prev(_iter);
				}
			}*/

			bool _is_root() const { return node_ptr(m_ptr) == node_ptr(m_root_ptr); }

			iterator(node_ptr _root, node_ptr _node) : m_root_ptr(_root), m_ptr(_node) {}

		public:
			node_ptr operator->() { if (_is_root()) { throw std::runtime_error("cannot dereference end iterator"); } return node_ptr(m_ptr); }
			node& operator*() { if (_is_root()) { throw std::runtime_error("cannot dereference end iterator"); } return *node_ptr(m_ptr); }

			iterator& operator++() {
				if (_is_root()) { throw std::runtime_error("cannot increment end iterator"); }
				node& _ref = *node_ptr(m_ptr);
				if (_ref.has_children()) {
					m_ptr = _ref.m_children.front();
				}
				else {
					m_ptr = _get_next(node_ptr(m_ptr));
				}
				return *this;
			}

			iterator operator++(int) {
				iterator _iter(*this);
				this->operator++();
				return _iter;
			}

			/*iterator& operator--() {
				if (_is_root()) { throw std::runtime_error("cannot decrement begin iterator"); }
				node& _ref = *node_ptr(m_ptr);
				if (_ref.has_children()) {
					m_ptr = _ref.m_children.back();
				}
				else {
					m_ptr = _get_prev(node_ptr(m_ptr));
				}
				return *this;
			}

			iterator operator--(int) {
				iterator _iter(*this);
				this->operator--();
				return _iter;
			}*/

			friend static bool operator==(const iterator& i, const iterator& j) { 
				if (node_ptr(i.m_root_ptr) != node_ptr(j.m_root_ptr)) { throw std::runtime_error("cannot compare iterators of different trees"); }
				return node_ptr(i.m_ptr) == node_ptr(j.m_ptr); 
			}
			friend static bool operator!=(const iterator& i, const iterator& j) { 
				if (node_ptr(i.m_root_ptr) != node_ptr(j.m_root_ptr)) { throw std::runtime_error("cannot compare iterators of different trees"); }
				return node_ptr(i.m_ptr) != node_ptr(j.m_ptr); 
			}
		};
		/*
		 * @returns iterator to first node of the tree, or to the end if there are none
		 */
		iterator begin() {
			return iterator(m_root, m_root->has_children() ? m_root->m_children.front() : m_root);
		}
		/*
		 * @returns iterator to end/header node of the tree
		 */
		iterator end() {
			return iterator(m_root, m_root);
		}

		//class const_iterator
		//{

		//};

		//const_iterator cbegin() {

		//}

		//const_iterator cend() {

		//}

		//class reverse_iterator
		//{

		//};

		//reverse_iterator rbegin() {

		//}

		//reverse_iterator rend() {

		//}

		////////////////////////////////////////
		/// modifiers
		////////////////////////////////////////
		/*
		 * @brief emplaces new node which holds value '_val' as last child of the root-node
		 * @param _val - the value which the node will hold
		 * @returns reference to the new node
		 */
		node& emplace_back(const value_type& _val) {
			return m_root->emplace_back(_val);
		}
		/*
		 * @brief emplaces new node which holds value '_val' as first child of the root-node
		 * @param _val - the value which the node will hold
		 * @returns reference to the new node
		 */
		node& emplace_front(const value_type& _val) {
			return m_root->emplace_front(_val);
		}

		////////////////////////////////////////
		/// node access
		////////////////////////////////////////

		/*
		 * @brief searches the tree for a given value
		 * @param _val - the value to look for
		 * @returns reference to the first node found with that value
		 */
		node& find_first(const value_type& _val) {
			for (iterator _iter = begin(); _iter != end(); ++_iter) {
				if (_iter->value() == _val) { return *_iter; }
			}
			throw std::runtime_error("tree does not contain node with given value");
		}
		/*
		 * @brief searches the tree for a given value
		 * @param _val - the value to look for
		 * @returns iterator to the first node found with that value
		 */
		iterator find_first_iterator(const value_type& _val) {
			for (iterator _iter = begin(); _iter != end(); ++_iter) {
				if (_iter->value() == _val) { return _iter; }
			}
			throw std::runtime_error("tree does not contain node with given value");
		}
		/*
		 * @brief searches the tree for all nodes of a given value
		 * @param _val - the value to look for
		 * @returns references to all nodes holding that value
		 */
		std::vector<node_ref> find(const value_type& _val) {
			std::vector<node_ref> _res;
			for (iterator _iter = begin(); _iter != end(); ++_iter) {
				if (_iter->value() == _val) {
					_res.emplace_back(*_iter);
				}
			}
			return _res;
		}
		/*
		 * @brief searches the tree for all nodes of a given value
		 * @param _val - the value to look for
		 * @returns iterators to all nodes holding that value
		 */
		std::vector<iterator> find_iterators(const value_type& _val) {
			std::vector<iterator> _res;
			for (iterator _iter = begin(); _iter != end(); ++_iter) {
				if (_iter->value() == _val) {
					_res.emplace_back(_iter);
				}
			}
			return _res;
		}
		/*
		 * @brief searches the tree for all nodes for which the filter_condition evaluates to true
		 * @param _cond - function of the signature '[](const value_type&) -> bool'
		 * @returns references to all nodes satisfying this condition
		 */
		std::vector<node_ref> filter(const filter_condition& _cond) {
			std::vector<node_ref> _res;
			for (iterator _iter = begin(); _iter != end(); ++_iter) {
				if (_cond(_iter->value())) {
					_res.emplace_back(*_iter);
				}
			}
			return _res;
		}
		/*
		 * @brief searches the tree for all nodes for which the filter_condition evaluates to true
		 * @param _cond - function of the signature '[](const value_type&) -> bool'
		 * @returns iterators to all nodes satisfying this condition
		 */
		std::vector<iterator> filter_iterators(const filter_condition& _cond) {
			std::vector<iterator> _res;
			for (iterator _iter = begin(); _iter != end(); ++_iter) {
				if (_cond(_iter->value())) {
					_res.emplace_back(_iter);
				}
			}
			return _res;
		}

		/*
		 * @brief erases every branch of the tree
		 */
		void clear() {
			m_root->m_children.clear();
		}
		/*
		 * @brief erases node at given iterator
		 * @brief will also erase every child node from the tree and invalidate references and iterators to these nodes
		 * @param _where - the node to erase
		 * @returns iterator to the next valid node
		 */
		iterator erase(iterator _where) {
			if (_where == end()) { throw std::runtime_error("cannot erase end-iterator"); }
			iterator _next = _where;  ++_next;
			_where->_parent()->m_children.erase(_where->_get_iter());
			return _next;
		}

		////////////////////////////////////////
		/// size
		////////////////////////////////////////

		/*
		 * @brief traverses the entire tree to get node count
		 * @returns total number of nodes
		 */
		std::size_t size() {
			std::size_t _res = 0ULL;
			for (iterator _iter = begin(); _iter != end(); ++_iter) { ++_res; }
			return _res;
		}

		////////////////////////////////////////
		/// special member functions
		////////////////////////////////////////

		/*
		 * @brief std::swap specialization by swapping the root nodes
		 */
		friend void swap(tree& a, tree& b) {
			std::swap(a.m_root, b.m_root);
		}
		/*
		 * @brief default constructor
		 */
		tree() {
			m_root = std::make_shared<node>();
		}
		/* 
		 * @brief construct a new tree from existing nodes (and all it's children) 
		 */
		tree(node& _node) : tree() {
			_copy_nodes(_node._parent(), m_root);
		}
		/*
		 * @brief copy constructor and assignment operator makes deep copy of every node preserving tree hierarchy
		 */
		tree(const tree& other) : tree() {
			_copy_nodes(other.m_root, m_root);
		}
		tree& operator=(tree& other) {
			swap(*this, other);
			return *this;
		}

	private:

		/*
		 * @brief recursive node copying utility
		 */
		static void _copy_nodes(node_ptr _src, node_ptr _target) {
			for (node_ptr& _ptr : _src->m_children) {
				node_ptr _new = _target->_emplace_back(_ptr->value());
				_new->_set_depth(_target->m_depth + 1);
				if (_ptr->has_children()) {
					_copy_nodes(_ptr, _new);
				}
			}
		}

		node_ptr m_root;
	};

	////////////////////////////////////////
}
////////////////////////////////////////
#endif 
////////////////////////////////////////
