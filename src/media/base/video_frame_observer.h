#ifndef MEDIA_BASE_VIDEO_FRAME_OBSERVER_H_
#define MEDIA_BASE_VIDEO_FRAME_OBSERVER_H_

#include "base/memory/ref_counted.h"
#include "media/base/video_frame.h"

namespace media {

class VideoFrameObserver {
 public:
  virtual ~VideoFrameObserver() = default;
  virtual void OnFrame(scoped_refptr<VideoFrame> frame) = 0;
};

}  // namespace media
#endif  // MEDIA_BASE_VIDEO_FRAME_OBSERVER_H_