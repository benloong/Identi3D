//
// File: OptionTree.h
// ==================
// Configuration tree.
//

#ifndef IDENTI3D_SRC_UTILS_OPTIONTREE_H
#define IDENTI3D_SRC_UTILS_OPTIONTREE_H

#include <src/identi3d/General.h>
#include <src/utils/DebugManager.h>
#include <string>
#include <iterator>

typedef unsigned long ULONG;

namespace Identi3D
{

	/*
	 * Max size of hash table.
	 */
	const int OPTIONTREE_HASHTABLE_SIZE = 1087;

	/*
	 * Group value specified for OptionElement.
	 */
	const std::wstring OPTIONTREE_GROUPVALUE = __T("");

	struct OptionElement
	{
		std::wstring name;
		std::wstring value;
		ULONG		 hash;

		struct OptionElement *child;
		struct OptionElement *father;
		struct OptionElement *next;
		struct OptionElement *table_next;

#if defined (_MEMORY_LEAK_DETECTION)
		static void *operator new(size_t size)
		{
			void *p = ::operator new(size, std::nothrow);
			if(p != NULL) {
				DebugManager::onAllocation(size);
			}
			return p;
		}

		static void *operator new[](size_t size)
		{
			void *p = ::operator new[](size, std::nothrow);
			if(p != NULL) {
				DebugManager::onAllocation(size);
			}
			return p;
		}

		static void operator delete(void *p, size_t size)
		{
			DebugManager::onDeallocation(size);
			return ::operator delete(p, std::nothrow);
		}

		static void operator delete[](void *p, size_t size)
		{
			DebugManager::onDeallocation(size);
			return ::operator delete[](p, std::nothrow);
		}
#endif // defined (_MEMORY_LEAK_DETECTION)
	};

	class __declspec(dllexport) OptionIterator : 
		public std::iterator<std::input_iterator_tag, OptionElement>
	{
	private:
		OptionElement *_ptr;

	public:
		OptionIterator(void) : _ptr(NULL) {} ;
		OptionIterator(OptionElement *elem) : _ptr(elem) {} ;
		OptionIterator(const OptionIterator &iter) : _ptr(iter._ptr) {} ;
		~OptionIterator(void) {} ;
		
		inline OptionIterator &operator ++() { if(_ptr) _ptr = _ptr->next; return *this; }
		inline OptionIterator operator ++(int) { OptionIterator tmp(*this); operator++(); return tmp; }
		inline bool operator ==(const OptionIterator &iter) const { return _ptr == iter._ptr; }
		inline bool operator !=(const OptionIterator &iter) const { return _ptr != iter._ptr; }
		inline bool operator !(void) const { return _ptr == NULL; }
		inline const OptionElement *operator ->(void) const { return _ptr; }
		inline const OptionElement &operator *(void) const { return *_ptr; }

		inline const OptionElement *get(void) const { return _ptr; }
		inline bool end(void) const { return _ptr == NULL; }
	};

	class __declspec(dllexport) OptionTree : public DebugFrame
	{
	private:
		OptionElement *_table[OPTIONTREE_HASHTABLE_SIZE];

		OptionElement *_root;

	private:
		OptionTree(const OptionTree &tree);
		OptionTree &operator=(OptionTree &rhs);

	private:
		void rawDelete(OptionElement *elem);

	public:
		OptionTree(DebugManager *debugger = NULL);
		~OptionTree(void);

		/*
		 * Get the hash value of a string.
		 */
		static ULONG hashString(const wchar_t *str);

		/*
		 * Remove entire option tree.
		 */
		bool clean(void);

		/*
		 * Get root level iterator.
		 */
		inline OptionIterator getRootIterator(void) const 
		{ 
			return OptionIterator(_root);
		}
		
		/*
		 * Add a option element (father == NULL specifies the Root node).
		 */
		OptionElement *addElement(OptionElement *father, const std::wstring &name, const std::wstring &value);

		/*
		 * Create the location and assign the value.
		 */
		OptionElement *addElement(const std::wstring &location, const std::wstring &value);

		/*
		 * Set value of the location.
		 */
		bool setValue(const std::wstring &location, const std::wstring &value);
			
		/*
		 * Get value of the location (return "" if not exists).
		 */
		const std::wstring getValue(const std::wstring &location) const;

		/*
		 * Check element's existence and fetch the object.
		 */
		OptionElement *getElement(const std::wstring &location) const;
	};

};

#endif // IDENTI3D_SRC_UTILS_OPTIONTREE_H