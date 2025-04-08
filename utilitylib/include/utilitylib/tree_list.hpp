//////////////////////////////////////////////////
#ifndef UTIL_TREELIST_HPP
#define UTIL_TREELIST_HPP
//////////////////////////////////////////////////
#include <memory>
#include <stdexcept>
#include <concepts>
//////////////////////////////////////////////////
/*
* WORK IN PROGRESS
 * to do:
 * - minimize _valid checks / derefs -> maybe unchecked/checked functions
 * - const iterator, different algo-iterators
 * - benchmark in release for proper tests
 * - filter
 * - documentation
 * - copying
 */
//////////////////////////////////////////////////
namespace util
{
	//////////////////////////////////////////////////

	template <typename Ty, typename _Ty>
	static bool operator==(const std::weak_ptr<Ty>& a, const std::weak_ptr<_Ty>& b) { return !a.owner_before(b) && !b.owner_before(a); }
	template <typename Ty, typename _Ty>
	static bool operator==(const std::weak_ptr<Ty>& a, const std::shared_ptr<_Ty>& b) { return !a.owner_before(b) && !b.owner_before(a); }
	
	//////////////////////////////////////////////////
	/*
	 * C++ implementation of an iterable tree-data-structure
	 *
	 * named tree_list because internally, every branch is stored as multiple
	 * linked lists between parent, child and sibling nodes and extra weak_ptrs
	 * to iterate over all lists in a tree-structure
	 * 
	 * ownership and memory-management is done using std::shared_ptr and
	 * std::weak_ptr with the following structure:
	 * 
	 * '||' and '=' means strong reference ~ shared_ptr
	 * '|' and '-' means weak reference ~ weak_ptr
	 * 
	 * +--------+
	 * | header |  // the header is the 'end' / 'invalid' / 'root' node that uniquely identifies one tree
	 * +--------+
	 *   ||  / \
	 *   ||   |               
	 *   ||   +---------------+  // strong ref to first_child, weak to last_child
	 *   ||   |               |  // weak ref to parent
	 *   \/  \ /             \ /
	 * +----------+ <- +----------+
	 * | parent_1 |    | parent_2 | // strong ref to next_sibling, weak to prev_sibling
	 * +----------+ => +----------+ 
	 *   ||  / \
	 *   ||   |
	 *   ||   +--------------+---------------+
	 *   ||   |              |               |
	 *   \/  \ /             |              \ /
	 * +---------+ <- +---------+ <- +----------+
	 * | child_1 |    | child_2 |    | child_3  | // see no weak_ref from parent_1 to child_2, as it is not first or last child
	 * +---------+ => +---------+ => +----------+ // also no weak_ref from child_1 to child_3
	 * 
	 */
	//////////////////////////////////////////////////
	
	template <typename _Ty>
		requires std::copy_constructible<_Ty> and std::is_copy_assignable<_Ty>::value and std::is_default_constructible<_Ty>::value
	class tree_list
	{
	public:

		////////////////////////////////////////
		/// forward and using declarations
		////////////////////////////////////////

		class tree_node;
		class iterator;

		friend tree_node;
		friend iterator;

		using value_type = _Ty;
		using node_ptr = std::shared_ptr<tree_node>;
		using node_wptr = std::weak_ptr<tree_node>;
		using node_ref = std::reference_wrapper<tree_node>;

		using filter_condition = std::function<bool(const tree_node&)>;

		////////////////////////////////////////
		/// exception thrown when a tree node 
		/// was illegally accessed (when deleted)
		////////////////////////////////////////

		class bad_tree_node : public std::exception
		{
		public:
			bad_tree_node(const char* const _Message) : std::exception(_Message) {}
		};

		////////////////////////////////////////
		/// treelist members
		////////////////////////////////////////

		node_ptr m_header;

		std::size_t m_size = 0ULL;
		std::size_t m_depth = 0ULL;

		////////////////////////////////////////
		/// data structure that 
		/// makes up the whole tree
		////////////////////////////////////////

		class tree_node : public std::enable_shared_from_this<tree_node>
		{
			friend tree_list;
			friend iterator;

			////////////////////////////////////////
			/// tree_node members
			////////////////////////////////////////

			node_wptr m_parent;

			node_ptr m_next_sibling;
			node_wptr m_prev_sibling;

			node_ptr m_first_child;
			node_wptr m_last_child;

			value_type m_value;
			std::size_t m_child_num = 0ULL;
			std::size_t m_depth = 0ULL;

			////////////////////////////////////////
			/// navigation utility
			////////////////////////////////////////

			bool _has_parent() { return _valid(m_parent); }
			bool _has_children() { return _valid(m_first_child); }
			bool _has_next_sibling() { return _valid(m_next_sibling); }
			bool _has_prev_sibling() { return _valid(m_prev_sibling); }

			node_ptr _parent_unchecked() const { return m_parent.lock(); }
			node_ptr _next_sibling_unchecked() const { return m_next_sibling; }
			node_ptr _prev_sibling_unchecked() const { return m_prev_sibling.lock(); }
			node_ptr _first_child_unchecked() const { return m_first_child; }
			node_ptr _last_child_unchecked() const { return m_last_child.lock(); }

		public:
			/*
			 * somehow make the constructor private but still accessible to shared_ptr / enable_shared_from_this
			 */
			tree_node(const value_type& _val) : m_value(_val)
			{

			}

			static node_ptr create(const value_type& _val) { return std::make_shared<tree_node>(_val); }

			////////////////////////////////////////
			/// public interface
			////////////////////////////////////////

			value_type& get() { return m_value; }
			tree_node& operator=(const value_type& _val) { m_value = _val; return *this; }
			value_type value() const { return m_value; }
			std::size_t child_num() const { return m_child_num; }
			std::size_t depth() const { return m_depth; }

			tree_node& next_sibling() const { return _deref_checked(m_next_sibling); }
		};

		////////////////////////////////////////
		/// special members
		////////////////////////////////////////

		tree_list()
		{
			m_header = tree_node::create(value_type());
		}

		tree_list(tree_list& _other)
		{

		}

		tree_list(tree_list&& _other)
		{

		}

		tree_list& operator=(tree_list& _other)
		{

		}

		tree_list& operator=(tree_list&& _other)
		{

		}

		void swap(tree_list& a, tree_list& b)
		{

		}

		~tree_list()
		{
			clear();
		}

		////////////////////////////////////////
		/// info
		////////////////////////////////////////

		std::size_t size() const { return m_size; }
		std::size_t depth() const { return m_depth; }

		////////////////////////////////////////
		/// modifiers
		////////////////////////////////////////

		tree_node& emplace_front(const value_type& _val)
		{

		}

		tree_node& emplace_front_child(tree_node& _parent, const value_type& _val)
		{

		}

		tree_node& emplace_front_sibling(tree_node& _parent, const value_type& _val)
		{

		}

		tree_node& emplace_back(const value_type& _val)
		{
			return emplace_back_child(_deref_checked(m_header), _val);
		}

		tree_node& emplace_back_child(tree_node& _parent, const value_type& _val)
		{
			std::shared_ptr<tree_node> _ptr = tree_node::create(_val);
			tree_node& _new = _deref_checked(_ptr);
			_set_parent(_parent, _new);
			_new.m_depth = _parent.m_depth + 1;
			m_depth = _new.m_depth > m_depth ? _new.m_depth : m_depth;
			if (_parent._has_children())
			{
				tree_node& _last_child = _deref_checked(_parent.m_last_child);
				_parent.m_last_child = _ptr;
				_connect_sibling(_last_child, _new);
			}
			else
			{
				_parent.m_first_child = _ptr;
				_parent.m_last_child = _ptr;
			}
			++m_size;
			++_parent.m_child_num;
			return _new;
		}

		tree_node& emplace_back_sibling(tree_node& _sibling, const value_type& _val)
		{
			std::shared_ptr<tree_node> _ptr = tree_node::create(_val);
			tree_node& _new = _deref_checked(_ptr);
			_new.m_depth = _sibling.m_depth;
			if (_valid(_sibling.m_parent)) 
			{ 
				tree_node& _parent = _deref_checked(_sibling.m_parent);
				_set_parent(_parent, _new); ++_parent.m_child_num; 
			}
			if (_sibling._has_next_sibling())
			{
				_insert_sibling(_sibling, _deref_checked(_sibling.m_next_sibling), _new);
			}
			else
			{
				_connect_sibling(_sibling, _new);
			}
			++m_size;
			return _new;
		}

		void clear()
		{
			for (iterator _iter = begin(); _iter != end();) { _iter = erase(_iter); }
		}

		iterator erase(iterator _where)
		{
			if (_where == end()) { throw std::runtime_error("cannot erase end iterator"); }
			if (_where.m_header != m_header) { throw std::runtime_error("cannot erase iterator to node of different tree"); }

			tree_node& _mynode = *_where;
			node_ptr _next_valid_node = nullptr;

			if (_mynode._has_next_sibling()) { _next_valid_node = _promote_checked(_mynode.m_next_sibling); }
			else if (_mynode._has_parent()) { _next_valid_node = _promote_checked(_mynode.m_parent); }
			else { _next_valid_node = m_header; }

			/*
			 * iteratively delete every child node here
			 */
			tree_node& _parent = _deref_checked(_mynode.m_parent);
			/*
			 * determine where strong refs are (parent for first child, else m_prev_sibling
			 */
			if (!_mynode._has_prev_sibling())
			{
				if (_mynode._has_next_sibling()) { _deref_checked(_mynode.m_next_sibling).m_prev_sibling.reset(); }
				_parent.m_first_child = _mynode.m_next_sibling; // deletion here through getting rid of the last strong ref
			}
			else
			{
				_connect_sibling(_deref_checked(_mynode.m_prev_sibling), _deref_checked(_mynode.m_next_sibling));
			}

			return iterator(_deref_checked(_next_valid_node), _deref_checked(_where.m_header));
		}

		////////////////////////////////////////
		/// lookup
		////////////////////////////////////////

		std::vector<node_ref> filter(const filter_condition& _cond)
		{
			std::vector<node_ref> _res;
			for (iterator _iter = begin(); _iter != end(); ++_iter)
			{
				if (_cond(*_iter)) { _res.emplace_back(std::ref(*_iter)); }
			}
			return _res;
		}

		std::vector<iterator> filter_iterators(const filter_condition& _cond)
		{
			std::vector<iterator> _res;
			for (iterator _iter = begin(); _iter != end(); ++_iter)
			{
				if (_cond(*_iter)) { _res.emplace_back(_iter); }
			}
			return _res;
		}

		tree_node& find_first(const value_type& _val)
		{
			for (iterator _iter = begin(); _iter != end(); ++_iter)
			{
				if (_val == _iter->m_value) { return std::ref(*_iter); }
			}
			throw std::runtime_error("tree does not contain node of given value");
		}

		iterator find_first_iterator(const value_type& _val)
		{
			for (iterator _iter = begin(); _iter != end(); ++_iter)
			{
				if (_val == _iter->m_value) { return _iter; }
			}
			throw std::runtime_error("tree does not contain node of given value");
		}

		tree_node& filter_first(const filter_condition& _cond)
		{
			for (iterator _iter = begin(); _iter != end(); ++_iter)
			{
				if (_cond(*_iter)) { return std::ref(*_iter); }
			}
			throw std::runtime_error("tree does not contain node which satisfies condition");
		}

	public:

		////////////////////////////////////////
		/// iteration
		////////////////////////////////////////

		class iterator
		{
			friend tree_list;
			friend tree_node;

			////////////////////////////////////////
			/// iterator members
			////////////////////////////////////////

			node_wptr m_ptr; // node that the iterator points to
			node_wptr m_header; // node to identify the tree that the iterator belongs to

			explicit iterator(tree_node& _node, tree_node& _header) : m_ptr(_node.weak_from_this()), m_header(_header.weak_from_this()) {}

		public:

			////////////////////////////////////////
			/// public interface
			////////////////////////////////////////

			bool operator==(const typename tree_list<_Ty>::iterator& i)
			{
				if (m_header != i.m_header) { throw std::runtime_error("cannot compare iterators of different trees"); }
				return m_ptr == i.m_ptr;
			}

			bool operator!=(const typename tree_list<_Ty>::iterator& i)
			{
				if (m_header != i.m_header) { throw std::runtime_error("cannot compare iterators of different trees"); }
				return m_ptr != i.m_ptr;
			}

			tree_node& operator*() { return _deref_checked(m_ptr); }
			tree_node* operator->() { return &_deref_checked(m_ptr); }

			iterator& operator++()
			{
				m_ptr = _find_next_depth_first(m_ptr, m_header);
				return *this;
			}
		};

		iterator begin() { tree_node& _header = _deref_checked(m_header); return iterator(_header._has_children() ? *_header.m_first_child : _header, _header); }
		iterator end() { tree_node& _header = _deref_checked(m_header); return iterator(_header, _header); }

	private:

		////////////////////////////////////////
		/// pointer handling utility
		////////////////////////////////////////

		static bool _valid(node_ptr _ptr) { return _ptr != nullptr; }
		static bool _valid(node_wptr _wptr) { return !_wptr.expired(); }
		static tree_node& _deref_checked(node_ptr _ptr) { if (!_valid(_ptr)) { throw bad_tree_node("tree_node was nullptr"); } return *_ptr; }
		static tree_node& _deref_checked(node_wptr _wptr) { if (!_valid(_wptr)) { throw bad_tree_node("tree_node was nullptr"); } return *_wptr.lock(); }
		static node_ptr _promote_checked(node_wptr _wptr) { if (!_valid(_wptr)) { throw bad_tree_node("tree_node was nullptr"); } return _wptr.lock(); }

		////////////////////////////////////////
		/// linked list/tree utility
		////////////////////////////////////////

		static void _connect_sibling(tree_node& _first, tree_node& _second) { _first.m_next_sibling = _second.shared_from_this(); _second.m_prev_sibling = _first.weak_from_this(); }
		static void _insert_sibling(tree_node& _first, tree_node& _second, tree_node& _insert) { _connect_sibling(_insert, _second); _connect_sibling(_first, _insert); }
		static void _set_parent(tree_node& _parent, tree_node& _child) { _child.m_parent = _parent.weak_from_this(); }

		static node_wptr _find_next_depth_first(node_wptr _node, node_wptr _header)
		{
			tree_node& _mynode = _deref_checked(_node);
			if (_mynode._has_children())
			{
				return _mynode.m_first_child;
			}
			else if (_mynode._has_next_sibling())
			{
				return _mynode.m_next_sibling;
			}
			else
			{
				node_ptr _current = _promote_checked(_mynode.m_parent);
				while (!_valid(_current->m_next_sibling) && _current != _header)
				{
					_current = _promote_checked(_current->m_parent);
				}
				return _current == _header ? _current : _current->m_next_sibling;
			}
		}
	};

	//////////////////////////////////////////////////
}
//////////////////////////////////////////////////
#endif 
//////////////////////////////////////////////////