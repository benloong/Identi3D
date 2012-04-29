//
// File: ResourceManager.cc
// ========================
//

#include <src/utils/ResourceManager.h>

namespace Identi3D
{

	ResourcePool::~ResourcePool(void)
	{
		ResourceList::iterator iter;

		for(iter = _list.begin(); iter != _list.end(); ++iter) {
			delete (*iter)->data;
			delete (*iter);
		}
	}

	Resource *ResourcePool::alloc(const std::wstring &path, const std::wstring &name)
	{
		ResourceList::iterator iter;
		for(iter = _list.begin(); iter != _list.end(); ++iter) {
			if((*iter)->name == name) {
				return ntnew Resource(*iter);
			}
			if((*iter)->ref_count == 0) {
				delete (*iter)->data;
				delete *iter;
			}
		}

		ResourceData *data = ntnew ResourceData;
		if(data == NULL) return NULL;

		FILE *fp;
		_wfopen_s(&fp, path.c_str(), L"rb");
		if(fp == NULL) {
			delete data;
			return NULL;
		}

		fseek(fp, 0, SEEK_END);
		data->size = ftell(fp);
		data->data = ntnew BYTE[data->size + 1];
		if(data->data == NULL) {
			delete data;
			return NULL;
		}

		fseek(fp, 0, SEEK_SET);
		if(data->size != fread(data->data, sizeof(BYTE), data->size, fp)) {
			delete data->data;
			delete data;
			return NULL;
		}

		Resource *res = ntnew Resource(data);
		if(res == NULL) {
			delete data->data;
			delete data;
			return NULL;
		}
		_list.push_back(data);

		return res;
	}

	void ResourcePool::free(UINT id)
	{
		if(id < _list.size()) {
			delete _list[id]->data;
			delete _list[id];
			_list.erase(_list.begin() + id);
		}
	}

	void ResourcePool::compress(void)
	{
		ResourceList::iterator iter;
		for(iter = _list.begin(); iter != _list.end(); ++iter) {
			if((*iter)->ref_count == 0) {
				delete (*iter)->data;
				delete *iter;
			}
		}
	}

	bool ResourceManager::addLocation(const std::wstring &location)
	{
		if(_waccess(location.c_str(), 0) != 0) return false;
		_location.push_back(location);
		return true;
	}

	bool ResourceManager::addLocationGroup(const OptionTree &tree)
	{
		std::wstring path = tree.getValue(L"System.ResourceLocation");
		if(path.length() == 0) return true;

		size_t pos, next;
		pos = 0;
		while(true) {
			next = path.find(L';', pos);
			addLocation(path.substr(pos, next - pos));
			if(next == std::wstring::npos) break;
			else pos = next + 1;
		}
		return true;
	}

	Resource *ResourceManager::load(const std::wstring &name)
	{
		std::wstring tmp;
		std::vector<std::wstring>::iterator iter;
		for(iter = _location.begin(); iter != _location.end(); ++iter) {
			tmp = *iter;
			tmp.push_back(L'\\');
			tmp += name;
			if(_waccess(tmp.c_str(), 4) == 0)
				return _pool.alloc(tmp, name);
		}
		return NULL;
	}

}
