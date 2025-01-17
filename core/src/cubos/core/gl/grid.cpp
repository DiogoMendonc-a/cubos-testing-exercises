#include <unordered_map>

#include <cubos/core/gl/grid.hpp>
#include <cubos/core/gl/palette.hpp>
#include <cubos/core/log.hpp>

using namespace cubos::core::gl;

Grid::Grid(const glm::uvec3& size)
{
    if (size.x < 1 || size.y < 1 || size.z < 1)
    {
        CUBOS_WARN("Grid size must be at least 1 in each dimension: was ({}, {}, {}), defaulting to (1, 1, 1).", size.x,
                   size.y, size.z);
        mSize = {1, 1, 1};
    }
    else
    {
        mSize = size;
    }

    mIndices.resize(
        static_cast<std::size_t>(mSize.x) * static_cast<std::size_t>(mSize.y) * static_cast<std::size_t>(mSize.z), 0);
}

Grid::Grid(const glm::uvec3& size, const std::vector<uint16_t>& indices)
{
    if (size.x < 1 || size.y < 1 || size.z < 1)
    {
        CUBOS_WARN("Grid size must be at least 1 in each dimension: was ({}, {}, {}), defaulting to (1, 1, 1).", size.x,
                   size.y, size.z);
        mSize = {1, 1, 1};
    }
    else if (indices.size() !=
             static_cast<std::size_t>(size.x) * static_cast<std::size_t>(size.y) * static_cast<std::size_t>(size.z))
    {
        CUBOS_WARN("Grid size and indices size mismatch: was ({}, {}, {}), indices size is {}.", size.x, size.y, size.z,
                   indices.size());
        mSize = {1, 1, 1};
    }
    else
    {
        mSize = size;
    }

    mIndices = indices;
}

Grid::Grid(Grid&& other) noexcept
    : mSize(other.mSize)
{
    new (&mIndices) std::vector<uint16_t>(std::move(other.mIndices));
}

Grid::Grid()
{
    mSize = {1, 1, 1};
    mIndices.resize(1, 0);
}

Grid& Grid::operator=(const Grid& rhs) = default;

void Grid::setSize(const glm::uvec3& size)
{
    if (size == mSize)
    {
        return;
    }
    if (size.x < 1 || size.y < 1 || size.z < 1)
    {
        CUBOS_WARN("Grid size must be at least 1 in each dimension: preserving original dimensions (tried to set to "
                   "({}, {}, {}))",
                   size.x, size.y, size.z);
        return;
    }

    mSize = size;
    mIndices.clear();
    mIndices.resize(
        static_cast<std::size_t>(mSize.x) * static_cast<std::size_t>(mSize.y) * static_cast<std::size_t>(mSize.z), 0);
}

const glm::uvec3& Grid::size() const
{
    return mSize;
}

void Grid::clear()
{
    for (auto& i : mIndices)
    {
        i = 0;
    }
}

uint16_t Grid::get(const glm::ivec3& position) const
{
    assert(position.x >= 0 && position.x < static_cast<int>(mSize.x));
    assert(position.y >= 0 && position.y < static_cast<int>(mSize.y));
    assert(position.z >= 0 && position.z < static_cast<int>(mSize.z));
    auto index = position.x + position.y * static_cast<int>(mSize.x) + position.z * static_cast<int>(mSize.x * mSize.y);
    return mIndices[static_cast<std::size_t>(index)];
}

void Grid::set(const glm::ivec3& position, uint16_t mat)
{
    assert(position.x >= 0 && position.x < static_cast<int>(mSize.x));
    assert(position.y >= 0 && position.y < static_cast<int>(mSize.y));
    assert(position.z >= 0 && position.z < static_cast<int>(mSize.z));
    auto index = position.x + position.y * static_cast<int>(mSize.x) + position.z * static_cast<int>(mSize.x * mSize.y);
    mIndices[static_cast<std::size_t>(index)] = mat;
}

bool Grid::convert(const Palette& src, const Palette& dst, float minSimilarity)
{
    // Find the mappings for every material in the source palette.
    std::unordered_map<uint16_t, uint16_t> mappings;
    for (uint16_t i = 0; i <= src.size(); ++i)
    {
        uint16_t j = dst.find(src.get(i));
        if (src.get(i).similarity(dst.get(j)) >= minSimilarity)
        {
            mappings[i] = j;
        }
    }

    // Check if the mappings are complete for every material being used in the grid.
    for (uint32_t i = 0; i < mSize.x * mSize.y * mSize.z; ++i)
    {
        if (mappings.find(mIndices[i]) == mappings.end())
        {
            return false;
        }
    }

    // Apply the mappings.
    for (uint32_t i = 0; i < mSize.x * mSize.y * mSize.z; ++i)
    {
        mIndices[i] = mappings[mIndices[i]];
    }

    return true;
}

void cubos::core::data::serialize(Serializer& serializer, const gl::Grid& grid, const char* name)
{
    serializer.beginObject(name);
    serializer.write(grid.mSize, "size");
    serializer.write(grid.mIndices, "data");
    serializer.endObject();
}

void cubos::core::data::deserialize(Deserializer& deserializer, gl::Grid& grid)
{
    deserializer.beginObject();
    deserializer.read(grid.mSize);
    deserializer.read(grid.mIndices);
    deserializer.endObject();

    if (grid.mSize.x * grid.mSize.y * grid.mSize.z != static_cast<unsigned int>(grid.mIndices.size()))
    {
        CUBOS_WARN("Grid size and indices size mismatch: was ({}, {}, {}), indices size is {}.", grid.mSize.x,
                   grid.mSize.y, grid.mSize.z, grid.mIndices.size());
        grid.mSize = {1, 1, 1};
        grid.mIndices.clear();
        grid.mIndices.resize(1, 0);
    }
}
