#include "videofilter.h"
#include "filterrunnable.h"

QVideoFilterRunnable *VideoFilter::createFilterRunnable()
{
    return new FilterRunnable(this);
}



