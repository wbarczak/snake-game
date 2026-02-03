#pragma once

#define CLASSY_ENUM(name, underlying, ...)			\
	namespace __classy_##name {						\
		enum name : underlying {__VA_ARGS__};		\
	}												\
	using name = __classy_##name::name;