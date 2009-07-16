/*
 * Copyright (C) 2008 Codership Oy <info@codership.com>
 *
 * $Id$
 */

#include <check.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <galerautils.h>

#include "../gcs_backend.h"
#include "gcs_backend_test.h"

// Fake backend definitons. Must be global for gcs_backend.c to see

GCS_BACKEND_NAME_FN(gcs_test_name)
{
    return "DUMMIEEEE!";
}

GCS_BACKEND_CREATE_FN(gcs_test_create)
{
    backend->name = gcs_test_name;
    return 0;
}

GCS_BACKEND_NAME_FN(gcs_spread_name)
{
    return "SPREAT";
}

GCS_BACKEND_CREATE_FN(gcs_spread_create)
{
    backend->name = gcs_spread_name;
    return 0;
}

GCS_BACKEND_NAME_FN(gcs_vs_name)
{
    return "vsssssssss";
}

GCS_BACKEND_CREATE_FN(gcs_vs_create)
{
    backend->name = gcs_vs_name;
    return 0;
}

GCS_BACKEND_NAME_FN(gcs_gcomm_name)
{
    return "gCOMMMMM!!!";
}

GCS_BACKEND_CREATE_FN(gcs_gcomm_create)
{
    backend->name = gcs_gcomm_name;
    return 0;
}

START_TEST (gcs_backend_test)
{
    gcs_backend_t backend;
    long ret;

    ret = gcs_backend_init (&backend, "wrong://kkk");
    fail_if (ret != -ESOCKTNOSUPPORT);

    ret = gcs_backend_init (&backend, "spread:");
    fail_if (ret != -EINVAL);

    ret = gcs_backend_init (&backend, "dummy://");
    fail_if (ret != 0, "ret = %d (%s)", ret, strerror(-ret));
//    fail_if (backend.name != gcs_test_name); this test is broken since we can
//    no longer use global gcs_dummy_create() symbol because linking with real
//    gcs_dummy.o

    ret = gcs_backend_init (&backend,
                  "gcomm://0.0.0.0:4567?gmcast.node=gcomm+tcp://1.2.3.4:4567");
    fail_if (ret != 0, "ret = %d (%s)", ret, strerror(-ret));
    fail_if (backend.name != gcs_gcomm_name);

    ret = gcs_backend_init (&backend, "vsbes://kkk");
    fail_if (ret != 0, "ret = %d (%s)", ret, strerror(-ret));
    fail_if (backend.name != gcs_vs_name);

    ret = gcs_backend_init (&backend, "spread://");
    fail_if (ret != 0, "ret = %d (%s)", ret, strerror(-ret));
    fail_if (backend.name != gcs_spread_name);

}
END_TEST

Suite *gcs_backend_suite(void)
{
    Suite *suite = suite_create("GCS backend interface");
    TCase *tcase = tcase_create("gcs_backend");

    suite_add_tcase (suite, tcase);
    tcase_add_test  (tcase, gcs_backend_test);
    return suite;
}

