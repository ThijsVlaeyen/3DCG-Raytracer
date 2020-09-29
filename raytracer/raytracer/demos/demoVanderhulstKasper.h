#pragma once

#include "pipeline/pipelines.h"
#include "imaging/bitmap.h"
#include <memory>

namespace demos {
    void demoVanderhulstKasper(std::shared_ptr<raytracer::pipeline::Consumer<std::shared_ptr<imaging::Bitmap>>> output);
}
