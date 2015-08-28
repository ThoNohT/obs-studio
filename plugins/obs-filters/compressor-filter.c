#include <media-io/audio-math.h>
#include <obs-module.h>
#include <math.h>

#define do_log(level, format, ...) \
	blog(level, "[compressor: '%s'] " format, \
			obs_source_get_name(ng->context), ##__VA_ARGS__)

#define warn(format, ...)  do_log(LOG_WARNING, format, ##__VA_ARGS__)
#define info(format, ...)  do_log(LOG_INFO,    format, ##__VA_ARGS__)

#define S_THRESHOLD                    "threshold"
#define S_RATIO                        "ratio"
#define S_IS_EXPANDER                  "is_expander"

#define MT_ obs_module_text
#define TEXT_THRESHOLD                 MT_("Compressor.Threshold")
#define TEXT_RATIO                     MT_("Compressor.Ratio")
#define TEXT_IS_EXPANDER               MT_("Compressor.IsExpander")

struct compressor_data {
	obs_source_t *context;

	size_t channels;

	float threshold;
	float ratio;
	bool is_expander;
};

#define VOL_MIN -96.0f
#define VOL_MAX 0.0f

static const char *compressor_name(void)
{
	return obs_module_text("Compressor");
}

static void compressor_destroy(void *data)
{
	struct compressor_data *cd = data;
	bfree(cd);
}

static void compressor_update(void *data, obs_data_t *s)
{
	struct compressor_data *cd = data;

	cd->channels = audio_output_get_channels(obs_get_audio());
	cd->threshold = db_to_mul((float)obs_data_get_double(s, S_THRESHOLD));
	cd->ratio = (float)obs_data_get_double(s, S_RATIO);
	cd->is_expander = (float)obs_data_get_bool(s, S_IS_EXPANDER);
}

static void *compressor_create(obs_data_t *settings, obs_source_t *filter)
{
	struct compressor_data *cd = bzalloc(sizeof(*cd));
	cd->context = filter;
	compressor_update(cd, settings);
	return cd;
}

static struct obs_audio_data *compressor_filter_audio(void *data,
		struct obs_audio_data *audio)
{
	struct compressor_data *cd = data;

	const size_t channels = cd->channels;
	const float thres = cd->threshold;
	const float ratio = cd->ratio;
	const bool expand = cd->is_expander;

	const size_t frame_count = audio->frames;

	for (size_t c = 0; c < channels; ++c) {
		float *adata = (float*)audio->data[c];
		for (size_t i = 0; i < frame_count; i++) {
			float level = fabsf(adata[i]);
			float old = adata[i];
			if (level == 0) continue;
			float over = level - thres;

			if (over > 0.0f)
			{
				if (expand)
					adata[i] = thres + (ratio * over); // Apply expander.
				else
					adata[i] = thres + ((1 / ratio) * over); // Apply compressor.

				if (old < 0.0f) adata[i] *= -1.0f; // Correct phase.
			}
		}
	}
	
	return audio;
}

static void compressor_defaults(obs_data_t *s)
{
	obs_data_set_default_double(s, S_THRESHOLD, VOL_MAX);
	obs_data_set_default_double(s, S_RATIO, 1.0f);
	obs_data_set_default_bool(s, S_IS_EXPANDER, false);
}

static obs_properties_t *compressor_properties(void *data)
{
	obs_properties_t *ppts = obs_properties_create();

	obs_properties_add_float_slider(ppts, S_THRESHOLD,
			TEXT_THRESHOLD, VOL_MIN, VOL_MAX, 1.0f);
	obs_properties_add_float_slider(ppts, S_RATIO,
			TEXT_RATIO, 1, 100, 0.1f);
	obs_properties_add_bool(ppts, S_IS_EXPANDER, TEXT_IS_EXPANDER);

	UNUSED_PARAMETER(data);
	return ppts;
}

struct obs_source_info compressor_filter = {
	.id = "compressor_filter",
	.type = OBS_SOURCE_TYPE_FILTER,
	.output_flags = OBS_SOURCE_AUDIO,
	.get_name = compressor_name,
	.create = compressor_create,
	.destroy = compressor_destroy,
	.update = compressor_update,
	.filter_audio = compressor_filter_audio,
	.get_defaults = compressor_defaults,
	.get_properties = compressor_properties,
};
