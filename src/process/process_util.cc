#include "php_lib.h"
#include "process.h"


ZEND_BEGIN_ARG_INFO_EX(arginfo_lib_process_construct,0,0,1)
ZEND_ARG_CALLABLE_INFO(0,func,0)
ZEND_END_ARG_INFO()

zend_class_entry lib_process_ce;
zend_class_entry *lib_process_ce_ptr;


static long cid = 0;
php_lib_fun funs[MAX_PROCESSES * 10];
//struct php_lib_fci_fcc
//{
//    zend_fcall_info fci;
//    zend_fcall_info_cache fcc;
//};

PHP_METHOD(process_obj,__construct)
{
    zend_fcall_info fci = empty_fcall_info;
    zend_fcall_info_cache fcc = empty_fcall_info_cache;
    ZEND_PARSE_PARAMETERS_START(1,1)
    Z_PARAM_FUNC(fci,fcc)
//    Z_PARAM_VARIADIC("*",fci.params,fci.param_count)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);

    php_lib_fun func;
    func.fci = fci;
    func.fcc = fcc;
    funs[cid] = func;
    zend_update_property_long(lib_process_ce_ptr,getThis(), ZEND_STRL("pid"), cid TSRMLS_CC);
    cid += 1;
}
//子进程 主事件循环的地方
void worker_process_cycle(long cid)
{
    php_printf("start worker cid:%ld \n",cid);
    php_lib_fun func = funs[cid];
//    func.fci.retval = getThis();
//    func.fci.params = func.obj;
//    func.fci.param_count = 1;

    if (zend_call_function(&func.fci, &func.fcc) != SUCCESS)
    {
        php_printf("start worker error\n");
        return;
    }
    php_printf("start worker end cid:%ld \n",cid);

}
//启动子进程
PHP_METHOD(process_obj,start)
{
    zval *re,r;

    re = zend_read_property(lib_process_ce_ptr,getThis(), "pid", sizeof("pid")-1, 1,&r);

    long cid = Z_LVAL_P(re);
    funs[cid].obj = getThis();

    pid_t p = spwan_process(worker_process_cycle,NULL,processes[cid].name,cid);
    printf("pid :%d cid:%ld \n",p,cid);
}

PHP_METHOD(process_obj,__destruct)
{
}
PHP_METHOD(process_obj,read)
{

}
PHP_METHOD(process_obj,write)
{
}

PHP_METHOD(process_obj,getpid)
{
    RETURN_LONG((long)ce_pid);
}
PHP_METHOD(process_obj,getppid)
{
    RETURN_LONG((long)ce_parent);
}

const zend_function_entry lib_process_methods[] =
        {
//                PHP_ME(lib_shamem_util,__construct,arginfo_lib_shamem_create,ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
                PHP_ME(process_obj,__construct,arginfo_lib_process_construct,ZEND_ACC_PUBLIC)
                PHP_ME(process_obj,__destruct,NULL,ZEND_ACC_PUBLIC)
                PHP_ME(process_obj,start,NULL,ZEND_ACC_PUBLIC)
                PHP_ME(process_obj,read,NULL,ZEND_ACC_PUBLIC)
                PHP_ME(process_obj,getpid,NULL,ZEND_ACC_PUBLIC)
                PHP_ME(process_obj,getppid,NULL,ZEND_ACC_PUBLIC)
                PHP_ME(process_obj,write,NULL,ZEND_ACC_PUBLIC)
                PHP_FE_END
        };




//注册该类
void lib_process_init()
{
    //设置主进程的pid
    ce_pid = getpid();
    //获取父进程pid
    ce_parent = getppid();

    INIT_NS_CLASS_ENTRY(lib_process_ce,"Lib","Process",lib_process_methods);
    lib_process_ce_ptr = zend_register_internal_class(&lib_process_ce TSRMLS_CC);

//    lib_shamem_ce_ptr->serialize = zend_class_serialize_deny;
//    lib_shamem_ce_ptr->unserialize = zend_class_unserialize_deny;
    //申明类的属性
    zend_declare_property_long(lib_process_ce_ptr, ZEND_STRL("pid"),0, ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_property_long(lib_process_ce_ptr, ZEND_STRL("ppid"), 0, ZEND_ACC_PRIVATE TSRMLS_CC);

}
