/**
 * @author     Lonely <shan.liu@msn.com>
 * @copyright  (c) 2017 ShanLiu <shan.liu@msn.com>
 * @license    http://www.apache.org/licenses/LICENSE-2.0
 */

#include "php.h"
#include "simhash.h"
#include "common.h"

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

ZEND_METHOD(simhash_entry, __construct)
{
    zend_long fingerprint;
    ZEND_PARSE_PARAMETERS_START(1,1)
        Z_PARAM_LONG(fingerprint)
    ZEND_PARSE_PARAMETERS_END();
    simhash_update_property_long(getThis(),ZEND_STRL("fingerprint"),fingerprint);
}

zend_long num_xor_data(zend_long num){
    if(num>0)return num;
    return ~num+1;
}

ZEND_METHOD(simhash_entry, hammingDistance)
{
    zval *other_object;
    ZEND_PARSE_PARAMETERS_START(1,1)
        Z_PARAM_OBJECT_OF_CLASS(other_object,simhash_entry_ptr)
    ZEND_PARSE_PARAMETERS_END();

    zval *self_f,*other_f;
    self_f=simhash_read_property(getThis(),ZEND_STRL("fingerprint"),1);
    other_f=simhash_read_property(other_object,ZEND_STRL("fingerprint"),1);
    int outnum=0;
    zend_long f=num_xor_data(Z_LVAL_P(self_f))^num_xor_data(Z_LVAL_P(other_f));
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
    zval *other_object;
    ZEND_PARSE_PARAMETERS_START(1,1)
            Z_PARAM_OBJECT_OF_CLASS(other_object,simhash_entry_ptr)
    ZEND_PARSE_PARAMETERS_END();
    zval *self_f,*other_f;
    self_f=simhash_read_property(getThis(),ZEND_STRL("fingerprint"),1);
    other_f=simhash_read_property(other_object,ZEND_STRL("fingerprint"),1);

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
    zval *self_f;
    self_f=simhash_read_property(getThis(),ZEND_STRL("fingerprint"),1);
    convert_to_string(self_f);
    RETURN_ZVAL(self_f,1,1);
}
ZEND_METHOD(simhash_entry, getBin)
{
    zval *self_f;
    self_f=simhash_read_property(getThis(),ZEND_STRL("fingerprint"),1);
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
    zval *self_f;
    self_f=simhash_read_property(getThis(),ZEND_STRL("fingerprint"),1);
    long slong=Z_LVAL_P(self_f);
    char buf[9];
    int len=sprintf(buf,"%lX",slong);
    RETURN_STRINGL(buf,len);
}
ZEND_METHOD(simhash_entry, getDec)
{
    zval *self_f;
    self_f=simhash_read_property(getThis(),ZEND_STRL("fingerprint"),1);
    RETURN_ZVAL(self_f,1,0);
}

static zend_function_entry simhash_entry_method[] = {
        ZEND_ME(simhash_entry, __construct, simhash_construct_args, ZEND_ACC_PUBLIC)
        ZEND_ME(simhash_entry, compareWith, simhash_compare_with_args, ZEND_ACC_PUBLIC)
        ZEND_ME(simhash_entry, hammingDistance, simhash_compare_with_args, ZEND_ACC_PUBLIC)
        ZEND_ME(simhash_entry, __toString, simhash_none_arginfo, ZEND_ACC_PUBLIC)
        ZEND_ME(simhash_entry, getBin, simhash_none_arginfo, ZEND_ACC_PUBLIC)
        ZEND_ME(simhash_entry, getHex, simhash_none_arginfo, ZEND_ACC_PUBLIC)
        ZEND_ME(simhash_entry, getDec, simhash_none_arginfo, ZEND_ACC_PUBLIC)
        PHP_FE_END
};

zend_class_entry *simhash_entry_ptr;
void simhash_init(){
    zend_class_entry simhash;
    INIT_NS_CLASS_ENTRY(simhash,SIMHASH_NS,"SimHash",simhash_entry_method);
    simhash_entry_ptr = zend_register_internal_class(&simhash);
    zend_declare_property_long(simhash_entry_ptr, ZEND_STRL("fingerprint"),0, ZEND_ACC_PROTECTED);
}