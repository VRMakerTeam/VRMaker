/********************************************************************
	Copyright (C) 2005 by Easlee
	@date:	    2010-1-23   22:37
	@file: 		QtLogger.h
	@author:    easlee
*********************************************************************/
#ifndef __LOGX_H__
#define __LOGX_H__

#include <QString>
#include <QDebug>

#ifdef LOGX_LOG4CXX
//#include "log4cxx/logger.h"
//#include "log4cxx/propertyconfigurator.h"
#endif

//------------------------------------------------------------------------------
// Thread safe
//------------------------------------------------------------------------------
#ifdef _LOGX_THREAD_SAFE_
#include <Windows.h>
static CRITICAL_SECTION LogXFileCS;
#endif

static void LockLogX()
{
#ifdef _LOGX_THREAD_SAFE_
	static bool Inited = false;
	if ( !Inited )
	{
		InitializeCriticalSection(&LogXFileCS);
		Inited = true;
	}
	EnterCriticalSection( &LogXFileCS );
#endif
}

static void UnLockLogX()
{
#ifdef _LOGX_THREAD_SAFE_
	LeaveCriticalSection( &LogXFileCS );
#endif
}
//------------------------------------------------------------------------------
// level
//------------------------------------------------------------------------------
static const unsigned int XL_ALL =			0;
static const unsigned int XL_HIGH_ROOT =	0xF0000000;
static const unsigned int XL_MEDIMU_ROOT =	0xE0000000;
static const unsigned int XL_LOW_ROOT =		0XD0000000;
/// custom define hierarchy
#define XH_CUSTOM(x,y)	\
	static char* XC_##x = #x; \
	static const unsigned int XL_HIGH_##x =		XL_HIGH_##y>>1; \
	static const unsigned int XL_MEDIMU_##x =	XL_MEDIMU_##y>>1; \
	static const unsigned int XL_LOW_##x =		XL_LOW_##y>>1;


//------------------------------------------------------------------------------
// class LogX
//------------------------------------------------------------------------------
///  
/// \class LogX
class LogX
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  .Methods
public:
	static LogX& GetSingleton()
	{
		static LogX singleton;
		return singleton;
	}
	
	/// \Param [in]configFile:	configure file
	void Configure(const QString& configFile)
	{
		#if defined( LOGX_LOG4CXX )
			//log4cxx::PropertyConfigurator::configure(configFile.toStdString());
		#endif
	}
	
	///@name write log method
	///@{

	/// write a debug log
	/// \Param [in]categary: which categary the log belong to
	/// \Param [in]file:	the file which call the LogX, it maybe include the directory path
	/// \Param [in]line:	the line number in the file which call the LogX 
	/// \Param [in]method:	the method which call the LogX 
	/// \Param [in]message:	will record the message text
	void WriteDebug(const std::string& categary, const unsigned int level, const std::string& file, const int line, const std::string& method, const QString& message)
	{
		#if defined( LOGX_LOG4CXX )
			//log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger(categary);
			//LOG4CXX_DEBUG(logger, m_activityID<<"|"<<getFileName(file)<<":"<<line<<"|"<<getClassName(method)<<"|"<<getMethodName(method)<<"| - "<<message.toStdString());
		#else
			qDebug() << QString("[DEBUG | %1 | %2 | %3] : %4").arg(categary.c_str()).arg(getClassName(method).c_str()).arg(getMethodName(method).c_str()).arg(message);
		#endif
	}

	/// write a info log
	/// \Param [in]categary: which categary the log belong to
	/// \Param [in]file:	the file which call the LogX, it maybe include the directory path
	/// \Param [in]line:	the line number in the file which call the LogX 
	/// \Param [in]method:	the method which call the LogX 
	/// \Param [in]message:	will record the message text
	void WriteInfo(const std::string& categary, const unsigned int level, const std::string& file, const int line, const std::string& method, const QString& message)
	{
		#if defined( LOGX_LOG4CXX )
			//log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger(categary);
			//LOG4CXX_INFO(logger, m_activityID<<"|"<<getFileName(file)<<":"<<line<<"|"<<getClassName(method)<<"|"<<getMethodName(method)<<"| - "<<message.toStdString());
		#else
			qDebug() << QString("[INFO | %1 | %2 | %3] : %4").arg(categary.c_str()).arg(getClassName(method).c_str()).arg(getMethodName(method).c_str()).arg(message);
		#endif
	}

	/// write a warn log
	/// \Param [in]categary: which categary the log belong to
	/// \Param [in]file:	the file which call the LogX, it maybe include the directory path
	/// \Param [in]line:	the line number in the file which call the LogX 
	/// \Param [in]method:	the method which call the LogX 
	/// \Param [in]message:	will record the message text
	void WriteWarn(const std::string& categary, const unsigned int level, const std::string& file, const int line, const std::string& method, const QString& message)
	{
		#if defined( LOGX_LOG4CXX )
			//log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger(categary);
			//LOG4CXX_WARN(logger, m_activityID<<"|"<<getFileName(file)<<":"<<line<<"|"<<getClassName(method)<<"|"<<getMethodName(method)<<"| - "<<message.toStdString());
		#else
			qDebug() << QString("[WARN | %1 | %2 | %3] : %4").arg(categary.c_str()).arg(getClassName(method).c_str()).arg(getMethodName(method).c_str()).arg(message);
		#endif
	}

	/// write a error log
	/// \Param [in]categary: which categary the log belong to
	/// \Param [in]file:	the file which call the LogX, it maybe include the directory path
	/// \Param [in]line:	the line number in the file which call the LogX 
	/// \Param [in]method:	the method which call the LogX 
	/// \Param [in]message:	will record the message text
	void WriteError(const std::string& categary, const unsigned int level, const std::string& file, const int line, const std::string& method, const QString& message)
	{
		#if defined( LOGX_LOG4CXX )
				//log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger(categary);
				//LOG4CXX_ERROR(logger, m_activityID<<"|"<<getFileName(file)<<":"<<line<<"|"<<getClassName(method)<<"|"<<getMethodName(method)<<"| - "<<message.toStdString());
		#else
				qDebug() << QString("[ERROR | %1 | %2 | %3] : %4").arg(categary.c_str()).arg(getClassName(method).c_str()).arg(getMethodName(method).c_str()).arg(message);
		#endif
	}

	/// write a fatal log
	/// \Param [in]categary: which categary the log belong to
	/// \Param [in]file:	the file which call the LogX, it maybe include the directory path
	/// \Param [in]line:	the line number in the file which call the LogX 
	/// \Param [in]method:	the method which call the LogX 
	/// \Param [in]message:	will record the message text
	void WriteFatal(const std::string& categary, const unsigned int level, const std::string& file, const int line, const std::string& method, const QString& message)
	{
		#if defined( LOGX_LOG4CXX )
				//log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger(categary);
				//LOG4CXX_FATAL(logger, m_activityID<<"|"<<getFileName(file)<<":"<<line<<"|"<<getClassName(method)<<"|"<<getMethodName(method)<<"| - "<<message.toStdString());
		#else
				qDebug() << QString("[FATAL | %1 | %2 | %3] : %4").arg(categary.c_str()).arg(getClassName(method).c_str()).arg(getMethodName(method).c_str()).arg(message);
		#endif
	}

	///@name activity method
	///@{

	/// use a integer set the activity id
	/// \Param [in]activityID:	activity id
	void SetActivityID(unsigned int activityID)
	{
		m_activityID = activityID;
	}

	/// Create a random activity id	
	void CreateActivityID()
	{

	}

	/// reset the activity id to zero
	void ResetActivityID()
	{
		m_activityID = 0;
	}
	///}@
protected:
	LogX()
	{
		m_activityID = 0;
	}
private:
	std::string getMethodName(const std::string methodStr) const
	{
		std::string tmp(methodStr);
		size_t colonPos = tmp.find("::");
		if (colonPos != std::string::npos) {
			tmp.erase(0, colonPos + 2);
		} else {
			size_t spacePos = tmp.find(' ');
			if (spacePos != std::string::npos) {
				tmp.erase(0, spacePos + 1);
			}
		}
		size_t parenPos = tmp.find('(');
		if (parenPos != std::string::npos) {
			tmp.erase(parenPos);
		}
		return tmp;
	}

	std::string getClassName(const std::string classStr) const 
	{
		std::string tmp(classStr);
		size_t colonPos = tmp.find("::");
		if (colonPos != std::string::npos) {
			tmp.erase(colonPos);
			size_t spacePos = tmp.find_last_of(' ');
			if (spacePos != std::string::npos) {
				tmp.erase(0, spacePos + 1);
			}
			return tmp;
		}
		tmp.erase(0, tmp.length() );
		return tmp;
	}

	std::string getFileName(const std::string fileStr) const 
	{
		std::string tmp(fileStr);
		size_t colonPos = tmp.find("\\");
		while (colonPos != std::string::npos)
		{
			tmp.erase(0, colonPos + 1);
			colonPos = tmp.find("\\");
		}
		return tmp;
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  .Fields
public:
protected:
private:
	unsigned int m_activityID;
};//class LogX


//------------------------------------------------------------------------------
// Identify
//------------------------------------------------------------------------------
#define LOGX_CONFIGURE(file) \
	LockLogX(); \
	LogX::GetSingleton().Configure(file); \
	UnLockLogX();

#define LOGX_DEBUG(categary, level, message){ \
	LockLogX(); \
	LogX::GetSingleton().WriteDebug(categary, level, __FILE__, __LINE__, __FUNCTION__, message); \
	UnLockLogX();}

#define LOGX_INFO(categary, level, message) {\
	LockLogX(); \
	LogX::GetSingleton().WriteInfo(categary, level, __FILE__, __LINE__, __FUNCTION__, message); \
	UnLockLogX();}

#define LOGX_WARN(categary, level, message) {\
	LockLogX(); \
	LogX::GetSingleton().WriteWarn(categary, level, __FILE__, __LINE__, __FUNCTION__, message); \
	UnLockLogX();}

#define LOGX_ERROR(categary, level, message) {\
	LockLogX(); \
	LogX::GetSingleton().WriteError(categary, level, __FILE__, __LINE__, __FUNCTION__, message); \
	UnLockLogX();}

#define LOGX_FATAL(categary, level, message) {\
	LockLogX(); \
	LogX::GetSingleton().WriteFatal(categary, level, __FILE__, __LINE__, __FUNCTION__, message); \
	UnLockLogX();}

#endif //__LOGX_H__