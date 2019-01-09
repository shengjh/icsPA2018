#include <am.h>
#include <x86.h>
#include <amdev.h>
#include <klib.h>

int W,H;
static uint32_t* const fb __attribute__((used)) = (uint32_t *)0x40000;

static inline int min(int x, int y) {
  return (x < y) ? x : y;
	}

size_t video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _VideoInfoReg *info = (_VideoInfoReg *)buf;
      uint32_t screen;
      screen = inl(0x100);
      W=info->width = screen >> 16;
      H=info->height = screen << 16 >> 16;
      return sizeof(_VideoInfoReg);
    }
  }
  return 0;
}

size_t video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
      _FBCtlReg *ctl = (_FBCtlReg *)buf;
			
			int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
		  uint32_t *pixels = ctl->pixels;
		  int cp_bytes = sizeof(uint32_t) * min(w, W - x);
		  for (int j = 0; j < h && y + j < H; j ++) {
		      memcpy(&fb[(y + j) * W + x], pixels, cp_bytes);
		      pixels += w;
		  }
      if (ctl->sync) {
        // do nothing, hardware syncs.
      }
      return sizeof(_FBCtlReg);
    }
  }
  return 0;
}

void vga_init() {
}
