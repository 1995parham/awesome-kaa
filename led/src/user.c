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
	printf("Attached :)\n");
	return KAA_ERR_NONE;
}

static kaa_error_t on_detached(void *context, const char *endpoint_access_token)
{
	printf("Detached :|\n");
	return KAA_ERR_NONE;
}
static kaa_error_t on_attach_success(void *context)
{
	printf("Success Attach :D\n");
	return KAA_ERR_NONE;
}
static kaa_error_t on_attach_failed(void *context, user_verifier_error_code_t error_code, const char *reason)
{
	printf("Attach Failed :(\n");
	return KAA_ERR_NONE;
}

kaa_error_t attach_endpoint_to_user(kaa_user_manager_t *manager, const char *user_external_id, const char *user_access_token)
{
	kaa_error_t error_code;

	printf("Start attaching to the user ...\n");

	kaa_attachment_status_listeners_t attachement_listeners = {
		NULL,
		on_attached,
		on_detached,
		on_attach_success,
		on_attach_failed
	};
	
	error_code = kaa_user_manager_set_attachment_listeners(manager,
			&attachement_listeners);
	KAA_RETURN_IF_ERROR(error_code, "Failed to user attaching listeners");
	
	error_code = kaa_user_manager_default_attach_to_user(manager,
			user_external_id,
			user_access_token);
	KAA_RETURN_IF_ERROR(error_code, "Failed to attach to the user");

	return error_code;
}
