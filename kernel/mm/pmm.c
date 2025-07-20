#include "mm/include/pmm.h"

void init_frame(Frame *frames, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        frames[i].flags = FRAME_RESERVED;
        frames[i].ref_count = 0;
    }
}

Frame *alloc_frame(Frame *frames, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        if (frames[i].flags & FRAME_FREE)
        {
            FRAME_SET_USED(&frames[i]);
            frames[i].ref_count = 1;
            return &frames[i];
        }
    }

    return NULL;
}

void free_frame(Frame *frame)
{
    if (frame->ref_count > 0)
        frame->ref_count--;

    if (frame->ref_count == 0)
        FRAME_SET_FREE(frame);
}

void inc_frame_ref(Frame *frame)
{
    frame->ref_count++;
}

void dec_frame_ref(Frame *frame)
{
    if (frame->ref_count > 0)
        frame->ref_count--;

    if (frame->ref_count == 0)
        FRAME_SET_FREE(frame);
}