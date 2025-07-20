#pragma once

#include <stdint.h>
#include <stddef.h>

#define MAX_PHYS_MEM                    (4ULL * 1024 * 1024 * 1024)
#define FRAME_SIZE                      4096
#define MAX_FRAMES                      (MAX_PHYS_MEM / FRAME_SIZE)

#define FRAME_FREE                      0x1
#define FRAME_USED                      0x2
#define FRAME_RESERVED                  0x4

#define FRAME_IS_FREE(f)                ((f)->flags & FRAME_FREE)
#define FRAME_SET_FREE(f)               ((f)->flags = ((f)->flags & ~FRAME_USED) | FRAME_FREE)
#define FRAME_SET_USED(f)               ((f)->flags = ((f)->flags & ~FRAME_FREE) | FRAME_USED)
#define FRAME_SET_RESERVED(f)           ((f)->flags = ((f)->flags & ~(FRAME_FREE | FRAME_USED)) | FRAME_RESERVED)

/*
    Structure of a physical frame

    Fields:
        flags - the frame flags, defined above
        ref_count - the references count of the frame
*/
typedef struct
{
    uint8_t flags;
    uint16_t ref_count;
} Frame;

void init_frame(Frame *frames, size_t count);
Frame *alloc_frame(Frame *frames, size_t count);
void free_frame(Frame *frame);
void inc_frame_ref(Frame *frame);
void dec_frame_ref(Frame *frame);