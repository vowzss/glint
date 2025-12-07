#include "glint/core/AssetManager.h"

namespace glint::engine::core {

    AssetManager::~AssetManager() {
        for (size_t i = 0; i < entries.size(); ++i) {
            AssetEntry asset = entries[i];
            if (!asset.data || !asset.deleter) continue;

            asset.deleter(asset.data);
        }
    }

    uint32_t AssetManager::computeUniqueId() {
        const uint32_t id = freeIds.empty() ? nextId++ : freeIds.back();

        if (!freeIds.empty()) {
            freeIds.pop_back();
        }

        if (id >= entries.size()) {
            entries.resize(id + 1);
        }

        return id;
    }

}
