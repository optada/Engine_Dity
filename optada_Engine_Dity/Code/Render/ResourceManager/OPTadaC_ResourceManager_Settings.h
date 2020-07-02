// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#pragma warning(disable : 4996)

// Safely release a COM object.
template<typename T>
inline void SafeRelease(T& ptr)
{
	if (ptr != nullptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}