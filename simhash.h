/**
 * @author     Lonely <shan.liu@msn.com>
 * @copyright  (c) 2017 ShanLiu <shan.liu@msn.com>
 * @license    http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef PHP_SIMHASH_CLASS_H
#define PHP_SIMHASH_CLASS_H

#include "php.h"
#include "ext/standard/info.h"

ZEND_BEGIN_ARG_INFO_EX(simhash_construct_args, 0,0,1)
    ZEND_ARG_INFO(0, fingerprint)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(simhash_compare_with_args, 0,0,1)
    ZEND_ARG_OBJ_INFO(0, otherHash, LsExt\\SimHash, 0)
ZEND_END_ARG_INFO()

extern zend_class_entry *simhash_entry_ptr;
void simhash_init();

#endif
