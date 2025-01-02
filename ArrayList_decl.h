#include <cstdint>
#include <iostream>

/// <summary>
/// <para> Custom implementation of the std::vector </para>
/// <para> - seems to be a lot faster than std::vector, but probably has some drawbacks </para>
/// <para> - the people who built std::vector probably designed it the way it is for a reason </para>
/// <para> - not entirely sure if this is 100% memory safe, so far no errors though </para>
/// <para> - implementations are stored in seperate .h file because templates compile a little wonky in .cpp files
/// </summary>
template <typename T>
class ArrayList
{
public:
	ArrayList();
	ArrayList(const ArrayList<T>& copy) noexcept;
	ArrayList(ArrayList<T>&& move) noexcept;
	ArrayList(std::initializer_list<T> initializer_list);
	~ArrayList();

	uint32_t getSize() const;
	uint32_t getCapacity() const;

	void add(T element);
	void emplace(T element);
	void remove(uint32_t index);
	void set(uint32_t index, T value);

	T& begin() const;
	T& end() const;
	T& get(uint32_t index) const;
	T& operator[](uint32_t index);

	ArrayList<T>& operator=(const ArrayList<T>& copy) noexcept;
	ArrayList<T>& operator=(ArrayList<T>&& move) noexcept;

protected:

	void free_internal_data();

	T* internal_data = nullptr;
	uint32_t size = NULL;
	uint32_t capacity = NULL;

};

template <typename T> std::ostream& operator<<(std::ostream& os, const ArrayList<T>& arraylist);

template <typename T> void print(const T& arg);
template <typename T> void println(const T& arg);
