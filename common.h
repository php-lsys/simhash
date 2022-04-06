/**
 * @author     Lonely <shan.liu@msn.com>
 * @copyright  (c) 2017 ShanLiu <shan.liu@msn.com>
 * @license    http://www.apache.org/licenses/LICENSE-2.0
 */
#ifndef PHP_ZXING_COMMON_H
#define PHP_ZXING_COMMON_H
#include "php.h"

#define SIMHASH_NS  "LsExt"

ZEND_BEGIN_ARG_INFO_EX(simhash_none_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

void simhash_update_property_bool(zval * z_object,const char *name, size_t name_length, zend_long val);
void simhash_update_property_str(zval * z_object,const char *name, size_t name_length, zend_string *val);
void simhash_update_property(zval * z_object,const char *name, size_t name_length, zval * val);
void simhash_update_property_long(zval * z_object,const char *name, size_t name_length, zend_long val);
zval * simhash_read_property(zval * z_object,const char *name, size_t name_length, zend_bool silent);
#endif
