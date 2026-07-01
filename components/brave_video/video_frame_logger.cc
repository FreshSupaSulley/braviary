// Copyright (c) 2024 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "brave/components/brave_video/video_frame_logger.h"

#include "base/logging.h"
#include "media/base/video_types.h"

namespace brave {

VideoFrameLogger::VideoFrameLogger() = default;
VideoFrameLogger::~VideoFrameLogger() = default;

void VideoFrameLogger::OnFrame(scoped_refptr<media::VideoFrame> frame) {
  ++frame_count_;
  LOG(INFO) << "[BraveVideo] Frame #" << frame_count_
            << " | " << frame->coded_size().ToString()
            << " | visible=" << frame->visible_rect().ToString()
            << " | ts=" << frame->timestamp()
            << " | fmt=" << media::VideoPixelFormatToString(frame->format());
}

}  // namespace brave
