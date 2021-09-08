
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_TIME_H_INCLUDED_
#define _NGX_TIME_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


typedef ngx_rbtree_key_t      ngx_msec_t;
typedef ngx_rbtree_key_int_t  ngx_msec_int_t;

typedef struct tm             ngx_tm_t;

#define ngx_tm_sec            tm_sec
#define ngx_tm_min            tm_min
#define ngx_tm_hour           tm_hour
#define ngx_tm_mday           tm_mday
#define ngx_tm_mon            tm_mon
#define ngx_tm_year           tm_year
#define ngx_tm_wday           tm_wday
#define ngx_tm_isdst          tm_isdst

#define ngx_tm_sec_t          int
#define ngx_tm_min_t          int
#define ngx_tm_hour_t         int
#define ngx_tm_mday_t         int
#define ngx_tm_mon_t          int
#define ngx_tm_year_t         int
#define ngx_tm_wday_t         int


#if (NGX_HAVE_GMTOFF)
#define ngx_tm_gmtoff         tm_gmtoff
#define ngx_tm_zone           tm_zone
#endif


#if (NGX_SOLARIS)

#define ngx_timezone(isdst) (- (isdst ? altzone : timezone) / 60)

#else

#define ngx_timezone(isdst) (- (isdst ? timezone + 3600 : timezone) / 60)

#endif


void ngx_timezone_update(void);
void ngx_localtime(time_t s, ngx_tm_t *tm);
void ngx_libc_localtime(time_t s, struct tm *tm);
void ngx_libc_gmtime(time_t s, struct tm *tm);

#define ngx_gettimeofday(tp)  (void) gettimeofday(tp, NULL);
#define ngx_msleep(ms)        (void) usleep(ms * 1000)
#define ngx_sleep(s)          (void) sleep(s)


#endif /* _NGX_TIME_H_INCLUDED_ */
r each mutex from 10 to 100 based on spin count taken
 *             previously.
 *   FreeBSD:  Deadlock detection.  The default spin count is 2000.
 *             It can be overridden using LIBPTHREAD_SPINLOOPS environment
 *             variable or by pthread_mutex_setspinloops_np().  If a lock
 *             is still busy, sched_yield() can be called on both UP and
 *             SMP systems.  The default yield loop count is zero, but
 *             it can be set by LIBPTHREAD_YIELDLOOPS environment
 *             variable or by pthread_mutex_setyieldloops_np().
 *   Solaris:  No PTHREAD_MUTEX_ADAPTIVE_NP.
 *   MacOSX:   No PTHREAD_MUTEX_ADAPTIVE_NP.
 *
 *
 * PTHREAD_MUTEX_ELISION_NP is a Linux extension to elide locks using
 * Intel Restricted Transactional Memory.  It is the most suitable for
 * rwlock pattern access because it allows simultaneous reads without lock.
 * Supported since glibc 2.18.
 *
 *
 * PTHREAD_MUTEX_DEFAULT is default mutex type.
 *
 *   Linux:    PTHREAD_MUTEX_NORMAL.
 *   FreeBSD:  PTHREAD_MUTEX_ERRORCHECK.
 *   Solaris:  PTHREAD_MUTEX_NORMAL.
 *   MacOSX:   PTHREAD_MUTEX_NORMAL.
 */


ngx_int_t
ngx_thread_mutex_create(ngx_thread_mutex_t *mtx, ngx_log_t *log)
{
    ngx_err_t            err;
    pthread_mutexattr_t  attr;

    err = pthread_mutexattr_init(&attr);
    if (err != 0) {
        ngx_log_error(NGX_LOG_EMERG, log, err,
                      "pthread_mutexattr_init() failed");
        return NGX_ERROR;
    }

    err = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
    if (err != 0) {
        ngx_log_error(NGX_LOG_EMERG, log, err,
                      "pthread_mutexattr_settype"
                      "(PTHREAD_MUTEX_ERRORCHECK) failed");
        return NGX_ERROR;
    }

    err = pthread_mutex_init(mtx, &attr);
    if (err != 0) {
        ngx_log_error(NGX_LOG_EMERG, log, err,
                      "pthread_mutex_init() failed");
        return NGX_ERROR;
    }

    err = pthread_mutexattr_destroy(&attr);
    if (err != 0) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_mutexattr_destroy() failed");
    }

    return NGX_OK;
}


ngx_int_t
ngx_thread_mutex_destroy(ngx_thread_mutex_t *mtx, ngx_log_t *log)
{
    ngx_err_t  err;

    err = pthread_mutex_destroy(mtx);
    if (err != 0) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_mutex_destroy() failed");
        return NGX_ERROR;
    }

    return NGX_OK;
}


ngx_int_t
ngx_thread_mutex_lock(ngx_thread_mutex_t *mtx, ngx_log_t *log)
{
    ngx_err_t  err;

    err = pthread_mutex_lock(mtx);
    if (err == 0) {
        return NGX_OK;
    }

    ngx_log_error(NGX_LOG_ALERT, log, err, "pthread_mutex_lock() failed");

    return NGX_ERROR;
}


ngx_int_t
ngx_thread_mutex_unlock(ngx_thread_mutex_t *mtx, ngx_log_t *log)
{
    ngx_err_t  err;

    err = pthread_mutex_unlock(mtx);

#if 0
    ngx_time_update();
#endif

    if (err == 0) {
        return NGX_OK;
    }

    ngx_log_error(NGX_LOG_ALERT, log, err, "pthread_mutex_unlock() failed");

    return NGX_ERROR;
}
