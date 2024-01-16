#ifndef __COMMON_H__
#define __COMMON_H__

#pragma warning( disable: 4251 ) // Avoid dll-interface warnings with STL
#pragma warning( disable: 4275 ) // Avoid dll-interface warnings with STL

#define VERSION 0.10
#define TITLE "Empire Defense"

#define _WINGDI_
#define _WINUSER_
#define _WINCON_
#define _PRSHT_H_
#define _WINSPOOL_
#define __oleidl_h__
#define _OLE2_H_
#define _INC_COMMDLG
#define _IMM_
 // AVOID Rectangle issues

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <sstream> 
#include <fstream>

#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <set>
#include <limits>
#include <map>

#include <boost/filesystem.hpp>
#include <boost/utility.hpp>
#include <boost/array.hpp>
#include <boost/format.hpp> 
//#include <boost/signal.hpp>
//#include <boost/variant.hpp>

#include <boost/pool/object_pool.hpp>

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/if.hpp>

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


#ifdef EDMODEL_EXPORTS
#define EDMODEL_API __declspec(dllexport)
#else
#define EDMODEL_API __declspec(dllimport)
#endif

#ifdef EDCONTROLLER_EXPORTS
#define EDCONTROLLER_API __declspec(dllexport)
#else
#define EDCONTROLLER_API __declspec(dllimport)
#endif

#ifdef EDVIEW_EXPORTS
#define EDVIEW_API __declspec(dllexport)
#else
#define EDVIEW_API __declspec(dllimport)
#endif

#ifdef EDSOUND_EXPORTS
#define EDSOUND_API __declspec(dllexport)
#else
#define EDSOUND_API __declspec(dllimport)
#endif

#include "memory.h"
#include "log.h"

#include "point.h"
#include "rectangle.h"

using namespace std;
using namespace boost::lambda;

float myRand(float min, float max);
char* copyText(const char* text);
char* readFile (const char* fileName);

const float tiers[4] = { 0, 1/3.0f, 2/3.0f, 1 };

class EmpireException {
protected:
	const string reason;
public:
	EmpireException(const string& reason):reason(reason){};
   ~EmpireException() {};

   const string& getFullDescription() const { 
	   return reason; 
   }
};

static int exceptionNumber = 0;
#define TEST_FOR_EXCEPTION(throw_exception_test ,msg)									\
{																						\
	const bool throw_exception = (throw_exception_test);								\
    if(throw_exception) {																\
		std::ostringstream omsg;														\
		omsg << "\n"																	\
			<< __FILE__ << ":" << __LINE__ << ":\n"										\
			<< "Throw number = " << ++exceptionNumber << "\n"							\
			<< "Throw test that evaluated to true: "#throw_exception_test << "\n"		\
			<< msg;																		\
		const string &omsgstr = omsg.str();												\
		throw EmpireException(omsgstr);													\
    }																					\
}


class EDMODEL_API  ObjectUpdator {
private:
	int timeSpent;
public:
	ObjectUpdator(int timeSpent) : timeSpent(timeSpent) {}

	template <typename T>
	bool operator()(T& obj) {
		return obj.update(timeSpent);
	}
};

struct EDMODEL_API DeleteObject 
{
	template <typename T>
	void operator()(const T* ptr) const
	{
		delete ptr;
	}
};

#endif