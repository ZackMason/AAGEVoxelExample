
#define WIN32_MEAN_AND_LEAN
#define NOMINMAX

#include "core.hpp"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Engine/window.hpp"
#include "Engine/orbit_camera.hpp"
#include "Engine/asset_loader.hpp"

#include "Graphics/shared_buffer.hpp"

#include "Math/transform.hpp"

#include "Voxel/world_chunk.hpp"

static constexpr v3f sky_color{0.3f, 0.2f, 1.0f};

struct game_t {
    orbit_camera_t camera{45.0f, 1.0f, 1.0f, 0.1f, 100.0f};
    world_chunk_t<32,32,32> current_chunk{};
    //world_chunk_t<64,128,64> current_chunk{};
    asset_loader_t asset_loader{};

};

struct camera_u { 
    using value_type = camera_u;
    void* data() {
        return &p;
    }

    size_t size() const {
        return 1;
    }

    v4f p;
    m44 vp;
};

int main(int argc, char** argv) {
    random_s::randomize();

    window_t window;
    window.width = 640;
    window.height = 480;
    window.open_window();

    window.set_title("AAGE Template");

    game_t game{};
    game.asset_loader.asset_dir = GAME_ASSETS_PATH;

    window.set_event_callback([&](auto& event){
        event_handler_t handler;

        handler.dispatch<mouse_scroll_event_t>(event, 
            std::bind(&orbit_camera_t::on_scroll_event, &game.camera, std::placeholders::_1));

        handler.dispatch<key_event_t>(event, [&](const key_event_t& e) {
            switch(e.key) {
                case GLFW_KEY_R:
                    game.asset_loader.reload<shader_t>();
                    break;
            }
            return true;
        });
    });



    shared_buffer_t<camera_u> camera_buffer{
        create_shared_buffer(camera_u{v4f{game.camera.get_position(), 0.0f}, game.camera.view_projection()})
    };
    bind_buffer(camera_buffer, 0);

    resource_handle_t<shader_t> world_chunk_shader;
    try {
        world_chunk_shader = game.asset_loader.get_shader("world_chunk", {
            "shaders/chunk.vs", "shaders/chunk.fs"
        });
    }
    catch (std::exception& e) {
        logger_t::error(fmt::format("Shader exception: {}", e.what()));
        std::terminate();
    }

    logger_t::info("Init Completed");

    glEnable(GL_DEPTH_TEST);

    while(!window.should_close()) {
        glClearColor(sky_color.r, sky_color.g, sky_color.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /////////////////////////////
        // Update 
        game.camera.update(window, 1.0f / 120.0f);

        /////////////////////////////
        // Render

        // update camera
        camera_u camera{v4f{game.camera.get_position(),0.0f}, game.camera.view_projection()};
        set_shared_buffer(camera_buffer, &camera);

        // draw world
        world_chunk_shader.get().bind();
        world_chunk_shader.get().set_mat4("uM", m44{1.0f});

        game.current_chunk.draw();


        /////////////////////////////
        window.poll_events();
        window.swap_buffers();
    }

    

    return 0;
}