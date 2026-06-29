#ifndef BRAVE_COMPONENTS_BRAVE_VIDEO_VIDEO_FRAME_LOGGER_H_
#define BRAVE_COMPONENTS_BRAVE_VIDEO_VIDEO_FRAME_LOGGER_H_

#include "media/base/video_frame_observer.h"
#include "base/memory/ref_counted.h"

namespace brave {

class VideoFrameLogger : public media::VideoFrameObserver {
 public:
  VideoFrameLogger();
  ~VideoFrameLogger() override;

  void OnFrame(scoped_refptr<media::VideoFrame> frame) override;

 private:
  int frame_count_ = 0;
};

}  // namespace brave
#endif  // BRAVE_COMPONENTS_BRAVE_VIDEO_VIDEO_FRAME_LOGGER_H_