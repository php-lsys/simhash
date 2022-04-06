/**
 * @author     Lonely <shan.liu@msn.com>
 * @copyright  (c) 2017 ShanLiu <shan.liu@msn.com>
 * @license    http://www.apache.org/licenses/LICENSE-2.0
 */
#include "zend.h"
#include "php.h"
#include "zend_API.h"
#include "common.h"


void simhash_update_property_bool(zval * z_object,const char *name, size_t name_length, zend_long val){
#if PHP_VERSION_ID < 80000
    zend_update_property_bool(Z_OBJCE_P(z_object), z_object,name,name_length,val);
#else
    zend_update_property_bool(Z_OBJCE_P(z_object), Z_OBJ_P(z_object),name,name_length,val);
#endif
}

void simhash_update_property_str(zval * z_object,const char *name, size_t name_length, zend_string *val){
#if PHP_VERSION_ID < 80000
    zend_update_property_str(Z_OBJCE_P(z_object), z_object,name,name_length,val);
#else
    zend_update_property_str(Z_OBJCE_P(z_object), Z_OBJ_P(z_object),name,name_length,val);
#endif
}


void simhash_update_property(zval * z_object,const char *name, size_t name_length, zval * val){
#if PHP_VERSION_ID < 80000
    zend_update_property(Z_OBJCE_P(z_object), z_object,name,name_length,val);
#else
    zend_update_property(Z_OBJCE_P(z_object), Z_OBJ_P(z_object),name,name_length,val);
#endif
}

void simhash_update_property_long(zval * z_object,const char *name, size_t name_length, zend_long val){
#if PHP_VERSION_ID < 80000
    zend_update_property_long(Z_OBJCE_P(z_object), z_object, name,name_length, val);
#else
    zend_update_property_long(Z_OBJCE_P(z_object), Z_OBJ_P(z_object),name,name_length,val);
#endif
}

zval * simhash_read_property(zval * z_object,const char *name, size_t name_length, zend_bool silent){
#if PHP_VERSION_ID < 80000
    return zend_read_property(Z_OBJCE_P(z_object), z_object, name,name_length, silent, NULL);
#else
    return zend_read_property(Z_OBJCE_P(z_object), Z_OBJ_P(z_object), name,name_length, silent, NULL);
#endif
}