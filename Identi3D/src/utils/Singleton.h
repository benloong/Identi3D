//
// File: Singleton.h
// =================
// A implementation of Singleton Pattern from Boost libraries.
//

#ifndef IDENTI3D_SRC_UTILS_SINGLETON_H
#define IDENTI3D_SRC_UTILS_SINGLETON_H

#include <src/identi3d/Identi3D.h>

namespace Identi3D
{

	template <typename T>
	struct Singleton{
		struct object_creator{
			object_creator(){ Singleton<T>::getInstance(); }
			inline void do_nothing()const {}
		};

		static object_creator create_object;

	public:
		typedef T object_type;
		static object_type& getInstance(){
			static object_type obj;
			create_object.do_nothing();
			return obj;
		} 
	};

	template <typename T>
	typename Singleton<T>::object_creator
	Singleton<T>::create_object;

};

#endif // IDENTI3D_SRC_UTILS_SINGLETON_H
