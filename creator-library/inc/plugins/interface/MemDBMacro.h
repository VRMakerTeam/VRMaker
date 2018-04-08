#ifndef __MEMDBMARCO_H__
#define __MEMDBMARCO_H__

#define NS(x) namespace x 
//////////////////////////////////////////////////////////////////////////
// KV
//////////////////////////////////////////////////////////////////////////
#define DECLARE_KV_V(var) \
	namespace KV { \
		namespace Query { \
			QVariant var(MemDB* _memDB); \
		}\
		namespace Update { \
			void var(MemDB* _memDB, const QVariant& _value); \
		} \
		namespace Remove { \
			void var(MemDB* _memDB); \
		} \
	}


#define KV_V(var, flag) \
	namespace KVKeys { \
		static const QString& var = #var; \
	}\
	namespace KV { \
		namespace Query { \
			QVariant var(MemDB* _memDB) \
			{	\
				return _memDB->KVGet(KVKeys::var); \
			} \
		}\
		namespace Update { \
			void var(MemDB* _memDB, const QVariant& _value) \
			{ \
				_memDB->KVSet(KVKeys::var, _value); \
				if(flag) \
					MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
		namespace Remove { \
			void var(MemDB* _memDB) \
			{ \
				_memDB->RemoveKV(KVKeys::var); \
				if(flag) \
					MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
	}


#define DECLARE_KV_1(var) \
	namespace KV { \
		namespace Query { \
			QVariant var(MemDB* _memDB, const QString _1); \
		}\
		namespace Update { \
			void var(MemDB* _memDB, const QString& _1, const QVariant& _value); \
		} \
		namespace Remove { \
			void var(MemDB* _memDB, const QString& _1); \
		} \
	}


#define KV_1(var, flag) \
	namespace KVKeys { \
		static const QString& var = #var":[%1]"; \
	}\
	namespace KV { \
		namespace Query { \
			QVariant var(MemDB* _memDB, const QString _1) \
			{	\
				return _memDB->KVGet(QString(KVKeys::var).arg(_1)); \
			} \
		}\
		namespace Update { \
			void var(MemDB* _memDB, const QString& _1, const QVariant& _value) \
			{ \
				_memDB->KVSet(QString(KVKeys::var).arg(_1), _value); \
				if(flag) \
					MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
		namespace Remove { \
			void var(MemDB* _memDB, const QString& _1) \
			{ \
				_memDB->RemoveKV(QString(KVKeys::var).arg(_1)); \
				if(flag) \
					MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
	}






#define DECLARE_KV_STR(var) \
	namespace KV { \
		namespace Query { \
			QString var(MemDB* _memDB); \
		}\
		namespace Update { \
			void var(MemDB* _memDB, const QString& _value); \
		} \
		namespace Remove { \
			void var(MemDB* _memDB); \
		} \
	}


#define KV_STR(var, flag) \
	namespace KVKeys { \
		static const QString& var = #var; \
	}\
	namespace KV { \
		namespace Query { \
			QString var(MemDB* _memDB) \
			{	\
				return _memDB->KVGet(KVKeys::var).toString(); \
			} \
		}\
		namespace Update { \
			void var(MemDB* _memDB, const QString& _value) \
			{ \
				_memDB->KVSet(KVKeys::var, _value); \
				if(flag) \
					MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
		namespace Remove { \
			void var(MemDB* _memDB) \
			{ \
				_memDB->RemoveKV(KVKeys::var); \
				if(flag) \
					MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
	}


#define DECLARE_KV_STR_1(var) \
	namespace KV { \
		namespace Query { \
			QString var(MemDB* _memDB, const QString _1); \
		}\
		namespace Update { \
			void var(MemDB* _memDB, const QString& _1, const QString& _value); \
		} \
		namespace Remove { \
			void var(MemDB* _memDB, const QString& _1); \
		} \
	}


#define KV_STR_1(var, flag) \
	namespace KVKeys { \
		static const QString& var = #var":[%1]"; \
	}\
	namespace KV { \
		namespace Query { \
			QString var(MemDB* _memDB, const QString _1) \
			{	\
				return _memDB->KVGet(QString(KVKeys::var).arg(_1)).toString(); \
			} \
		}\
		namespace Update { \
			void var(MemDB* _memDB, const QString& _1, const QString& _value) \
			{ \
				_memDB->KVSet(QString(KVKeys::var).arg(_1), _value); \
				if(flag) \
					MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
		namespace Remove { \
			void var(MemDB* _memDB, const QString& _1) \
			{ \
				_memDB->RemoveKV(QString(KVKeys::var).arg(_1)); \
				if(flag) \
					MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
	}

#define DECLARE_KV_STR_2(var) \
	namespace KV { \
		namespace Query { \
			QString var(MemDB* _memDB, const QString _1, const QString _2); \
		}\
		namespace Update { \
			void var(MemDB* _memDB, const QString& _1, const QString _2, const QString& _value); \
		} \
		namespace Remove { \
			void var(MemDB* _memDB, const QString& _1, const QString _2); \
		} \
	}

#define KV_STR_2(var, flag) \
	namespace KVKeys { \
		static const QString& var = #var":[%1]:[%2]"; \
	}\
	namespace KV { \
		namespace Query { \
			QString var(MemDB* _memDB, const QString _1, const QString _2) \
			{	\
				return _memDB->KVGet(QString(KVKeys::var).arg(_1).arg(_2)).toString(); \
			} \
		}\
		namespace Update { \
			void var(MemDB* _memDB, const QString& _1, const QString _2, const QString& _value) \
			{ \
				_memDB->KVSet(QString(KVKeys::var).arg(_1).arg(_2), _value); \
				if(flag) \
					MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
		namespace Remove { \
			void var(MemDB* _memDB, const QString& _1, const QString _2) \
			{ \
				_memDB->RemoveKV(QString(KVKeys::var).arg(_1).arg(_2)); \
				if(flag) \
					MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
	}

#define DECLARE_KV_DOUBLE_1( var) \
	namespace KV { \
		namespace Query { \
			double var(MemDB* _memDB, const QString& _1); \
		}\
		namespace Update { \
			void var(MemDB* _memDB, const QString& _1, double _value); \
		} \
		namespace Remove { \
			void var(MemDB* _memDB, const QString& _1); \
		} \
	}


#define KV_DOUBLE_1(var, flag) \
	namespace KVKeys { \
		static const QString& var = #var":[%1]"; \
	}\
	namespace KV { \
		namespace Query { \
			double var(MemDB* _memDB, const QString& _1) \
			{	\
				return _memDB->KVGet(QString(KVKeys::var).arg(_1)).toDouble(); \
			} \
		}\
		namespace Update { \
			void var(MemDB* _memDB, const QString& _1, double _value) \
			{ \
				_memDB->KVSet(QString(KVKeys::var).arg(_1), _value); \
				if(flag) \
					MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
		namespace Remove { \
			void var(MemDB* _memDB, const QString& _1) \
			{ \
				_memDB->RemoveKV(QString(KVKeys::var).arg(_1)); \
				if(flag) \
					MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
	}

#define DECLARE_KV_DOUBLE_2(var) \
	namespace KV { \
		namespace Query { \
			double var(MemDB* _memDB, const QString _1, const QString _2); \
		}\
		namespace Update { \
			void var(MemDB* _memDB, const QString& _1, const QString _2, double _value); \
		} \
		namespace Remove { \
			void var(MemDB* _memDB, const QString& _1, const QString _2); \
		} \
	}

#define KV_DOUBLE_2(var, flag) \
	namespace KVKeys { \
		static const QString& var = #var":[%1]:[%2]"; \
	}\
	namespace KV { \
		namespace Query { \
			double var(MemDB* _memDB, const QString _1, const QString _2) \
			{	\
				return _memDB->KVGet(QString(KVKeys::var).arg(_1).arg(_2)).toDouble(); \
			} \
		}\
		namespace Update { \
			void var(MemDB* _memDB, const QString& _1, const QString _2, double _value) \
			{ \
				_memDB->KVSet(QString(KVKeys::var).arg(_1).arg(_2), _value); \
				if(flag) \
					MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
		namespace Remove { \
			void var(MemDB* _memDB, const QString& _1, const QString _2) \
			{ \
				_memDB->RemoveKV(QString(KVKeys::var).arg(_1).arg(_2)); \
				if(flag) \
					MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
	}

//////////////////////////////////////////////////////////////////////////
// Sets
//////////////////////////////////////////////////////////////////////////

#define DECLARE_SETS_STR(var) \
	namespace Sets { \
		namespace Query { \
			QList<QString> var(MemDB* _memDB); \
		}\
		namespace Update { \
			namespace Add { \
				void var(MemDB* _memDB, const QString& _value); \
			} \
			namespace Remove { \
				void var(MemDB* _memDB, const QString& _value); \
			} \
		} \
		namespace Remove { \
			void var(MemDB* _memDB); \
		} \
	}


#define DECLARE_SETS_STR_1(var) \
	namespace Sets { \
		namespace Query { \
			QList<QString> var(MemDB* _memDB, const QString _1); \
		}\
		namespace Update { \
			namespace Add { \
				void var(MemDB* _memDB, const QString& _1, const QString& _value); \
			} \
			namespace Remove { \
				void var(MemDB* _memDB, const QString& _1, const QString& _value); \
			} \
		} \
		namespace Remove { \
			void var(MemDB* _memDB, const QString& _1); \
		} \
	}



#define SETS_STR(var, flag) \
	namespace SetsKeys { \
		static const QString& var = #var; \
	}\
	namespace Sets { \
		namespace Query { \
			QList<QString> var(MemDB* _memDB) \
			{	\
				QList<QVariant>& list = _memDB->SetsMembers(SetsKeys::var); \
				QList<QString> result; \
				for each (QVariant var in list) \
				{ \
					result.append(var.toString()); \
				} \
				return result; \
			} \
		}\
		namespace Update { \
			namespace Add { \
				void var(MemDB* _memDB, const QString& _value) \
				{ \
					_memDB->SetsAdd(SetsKeys::var, _value); \
					if(flag) \
						MemDBUtil::SaveBucket(_memDB->GetBucket()); \
				} \
			} \
			namespace Remove { \
				void var(MemDB* _memDB, const QString& _value) \
				{ \
					_memDB->SetsRemove(SetsKeys::var, _value); \
					if(flag) \
						MemDBUtil::SaveBucket(_memDB->GetBucket()); \
				} \
			} \
		} \
		namespace Remove { \
			void var(MemDB* _memDB) \
			{ \
				_memDB->RemoveSets(SetsKeys::var); \
				if(flag) \
					MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
	}

#define SETS_STR_1(var, flag) \
	namespace SetsKeys { \
		static const QString& var = #var":[%1]"; \
	}\
	namespace Sets { \
		namespace Query { \
			QList<QString> var(MemDB* _memDB, const QString _1) \
			{	\
				QList<QVariant>& list = _memDB->SetsMembers(QString(SetsKeys::var).arg(_1)); \
				QList<QString> result; \
				for each (QVariant var in list) \
				{ \
					result.append(var.toString()); \
				} \
				return result; \
			} \
		}\
		namespace Update { \
			namespace Add { \
				void var(MemDB* _memDB, const QString& _1, const QString& _value) \
				{ \
					_memDB->SetsAdd(QString(SetsKeys::var).arg(_1), _value); \
					if(flag) \
						MemDBUtil::SaveBucket(_memDB->GetBucket()); \
				} \
			} \
			namespace Remove { \
				void var(MemDB* _memDB, const QString& _1, const QString& _value) \
				{ \
					_memDB->SetsRemove(QString(SetsKeys::var).arg(_1), _value); \
					if(flag) \
						MemDBUtil::SaveBucket(_memDB->GetBucket()); \
				} \
			} \
		} \
		namespace Remove { \
			void var(MemDB* _memDB, const QString& _1) \
			{ \
				_memDB->RemoveSets(QString(SetsKeys::var).arg(_1)); \
				if(flag) \
					MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
	}

//////////////////////////////////////////////////////////////////////////
// Hash
//////////////////////////////////////////////////////////////////////////

#define DECLARE_HASH_FIELD(var, field) \
	namespace HashFields { \
		 namespace var { \
				static const QString field = #field;\
		}; \
	}

#define DECLARE_HASH_STR_1(var) \
	namespace Hash { \
		namespace List { \
			QList<QString> var(MemDB* _memDB, const QString& _1); \
		} \
		namespace Query { \
			QString var(MemDB* _memDB, const QString& _1, const QString& _field); \
		}\
		namespace Update { \
			void var(MemDB* _memDB, const QString& _1, const QString& _field, const QVariant& _value); \
		} \
		namespace Remove { \
			void var(MemDB* _memDB, const QString& _1); \
		} \
	}


#define HASH_STR_1(var, flag) \
	namespace HashKeys { \
		static const QString& var = #var":[%1]"; \
	}\
	namespace Hash { \
		namespace List { \
			QList<QString> var(MemDB* _memDB, const QString& _1) \
			{ \
				return _memDB->HashFields(QString(HashKeys::var).arg(_1)); \
			} \
		} \
		namespace Query { \
			QString var(MemDB* _memDB, const QString& _1, const QString& _field) { \
				return _memDB->HashGet(QString(HashKeys::var).arg(_1), _field).toString(); \
			} \
		}\
		namespace Update { \
			void var(MemDB* _memDB, const QString& _1, const QString& _field, const QVariant& _value){ \
				_memDB->HashSet(QString(HashKeys::var).arg(_1), _field, _value); \
				if(flag) \
					MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
		namespace Remove { \
			void var(MemDB* _memDB, const QString& _1){ \
				_memDB->RemoveHash(QString(HashKeys::var).arg(_1)); \
				if(flag) \
					MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
	}

#define DECLARE_HASH_STR_2(var) \
	namespace Hash { \
		namespace List { \
			QList<QString> var(MemDB* _memDB, const QString& _1, const QString& _2); \
		} \
		namespace Query { \
			QString var(MemDB* _memDB, const QString& _1, const QString& _2, const QString& _field); \
		}\
		namespace Update { \
			void var(MemDB* _memDB, const QString& _1, const QString& _2, const QString& _field, const QVariant& _value); \
		} \
		namespace Remove { \
			void var(MemDB* _memDB, const QString& _1, const QString& _2); \
		} \
	}

#define HASH_STR_2(var, flag) \
	namespace HashKeys { \
		static const QString& var = #var":[%1]:[%2]"; \
	}\
	namespace Hash { \
		namespace List { \
			QList<QString> var(MemDB* _memDB, const QString& _1, const QString& _2) \
			{ \
				return _memDB->HashFields(QString(HashKeys::var).arg(_1).arg(_2)); \
			} \
		} \
		namespace Query { \
			QString var(MemDB* _memDB, const QString& _1, const QString& _2, const QString& _field) { \
				return _memDB->HashGet(QString(HashKeys::var).arg(_1).arg(_2), _field).toString(); \
			} \
		}\
		namespace Update { \
			void var(MemDB* _memDB, const QString& _1, const QString& _2, const QString& _field, const QVariant& _value){ \
				_memDB->HashSet(QString(HashKeys::var).arg(_1).arg(_2), _field, _value); \
				if(flag) \
					MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
		namespace Remove { \
			void var(MemDB* _memDB, const QString& _1, const QString& _2){ \
				_memDB->RemoveHash(QString(HashKeys::var).arg(_1).arg(_2)); \
				if(flag) \
					MemDBUtil::SaveBucket(_memDB->GetBucket()); \
			} \
		} \
	}



#endif //__MEMDBMARCO_H__