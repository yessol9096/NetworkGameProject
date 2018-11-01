#pragma once

template <typename T>
void SafeDelete(T& obj)
{
	if (NULL != obj)
	{
		delete obj;
		obj = NULL;
	}
}