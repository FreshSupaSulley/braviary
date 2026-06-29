# VideoFrameLogger Integration Guide

## Overview

This document describes how to integrate the `VideoFrameLogger` with Chromium's `VideoRenderer` to capture and log video frame metadata.

## Integration Steps

### Step 1: Create VideoFrameLogger Instance

In the renderer initialization code where `VideoRenderer` is created, instantiate the logger:

```cpp
// File: src/content/renderer/media/render_media_client.cc (or similar initialization)

#include "brave/components/brave_video/video_frame_logger.h"
#include "media/renderers/video_renderer.h"

// During initialization:
auto video_logger = base::MakeRefCounted<brave::VideoFrameLogger>();
```

### Step 2: Attach Observer to VideoRenderer

After creating the `VideoRenderer`, attach the observer:

```cpp
// Create VideoRenderer
auto video_renderer = std::make_unique<media::VideoRenderer>(
    media_task_runner,
    video_decoder_selector,
    ...);

// Attach frame observer (logging)
video_renderer->SetFrameObserver(video_logger);

// Continue with pipeline setup...
```

### Step 3: Verify Output

When YouTube video is played with frame logging enabled:

```
Frame #1 | Size: 1920x1080 | Timestamp: 00:00:00.000000 | Format: YV12
Frame #2 | Size: 1920x1080 | Timestamp: 00:00:00.033333 | Format: YV12
Frame #3 | Size: 1920x1080 | Timestamp: 00:00:00.066667 | Format: YV12
...
```

## Architecture

```
VideoRenderer::OnFrameReady(frame)
    ↓
VideoFrameObserver::OnFrame(frame)  ← Called here
    ↓
VideoFrameLogger::OnFrame(frame)    ← Logs metadata
    ↓
VLOG(1) << "Frame #N | Size: ... | Timestamp: ... | Format: ..."
```

## Key Points

- **No pixel copying**: The `scoped_refptr<VideoFrame>` provides only reference-counted access to metadata
- **Low overhead**: VLOG(1) calls only occur when `--v=1` or higher is specified
- **Multiple observers**: Can attach multiple observers if needed (extend the implementation to support multiple subscribers)
- **Automatic cleanup**: RefCounting ensures proper lifetime management

## Frame Metadata Available

The `VideoFrame` object passed to the observer provides:

- `timestamp()` → Presentation timestamp (base::TimeDelta)
- `coded_size()` → Full encoded dimensions (gfx::Size, e.g., 1920x1080)
- `visible_rect()` → Visible portion (gfx::Rect)
- `format()` → Pixel format enum (YV12, NV12, I420, etc.)
- `color_space()` → Color space information (gfx::ColorSpace)
- `data(plane)` → Raw pixel data pointer (NOT used in this logger to avoid copies)

## Build Process

```bash
# Sync and initialize
npm run sync -- --init

# Generate build configuration
npm run gn -- gen out/Default

# Build
npm run build

# Test with logging
./out/Default/brave --enable-logging --v=1
```

## Expected Build Output

When building with these patches, you should see:

```
[1234/5678] CXX obj/media/renderers/video_renderer.o
[1235/5678] SOLINK obj/libmedia.so
```

No errors should occur since the changes are backward compatible (observer is optional).

## Troubleshooting

### No logs appearing?

1. Verify `--enable-logging --v=1` flags are set
2. Check that YouTube video is actually playing
3. Ensure frame_observer is being set (add a DCHECK in VideoRenderer::SetFrameObserver)

### Build errors?

1. Ensure `#include "media/base/video_frame_observer.h"` is at the top of files
2. Verify `brave/components/brave_video/BUILD.gn` has correct deps
3. Check that VideoFrame enum methods match your Chromium version

## Future Enhancements

- Support multiple observers (use callback list instead of single pointer)
- Add frame filtering (log only certain formats or sizes)
- Implement frame capture (save YUV frames to disk)
- Add performance profiling (frame processing latency)
