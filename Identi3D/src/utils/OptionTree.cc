//
// File: OptionTree.cc
// ===================
//

#include <src/utils/OptionTree.h>
#include <src/utils/DebugManager.h>

namespace Identi3D
{

	ULONG OptionTree::hashString(const TCHAR *str)
	{
		const ULONG seed = 13131;
		ULONG hash = 0;

		while(*str) {
			hash = hash * seed + (*str++);
		}
		return hash;
	}

	OptionElement *OptionTree::checkExistence(OptionElement *father, const TCHAR *name)
	{
		if(name == NULL) {
			_DebugPrint(_debugger, E_INVALID_PARAMETERS);
			return NULL;
		}

		ULONG hash;
		OptionIterator iter((father == NULL) ? _root : father->child);

		hash = hashString(name);
		while(!iter.end()) {
			if((*iter).hash == hash && _tcscmp((*iter).name, name) == 0) {
				return iter.get();
			}
			++iter;
		}
		return NULL;
	}
		
	OptionElement *OptionTree::addElement(OptionElement *father, const TCHAR *name, const TCHAR *value)
	{
		// Check parameters.
		if(name == NULL || _tcslen(name) > OPTIONTREE_NAME_MAXLEN ||
			(value != NULL && _tcslen(value) > OPTIONTREE_VALUE_MAXLEN)){
				_DebugPrint(_debugger, E_INVALID_PARAMETERS);
				return NULL;
		}

		OptionElement *p;						// Temporary node.
		
		p = checkExistence(father, name);		// If already exists, assign the value and return.
		if(p) {
			_DebugPrintV(_debugger, W_OPTION_ELEMENT_ALREADY_EXISTS, name);
			if(value) {
				_tcscpy_s(p->value, OPTIONTREE_VALUE_MAXLEN, value);
			} else {
				p->value[0] = __T('\0');
			}
			return p;
		}

		p = new (std::nothrow) OptionElement;
		if(p == NULL) {
			_DebugPrint(_debugger, E_OUT_OF_MEMORY, name);
			return NULL;
		}

		_tcscpy_s(p->name, OPTIONTREE_NAME_MAXLEN + 1, name);
		p->hash = hashString(name);
		p->child = NULL;
		if(value) {
			_tcscpy_s(p->value, OPTIONTREE_VALUE_MAXLEN + 1, value);
		} else {
			p->value[0] = __T('\0');
		}

		// Attach child node to father.
		if(father == NULL) {					// Null father specify the root node.
			p->next = _root;
			_root = p;
			p->father = NULL;
		} else {
			p->next = father->child;
			father->child = p;
			p->father = father;
		}

		_DebugPrintV(_debugger, I_NEW_OPTION_ELEMENT_ADDED, p->name, p->value);
		return p;
	}
	
	OptionElement *OptionTree::addElement(const TCHAR *location, const TCHAR *value)
	{
		if(location == NULL) {
			_DebugPrint(_debugger, E_INVALID_PARAMETERS);
			return NULL;
		}
		OptionElement *dest, *p;
		TCHAR tmploc[OPTIONTREE_LOCATION_MAXLEN + 1];
		int s, t, len;
		bool need_create = false;
		
		_tcsncpy_s(tmploc, location, OPTIONTREE_LOCATION_MAXLEN);
		len = _tcslen(tmploc);
		dest = NULL;
		s = 0;
		
		for(t = 0; t < len; t++) {
			if(tmploc[t] == __T('.')) {
				tmploc[t] = __T('\0');
				if(need_create) {
					addElement(dest, tmploc + s, __T(""));
				} else {
					p = checkExistence(dest, tmploc + s);
					if(p == NULL) {
						dest = addElement(dest, tmploc + s, __T(""));
						need_create = true;
					} else {
						dest = p;
					}
				}
				s = t + 1;
			}
		}
		return addElement(dest, tmploc + s, value);
	}

	void OptionTree::removeElement(OptionElement *elem)
	{
		if(elem == NULL) {
			_DebugPrint(_debugger, E_INVALID_PARAMETERS);
			return ;
		}
		
		_DebugPrintV(_debugger, I_OPTION_ELEMENT_REMOVAL_SCHEDULED, elem->name);

		if(elem->child) {		// Remove children.
			OptionIterator iter(elem->child);
			while(!iter.end()) removeElement((iter++).get());
		}

		if(elem->father && elem->father->child == elem) {		// Delete the first node in a layer.
			elem->father->child = elem->next;
		}
		delete elem;
	}
	
	void OptionTree::clean(void)
	{
		OptionIterator iter = getRootIterator();

		_DebugPrintV(_debugger, I_REMOVING_ENTIRE_OPTION_TREE);
		while(!iter.end()) removeElement((iter++).get());
		_root = NULL;
	}
	
	HRESULT OptionTree::getLocation(OptionElement *elem, TCHAR *location, UINT buffer_size)
	{
		if(elem == NULL || location == NULL) {
			_DebugPrint(_debugger, E_INVALID_PARAMETERS);
			return E_FAIL;
		}

		int i, length, k;
		OptionElement *stack[OPTIONTREE_MAX_DEPTH];

		stack[0] = elem;
		for(i = 1; i < OPTIONTREE_MAX_DEPTH; i++) {
			if(stack[i - 1]->father) stack[i] = stack[i - 1]->father;
			else break;
		}
		if(i >= OPTIONTREE_MAX_DEPTH) {
			_DebugPrint(_debugger, E_OPTION_ELEMENT_STACK_OVERFLOW, OPTIONTREE_MAX_DEPTH);
			return E_FAIL;			// Too many layers: Stack overflow.
		}

		length = 0;
		*location = __T('\0');
		for(--i; i >= 0; i--) {
			k = _tcslen(stack[i]->name);
			if(length + k + 1 > (int)buffer_size) break;
			length += k;
			_tcscat_s(location, buffer_size, stack[i]->name);
			*(location + length++) = __T('.');
			*(location + length) = __T('\0');
		}
		if(i >= 0) {
			_DebugPrint(_debugger, E_INVALID_PARAMETERS);
			return E_FAIL;		// Space not enough.
		}
		
		*(location + length - 1) = __T('\0');
		_DebugPrintV(_debugger, I_OPTION_ELEMENT_LOCATION, location);
		return S_OK;			
	}

	OptionElement *OptionTree::getElement(const TCHAR *location)
	{
		if(location == NULL) {
			_DebugPrint(_debugger, E_INVALID_PARAMETERS);
			return NULL;
		}

		OptionElement *dest;
		TCHAR tmploc[OPTIONTREE_LOCATION_MAXLEN + 1];
		int s, t, len;

		_tcsncpy_s(tmploc, location, OPTIONTREE_LOCATION_MAXLEN);
		len = _tcslen(tmploc);
		dest = NULL;
		s = 0;

		for(t = 0; t < len; t++) {
			if(tmploc[t] == __T('.')) {
				tmploc[t] = __T('\0');
				dest = checkExistence(dest, tmploc + s);
				if(dest == NULL) return NULL;
				s = t + 1;
			}
		}

		dest = checkExistence(dest, tmploc + s);
		return dest;
	}
	
	HRESULT OptionTree::setValue(const TCHAR *location, const TCHAR *value)
	{
		OptionElement *elem;

		elem = getElement(location);
		if(elem == NULL || 
			value == NULL || _tcslen(value) > OPTIONTREE_VALUE_MAXLEN) {
				_DebugPrint(_debugger, E_INVALID_PARAMETERS);
				return E_FAIL;
		}
		_tcscpy_s(elem->value, value);
		_DebugPrintV(_debugger, I_OPTION_VALUE_MODIFIED, elem->name, value);
		return S_OK;
	}

	HRESULT OptionTree::getValue(const TCHAR *location, TCHAR *value, UINT buffer_size)
	{
		OptionElement *elem;
		
		elem = getElement(location);
		if(elem == NULL ||
			value == NULL || buffer_size < _tcslen(elem->value)) {
				_DebugPrint(_debugger, E_INVALID_PARAMETERS);
				return E_FAIL;
		}

		_tcscpy_s(value, buffer_size, elem->value);
		return S_OK;
	}

};
