/**
 * lsys core
 * @author     Lonely <shan.liu@msn.com>
 * @copyright  (c) 2017 Lonely <shan.liu@msn.com>
 * @license    http://www.apache.org/licenses/LICENSE-2.0
 */

#include "php.h"
#include "ext/standard/info.h"




#define  NR (sizeof(zend_long)*8)
static int int_to_bit_arr(int num,char *bitArr){
    memset(bitArr,'\0',NR);
    int i = -1;
    do {
        i += 1;
        bitArr[i] = num % 2;
        num /= 2;
    } while (num != 0);
    return i;
}

zend_class_entry *simhash_entry;


ZEND_BEGIN_ARG_INFO_EX(simhash_construct_args, 0,0,1)
    ZEND_ARG_INFO(0, fingerprint)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(simhash_compare_with_args, 0,0,1)
    ZEND_ARG_OBJ_INFO(0, otherHash, LsExt\\SimHash, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(simhash_get_args, 0,0,0)
ZEND_END_ARG_INFO()

#define FINGERPRINT_ATTR "fingerprint",strlen("fingerprint")


ZEND_METHOD(simhash_entry, __construct)
{
	zend_string* str;
	zend_long fingerprint;
	zval *object;
	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "OS",&object,simhash_entry,&str) == FAILURE) {
		RETURN_FALSE;
	}
    zval strval;
	ZVAL_STR(&strval,str);
    fingerprint=zval_get_long(&strval);
    zend_update_property_long(simhash_entry,object,FINGERPRINT_ATTR,fingerprint TSRMLS_DC);
}

zend_long num_xor_data(zend_long num){
    if(num>0)return num;
    return ~num+1;
}

ZEND_METHOD(simhash_entry, hammingDistance)
{
    zval *object,*other_object;
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "OO",&object,simhash_entry,&other_object,simhash_entry) == FAILURE) {
        RETURN_FALSE;
    }
    zval *self_f,*other_f;
    self_f=zend_read_property(simhash_entry,object,FINGERPRINT_ATTR,1,NULL TSRMLS_DC);
    other_f=zend_read_property(simhash_entry,other_object,FINGERPRINT_ATTR,1,NULL TSRMLS_DC);
    convert_to_long(self_f);
    convert_to_long(other_f);

    int outnum=0;
    zend_long f=num_xor_data(Z_LVAL_P(self_f))^num_xor_data(Z_LVAL_P(other_f));

    zend_long _t;
    while(1){
        if(!f)break;
        if(f&1) outnum++;
        f=f>>1;
    }
    if(Z_LVAL_P(self_f)>0&&Z_LVAL_P(other_f)<0
        ||Z_LVAL_P(self_f)<0&&Z_LVAL_P(other_f)>0) outnum++;
    RETURN_LONG(outnum);
}

ZEND_METHOD(simhash_entry, compareWith)
{
	zval *object,*other_object;
	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "OO",&object,simhash_entry,&other_object,simhash_entry) == FAILURE) {
        RETURN_FALSE;
    }
    zval *self_f,*other_f;
    self_f=zend_read_property(simhash_entry,object,FINGERPRINT_ATTR,1,NULL TSRMLS_DC);
    other_f=zend_read_property(simhash_entry,other_object,FINGERPRINT_ATTR,1,NULL TSRMLS_DC);
    convert_to_long(self_f);
    convert_to_long(other_f);

    if((Z_LVAL_P(self_f)==0&&Z_LVAL_P(other_f)!=0)
    ||(Z_LVAL_P(self_f)!=0&&Z_LVAL_P(other_f)==0)
    ) RETURN_DOUBLE(0);
    if(Z_LVAL_P(self_f)==0&&Z_LVAL_P(other_f)==0) RETURN_DOUBLE(1);

    long num=num_xor_data(Z_LVAL_P(self_f))^num_xor_data(Z_LVAL_P(other_f));

    char bitArr[NR];
    int differences=0;
    int i=int_to_bit_arr(num,bitArr);
    for (i ; i >= 0; i--)
    {
        if(bitArr[i])differences++;
    }
    if(Z_LVAL_P(self_f)>0&&Z_LVAL_P(other_f)<0
       ||Z_LVAL_P(self_f)<0&&Z_LVAL_P(other_f)>0) num++;
    int j=int_to_bit_arr(Z_LVAL_P(self_f),bitArr);

    int fpLength = j * 2;
    if(fpLength==0) RETURN_DOUBLE(0);
    float outnum;
    outnum=1.0 - ((float)differences / (float)fpLength);

    RETURN_DOUBLE(outnum);
}
ZEND_METHOD(simhash_entry, __toString)
{
    zval *object;
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O",&object,simhash_entry) == FAILURE) {
        RETURN_FALSE;
    }
    zval *self_f;
    self_f=zend_read_property(simhash_entry,object,FINGERPRINT_ATTR,1,NULL TSRMLS_DC);
    convert_to_string(self_f);
    RETURN_ZVAL(self_f,0,1);
}
ZEND_METHOD(simhash_entry, getBin)
{
    zval *object;
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O",&object,simhash_entry) == FAILURE) {
        RETURN_FALSE;
    }
    zval *self_f;
    self_f=zend_read_property(simhash_entry,object,FINGERPRINT_ATTR,1,NULL TSRMLS_DC);
    char bitArr[NR],outbit[NR];
    int i=int_to_bit_arr(Z_LVAL_P(self_f),bitArr),j=i;
    for (i ; i >= 0; i--)
    {
        if(bitArr[i])outbit[j-i]='1';
        else outbit[j-i]='0';
    }
    RETURN_STRINGL(outbit,j+1);
}
ZEND_METHOD(simhash_entry, getHex)
{
    zval *object;
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O",&object,simhash_entry) == FAILURE) {
        RETURN_FALSE;
    }
    zval *self_f;
    self_f=zend_read_property(simhash_entry,object,FINGERPRINT_ATTR,1,NULL TSRMLS_DC);
    long slong=Z_LVAL_P(self_f);
    char buf[9];
    int len=sprintf(buf,"%X",slong);
    RETURN_STRINGL(buf,len);
}
ZEND_METHOD(simhash_entry, getDec)
{
    zval *object;
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O",&object,simhash_entry) == FAILURE) {
        RETURN_FALSE;
    }
    zval *self_f;
    self_f=zend_read_property(simhash_entry,object,FINGERPRINT_ATTR,0,NULL TSRMLS_DC);
    RETURN_ZVAL(self_f,1,0);
}

static zend_function_entry simhash_entry_method[] = {
        ZEND_ME(simhash_entry, __construct, simhash_construct_args, ZEND_ACC_PUBLIC)
        ZEND_ME(simhash_entry, compareWith, simhash_compare_with_args, ZEND_ACC_PUBLIC)
        ZEND_ME(simhash_entry, hammingDistance, simhash_compare_with_args, ZEND_ACC_PUBLIC)
        ZEND_ME(simhash_entry, __toString, simhash_get_args, ZEND_ACC_PUBLIC)
        ZEND_ME(simhash_entry, getBin, simhash_get_args, ZEND_ACC_PUBLIC)
        ZEND_ME(simhash_entry, getHex, simhash_get_args, ZEND_ACC_PUBLIC)
        ZEND_ME(simhash_entry, getDec, simhash_get_args, ZEND_ACC_PUBLIC)
        PHP_FE_END
};

extern void simhash_init(){
        zend_class_entry simhash;
        INIT_NS_CLASS_ENTRY(simhash,"LsExt","SimHash",simhash_entry_method);
        simhash_entry = zend_register_internal_class(&simhash TSRMLS_CC);
        zend_declare_property_long(simhash_entry, FINGERPRINT_ATTR,0, ZEND_ACC_PROTECTED TSRMLS_CC);
}