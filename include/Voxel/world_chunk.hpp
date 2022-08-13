// Author: Zackery Mason-Blaug
// Date: 2022-08-12
//////////////////////////////////////////////////////////


#pragma once

#include "core.hpp"

#include "Graphics/buffer.hpp"
#include "Graphics/vertex_array.hpp"

#include "Util/random.hpp"
#include "Util/logger.hpp"

struct voxel_t {
    int     flag{0};
};

template <size_t W, size_t H, size_t D>
struct world_chunk_t {    
    struct vertex_t {
        v3f position{0.0f};
        v3f normal{0.0f, 1.0f, 0.0f};
        v2f uv{0.0f};
    };

    template <typename T>
    using chunk_array_t = std::vector<T>;

    buffer_t<chunk_array_t<vertex_t>> buffer;
    vertex_array_t vertex_array;
    chunk_array_t<voxel_t> voxels{};

    world_chunk_t()
        : buffer(), vertex_array(W*H*D*6*6)
    {
        voxels.reserve(W*H*D);
        voxels.resize(W*H*D);
     
        for (int x = 0; x < W; x++) {
            for (int y = 0; y < H; y++) {
                for (int z = 0; z < D; z++) {
                    get_voxel(x,y,z).flag = random_s::rand() % 2;
                    if (get_voxel(x,y,z).flag) {
                        emit_cube(v3f{x,y,z});
                    }
                }
            }
        }

        buffer.create();

        vertex_array.bind_ref()
            .set_attrib(0, 3, GL_FLOAT, sizeof(vertex_t), offsetof(vertex_t, position))
            .set_attrib(1, 3, GL_FLOAT, sizeof(vertex_t), offsetof(vertex_t, normal))
            .set_attrib(2, 2, GL_FLOAT, sizeof(vertex_t), offsetof(vertex_t, uv))
            .unbind();


        buffer.update_buffer();
        buffer.unbind();
    }

    void draw() {
        vertex_array.draw();
    }

    voxel_t& get_voxel(auto x, auto y, auto z) {
        return voxels[x + y*(W*H) + z * (W)];
    }

    voxel_t& get_voxel(v3i p) {
        return get_voxel(p.x, p.y, p.z);
    }

    void emit_cube(const v3f& pos) {
        emit_face(pos, {1,0,0});
        emit_face(pos, {-1,0,0});
        emit_face(pos, {0,1,0});
        emit_face(pos, {0,-1,0});
        emit_face(pos, {0,0,1});
        emit_face(pos, {0,0,-1});
    }

    void emit_face(const v3f& pos, const v3f& dir) {
        const v3f tangent = v3f{dir.y, dir.z, dir.x} * 0.45f;
        const v3f bitangent = glm::normalize(glm::cross(dir, tangent)) * 0.45f;

        const auto emit = [&](float i, float j) {
            vertex_t vertex;
            vertex.position = pos + dir * 0.45f + tangent * i + bitangent * j;
            vertex.normal = dir;
            vertex.uv = v2f{ i < 0 ? 0 : i, j < 0 ? 0 : j};
            buffer.data.push_back(vertex);
        };

        emit(1,1);
        emit(1,-1);
        emit(-1, 1);

        emit(1,-1);
        emit(-1,1);
        emit(-1,-1);

        vertex_array.size = static_cast<GLsizei>(buffer.data.size());
    }
};

