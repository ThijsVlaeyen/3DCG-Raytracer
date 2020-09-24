#include "demos/demo.h"
#include "tasks/task-schedulers.h"
#include "math/functions.h"

using namespace raytracer;
using namespace animation;
using namespace math;
using namespace imaging;


demos::Demo::Demo(unsigned bitmap_size, Duration duration, unsigned fps, unsigned antialias )
  : m_bitmap_size(bitmap_size), m_duration(duration), m_fps(fps), m_antialias(antialias)
{
    // NOP
}

void demos::Demo::render(std::shared_ptr<raytracer::pipeline::Consumer<std::shared_ptr<imaging::Bitmap>>> consumer)
{
    auto scene_animation = create_scene_animation();
    auto renderer = create_renderer();

    pipeline::start(create_scene_animation())
        >> pipeline::animation(m_fps)
        >> pipeline::renderer(renderer)
        >> consumer;
}

RayTracer demos::Demo::create_ray_tracer()
{
    return raytracer::raytracers::v1();
}

Renderer demos::Demo::create_renderer()
{
    return raytracer::renderers::standard(m_bitmap_size, m_bitmap_size, create_sampler(), create_ray_tracer(), create_scheduler());
}

Sampler demos::Demo::create_sampler()
{
    return raytracer::samplers::single();
}

tasks::TaskScheduler demos::Demo::create_scheduler()
{
    return tasks::schedulers::serial();
}

Animation<std::shared_ptr<Scene>> demos::Demo::create_scene_animation()
{
    std::function<std::shared_ptr<Scene>(TimeStamp)> lambda = [this](TimeStamp now) {
        auto camera = create_camera(now);
        auto root = create_root(now);
        auto light_sources = create_light_sources(now);
        auto scene = std::make_shared<Scene>(camera, root, light_sources);

        return scene;
    };

    auto function = from_lambda(lambda);

    return make_animation<std::shared_ptr<Scene>>(function, m_duration);
}
