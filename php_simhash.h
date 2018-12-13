/**
 * lsys core
 * @author     Lonely <shan.liu@msn.com>
 * @copyright  (c) 2017 Lonely <shan.liu@msn.com>
 * @license    http://www.apache.org/licenses/LICENSE-2.0
 */

/* $Id$ */

#ifndef PHP_SIMHASH_H
#define PHP_SIMHASH_H

extern zend_module_entry simhash_module_entry;
#define phpext_simhash_ptr &simhash_module_entry

#define PHP_SIMHASH_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_SIMHASH_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_SIMHASH_API __attribute__ ((visibility("default")))
#else
#	define PHP_SIMHASH_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#define SIMHASH_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(simhash, v)

#if defined(ZTS) && defined(COMPILE_DL_SIMHASH)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif
