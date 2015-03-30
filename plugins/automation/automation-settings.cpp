#include "automation-settings.hpp"

AutomationSettings::AutomationSettings(obs_data_t *settingsData) {
	this->setDefaults(settingsData);

	this->x = obs_data_get_string(settingsData, "x");
}

AutomationSettings::~AutomationSettings() {
}

obs_data_t *AutomationSettings::GetSettingsData() {
	obs_data_t *settingsData = obs_data_create();

	obs_data_set_string(settingsData, "x", this->x);

	return settingsData;
}

void AutomationSettings::setDefaults(obs_data_t *settingsData) {
	obs_data_set_default_string(settingsData, "x", "Initial x");
}

const char* AutomationSettings::GetX() {
	return this->x;
}

void AutomationSettings::SetX(const char* value) {
	this->x = value;
}