#include <obs-module.h>
#include "automation-handler.hpp"
#include <util/threading.h>

// The plug-in.
AutomationHandler *automationHandler;

/* Module declaration. */

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

/* End of module declaration. */

/* OBS signals. */

void channel_changed(void *data, calldata_t *params) {
	automationHandler->ChannelChanged(data, params);
}

/* End of OBS signals. */

/* Processing thread. */
bool stop;
pthread_t processingThread;
int processingTimeout;

void *process(void*) {
	while (!stop)
	{
		automationHandler->Process();
		_sleep(processingTimeout);
	}
	return NULL;
}

bool start_processing() {
	stop = false;

	if (pthread_create(&processingThread, NULL, process, NULL))
	{
		warn("Failed to create the processing thread.");
		return false;
	}

	info("Created the processing thread.");
	return true;
}

void stop_processing() {
	stop = true;
	if (pthread_join(processingThread, NULL)) {
		warn("Failed to stop the processing thread.");
		return;
	}

	info("Stopped the processing thread.");
}

/* End of processing thread. */

/* OBS callbacks. */

bool obs_module_load()
{
	info("Starting plug-in.");

	automationHandler = new AutomationHandler();
	automationHandler->LoadSettings(
		obs_get_module_data_path(obs_current_module()));
	processingTimeout = automationHandler->GetTimeout();

	// Connect all relevant signals from OBS.
	signal_handler_connect(obs_get_signal_handler(), "channel_change",
		channel_changed, NULL);

	return start_processing();
}

void obs_module_unload()
{
	info("Stopping plug-in.");

	stop_processing();
	delete automationHandler;
}

/* End of OBS callbacks. */