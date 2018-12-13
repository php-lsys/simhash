/**
 * lsys core
 * @author     Lonely <shan.liu@msn.com>
 * @copyright  (c) 2017 Lonely <shan.liu@msn.com>
 * @license    http://www.apache.org/licenses/LICENSE-2.0
 */
#include "php.h"
#include <stdlib.h>
#include "ext/standard/info.h"
#include "ext/standard/md5.h"
#include "zend_smart_str.h"
#include "zend_smart_string.h"

zend_class_entry *simhash_factory_entry;

ZEND_BEGIN_ARG_INFO_EX(simhash_factory_construct_args, 0,0,1)
    ZEND_ARG_INFO(0, hash_size)
ZEND_END_ARG_INFO()


#define HASHSIZE_ATTR "hash_size",strlen("hash_size")
#define WEIGHT_ATTR "weight",strlen("weight")



ZEND_METHOD(simhash_factory_entry, __construct)
{
	zend_long size;
	zval *object;
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol",&object,simhash_factory_entry,&size) == FAILURE) {
        RETURN_FALSE;
    }
    zend_update_property_long(simhash_factory_entry,object,HASHSIZE_ATTR,size TSRMLS_DC);
}
ZEND_METHOD(simhash_factory_entry, setWeight)
{
    zval *wg_arr;
    zval *object;
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Oa",&object,simhash_factory_entry,&wg_arr) == FAILURE) {
        RETURN_FALSE;
    }
    zend_update_property(simhash_factory_entry,object,WEIGHT_ATTR,wg_arr TSRMLS_DC);

    RETURN_ZVAL(object,1,0);

}
ZEND_METHOD(simhash_factory_entry, fingerprint)
{
    zval *word_arr;
    zval *object;
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Oa",&object,simhash_factory_entry,&word_arr) == FAILURE) {
        RETURN_FALSE;
    }

    zval param[1];
    ZVAL_ZVAL(&param[0],word_arr,1,1);

    zval retval,handle;
    ZVAL_STRING(&handle,"array_count_values");

    if(call_user_function(EG(function_table), NULL, &handle, &retval, 1, param)!= SUCCESS){
        zend_string_release(Z_STR(handle));
        RETURN_FALSE;
    }
    zend_string_release(Z_STR(handle));


    zend_array *warr=NULL;
    zval *warrval=zend_read_property(simhash_factory_entry,object,WEIGHT_ATTR,1,NULL TSRMLS_DC);
    if(Z_TYPE_P(warrval)==IS_ARRAY)warr=Z_ARR_P(warrval);


    long vector_len;
    vector_len=Z_LVAL_P(zend_read_property(simhash_factory_entry,object,HASHSIZE_ATTR,1,NULL TSRMLS_DC));
    long * vector=(long*)emalloc(vector_len*sizeof(long));

    memset(vector,0,vector_len*sizeof(long));


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
            long * _v=vector+j;
            long swig=(long)ceil(wig*difwig);
            if (buf.c[j] == '1'){
                *_v += swig;
            }else{
                *_v -= swig;
            }
        }
        smart_string_free(&buf);

		zend_hash_move_forward(Z_ARRVAL(retval));
	}

    zend_array_destroy(Z_ARR(retval));

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
        ZEND_ME(simhash_factory_entry, fingerprint, NULL, ZEND_ACC_PUBLIC)
        ZEND_ME(simhash_factory_entry, setWeight, NULL, ZEND_ACC_PUBLIC)
        PHP_FE_END
};

extern void simhash_factory_init(){
    zend_class_entry simhash_factory;
    INIT_NS_CLASS_ENTRY(simhash_factory,"LsExt","SimHashFactory",simhash_factory_entry_method);
    simhash_factory_entry = zend_register_internal_class(&simhash_factory TSRMLS_CC);
    zend_declare_property_null(simhash_factory_entry, HASHSIZE_ATTR, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(simhash_factory_entry, WEIGHT_ATTR, ZEND_ACC_PROTECTED TSRMLS_CC);
}
