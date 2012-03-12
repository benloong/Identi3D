//
// File: OptionTree.h
// ==================
// Configuration tree.
//

#ifndef CORN3D_SRC_UTILS_OPTIONTREE_H
#define CORN3D_SRC_UTILS_OPTIONTREE_H

#include <src/corn3d/General.h>
#include <iterator>

typedef unsigned long ULONG;

#define OPTIONTREE_NAME_MAXLEN		32
#define OPTIONTREE_VALUE_MAXLEN		32
#define OPTIONTREE_LOCATION_MAXLEN	260

#define OPTIONTREE_MAX_DEPTH		10

#define OPTIONELEMENT_GROUPVALUE	__T("")

namespace Corn3D
{

	struct OptionElement
	{
		TCHAR		name[OPTIONTREE_NAME_MAXLEN + 1];
		TCHAR		value[OPTIONTREE_VALUE_MAXLEN + 1];
		UINT		hash;

		struct OptionElement *child;
		struct OptionElement *father;
		struct OptionElement *next;
	};

	class __declspec(dllexport) OptionIterator : 
		public std::iterator<std::input_iterator_tag, OptionElement>
	{
		OptionElement *_ptr;

	public:
		OptionIterator(OptionElement *elem) : _ptr(elem) {} ;
		OptionIterator(const OptionIterator &iter) : _ptr(iter._ptr) {} ;
		
		OptionIterator &operator ++() { if(_ptr) _ptr = _ptr->next; return *this; }
		OptionIterator operator ++(int) { OptionIterator tmp(*this); operator++(); return tmp; }
		bool operator ==(const OptionIterator &iter) const { return _ptr == iter._ptr; }
		bool operator !=(const OptionIterator &iter) const { return _ptr != iter._ptr; }
		OptionElement &operator *() { return *_ptr; }

		OptionElement *get(void) { return _ptr; }
		bool end(void) const { return _ptr == NULL; }
	};

	class __declspec(dllexport) OptionTree
	{
	private:
		OptionElement *_root;
		DebugManager *_debugger;

	private:
		OptionTree(const OptionTree &tree);

	public:
		OptionTree(void) : _root(NULL), _debugger(NULL) {} ;
		~OptionTree(void) { clean(); } ;

		/*
		 * Get the hash value of a string.
		 */
		static ULONG hashString(const TCHAR *str);

		/*
		 * Remove entire option tree.
		 */
		void clean(void);

		/*
		 * Get root level iterator.
		 */
		const OptionIterator getRootIterator(void) const { return OptionIterator(_root); }
		
		/*
		 * Add a option element (father == NULL specifies the Root node).
		 */
		OptionElement *addElement(OptionElement *father, const TCHAR *name, const TCHAR *value);

		/*
		 * Add a option element according to the location.
		 */
		OptionElement *addElement(const TCHAR *location, const TCHAR *value);
		
		/*
		 * Remove the specified element.
		 */
		void removeElement(OptionElement *elem);

		/*
		 * Set key in the location.
		 */
		HRESULT setValue(const TCHAR *location, const TCHAR *value);
			
		/*
		 * Get key value in the location.
		 * Calls will fail if expected_type is not correct.
		 */
		HRESULT getValue(const TCHAR *location, TCHAR *value, UINT buffer_size);

		/*
		 * Translate element to location.
		 */
		HRESULT getLocation(OptionElement *elem, TCHAR *location, UINT buffer_size);

		/*
		 * Translate location to element.
		 */
		OptionElement *getElement(const TCHAR *location);

		/*
		 * Does item exist.
		 */
		OptionElement *checkExistence(OptionElement *father, const TCHAR *name);

		/*
		 * Set a debug manager.
		 */
		void setDebugManager(DebugManager *new_debugger = NULL) { _debugger = new_debugger; }
	};

};

#endif // CORN3D_SRC_UTILS_OPTIONTREE_H