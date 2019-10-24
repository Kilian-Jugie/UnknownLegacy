#pragma once

/*
----------									  ----------
 **********		kLib preprocessor library	 **********
----------									  ----------
 Presentation :
 kLib preprocessor library (KPL) is a library that 
 provide stuff for various usage only with C 
 preprocessor (so it's C compatible). 

 Note :
 This library is designed to work with MSVC compilers
 and may not work with other compiler.

 Modules :
 This library is composed of differents modules which
 are :
	- UTILS
	- VERSION
 
 Each module define K_PP_[MODULE] if present. Each module
 also have a KVER-like version which can be handled with
 VERSION module (c.f VERSION module). The version of
 each module is defined by K_VER_H_[MODULE].

 Programming rules :
 If a macro starts with an underscore ('_') that mean 
 that the macro is not designed to be used outside of 
 the library (or use only if you know what you are 
 doing).

 */

/*
 ----------									  ----------
  **********   general preprocessor defines  **********
 ----------									  ----------
*/

//Activate klib developpment annotations
#define K_AN_DEV_KLIB
//Configure klib developpment annotations debug level
#define K_AN_DEV_KLIB_DLEVEL 1

 /*
 ----------									  ----------
  **********	kLib preprocessor utils		 **********
 ----------									  ----------

 Presentation :
 This module provide some macros which can help in many
 scenarios including in the KPL iteself. 

 Notable macros :
	EVAL(...) : force compiler to make multiple passes
		on an expression.
	KU_IF_ELSE(condition) : provide an if-else like
		implementation. Patern :
		KU_IF_ELSE(condition)(if true)(if false)
	KU_(FIRST,SECOND,THIRD) : give the correspondant
		argument number in a VA_ARGS context.
	KU_NOT(x) : perform an logical NOT operation
		(return 1 for x=0 and 0 for x=1)
 */

#define K_PP_UTILS

#define K_GETLASTOF(arr) (arr)[(arr).size()-1]

#define K_CAT(x, y) x##y
#define K_STR(x) #x
#define K_COMMA() ,
#define K_DOT() .
#define K_EMPTY()
#define K_ARGEND() 0

#define K_DEFER_1(m) m K_EMPTY()
#define K_DEFER_2(m) m K_EMPTY K_EMPTY()()
#define K_DEFER_3(m) m K_EMPTY K_EMPTY K_EMPTY()()()
#define K_DEFER_4(m) m K_EMPTY K_EMPTY K_EMPTY K_EMPTY()()()()

#define K_EVAL(...) EVAL1024(__VA_ARGS__)
#define EVAL1024(...) EVAL512(EVAL512(__VA_ARGS__))
#define EVAL512(...) EVAL256(EVAL256(__VA_ARGS__))
#define EVAL256(...) EVAL128(EVAL128(__VA_ARGS__))
#define EVAL128(...) EVAL64(EVAL64(__VA_ARGS__))
#define EVAL64(...) EVAL32(EVAL32(__VA_ARGS__))
#define EVAL32(...) EVAL16(EVAL16(__VA_ARGS__))
#define EVAL16(...) EVAL8(EVAL8(__VA_ARGS__))
#define EVAL8(...) EVAL4(EVAL4(__VA_ARGS__))
#define EVAL4(...) EVAL2(EVAL2(__VA_ARGS__))
#define EVAL2(...) EVAL1(EVAL1(__VA_ARGS__))
#define EVAL1(...) __VA_ARGS__

#define K_IF_ELSE(c) _IF_ ## c
#define _IF_1(...) __VA_ARGS__ _IF_1_ELSE
#define _IF_0(...) _IF_0_ELSE

#define _IF_1_ELSE(...)
#define _IF_0_ELSE(...) __VA_ARGS__

#define K_FIRST(a,...) a
#define K_SECOND(a,b,...) b
#define K_THIRD(a,b,c,...) c

#define K_ISOLATEONE(a,...) __VA_ARGS__
#define K_ISOLATETWO(a,b,...) __VA_ARGS__
#define K_ISOLATETREE(a,b,c,...) __VA_ARGS__

#define _K_IS_PROBE(...) EVAL2(K_SECOND(__VA_ARGS__, 0))
#define _K_PROBE() ~,1 //'~' is arbitrary

#define K_NOT(x) _K_IS_PROBE(K_CAT(_K_NOT_,x))
#define _K_NOT_0 _K_PROBE()

#define K_VER_PP_UTILS 1,0,06

/*
----------									  ----------
 ********** kLib preprocessor version system **********
----------									  ----------

 How to use :
 Create a macro which define a version and replace dots
 in version by comma like the following example :
 #define EXAMPLE_VERSION 1,2,3 //Version 1.2.3

 Here we define in this version the number '1' as the
 major version number, '2' as the minor version number
 and '3' as the edit version number. Each of them 
 haven't any numerical limit.

 We can get each of them with the following macros :
 KVER_MAJ(EXAMPLE_VERSION) will be replaced by 1
 KVER_MIN(EXAMPLE_VERSION) will be replaced by 2
 KVER_EDIT(EXAMPLE_VERSION) will be replaced by 3

 For version comparison, a macro also can concatenate
 all numbers of a version to create a numerical form.
 KVER_NUM(EXAMPLE_VERSION) will be replaced by 123

 For version display, a macro can create a c string
 with version number separated by dot.
 KVER_STR(EXAMPLE_VERSION) will become "1.2.3"
 */
#define K_PP_VERSION

#define KVER_MAJ(v) K_EVAL(K_FIRST(v))
#define _KVER_MAJ(mj,mi,ed) mj
#define KVER_MIN(v) K_EVAL(K_SECOND(v))
#define _KVER_MIN(mj,mi,ed) mi
#define KVER_EDIT(v) K_EVAL(K_THIRD(v))
#define _KVER_EDIT(mj,mi,ed) ed

#define _KVER_NUM_F(...) K_CAT(_KVER_MAJ(__VA_ARGS__), _KVER_MIN(__VA_ARGS__))
#define _KVER_NUM_S(...) K_CAT(_KVER_EDIT(__VA_ARGS__),) //Cleaner : eval ?
#define KVER_NUM(v) K_EVAL(K_CAT(_KVER_NUM_F(v),_KVER_NUM_S(v)))

#define _KVER_STR_F1(...) EVAL2(K_DEFER_1(K_CAT)(K_DOT(),_KVER_MIN(__VA_ARGS__)))
#define _KVER_STR_F(...) K_CAT(_KVER_MAJ(__VA_ARGS__),_KVER_STR_F1(__VA_ARGS__))
#define _KVER_STR_S(...) K_CAT(K_DOT(),_KVER_EDIT(__VA_ARGS__))
#define KVER_STR(v) EVAL1(K_DEFER_1(K_STR)(K_EVAL(K_CAT(_KVER_STR_F(v), _KVER_STR_S(v)))))

#define K_VER_PP_VERSION 2,0,02

 /*
 ----------									  ----------
  **********     kLib annotation system      **********
 ----------									  ----------
 TODO: Doc
 */

#define K_VER_H_ANNOTE 1,0,1
#ifndef K_AN_DEV_KLIB_DLEVEL
#define K_AN_DEV_KLIB_DLEVEL 0
#endif // !K_AN_DEV_KLIB_DLEVEL

#define _K_AN_BASE(t, c, ...) __pragma(message(__FILE__"("EVAL2(K_DEFER_1(K_STR)(__LINE__))"): "#t" "#c": "__VA_ARGS__))
#define _K_AN_ERROR(c, ...) _K_AN_BASE(error, c, __VA_ARGS__)
#define _K_AN_WARNING(c, ...) _K_AN_BASE(warning, c, __VA_ARGS__)
#define _K_AN_MESSAGE(c, ...) _K_AN_BASE(message,  , __VA_ARGS__)
#define _K_AN_ADD_PREFIX(p, an, c, ...)  EVAL2(K_DEFER_2(an)(c, p __VA_ARGS__))
#ifdef K_AN_DEV_KLIB
//TODO: Rework this from scratch with new prefix system
#define _K_AN_KLIBDEV_WORKAROUND_PREFIX() "To work around: "
#define _K_AN_KLIBDEV_WARNWA(...)  _K_AN_ADD_PREFIX(_K_AN_KLIBDEV_WORKAROUND_PREFIX(), _K_AN_WARNING, WA, __VA_ARGS__)
#define _K_AN_KLIBDEV_ERRWA(...) _K_AN_ADD_PREFIX(_K_AN_KLIBDEV_WORKAROUND_PREFIX(), _K_AN_ERROR, WA, __VA_ARGS__)
#define _K_AN_KLIBDEV_MSGWA(...) _K_AN_ADD_PREFIX(_K_AN_KLIBDEV_WORKAROUND_PREFIX(), _K_AN_MESSAGE, WA, __VA_ARGS__)

#if K_AN_DEV_KLIB_DLEVEL > 0
#define _K_AN_KLIBDEV_TODO(...) _K_AN_ADD_PREFIX(" TODO: ",_K_AN_MESSAGE,TODO,__VA_ARGS__)
#else
#define _K_AN_KLIBDEV_TODO(c,...)
#endif // K_AN_DEV_KLIB_DLEVEL > 0
#else
#define _K_AN_KLIBDEV_WARNWA(...)
#define _K_AN_KLIBDEV_ERRWA(...)
#define _K_AN_KLIBDEV_MSGWA(...)
#define _K_AN_KLIBDEV_TODO(...)
#endif


