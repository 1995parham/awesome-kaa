/*
 * In The Name Of God
 * ========================================
 * [] File Name : user.c
 *
 * [] Creation Date : 28-02-2016
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2016 Parham Alvani.
*/
#include <stdio.h>
#include <stdint.h>

#include <kaa/kaa_error.h>
#include <kaa/kaa_user.h>
#include <kaa/platform/ext_user_callback.h>

#include "user.h"
#include "util.h"

static kaa_error_t on_attached(void *context, const char *user_external_id, const char *endpoint_access_token)
{
	    return KAA_ERR_NONE;
}

static kaa_error_t on_detached(void *context, const char *endpoint_access_token)
{
	    return KAA_ERR_NONE;
}
static kaa_error_t on_attach_success(void *context)
{
	    return KAA_ERR_NONE;
}
static kaa_error_t on_attach_failed(void *context, user_verifier_error_code_t error_code, const char *reason)
{
	    return KAA_ERR_NONE;
}

kaa_error_t attach_endpoint_to_user(kaa_user_manager_t *manager, const char *user_external_id, const char *user_access_token)
{
	kaa_error_t error_code;

	kaa_attachment_status_listeners_t attachement_listeners = {
		NULL,
		on_attached,
		on_detached,
		on_attach_success,
		on_attach_failed
	};
	
	error_code = kaa_user_manager_set_attachment_listeners(manager,
			&attachement_listeners);
	
	error_code = kaa_user_manager_default_attach_to_user(manager,
			user_external_id,
			user_access_token);

	return error_code;
}
