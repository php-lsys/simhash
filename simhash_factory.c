/**
 * @author     Lonely <shan.liu@msn.com>
 * @copyright  (c) 2017 ShanLiu <shan.liu@msn.com>
 * @license    http://www.apache.org/licenses/LICENSE-2.0
 */
#include "php.h"
#include <stdlib.h>
#include "ext/standard/info.h"
#include "ext/standard/md5.h"
#include "zend_smart_str.h"
#include "ext/standard/php_smart_string.h"
#include "simhash_factory.h"
#include "common.h"
#include "zend_exceptions.h"


ZEND_METHOD(simhash_factory_entry, __construct)
{
    zend_long size;
    ZEND_PARSE_PARAMETERS_START(1,1)
            Z_PARAM_LONG(size)
    ZEND_PARSE_PARAMETERS_END();
    simhash_update_property_long(getThis(),ZEND_STRL("hash_size"),size);
}
ZEND_METHOD(simhash_factory_entry, setWeight)
{
    zval  *arr;
    ZEND_PARSE_PARAMETERS_START(1,1)
        Z_PARAM_ARRAY(arr)
    ZEND_PARSE_PARAMETERS_END();
    simhash_update_property(getThis(),ZEND_STRL("hash_size"),arr);
    RETURN_ZVAL(getThis(),1,0);
}
ZEND_METHOD(simhash_factory_entry, fingerprint)
{
    zval *word_arr;
    ZEND_PARSE_PARAMETERS_START(1,1)
        Z_PARAM_ARRAY(word_arr)
    ZEND_PARSE_PARAMETERS_END();

    zval param[1];
    ZVAL_ZVAL(&param[0],word_arr,1,0);

    zval retval,handle;
    ZVAL_STRING(&handle,"array_count_values");

    if(call_user_function(EG(function_table), NULL, &handle, &retval, 1, param)!= SUCCESS){
        zval_dtor(&param[0]);
        zval_dtor(&handle);
        RETURN_FALSE;
    }
    zval_dtor(&param[0]);
    zval_dtor(&handle);

    zend_array *warr=NULL;
    zval *warrval=simhash_read_property(getThis(),ZEND_STRL("weight"),1);
    if(Z_TYPE_P(warrval)==IS_ARRAY)warr=Z_ARR_P(warrval);

    zend_long vector_len;
    vector_len=Z_LVAL_P(simhash_read_property(getThis(),ZEND_STRL("hash_size"),1));
    if (vector_len>64){
        zend_throw_exception_ex(zend_ce_exception, 1, "your hash size >: %d",(int)vector_len);
        zval_dtor(&retval);
        return ;
    }


    zend_long * vector=(zend_long*)emalloc(vector_len*sizeof(zend_long));

    memset(vector,0,vector_len*sizeof(zend_long));

    char * cvmap[103];
    cvmap['0']="0000";
    cvmap['1']="0001";
    cvmap['2']="0010";
    cvmap['3']="0011";
    cvmap['4']="0100";
    cvmap['5']="0101";
    cvmap['6']="0110";
    cvmap['7']="0111";
    cvmap['8']="1000";
    cvmap['9']="1001";
    cvmap['a']="1010";
    cvmap['b']="1011";
    cvmap['c']="1100";
    cvmap['d']="1101";
    cvmap['e']="1110";
    cvmap['f']="1111";

    int i=0, count = zend_hash_num_elements(Z_ARRVAL(retval));
	zend_hash_internal_pointer_reset(Z_ARRVAL(retval));
	zval *z_item;

	for (i = 0; i < count; i ++) {
		zend_string* key;
		zend_ulong idx;
        if (zend_hash_get_current_key(Z_ARRVAL(retval), &key, &idx) != HASH_KEY_IS_STRING){
            continue;
        }
		z_item=zend_hash_get_current_data(Z_ARRVAL(retval));

        int wig=Z_LVAL_P(z_item);


        char * str=ZSTR_VAL(key);
        char md5str[33];
        PHP_MD5_CTX context;
        unsigned char digest[16];

        md5str[0] = '\0';
        PHP_MD5Init(&context);
        PHP_MD5Update(&context, str, ZSTR_LEN(key));
        PHP_MD5Final(digest, &context);
        make_digest(md5str, digest);

        smart_string buf = {0};
        for(int j=0;j<32;j++){
            char * tmp=cvmap[md5str[j]];
            smart_string_appends(&buf,tmp);
        }
        smart_string_0(&buf);

        double difwig=1.0;
        if(warr){
            zval *strw=zend_hash_find(warr,key);
            if(strw&&(Z_TYPE_P(strw)==IS_LONG||Z_TYPE_P(strw)==IS_DOUBLE)){
                if(Z_LVAL_P(strw)>0){
                    difwig=(double)Z_LVAL_P(strw);
                }
            }
        }

        for(int j=0;j<vector_len;j++){
            zend_long * _v=vector+j;
            zend_long swig=(zend_long)ceil(wig*difwig);
            if (buf.c[j] == '1'){
                *_v += swig;
            }else{
                *_v -= swig;
            }
        }
        smart_string_free(&buf);

		zend_hash_move_forward(Z_ARRVAL(retval));
	}

    zval_dtor(&retval);

    zend_long fingerprint=0;


    for(int j=0;j<vector_len;j++){
        if (vector[j] >= 0){
            int t= 1<<vector_len-j-1;
            fingerprint += t;
        }
    }

    efree(vector);

    RETURN_LONG(fingerprint);
}

static zend_function_entry simhash_factory_entry_method[] = {
        ZEND_ME(simhash_factory_entry, __construct, simhash_factory_construct_args, ZEND_ACC_PUBLIC)
        ZEND_ME(simhash_factory_entry, fingerprint, simhash_factory_fingerprint_args, ZEND_ACC_PUBLIC)
        ZEND_ME(simhash_factory_entry, setWeight, simhash_factory_set_weight_args, ZEND_ACC_PUBLIC)
        PHP_FE_END
};
zend_class_entry *simhash_factory_entry_ptr;
void simhash_factory_init(){
    zend_class_entry simhash_factory;
    INIT_NS_CLASS_ENTRY(simhash_factory,SIMHASH_NS,"SimHashFactory",simhash_factory_entry_method);
    simhash_factory_entry_ptr = zend_register_internal_class(&simhash_factory);
    zend_declare_property_null(simhash_factory_entry_ptr, ZEND_STRL("hash_size"), ZEND_ACC_PROTECTED);
    zend_declare_property_null(simhash_factory_entry_ptr, ZEND_STRL("weight"), ZEND_ACC_PROTECTED );
}
