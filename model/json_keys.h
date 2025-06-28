#ifndef JSON_KEYS_H
#define JSON_KEYS_H

namespace JsonKeys {

namespace Common {
inline constexpr const char *NAME = "name";
inline constexpr const char *URL = "url";
} // namespace Common

namespace Pokemon {
inline constexpr const char *ID = "id";
inline constexpr const char *NAME = JsonKeys::Common::NAME;
inline constexpr const char *HEIGHT = "height";
inline constexpr const char *WEIGHT = "weight";
inline constexpr const char *BASE_EXPERIENCE = "base_experience";
inline constexpr const char *TYPES = "types";
inline constexpr const char *TYPE = "type";
inline constexpr const char *ABILITIES = "abilities";
inline constexpr const char *ABILITY = "ability";
inline constexpr const char *SPRITES = "sprites";
inline constexpr const char *OTHER = "other";
inline constexpr const char *OFFICIAL_ARTWORK = "official-artwork";
inline constexpr const char *FRONT_DEFAULT = "front_default";
} // namespace Pokemon

} // namespace JsonKeys

#endif // JSON_KEYS_H
