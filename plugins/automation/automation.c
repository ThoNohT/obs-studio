#include <obs-module.h>
#include <util/threading.h>

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

pthread_t processing_thread;
bool stop;

const char *obs_module_name()
{
	return "OBS Automation";
}

const char *obs_module_description()
{
	return "Provides several automation options for obs-studio.";
}

void process(void)
{
	while (!stop) {
		info("Processing.");

		_sleep(1000);
	}

	info("Processing complete.");
}

bool obs_module_load(void)
{
	info("Starting plugin.");

	stop = false;
	if (pthread_create(&processing_thread, NULL, process, NULL))
	{
		warn("Failed to create the processing thread.");
		return false;
	}

	return true;
}

bool obs_module_unload(void)
{
	info("Stopping plugin.");
	
	stop = true;
	if(pthread_join(processing_thread, NULL)) {
		warn("Failed to stop the processing thread.");
		return false;
	}

	return true;
}
