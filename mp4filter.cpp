#include "mp4filter.h"
#include "mp4runnable.h"

QVideoFilterRunnable *Mp4Filter::createFilterRunnable()
{
    return new Mp4Runnable(this);
}


