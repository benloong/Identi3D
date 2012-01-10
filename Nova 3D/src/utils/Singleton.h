//
// File: Singleton.h
// =================
// A implementation of Singleton Pattern.
//
// Copyright (c) Sparkfire, 2011 - 2012.
//

#ifndef NOVA3D_UTILS_SINGLETON_H
#define NOVA3D_UTILS_SINGLETON_H

#include "src/nova-3d/NovaGeneral.h"

namespace Nova3D
{
	template <typename T>
	class Singleton
	{
	public:
		static T &getInstance(void)
		{
			static T _instance;
			return _instance;
		}
	};

};

#endif
