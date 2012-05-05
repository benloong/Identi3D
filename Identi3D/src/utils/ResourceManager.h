//
// File: ResourceManager.h
// =======================
// Manages resources of application.
//

#ifndef IDENTI3D_SRC_UTILS_RESOURCEMANAGER_H
#define IDENTI3D_SRC_UTILS_RESOURCEMANAGER_H

#include <src/identi3d/General.h>
#include <src/utils/OptionTree.h>

namespace Identi3D
{

	struct ResourceData
	{
		UINT res_id;
		std::wstring name;
		size_t size;
		BYTE *data;
		UINT ref_count;
	};

	class Resource
	{
		friend class ResourcePool;
	public:
		
		/*
		 * Constructor: Do nothing.
		 */
		Resource(ResourceData *data)
		{
			_data = data;
			_data->ref_count++;
		}

		/*
		 * Copy constructor.
		 */
		Resource(Resource &res)
		{
			_data = res._data;
			_data->ref_count++;
		}

		/*
		 * Destructor.
		 */
		~Resource(void)
		{
			_data->ref_count--;
		}

		/*
		 * Assignment operator.
		 */
		Resource &operator=(Resource &rhs)
		{
			_data = rhs._data;
			_data->ref_count++;
		}

		/*
		 * Get actual data.
		 */
		inline const BYTE *getData(void) const
		{
			return _data->data;
		}

		/*
		 * Get data size.
		 */
		inline size_t getSize(void) const
		{
			return _data->size;
		}

		/*
		 * Get resource name.
		 */
		inline const std::wstring &getName(void) const
		{
			return _data->name;
		}

		/*
		 * Get resource ID.
		 */
		inline UINT getID(void) const
		{
			return _data->res_id;
		}

	private:
		explicit Resource(void);

	private:
		ResourceData *_data;
	};

	typedef std::vector<ResourceData *> ResourceList;

	class __declspec(dllexport) ResourcePool
	{
	public:

		/*
		 * Constructor: Do nothing.
		 */
		ResourcePool(void) {};

		/*
		 * Deconstructor.
		 */
		~ResourcePool(void);

		/*
		 * Create a resource.
		 */
		Resource *alloc(const std::wstring &path, const std::wstring &name);

		/*
		 * Force to release a resource.
		 */
		void free(UINT id);

		/*
		 * Free useless resources.
		 */
		void compress(void);

	private:
		ResourceList _list;
	};

	class __declspec(dllexport) ResourceManager
	{
	public:

		/*
		 * Constructor: Do nothing.
		 */
		ResourceManager(void) {} ;

		/*
		 * Destructor.
		 */
		~ResourceManager(void) {} ;

		/*
		 * Add a resource search location.
		 */
		bool addLocation(const std::wstring &location);

		/*
		 * Read group in options: System.ResourceLocation.
		 */
		bool addLocationGroup(const OptionTree &tree);

		/*
		 * Remove a location.
		 */
		void removeLocation(const std::wstring &location);

		/*
		 * Load a resource.
		 */
		Resource *load(const std::wstring &name);

		/*
		 * Release a resource.
		 */
		inline void unload(Resource *res)
		{
			_pool.free(res->getID());
		}

		/*
		 * Free useless resources.
		 */
		inline void compressPool(void)
		{
			_pool.compress();
		}

	private:
		std::vector<std::wstring> _location;
		ResourcePool _pool;
	};

}
#endif // IDENTI3D_SRC_UTILS_RESOURCEMANAGER_H