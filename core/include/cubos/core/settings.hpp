/// @file
/// @brief Class @ref cubos::core::Settings.
/// @ingroup core

#pragma once

#include <string>
#include <unordered_map>

namespace cubos::core
{
    /// @brief Stores settings as key-value pairs and provides methods to retrieve them.
    /// @todo Should be moved to the engine.
    /// @ingroup core
    class Settings final
    {
    public:
        Settings() = default;
        ~Settings() = default;

        /// @brief Clears all the settings.
        void clear();

        /// @brief Defines a new boolean setting.
        ///
        /// If the setting already exists, overwrites its value.
        ///
        /// @param key Key.
        /// @param value Value.
        void setBool(const std::string& key, const bool& value);

        /// @brief Retrieves the bool setting with the given @p key.
        ///
        /// If no setting exists with such key, returns @p defaultValue.
        /// If the setting exists but its value is not "true", returns false.
        ///
        /// @param key Key.
        /// @param defaultValue Default value.
        /// @return Current value.
        bool getBool(const std::string& key, const bool& defaultValue) const;

        /// @brief Defines a new string setting.
        ///
        /// If the setting already exists, overwrites its value.
        ///
        /// @param key Key.
        /// @param value Value.
        void setString(const std::string& key, const std::string& value);

        /// @brief Retrieves the string setting with the given @p key.
        ///
        /// If no setting exists with such key, returns @p defaultValue.
        ///
        /// @param key Key.
        /// @param defaultValue Default value.
        /// @return Current value.
        std::string getString(const std::string& key, const std::string& defaultValue) const;

        /// @brief Defines a new integer setting.
        ///
        /// If the setting already exists, overwrites its value.
        ///
        /// @param key Key.
        /// @param value Value.
        /// @return Current value.
        void setInteger(const std::string& key, int value);

        /// @brief Retrieves the integer setting with the given @p key.
        ///
        /// If no setting exists with such key, returns @p defaultValue.
        /// If the setting exists but its value is not a valid integer, returns @p defaultValue.
        ///
        /// @param key Key.
        /// @param defaultValue Default value.
        /// @return Current value.
        int getInteger(const std::string& key, int defaultValue) const;

        /// @brief Defines a new double setting.
        ///
        /// If the setting already exists, overwrites its value.
        ///
        /// @param key Key.
        /// @param value Value.
        void setDouble(const std::string& key, double value);

        /// @brief Retrieves the double setting with the given @p key.
        ///
        /// If no setting exists with such key, returns the default value.
        /// If the setting exists but its value is not a valid double, returns the default value.
        ///
        /// @param key Key.
        /// @param defaultValue Default value.
        /// @return Current value.
        double getDouble(const std::string& key, double defaultValue) const;

        /// @brief Merges the settings from @p settingsToMerge.
        ///
        /// If a setting from @p settingsToMerge is already defined in this instance, its value is
        /// overwritten.
        ///
        /// @param settingsToMerge Settings to be merged to this instance.
        void merge(const Settings& settingsToMerge);

        /// @return Underlying `std::unordered_map` with the settings.
        const std::unordered_map<std::string, std::string>& getValues() const;

        /// @return Underlying `std::unordered_map` with the settings.
        std::unordered_map<std::string, std::string>& getValues();

    private:
        std::unordered_map<std::string, std::string> mValues;
    };
} // namespace cubos::core
