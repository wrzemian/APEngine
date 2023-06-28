//
// Created by micha on 28.06.2023.
//

#ifndef APENGINE_LEVELHITBOXES_H
#define APENGINE_LEVELHITBOXES_H

#include <string>
#include <map>
#include <glm/glm.hpp>

class LevelHitboxes {
        public:

        struct HitboxData {
            glm::vec3 min;
            glm::vec3 max;
        };

        static LevelHitboxes& getInstance() {
            static LevelHitboxes instance;
            return instance;
        }

        HitboxData getHitbox(const std::string& key, const std::string& levelName) {
            char lastChar = levelName.back();  // Extract the last character from the levelName

            // Find the correct map based on the last character
            auto mapIter = hitboxMaps.find(lastChar);
            if (mapIter != hitboxMaps.end()) {
                // Find the hitbox in the map based on the key
                auto hitboxIter = mapIter->second.find(key);
                if (hitboxIter != mapIter->second.end()) {
                    return hitboxIter->second;
                }
            }

            // Return a default-constructed Hitbox if not found
            return HitboxData{};
        }

        private:
        LevelHitboxes() {
            // Initialize your maps here
            // Map 1
            hitboxMaps['1'] = {
                    {"key1", {glm::vec3(1.0f), glm::vec3(2.0f)}},
                    {"key2", {glm::vec3(3.0f), glm::vec3(4.0f)}}
            };

            // Map 2
            hitboxMaps['2'] = {
                    {"key3", {glm::vec3(5.0f), glm::vec3(6.0f)}},
                    {"key4", {glm::vec3(7.0f), glm::vec3(8.0f)}}
            };

            // and so on...
        }

        std::map<char, std::map<std::string, HitboxData>> hitboxMaps;
};

#endif //APENGINE_LEVELHITBOXES_H
