# VideoFrameLogger Integration Guide

## Hook Point: VideoFrameCompositor

The observer attaches at `VideoFrameCompositor` — the single point where every
displayed frame passes from the media pipeline to the compositor.

File: `src/media/renderers/video_frame_compositor.h`

## Chromium-side patch (minimal)

### video_frame_compositor.h

```cpp
#include "media/base/video_frame_observer.h"
#include "base/memory/raw_ptr.h"

class MEDIA_EXPORT VideoFrameCompositor : ... {
 public:
  // Add:
  void SetFrameObserver(VideoFrameObserver* observer);

 private:
  // Add:
  raw_ptr<VideoFrameObserver> frame_observer_ = nullptr;
};
```

### video_frame_compositor.cc

In `UpdateCurrentFrame()`, after `current_frame_` is set:

```cpp
void VideoFrameCompositor::SetFrameObserver(VideoFrameObserver* observer) {
  frame_observer_ = observer;
}

// Inside UpdateCurrentFrame(), at the end:
if (frame_observer_ && current_frame_) {
  frame_observer_->OnFrame(current_frame_);
}
```

## Brave-side wiring

### chromium_src override (preferred)

Create: `src/brave/chromium_src/third_party/blink/renderer/platform/media/web_media_player_impl.cc`

Use Brave's preprocessor-based override pattern to inject the observer
during `WebMediaPlayerImpl` construction:

```cpp
#include "brave/components/brave_video/video_frame_logger.h"

// Override CreateVideoFrameCompositor or post-construction hook:
// After compositor_ is created:
static brave::VideoFrameLogger g_frame_logger;
compositor_->SetFrameObserver(&g_frame_logger);
```

For a proper implementation, the logger lifetime should be tied to
the `WebMediaPlayerImpl` instance (use a member unique_ptr).

### Alternative: direct patch to WebMediaPlayerImpl

If `chromium_src` override is too complex for the compositor wiring:

```diff
--- a/third_party/blink/renderer/platform/media/web_media_player_impl.cc
+++ b/third_party/blink/renderer/platform/media/web_media_player_impl.cc
@@ -XXX,6 +XXX,10 @@
+#include "brave/components/brave_video/video_frame_logger.h"
+
 WebMediaPlayerImpl::WebMediaPlayerImpl(...) {
   ...
   compositor_ = ...;
+  frame_logger_ = std::make_unique<brave::VideoFrameLogger>();
+  compositor_->SetFrameObserver(frame_logger_.get());
   ...
 }
```

## Build

Add dep to `//brave/browser:browser` or the appropriate top-level target:

```gn
deps += [ "//brave/components/brave_video" ]
```

## Test

```bash
npm run build -- Debug
npm start -- Debug --args --enable-logging=stderr --v=1 --vmodule="video_frame_logger*=1"
```

Play YouTube → terminal shows:
```
Frame #1 | 1920x1080 | visible=0,0 1920x1080 | ts=0:00:00 | fmt=I420
Frame #2 | 1920x1080 | visible=0,0 1920x1080 | ts=0:00:00.033 | fmt=I420
```
