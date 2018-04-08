
#pragma once


#define DECLARE_RUNTIME_MODEL(ns) \
	namespace ns{ \
		namespace Runtime{ \
				static void InitializeModel(); \
				static MemDB* GetMemDB(); \
		}\
	}

#define DECLARE_RUNTIME_KV(ns, clazz, key, type) \
	namespace ns { \
		namespace Runtime { \
			namespace KV { \
				namespace Query { \
					static type key();
				}\
				namespace Update { \
				} \
			} \
		} \
	}
