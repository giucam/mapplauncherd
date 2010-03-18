/*
 * report.c
 *
 * This file is part of applauncherd
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syslog.h>

#include "report.h"

static enum report_output output = report_console;

void report_set_output(enum report_output new_output)
{
    if (output == new_output)
        return;

    if (output == report_syslog)
        closelog();

    if (new_output == report_syslog)
        openlog(PROG_NAME, LOG_PID, LOG_DAEMON);

    output = new_output;
}

static void vreport(enum report_type type, char *msg, va_list arg)
{
    char str[400];
    char *str_type = "";
    int log_type;

    switch (type)
    {
    case report_debug:
        log_type = LOG_DEBUG;
        break;
    default:
    case report_info:
        log_type = LOG_INFO;
        break;
    case report_warning:
        str_type = "warning: ";
        log_type = LOG_WARNING;
        break;
    case report_error:
        str_type = "error: ";
        log_type = LOG_ERR;
        break;
    case report_fatal:
        str_type = "died: ";
        log_type = LOG_ERR;
        break;
    }

    vsnprintf(str, sizeof(str), msg, arg);

    if (output == report_console)
        printf("%s: %s%s", PROG_NAME, str_type, str);
    else if (output == report_syslog)
        syslog(log_type, "%s%s", str_type, str);
}

void report(enum report_type type, char *msg, ...)
{
    va_list arg;

    va_start(arg, msg);
    vreport(type, msg, arg);
    va_end(arg);
}

void die(int status, char *msg, ...)
{
    va_list arg;

    va_start(arg, msg);
    vreport(report_fatal, msg, arg);
    va_end(arg);

    exit(status);
}

