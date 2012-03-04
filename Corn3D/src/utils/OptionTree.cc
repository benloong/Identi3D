//
// File: OptionTree.cc
// ===================
//

#include <src/utils/OptionTree.h>

namespace Corn3D
{

	HRESULT OptionTree::addElement(OptionElement *father, const TCHAR *name, OptionType type, OptionElement **dest)
	{
		if(name == NULL) return E_FAIL;
		unsigned int length = _tcslen(name) + 1;
		OptionElement *p;
		
		p = new (std::nothrow) OptionElement;
		if(p == NULL) return E_FAIL;				// Memory insufficient.
		p->type = type;
		p->name = new (std::nothrow) TCHAR[length];
		if(p->name == NULL) {						// Memory insufficient.
			delete p;
			return E_FAIL;
		}
		memcpy(p->name, name, sizeof(TCHAR) * length);
		p->value = NULL;

		if(father == NULL) {
			p->next = root;
			if(p->next) p->next->prev = p;
			root = p;
			p->prev = NULL;
			p->father = NULL;
		} else if(father->type == OptionType_Group) {
			p->next = static_cast<OptionElement *>(father->value);
			if(p->next) p->next->prev = p;
			father->value = static_cast<void *>(p);
			p->father = father;
			p->prev = NULL;
		} else return E_FAIL;

		if(dest != NULL) *dest = p;
		return S_OK;
	}

	void OptionTree::removeElement(OptionElement *elem)
	{
		if(elem->type == OptionType_Group && elem->value != NULL) {
			removeElement(static_cast<OptionElement *>(elem->value));
		}
		if(elem->father && elem->father->value == static_cast<void *>(elem)) {
			elem->father->value = static_cast<void *>(elem->next);
		}
		if(elem->prev) elem->prev->next = elem->next;
		if(elem->next) elem->next->prev = elem->prev;
		
		switch(elem->type)
		{
		case OptionType_String:
			delete[] static_cast<TCHAR *>(elem->value);
			break;
		case OptionType_Bool:
			delete static_cast<bool *>(elem->value);
			break;
		case OptionType_Int:
			delete static_cast<int *>(elem->value);
			break;
		}
		delete[] elem->name;

		elem->name = NULL;
		elem->value = NULL;
		delete elem;
	}

	void OptionTree::clean(void)
	{
		OptionElement *t = root;

		while(t->next) {
			t = t->next;
			removeElement(t->prev);
		}
		removeElement(t);
	}

	HRESULT OptionTree::getElementLocation(OptionElement *src, TCHAR *location, UINT buffer_size)
	{
		if(location == NULL || src == NULL) return E_FAIL;
		OptionElement *stack[10], *p;	// Trace up to 10 layers.
		unsigned int i = 0, k = 0;
		int s = 0;

		stack[s++] = p = src;
		while(p->father && s < 10) {
			p = p->father;
			stack[s++] = p;
		}
		if(s == 10) return E_FAIL;		// Buffer overflow.
		s--;

		while(i < buffer_size - 1 && s >= 0) {
			if(stack[s]->name[k] != '\0') {
				*(location + (i++)) = stack[s]->name[k++];
			} else {
				if((--s) >= 0) *(location + (i++)) = __T('.');
				k = 0;
			}
		}
		if(s >= 0) return E_FAIL;		// Buffer size not enough.
		*(location + i) = '\0';

		return S_OK;
	}

	HRESULT OptionTree::getSpecifiedElement(const TCHAR *location, OptionElement **dest)
	{
		if(location == NULL || dest == NULL) return E_FAIL;

		OptionElement *p, *parent;
		TCHAR tmp[256];
		HRESULT hr;
		unsigned int i, s, length;

		length = _tcslen(location);
		p = root;
		parent = NULL;
		for(i = 0, s = 0; i < length; i++) {
			if(*(location + i) == __T('.')) {							// Discover a group and expand it.
				while(p) {
					if(_tcsncmp(location + s, p->name, i - s) == 0) {	// If group exists.
						if(p->type != OptionType_Group) return E_FAIL;	// Type not match.

						// Sort tree to the most visited order.
						if(parent == NULL && root != p) {
							p->next = root;
							if(root) root->prev = p;
							root = p;
						} else if(parent != NULL && parent->value != (void *)p) {
							if(p->prev) p->prev->next = p->next;
							if(p->next) p->next->prev = p->prev;
							p->prev = NULL;
							p->next = (OptionElement *)parent->value;
							parent->value = (void *)p;
						}

						parent = p;
						p = (OptionElement *)(p->value);
						break;
					} else {
						p = p->next;
					}
				}
				if(p == NULL) {
					if(i - s > 255) return E_FAIL;
					_tcsncpy_s(tmp, location + s, i - s);
					hr = addElement(parent, tmp, OptionType_Group, &p);
					if(FAILED(hr)) return E_FAIL;
					parent = p;
					p = (OptionElement *)(p->value);
				}
				s = i + 1;
			}
		}

		while(p) {
			if(_tcsncmp(location + s, p->name, i - s) == 0) {
				*dest = p;
				break;
			} else {
				p = p->next;
			}
		}

		if(p) return S_OK;
		return E_FAIL;
	}

	HRESULT OptionTree::setKey(const TCHAR *location, const TCHAR *value)
	{
		OptionElement *elem;
		unsigned int length;

		if(value == NULL || FAILED(getSpecifiedElement(location, &elem))) return E_FAIL;
		length = _tcslen(value) + 1;
		switch(elem->type)
		{
		case OptionType_String:
			if(elem->value != NULL) delete[] static_cast<TCHAR *>(elem->value);
			elem->value = new (std::nothrow) TCHAR[length];
			if(elem->value == NULL) return E_FAIL;			// Memory insufficient.
			memcpy(elem->value, value, length * sizeof(TCHAR));
			break;
		case OptionType_Bool:
			elem->value = new (std::nothrow) bool;
			if(elem->value == NULL) return E_FAIL;			// Memory insufficient.
			if(length >= 5 && _totlower(value[0]) == __T('t') && _totlower(value[1]) == __T('r') && 
				_totlower(value[2]) == __T('u') && _totlower(value[3]) == __T('e')) {
					*((bool *)elem->value) = true;
			} else if(length >= 6 && _totlower(value[0]) == __T('f') && _totlower(value[1]) == __T('a') && 
				_totlower(value[2]) == __T('l') && _totlower(value[3]) == __T('s') && _totlower(value[4]) == __T('e')) {
					*((bool *)elem->value) = false;
			} else {
				elem->value = false;
				return S_FALSE;
			}
			break;
		case OptionType_Int:
			elem->value = new (std::nothrow) int;
			if(elem->value == NULL) return E_FAIL;			// Memory insufficient.
			*((int *)elem->value) = _tstoi(value);
			break;
		default:
			return E_FAIL;
		}

		return S_OK;
	}

	HRESULT OptionTree::getKey(const TCHAR *location, OptionType expected_type, void **value)
	{
		OptionElement *elem;

		if(value == NULL || FAILED(getSpecifiedElement(location, &elem))) return E_FAIL;
		if(elem->type != expected_type) return E_FAIL;
		*value = elem->value;
		return S_OK;
	}

};
