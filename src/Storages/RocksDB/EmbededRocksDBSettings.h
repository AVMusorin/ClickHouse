#pragma once

#include <Core/Defines.h>
#include <Core/BaseSettings.h>
#include <Core/SettingsEnums.h>
#include <Interpreters/Context_fwd.h>


namespace Poco::Util
{
    class AbstractConfiguration;
}


namespace DB
{
class ASTStorage;
class ASTSetQuery;

#define LIST_OF_ROCKSDB_SETTINGS(M, ALIAS) \
    M(Bool, optimize_trivial_approximate_count_query, true, "Use an approximate value for trivial count optimization of storages that support such estimations.", 0)

DECLARE_SETTINGS_TRAITS(EmbededRocksDBSettingsTrait, LIST_OF_ROCKSDB_SETTINGS)

struct EmbededRocksDBSettings : public BaseSettings<EmbededRocksDBSettingsTrait>
{
    void loadFromQuery(ASTStorage & storage_def);
    void loadFromQuery(const ASTSetQuery & settings_def);
};

using EmbededRocksDBSettingsPtr = std::shared_ptr<const EmbededRocksDBSettings>;
}
