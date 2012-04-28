//
// File: AlignedAllocator.h
// ========================
// Aligned allocator for stl container.
//

#ifndef IDENTI3D_SRC_MATH_ALIGNEDALLOCATOR_H
#define IDENTI3D_SRC_MATH_ALIGNEDALLOCATOR_H

#include <src/identi3d/General.h>
#include <memory>
#include <stdexcept>

/*
 * NOTICE for MSVC Users:
 * ======================
 * Inorder to use AlignedAllocator correctly,
 * you should hack your <vector> file at line 870 or around,
 * changing "_Ty _Val" to "_Ty &_Val" because aligned variables
 * doesn't support to be passed by val.
 */

namespace Identi3D
{

	template<class T, size_t alignment>
	struct AlignedAllocator : std::allocator<T>
	{

		template<class U>
		struct rebind { typedef AlignedAllocator<U, alignment> other; };

		typedef std::allocator<T> base;

		typedef typename base::pointer pointer;
		typedef typename base::size_type size_type;

		pointer allocate(size_type n)
		{
			return (pointer)_aligned_malloc(n, alignment);
		}

		pointer allocate(size_type n, void const *)
		{
			return this->allocate(n);
		}

		void deallocate(pointer p, size_type n)
		{
			_aligned_free(p);
		}

	};

}

#endif // IDENTI3D_SRC_MATH_ALIGNEDALLOCATOR_H