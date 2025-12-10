#include "glint/core/managers/AssetManager.h"

namespace glint::engine::core {

    AssetManager::~AssetManager() {
        for (size_t i = 0; i < m_entries.size(); ++i) {
            AssetEntry asset = m_entries[i];
            if (!asset.data || !asset.deleter) continue;

            asset.deleter(asset.data);
        }
    }

}
