#include <obs-scene.h>
#include <string>
#include <obs-data.h>
#include "automation-settings.hpp"

#define blog(log_level, format, ...) \
	blog(log_level, "[automation] " format, ##__VA_ARGS__)

#define debug(format, ...) \
	blog(LOG_DEBUG, format, ##__VA_ARGS__)
#define info(format, ...) \
	blog(LOG_INFO, format, ##__VA_ARGS__)
#define warn(format, ...) \
	blog(LOG_WARNING, format, ##__VA_ARGS__)
	

class AutomationHandler {

public:
	AutomationHandler();
	~AutomationHandler();

	void LoadSettings(const char *filepath);
	int GetTimeout();

	void ChannelChanged(void *data, calldata_t *params);
	void Process();

private:
	AutomationSettings *settings;
	std::string settingsFilename;

	void saveSettings();

	void setScene(const char *name);
};