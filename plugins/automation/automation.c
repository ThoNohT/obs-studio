#include <obs-module.h>

#define blog(log_level, format, ...) \
	blog(log_level, "[automation] " format, ##__VA_ARGS__)

#define debug(format, ...) \
	blog(LOG_DEBUG, format, ##__VA_ARGS__)
#define info(format, ...) \
	blog(LOG_INFO, format, ##__VA_ARGS__)
#define warn(format, ...) \
	blog(LOG_WARNING, format, ##__VA_ARGS__)

OBS_DECLARE_MODULE()

OBS_MODULE_USE_DEFAULT_LOCALE("automation", "en-US")

OBS_MODULE_AUTHOR("ThoNohT")

const char *obs_module_name()
{
	return "OBS Automation";
}

const char *obs_module_description()
{
	return "Provides several automation options for obs-studio.";
}

bool obs_module_load(void)
{
	info("Starting plugin.");

	return true;
}

bool obs_module_unload(void)
{
	info("Stopping plugin.");

	return true;
}
