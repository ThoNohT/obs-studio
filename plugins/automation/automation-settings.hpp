#include <obs-data.h>

class AutomationSettings {

public:
	AutomationSettings(obs_data_t *settingsData);
	~AutomationSettings();

	obs_data_t *GetSettingsData();

	const char *GetX();
	void SetX(const char *value);

private:
	const char *x;

	void setDefaults(obs_data_t *settingsData);
};