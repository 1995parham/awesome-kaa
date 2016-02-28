#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include <kaa/kaa_error.h>
#include <kaa/platform/kaa_client.h>
#include <kaa/utilities/kaa_log.h>
#include <kaa/kaa_profile.h>
#include <kaa/gen/kaa_profile_gen.h>

#include "util.h"
#include "notification.h"
#include "event.h"

static kaa_client_t *kaa_client;

int main(int argc, char *argv[])
{
	printf("Kaa SDK client started\n");
	kaa_error_t error_code;
	
	/* Build Kaa client */

	error_code = kaa_client_create(&kaa_client, NULL);
	KAA_RETURN_IF_ERROR(error_code, "Failed create Kaa client");

	/* Build notification related things */

	error_code = kaa_notification_register(
			kaa_client_get_context(kaa_client)->notification_manager);
	KAA_RETURN_IF_ERROR(error_code, "Failed to register notification");
	
	/* Build event related things */

	error_code = kaa_event_register(
			kaa_client_get_context(kaa_client)->event_manager);
	KAA_RETURN_IF_ERROR(error_code, "Failed to register event");

	/* Build profile related things */

	kaa_profile_t *profile = kaa_profile_parham_create();
	profile->grade = 20;
	profile->message = kaa_string_move_create("Hello world", NULL);
	
	error_code = kaa_profile_manager_update_profile(
			kaa_client_get_context(kaa_client)->profile_manager,
			profile);
	KAA_RETURN_IF_ERROR(error_code, "Failed to update profile");

	error_code = kaa_client_start(kaa_client, NULL, NULL, 0);
	KAA_RETURN_IF_ERROR(error_code, "Failed to start Kaa main loop");

	kaa_client_destroy(kaa_client);
	return error_code;
}
