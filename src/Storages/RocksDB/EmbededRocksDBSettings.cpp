#include <Storages/RocksDB/EmbededRocksDBSettings.h>
#include "Interpreters/Context.h"
#include "Parsers/ASTCreateQuery.h"
#include "Parsers/ASTSetQuery.h"
#include <Parsers/ASTFunction.h>


namespace DB
{

namespace ErrorCodes
{
    extern const int UNKNOWN_SETTING;
}

IMPLEMENT_SETTINGS_TRAITS(EmbededRocksDBSettingsTrait, LIST_OF_ROCKSDB_SETTINGS)

void EmbededRocksDBSettings::loadFromQuery(const ASTSetQuery & settings_def)
{
    applyChanges(settings_def.changes);
}


void EmbededRocksDBSettings::loadFromQuery(ASTStorage & storage_def)
{
    if (storage_def.settings)
    {
        try
        {
            loadFromQuery(*storage_def.settings);
        }
        catch (Exception & e)
        {
            if (e.code() == ErrorCodes::UNKNOWN_SETTING)
                e.addMessage("for storage " + storage_def.engine->name);
            throw;
        }
    }
    else
    {
        auto settings_ast = std::make_shared<ASTSetQuery>();
        settings_ast->is_standalone = false;
        storage_def.set(storage_def.settings, settings_ast);
    }
}
}
