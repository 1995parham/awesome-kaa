#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include <kaa/kaa_error.h>
#include <kaa/platform/kaa_client.h>
#include <kaa/utilities/kaa_log.h>
#include <kaa/gen/kaa_led_event_class_family.h>
#include <kaa/kaa_event.h>
#include <kaa/platform/ext_event_listeners_callback.h>

#include "util.h"
#include "user.h"

static kaa_client_t *kaa_client;

kaa_error_t event_listeners_callback(void *context, const kaa_endpoint_id listeners[], size_t listeners_count)
{
	int i;
	
	printf("Event Listerners: %zd\n", listeners_count);
	for (i = 0; i < listeners_count; i++) {
		kaa_error_t error_code;
		
		/* Create and send an event */
	
		kaa_led_event_class_family_led_on_event_t* led_on_request = kaa_led_event_class_family_led_on_event_create();
		led_on_request->red = 10;
		led_on_request->green = 20;
		led_on_request->blue = 30;
	
		error_code = kaa_event_manager_send_kaa_led_event_class_family_led_on_event(kaa_client_get_context(kaa_client)->event_manager
				, led_on_request
				, listeners[i]);
		KAA_RETURN_IF_ERROR(error_code, "Failed to send LEDOnEvent");
	
		led_on_request->destroy(led_on_request);

	}

	return KAA_ERR_NONE;
}

kaa_error_t event_listeners_request_failed(void *context)
{
	printf("Event Listeners founding faild :(\n");
	
	return KAA_ERR_NONE;
}

void external_process(void *context)
{
	kaa_error_t error_code;
	
	/* Find endpoint by event FQDN */
	
	const char *fqns[] = {"ir.ac.aut.aolab.led.led_on_event"};
	
	kaa_event_listeners_callback_t callback = {NULL, event_listeners_callback, event_listeners_request_failed};
	
	error_code = kaa_event_manager_find_event_listeners(kaa_client_get_context(kaa_client)->event_manager
			, fqns
			, 1
			, &callback);
	KAA_EXIT_IF_ERROR(error_code, "Failed to attach event listener");	
}

int main(int argc, char *argv[])
{
	printf("Kaa SDK client started\n");
	kaa_error_t error_code;
	
	/* Build Kaa client */

	error_code = kaa_client_create(&kaa_client, NULL);
	KAA_RETURN_IF_ERROR(error_code, "Failed create Kaa client");

	/* Attach to user :) */
	
	error_code = attach_endpoint_to_user(
			kaa_client_get_context(kaa_client)->user_manager,
			"Parham",
			"");
	KAA_RETURN_IF_ERROR(error_code, "Failed to attach to the user");

	error_code = kaa_client_start(kaa_client, external_process, NULL, 1);
	KAA_RETURN_IF_ERROR(error_code, "Failed to start Kaa main loop");

	kaa_client_destroy(kaa_client);
	return error_code;
}
