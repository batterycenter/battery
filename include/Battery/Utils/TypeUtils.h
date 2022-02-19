#pragma once

namespace Battery {

	/// <summary>
	/// This class can be used to separate a class interface from the implementation. By wrapping a class member
	/// in this wrapper class, it does not need to be defined in the header. By using this class, you can forward declare
	/// all members in the header, but the actual definition is only in the source file. This is useful if you do not want
	/// the definition to be propagated to the end user of a library.
	/// </summary>
	template<typename T>
	class IncompleteTypeWrapper {
		T* data;
	public:
		IncompleteTypeWrapper(T* data) : data(data) {}
		~IncompleteTypeWrapper() { delete data; }
		T* operator->() { return data; }
		T* get() { return data; }

		IncompleteTypeWrapper(const IncompleteTypeWrapper&) = delete;
		IncompleteTypeWrapper& operator=(const IncompleteTypeWrapper&) = delete;
	};
}
