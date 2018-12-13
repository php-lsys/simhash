/**
 * lsys core
 * @author     Lonely <shan.liu@msn.com>
 * @copyright  (c) 2017 Lonely <shan.liu@msn.com>
 * @license    http://www.apache.org/licenses/LICENSE-2.0
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_simhash.h"
#include "simhash.c"
#include "simhash_factory.c"


PHP_MINIT_FUNCTION(simhash)
{
    simhash_init();
    simhash_factory_init();
    return SUCCESS;
}

PHP_RINIT_FUNCTION(simhash)
{
    #if defined(COMPILE_DL_SIMHASH) && defined(ZTS)
    ZEND_TSRMLS_CACHE_UPDATE();
    #endif
    return SUCCESS;
}


PHP_MINFO_FUNCTION(simhash)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "simhash support", "enabled");
}

PHP_FUNCTION(dddddd)
{
    RETURN_STRING("FASD");
}

const zend_function_entry sssss_functions[] = {
        PHP_FE(dddddd,	NULL)		/* For testing, remove later. */
        PHP_FE_END	/* Must be the last line in lly_functions[] */
};

zend_module_entry simhash_module_entry = {
        STANDARD_MODULE_HEADER,
        "simhash",
        sssss_functions,
        PHP_MINIT(simhash),
        NULL,
        PHP_RINIT(simhash),		/* Replace with NULL if there's nothing to do at request start */
        NULL,	/* Replace with NULL if there's nothing to do at request end */
        PHP_MINFO(simhash),
        PHP_SIMHASH_VERSION,
        STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_SIMHASH
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(simhash)
#endif