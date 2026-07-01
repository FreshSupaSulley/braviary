// Copyright (c) 2024 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef BRAVE_COMPONENTS_BRAVE_VIDEO_VIDEO_FRAME_LOGGER_H_
#define BRAVE_COMPONENTS_BRAVE_VIDEO_VIDEO_FRAME_LOGGER_H_

#include "media/base/video_frame_observer.h"

namespace brave {

class VideoFrameLogger : public media::VideoFrameObserver {
 public:
  VideoFrameLogger();
  ~VideoFrameLogger() override;

  void OnFrame(scoped_refptr<media::VideoFrame> frame) override;

 private:
  uint64_t frame_count_ = 0;
};

}  // namespace brave

#endif  // BRAVE_COMPONENTS_BRAVE_VIDEO_VIDEO_FRAME_LOGGER_H_
