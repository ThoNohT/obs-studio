#include "automation-handler.hpp"
#include <obs-scene.h>
#include <util/threading.h>
#include <util/util.hpp>

AutomationHandler::AutomationHandler() {

}

AutomationHandler::~AutomationHandler() {
	this->saveSettings();
	obs_data_release(this->settings);
}

void AutomationHandler::LoadSettings(const char *filepath) {
	std::string filename = "/settings.json";
	this->settingsFilename = filepath + filename;

	info("Loading settings from: %s", this->settingsFilename.c_str());

	BPtr<char> jsonText = os_quick_read_utf8_file(
		this->settingsFilename.c_str());
	if (!jsonText)
	{
		this->settings = obs_data_create();
	}
	else {
		this->settings = obs_data_create_from_json(jsonText);
	}

	const char *x = obs_data_get_string(this->settings, "x");
	if (strcmp(x, "") == 0)
		obs_data_set_string(this->settings, "x", "initial value.");

	x = obs_data_get_string(this->settings, "x");

	info("X is %s", x);
}

int AutomationHandler::GetTimeout()
{
	return 1000;
}

void AutomationHandler::ChannelChanged(void *data, calldata_t *params)
{
	const obs_source_t *source =
		(obs_source_t*)calldata_ptr(params, "source");
	if (source == NULL) return;

	uint32_t channel = (uint32_t)calldata_int(params, "channel");

	info("Channel changed: '%s', channel: %d",
		obs_source_get_name(source), channel);

	UNUSED_PARAMETER(data);
}

// Private static

void AutomationHandler::Process()
{
	info("Processing.");
}

// Private

void AutomationHandler::setScene(const char *name)
{
	obs_source_t *source = obs_get_source_by_name(name);
	if (source == NULL)
	{
		warn("Source not found: \"%s\"", name);
		return;
	}

	if (obs_scene_from_source(source) == NULL)
	{
		warn("\"%s\" is not a scene", name);
		return;
	}

	obs_set_output_source(0, source);
	obs_source_release(source);

	info("Changed to scene \"%s\"", name);
}

void AutomationHandler::saveSettings()
{
	if (this->settings == NULL)
		return;

	const char *jsonData = obs_data_get_json(this->settings);

	if (!!jsonData) {
		bool success = os_quick_write_utf8_file(
			this->settingsFilename.c_str(), jsonData,
			strlen(jsonData), false);

		if (!success)
			warn("Could not save settings to %s",
				this->settingsFilename.c_str());
	}
}