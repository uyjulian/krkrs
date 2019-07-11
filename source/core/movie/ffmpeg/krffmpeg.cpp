
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavfilter/avfilter.h"
}

static bool FFInitilalized = false;
void TVPInitLibAVCodec() {
	if (!FFInitilalized) {
		/* register all codecs, demux and protocols */
		// ffmpeg needs to build with thread support instead of using this function
		// if (av_lockmgr_register(lockmgr)) {
		// 	TVPThrowExceptionMessage(TJS_W("Could not initialize lock manager!"));
		// }
		// official deprecation
		// avcodec_register_all();
		// official deprecation
		// av_register_all();
		// copy paste function next
		// avfilter_register_all();
	    AVFilter *prev = NULL, *p;
	    void *i = 0;
	    while ((p = (AVFilter*)av_filter_iterate(&i))) {
	        if (prev)
	            prev->next = p;
	        prev = p;
	    }
		avformat_network_init();
	//	av_log_set_callback(ff_avutil_log);
		FFInitilalized = true;
	}
}
