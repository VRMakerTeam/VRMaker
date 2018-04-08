#pragma once

#define STORE_TAG(x) \
	namespace Tags \
	{ \
		static const QString& x = #x; \
	}
