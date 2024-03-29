//
// File: OptionTree.cc
// ===================
//

#include <src/utils/OptionTree.h>
#include <src/utils/DebugManager.h>

namespace Identi3D
{

	OptionTree::OptionTree(DebugManager *debugger) 
		: _root(NULL), DebugFrame(debugger)
	{
		memset(_table, 0, sizeof(_table));
	}

	OptionTree::~OptionTree(void)
	{
		clean();
	}

	ULONG OptionTree::hashString(const wchar_t *str)
	{
		const ULONG seed = 13131;
		ULONG hash = 0;

		while(*str) {
			hash = hash * seed + (*str++);
		}
		return hash % OPTIONTREE_HASHTABLE_SIZE;
	}
		
	OptionElement *OptionTree::addElement(OptionElement *father, const std::wstring &name, const std::wstring &value)
	{
		OptionElement *p;		// Temporary node of OptionTree.
		std::wstring location;	// Real location of the node.

		try
		{
			if(father == NULL) {
				// Top node should assign the name directly.
				location = name;
			} else {
				// Otherwise assign the location to name field.
				location = father->name;
				location.push_back(__T('.'));
				location += name;
			}

			p = getElement(location);		// If already exists, assign the value and return.
			if(p != NULL) {
				_printVerboseMessage(__FILE__, __LINE__, W_OPTION_ELEMENT_ALREADY_EXISTS, name);
				p->value = value;
				return p;
			}

			p = new OptionElement;
			p->name = location;
			p->value = value;
			p->hash = hashString((p->name).c_str());
			p->father = father;
			p->child = NULL;

			// Attach child node to father.
			if(father == NULL) {
				p->next = _root;
				_root = p;
			} else {
				p->next = father->child;
				father->child = p;
			}

			p->table_next = _table[p->hash];
			_table[p->hash] = p;

			_printVerboseMessage(__FILE__, __LINE__, I_NEW_OPTION_ELEMENT_ADDED, p->name.c_str());
		} catch(std::exception &e) {
			if(p != NULL) {
				if(father == NULL) {
					if(_root == p) {
						_root = p->next;
					}
				} else if(father->child == p) {
					father->child = p->next;
				}
				delete p;
				p = NULL;
			}
			_printException(__FILE__, __LINE__, e);
		}
		return p;
	}
	
	OptionElement *OptionTree::addElement(const std::wstring &location, const std::wstring &value)
	{
		OptionElement *p = NULL, *s = NULL;

		try {
			std::wstring tmploc(location);
			size_t pos, next;

			pos = 0;
			while(true) {
				next = tmploc.find(L'.', pos);
				p = _table[hashString(tmploc.substr(0, next).c_str())];
				if(p == NULL) {
					break;
				}
				if(next == std::wstring::npos) break;
				else pos = next + 1;
				s = p;
			}
			if(pos == std::wstring::npos) {
				if(p) p->value = value;
				return p;
			} else if(tmploc[pos] == '.') pos++;

			while(true) {
				next = tmploc.find(L'.', pos);
				s = addElement(s, tmploc.substr(pos, next - pos), OPTIONTREE_GROUPVALUE);
				if(s == NULL) return NULL;
				if(next == std::wstring::npos) break;
				else pos = next + 1;
			}
			if(s) s->value = value;
		} catch(std::exception &e) {
			_printException(__FILE__, __LINE__, e);
		}
		return s;
	}

	void OptionTree::rawDelete(OptionElement *elem)
	{
		OptionElement *p, *s;
		if(elem == NULL) return ;

		if(elem->child) {
			s = elem->child;
			p = s->next;
			while(p) {
				rawDelete(s);
				s = p;
				p = p->next;
			}
			rawDelete(s);
		}
		delete elem;
	}
	
	bool OptionTree::clean(void)
	{
		if(_root == NULL) return true;

		try
		{
			OptionElement *p, *s;

			s = _root;
			p = s->next;
			while(p) {
				rawDelete(s);
				s = p;
				p = p->next;
			}
			rawDelete(s);

			_root = NULL;
			memset(_table, 0, sizeof(_table));
		} catch(std::exception &e) {
			_printException(__FILE__, __LINE__, e);
			return false;
		}

		_printVerboseMessage(__FILE__, __LINE__, I_OPTION_TREE_REMOVED);
		return true;
	}

	OptionElement *OptionTree::getElement(const std::wstring &location) const
	{
		OptionElement *p;

		p = _table[hashString(location.c_str())];
		while(p) {
			if(p->name == location) {
				return p;
			}
		}
		return NULL;
	}
	
	bool OptionTree::setValue(const std::wstring &location, const std::wstring &value)
	{
		OptionElement *p;

		try {
			p = getElement(location);
			if(p != NULL) {
				p->value = value;
				_printVerboseMessage(__FILE__, __LINE__, I_OPTION_VALUE_MODIFIED, p->name, value);
			}
		} catch(std::exception &e) {
			_printException(__FILE__, __LINE__, e);
		}

		return false;
	}

	const std::wstring OptionTree::getValue(const std::wstring &location) const
	{
		try
		{
			OptionElement *p = _table[hashString(location.c_str())];

			while(p) {
				if(p->name == location) {
					return p->value;
				}
				p = p->table_next;
			}
		} catch(std::exception &e) {
			_printException(__FILE__, __LINE__, e);
		}

		return std::wstring(__T(""));
	}

};
