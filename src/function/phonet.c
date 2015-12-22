/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */

#include "usbg/usbg.h"
#include "usbg/usbg_internal.h"

#include <malloc.h>
#ifdef HAS_LIBCONFIG
#include <libconfig.h>
#endif

static int phonet_set_attrs(struct usbg_function *f,
			    const usbg_function_attrs *f_attrs)
{
	int ret = USBG_ERROR_INVALID_PARAM;
	const usbg_f_phonet_attrs *attrs = &f_attrs->attrs.phonet;

	if (f_attrs->header.attrs_type &&
	    f_attrs->header.attrs_type != USBG_F_ATTRS_PHONET)
		goto out;

	ret = attrs->ifname && attrs->ifname[0] ?
		USBG_ERROR_INVALID_PARAM : USBG_SUCCESS;

out:
	return ret;
}

static int phonet_get_attrs(struct usbg_function *f,
			    usbg_function_attrs *f_attrs)
{
	int ret;

	ret = usbg_read_string_alloc(f->path, f->name, "ifname",
				     &(f_attrs->attrs.phonet.ifname));
	if (ret != USBG_SUCCESS)
		goto out;

	f_attrs->header.attrs_type = USBG_F_ATTRS_PHONET;
out:
	return ret;
}

static void phonet_cleanup_attrs(struct usbg_function *f,
				usbg_function_attrs *f_attrs)
{
	free((char*)f_attrs->attrs.phonet.ifname);
	f_attrs->attrs.phonet.ifname = NULL;
}

static int phonet_libconfig_import(struct usbg_function *f,
				  config_setting_t *root)
{
	return USBG_SUCCESS;
}

static int phonet_libconfig_export(struct usbg_function *f,
				  config_setting_t *root)
{
	return USBG_SUCCESS;
}

struct usbg_function_type usbg_f_type_phonet = {
	.name = "phonet",
	.set_attrs = phonet_set_attrs,
	.get_attrs = phonet_get_attrs,
	.cleanup_attrs = phonet_cleanup_attrs,
	.import = phonet_libconfig_import,
	.export = phonet_libconfig_export,
};
