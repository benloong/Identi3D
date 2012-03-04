//
// File: OptionTree.h
// ==================
// Configuration tree.
//

#ifndef CORN3D_SRC_UTILS_OPTIONTREE_H
#define CORN3D_SRC_UTILS_OPTIONTREE_H

#include <src/corn3d/General.h>

namespace Corn3D
{

	enum OptionType
	{
		OptionType_Bool,
		OptionType_String,
		OptionType_Int,
		OptionType_Group
	};

	struct OptionElement
	{
		TCHAR		*name;
		OptionType	type;
		void		*value;

		struct OptionElement *father;
		struct OptionElement *prev;
		struct OptionElement *next;
	};

	class __declspec(dllexport) OptionTree
	{
	private:
		OptionElement *root;

	public:

		OptionTree(void) : root(NULL) {} ;
		~OptionTree(void) { clean(); } ;

		/*
		 * Remove entire option tree.
		 */
		void clean(void);

		/*
		 * Add a option element and return it in 'dest'.
		 * father == NULL specifies the Root node.
		 */
		HRESULT addElement(OptionElement *father, const TCHAR *name, OptionType type, OptionElement **dest = NULL);

		/*
		 * Remove the specified element.
		 */
		void removeElement(OptionElement *elem);

		/*
		 * Set key in the location.
		 */
		HRESULT setKey(const TCHAR *location, const TCHAR *value);
			
		/*
		 * Get key value in the location.
		 * Calls will fail if expected_type is not correct.
		 */
		HRESULT getKey(const TCHAR *location, OptionType expected_type, void **value);

		/*
		 * Translate element to location.
		 */
		HRESULT getElementLocation(OptionElement *src, TCHAR *location, UINT buffer_size);

		/*
		 * Translate location to element.
		 */
		HRESULT getSpecifiedElement(const TCHAR *location, OptionElement **dest);

		/*
		 * Convert bool to string.
		 */
		const TCHAR *BoolToString(bool b) const { return b ? __T("true") : __T("false"); }
	};

};

#endif // CORN3D_SRC_UTILS_OPTIONTREE_H