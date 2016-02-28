/*
 * In The Name Of God
 * ========================================
 * [] File Name : event.c
 *
 * [] Creation Date : 28-02-2016
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
/*
 * Copyright (c) 2016 Parham Alvani.
*/
#include <stdint.h>
#include <stdio.h>

#include <kaa/kaa_event.h>
#include <kaa/gen/kaa_led_event_class_family.h>

#include "event.h"
#include "util.h"

static void on_led_event_class_family_led_on_event(void *context,
		kaa_led_event_class_family_led_on_event_t *event, kaa_endpoint_id_p source)
{
	int r, g, b;
	
	r = event->red;
	g = event->green;
	b = event->blue;

	printf("%d %d %d\n", r, g, b);

	event->destroy(event);
}

kaa_error_t kaa_event_register(kaa_event_manager_t *manager)
{
	kaa_error_t error_code;
	
	error_code = kaa_event_manager_set_kaa_led_event_class_family_led_on_event_listener(manager,
			on_led_event_class_family_led_on_event, NULL);
	KAA_RETURN_IF_ERROR(error_code, "Failed add event listener");

	return error_code;
}
