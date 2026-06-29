# VideoFrameObserver Implementation Patch

## Overview
This patch adds a minimal VideoFrameObserver interface to Chromium's media layer and implements a VideoFrameLogger in Brave for logging video frame metadata without copying pixel data.

## Files Added

### 1. `src/media/base/video_frame_observer.h`
- Defines the `VideoFrameObserver` interface
- Located in Chromium (minimal change)
- Receives `scoped_refptr<VideoFrame>` for each frame

### 2. `src/brave/components/brave_video/video_frame_logger.h`
- Implements `VideoFrameObserver` in Brave
- Logs frame metadata: size, timestamp, format

### 3. `src/brave/components/brave_video/video_frame_logger.cc`
- Implementation details
- Frame counter increments per frame
- Outputs via VLOG(1) for low-overhead logging

### 4. `src/brave/components/brave_video/BUILD.gn`
- GN build configuration
- Links against `//base` and `//media`

## Integration Points

To wire this observer into the video renderer pipeline:

### In `src/content/renderer/media/render_media_client.cc`
Add observer creation during renderer initialization:

```cpp
#include "brave/components/brave_video/video_frame_logger.h"

// In renderer initialization:
auto video_logger = base::MakeRefCounted<brave::VideoFrameLogger>();
video_sink->AddFrameObserver(video_logger);
```

### In Chromium's video sink/renderer
Call observer for each frame received:

```cpp
void VideoRenderer::OnFrameReady(scoped_refptr<media::VideoFrame> frame) {
  // Existing rendering logic...
  
  // Call observer (no-op if not set)
  if (frame_observer_) {
    frame_observer_->OnFrame(frame);
  }
}
```

## Build Instructions

```bash
# 1. Sync dependencies
npm run sync -- --init

# 2. Generate build files
npm run gn -- gen out/Default

# 3. Build Brave
npm run build

# 4. Run with verbose logging
./out/Default/brave --enable-logging --v=1
```

## Expected Output

When playing a YouTube video:

```
Frame #1 | Size: 1920x1080 | Timestamp: 00:00:00.000000 | Format: YV12
Frame #2 | Size: 1920x1080 | Timestamp: 00:00:00.033333 | Format: YV12
Frame #3 | Size: 1920x1080 | Timestamp: 00:00:00.066667 | Format: YV12
...
```

## Key Design Points

✅ **No pixel copying** - Uses `scoped_refptr<VideoFrame>` for reference-counted access
✅ **Minimal Chromium changes** - Single header in `media/base/`
✅ **Brave-specific implementation** - Main logic in `brave/components/`
✅ **Low overhead** - VLOG(1) only when logging enabled
✅ **Automatic cleanup** - RefCounting handles observer lifetime

## Frame Metadata Available

- `frame->timestamp()` - Presentation timestamp
- `frame->coded_size()` - Full encoded dimensions (e.g., 1920x1080)
- `frame->visible_rect()` - Visible portion of frame
- `frame->format()` - Pixel format (YV12, NV12, I420, etc.)
- `frame->color_space()` - Color space information
- `frame->data(plane)` - Raw pixel data (not used in this implementation)
