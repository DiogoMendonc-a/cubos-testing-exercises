#pragma once

#include <stack>

#include <nlohmann/json.hpp>

#include <cubos/core/data/serializer.hpp>
#include <cubos/core/memory/stream.hpp>

namespace cubos::core::data
{
    /// Implementation of the abstract Serializer class for serializing to JSON.
    /// Each time a top-level primitive/object/array/dictionary is written, its JSON output is written to the underlying
    /// stream.
    class JSONSerializer : public Serializer
    {
    public:
        /// @param stream The stream to serialize to.
        /// @param indent The JSON output indentantion (-1 means no indentation).
        JSONSerializer(memory::Stream& stream, int indent = -1);

        // Implement interface methods.

        void writeI8(int8_t value, const char* name) override;
        void writeI16(int16_t value, const char* name) override;
        void writeI32(int32_t value, const char* name) override;
        void writeI64(int64_t value, const char* name) override;
        void writeU8(uint8_t value, const char* name) override;
        void writeU16(uint16_t value, const char* name) override;
        void writeU32(uint32_t value, const char* name) override;
        void writeU64(uint64_t value, const char* name) override;
        void writeF32(float value, const char* name) override;
        void writeF64(double value, const char* name) override;
        void writeBool(bool value, const char* name) override;
        void writeString(const char* value, const char* name) override;
        void beginObject(const char* name) override;
        void endObject() override;
        void beginArray(std::size_t length, const char* name) override;
        void endArray() override;
        void beginDictionary(std::size_t length, const char* name) override;
        void endDictionary() override;

    private:
        void writeJSON(nlohmann::ordered_json&& json, const char* name);

        /// The possible state modes of serialization.
        enum class Mode
        {
            Object,
            Array,
            Dictionary
        };

        /// Holds the whole state of serialization.
        struct Frame
        {
            Mode mode;                   ///< The mode of the frame.
            nlohmann::ordered_json json; ///< The JSON object to serialize to.
            nlohmann::ordered_json key;  ///< The JSON key value.
            std::string name;            ///< The name of the value being serialized.
        };

        memory::Stream& mStream;   ///< The stream to serialize to.
        std::stack<Frame> mFrames; ///< The stack of frames.
        int mIndent;               ///< The indentation of the JSON output.
    };
} // namespace cubos::core::data
