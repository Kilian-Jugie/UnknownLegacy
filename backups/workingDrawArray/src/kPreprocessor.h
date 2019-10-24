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
 
 Each module define K_H_[MODULE] if present. Each module
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

#define K_H_UTILS

#define getLastOf(arr) (arr)[(arr).size()-1]

#define KCAT(x, y) x##y
#define KSTR(x) #x
#define KTOKSTR(x) KCAT(x, _k)
#define KCOMMA() ,
#define KDOT() .
#define KEMPTY()
#define KARGEND() 0

#define KDEFER1(m) m KEMPTY()
#define KDEFER2(m) m KEMPTY KEMPTY()()
#define KDEFER3(m) m KEMPTY KEMPTY KEMPTY()()()
#define KDEFER4(m) m KEMPTY KEMPTY KEMPTY KEMPTY()()()()

#define EVAL(...) EVAL1024(__VA_ARGS__)
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

#define KU_IF_ELSE(c) _IF_ ## c
#define _IF_1(...) __VA_ARGS__ _IF_1_ELSE
#define _IF_0(...) _IF_0_ELSE

#define _IF_1_ELSE(...)
#define _IF_0_ELSE(...) __VA_ARGS__

#define KU_FIRST(a,...) a
#define KU_SECOND(a,b,...) b
#define KU_THIRD(a,b,c,...) c

#define KU_ISOLATEONE(a,...) __VA_ARGS__
#define KU_ISOLATETWO(a,b,...) __VA_ARGS__
#define KU_ISOLATETREE(a,b,c,...) __VA_ARGS__

#define _KU_IS_PROBE(...) EVAL2(KU_SECOND(__VA_ARGS__, 0))
#define _KU_PROBE() ~,1 //'~' is arbitrary

#define KU_NOT(x) _KU_IS_PROBE(KCAT(_KU_NOT_,x))
#define _KU_NOT_0 _KU_PROBE()

#define KFAILED(x) x!=0

#define K_VER_H_UTILS 1,0,06

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
#define K_H_VERSION

#define KVER_MAJ(v) EVAL(KU_FIRST(v))
#define _KVER_MAJ(mj,mi,ed) mj
#define KVER_MIN(v) EVAL(KU_SECOND(v))
#define _KVER_MIN(mj,mi,ed) mi
#define KVER_EDIT(v) EVAL(KU_THIRD(v))
#define _KVER_EDIT(mj,mi,ed) ed

#define _KVER_NUM_F(...) KCAT(_KVER_MAJ(__VA_ARGS__), _KVER_MIN(__VA_ARGS__))
#define _KVER_NUM_S(...) KCAT(_KVER_EDIT(__VA_ARGS__),) //Cleaner : eval ?
#define KVER_NUM(v) EVAL(KCAT(_KVER_NUM_F(v),_KVER_NUM_S(v)))

#define _KVER_STR_F1(...) EVAL2(KDEFER1(KCAT)(KDOT(),_KVER_MIN(__VA_ARGS__)))
#define _KVER_STR_F(...) KCAT(_KVER_MAJ(__VA_ARGS__),_KVER_STR_F1(__VA_ARGS__))
#define _KVER_STR_S(...) KCAT(KDOT(),_KVER_EDIT(__VA_ARGS__))
#define KVER_STR(v) EVAL1(KDEFER1(KSTR)(EVAL(KCAT(_KVER_STR_F(v), _KVER_STR_S(v)))))

#define K_VER_H_VERSION 2,0,02


#define K_H_LOOP
#define K_LOOP_H_VERSION 1,0,0

#define _KFOREACH(m, ...)  \
		KDEFER1(KU_IF_ELSE)(KU_NOT(KU_FIRST(__VA_ARGS__)))()(\
		EVAL1(KDEFER1(m)(KU_FIRST(__VA_ARGS__))) \
		KDEFER1(__KFOREACH)(m,KU_ISOLATEONE(__VA_ARGS__),KARGEND()))

#define __KFOREACH(m, ...) _KFOREACH(m,__VA_ARGS__)

#define KFOREACH(macro, args, ...) EVAL(_KFOREACH(macro, args, __VA_ARGS__, KARGEND())) 

#define K_VER_H_ANNOTE 1,0,0
#ifndef K_AN_DEV_KLIB_DLEVEL
#define K_AN_DEV_KLIB_DLEVEL 0
#endif // !K_AN_DEV_KLIB_DLEVEL


#define __k_an(t, c, ...) __pragma(message(__FILE__"("EVAL2(KDEFER1(KSTR)(__LINE__))"): "#t" "#c": "__VA_ARGS__))
#define _k_e_an(c, ...) __k_an(error, c, __VA_ARGS__)
#define _k_w_an(c, ...) __k_an(warning, c, __VA_ARGS__)
#define _k_i_an(c, ...) __k_an(message,  , __VA_ARGS__)
#define _k_an_add_p(p, an, c, ...)  EVAL2(KDEFER2(an)(c, p __VA_ARGS__))
#ifdef K_AN_DEV_KLIB
#define __k_kd_an_wa_prefix() "To work around: "
#define _k_an_kd_wwa(...)  _k_an_add_p(__k_kd_an_wa_prefix(), _k_w_an, WA, __VA_ARGS__)
#define _k_an_kd_ewa(...) _k_an_add_p(__k_kd_an_wa_prefix(), _k_e_an, WA, __VA_ARGS__)
#define _k_an_kd_iwa(...) _k_an_add_p(__k_kd_an_wa_prefix(), _k_i_an, WA, __VA_ARGS__)

#if K_AN_DEV_KLIB_DLEVEL > 0
#define _k_an_kd_dtodo(...) _k_an_add_p(" TODO: ",_k_i_an,TODO,__VA_ARGS__)
#else
#define _k_an_kd_dtodo(c,...)
#endif // K_AN_DEV_KLIB_DLEVEL > 0
#else
#define _k_an_kd_wwa(...)
#define _k_an_kd_ewa(...)
#define _k_an_kd_iwa(...)
#define _k_an_kd_dtodo(...)
#endif


