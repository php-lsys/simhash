/**
 * @author     Lonely <shan.liu@msn.com>
 * @copyright  (c) 2017 ShanLiu <shan.liu@msn.com>
 * @license    http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef PHP_SIMHASH_FACTORY_CLASS_H
#define PHP_SIMHASH_FACTORY_CLASS_H

ZEND_BEGIN_ARG_INFO_EX(simhash_factory_construct_args, 0,0,1)
    ZEND_ARG_INFO(0, hash_size)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(simhash_factory_fingerprint_args, 0,0,1)
    ZEND_ARG_INFO(0, weight)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(simhash_factory_set_weight_args, 0,0,1)
                ZEND_ARG_INFO(0, weight)
ZEND_END_ARG_INFO()


extern zend_class_entry *simhash_factory_entry_ptr;
void simhash_factory_init();

#endif



