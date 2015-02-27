/*
 * This file is part of KDevelop
 *
 * Copyright 2010 Andreas Pakulat <apaku@gmx.de>
 * Copyright 2014 Sergey Kalinichev <kalinichev.so.0@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "settingsmanager.h"

#include <KConfig>
#include <KConfigGroup>

#include <interfaces/iproject.h>
#include <interfaces/icore.h>
#include <interfaces/iplugincontroller.h>
#include <project/projectmodel.h>

#include "compilerprovider.h"

using namespace KDevelop;

namespace ConfigConstants
{
const QString configKey = QLatin1String( "CustomDefinesAndIncludes" );
const QString definesKey = QLatin1String( "Defines" );
const QString includesKey = QLatin1String( "Includes" );
const QString projectPathPrefix = QLatin1String( "ProjectPath" );
const QString projectPathKey = QLatin1String( "Path" );

const QString customBuildSystemGroup = QLatin1String( "CustomBuildSystem" );
const QString definesAndIncludesGroup = QLatin1String( "Defines And Includes" );

const QString compilersGroup = QLatin1String( "Compilers" );
const QString compilerNameKey = QLatin1String( "Name" );
const QString compilerPathKey = QLatin1String( "Path" );
const QString compilerTypeKey = QLatin1String( "Type" );
const QString compilerStandardKey = QLatin1String( "Standard" );
}

namespace
{
CompilerPointer createCompilerFromConfig(KConfigGroup& cfg)
{
    auto grp = cfg.group("Compiler");
    auto name = grp.readEntry( ConfigConstants::compilerNameKey, QString() );
    if (name.isEmpty()) {
        return SettingsManager::globalInstance()->provider()->checkCompilerExists({});
    }

    for (auto c : SettingsManager::globalInstance()->provider()->compilers()) {
        if (c->name() == name) {
            return c;
        }
    }

    auto path = grp.readEntry( ConfigConstants::compilerPathKey, QString() );
    auto type = grp.readEntry( ConfigConstants::compilerTypeKey, QString() );

    auto cf = SettingsManager::globalInstance()->provider()->compilerFactories();
    for (auto f : cf) {
        if (f->name() == type) {
            auto compiler = f->createCompiler(name, path, true);
            compiler->setLanguageStandard(grp.readEntry(ConfigConstants::compilerStandardKey, QString()));
            return compiler;
        }
    }

    return SettingsManager::globalInstance()->provider()->checkCompilerExists({});
}

void writeCompilerToConfig(KConfigGroup& cfg, const CompilerPointer& compiler)
{
    Q_ASSERT(compiler);

    auto grp = cfg.group("Compiler");
    grp.writeEntry(ConfigConstants::compilerNameKey, compiler->name());
    grp.writeEntry(ConfigConstants::compilerPathKey, compiler->path());
    grp.writeEntry(ConfigConstants::compilerTypeKey, compiler->factoryName());
    grp.writeEntry(ConfigConstants::compilerStandardKey, compiler->languageStandard());
}

void doWriteSettings( KConfigGroup grp, const QList<ConfigEntry>& paths )
{
    int pathIndex = 0;
    for ( const auto& path : paths ) {
        KConfigGroup pathgrp = grp.group( ConfigConstants::projectPathPrefix + QString::number( pathIndex++ ) );
        pathgrp.writeEntry( ConfigConstants::projectPathKey, path.path );
        {
            int index = 0;
            KConfigGroup includes(pathgrp.group(ConfigConstants::includesKey));
            for( auto it = path.includes.begin() ; it != path.includes.end(); ++it){
                includes.writeEntry(QString::number(++index), *it);
            }

        }
        {
            KConfigGroup defines(pathgrp.group(ConfigConstants::definesKey));
            for (auto it = path.defines.begin(); it != path.defines.end(); ++it) {
                defines.writeEntry(it.key(), it.value());
            }
        }
        writeCompilerToConfig(pathgrp, path.compiler);
    }
}

/// @param remove if true all read entries will be removed from the config file
QList<ConfigEntry> doReadSettings( KConfigGroup grp, bool remove = false )
{
    QList<ConfigEntry> paths;
    for( const QString &grpName : grp.groupList() ) {
        if ( grpName.startsWith( ConfigConstants::projectPathPrefix ) ) {
            KConfigGroup pathgrp = grp.group( grpName );

            ConfigEntry path;
            path.path = pathgrp.readEntry( ConfigConstants::projectPathKey, "" );

            {
                // Backwards compatibility with old config style
                if(pathgrp.hasKey(ConfigConstants::definesKey)) {
                    QByteArray tmp = pathgrp.readEntry( ConfigConstants::definesKey, QByteArray() );
                    QDataStream s( tmp );
                    s.setVersion( QDataStream::Qt_4_5 );
                    // backwards compatible reading
                    QHash<QString, QVariant> defines;
                    s >> defines;
                    path.setDefines(defines);
                } else {
                    KConfigGroup defines(pathgrp.group(ConfigConstants::definesKey));
                    QMap<QString, QString> defMap = defines.entryMap();
                    path.defines.reserve(defMap.size());
                    for(auto it = defMap.constBegin(); it != defMap.constEnd(); ++it) {
                        QString key = it.key();
                        if(key.isEmpty()) {
                            // Toss out the invalid key and value since a valid
                            // value needs a valid key
                            continue;
                        } else {
                             path.defines.insert(key, it.value());
                        }
                    }
                }
            }

            {
                // Backwards compatibility with old config style
                if(pathgrp.hasKey(ConfigConstants::includesKey)){
                    QByteArray tmp = pathgrp.readEntry( ConfigConstants::includesKey, QByteArray() );
                    QDataStream s( tmp );
                    s.setVersion( QDataStream::Qt_4_5 );
                    s >> path.includes;
                } else {
                    KConfigGroup includes(pathgrp.group(ConfigConstants::includesKey));
                    QMap<QString, QString> incMap = includes.entryMap();
                    for(auto it = incMap.begin(); it != incMap.end(); ++it){
                        QString value = it.value();
                        if(value.isEmpty()){
                            continue;
                        }
                        path.includes += value;
                    }
                }
            }

            path.compiler = createCompilerFromConfig(pathgrp);

            if ( remove ) {
                pathgrp.deleteGroup();
            }
            paths << path;
        }
    }

    return paths;
}

/**
 * Reads and converts paths from old (Custom Build System's) format to the current one.
 * @return all converted paths (if any)
 */
QList<ConfigEntry> convertedPaths( KConfig* cfg )
{
    KConfigGroup group = cfg->group( ConfigConstants::customBuildSystemGroup );
    if ( !group.isValid() )
        return {};

    QList<ConfigEntry> paths;
    foreach( const QString &grpName, group.groupList() ) {
        KConfigGroup subgroup = group.group( grpName );
        if ( !subgroup.isValid() )
            continue;

        paths += doReadSettings( subgroup, true );
    }

    return paths;
}

} // Empty Namespace

void ConfigEntry::setDefines(const QHash<QString, QVariant>& newDefines)
{
    defines.clear();
    defines.reserve(newDefines.size());
    for (auto it = newDefines.begin(); it != newDefines.end(); ++it) {
        defines[it.key()] = it.value().toString();
    }
}

SettingsManager* SettingsManager::s_globalInstance = nullptr;

SettingsManager::SettingsManager(bool globalInstance)
  : m_provider(this)
{
    if (globalInstance) {
        Q_ASSERT(!s_globalInstance);
        s_globalInstance = this;
    }
}

SettingsManager::~SettingsManager()
{
    if (s_globalInstance == this) {
        s_globalInstance = nullptr;
    }
}

SettingsManager* SettingsManager::globalInstance()
{
    Q_ASSERT(s_globalInstance);
    return s_globalInstance;
}

CompilerProvider* SettingsManager::provider()
{
    return &m_provider;
}

const CompilerProvider* SettingsManager::provider() const
{
    return &m_provider;
}

void SettingsManager::writePaths( KConfig* cfg, const QList< ConfigEntry >& paths )
{
    KConfigGroup grp = cfg->group( ConfigConstants::configKey );
    if ( !grp.isValid() )
        return;

    grp.deleteGroup();

    doWriteSettings( grp, paths );
}

QList<ConfigEntry> SettingsManager::readPaths( KConfig* cfg ) const
{
    auto converted = convertedPaths( cfg );
    if ( !converted.isEmpty() ) {
        const_cast<SettingsManager*>(this)->writePaths( cfg, converted );
        return converted;
    }

    KConfigGroup grp = cfg->group( ConfigConstants::configKey );
    if ( !grp.isValid() ) {
        return {};
    }

    return doReadSettings( grp );
}

bool SettingsManager::needToReparseCurrentProject( KConfig* cfg ) const
{
    auto grp = cfg->group( ConfigConstants::definesAndIncludesGroup );
    return grp.readEntry( "reparse", true );
}

void SettingsManager::writeUserDefinedCompilers(const QVector< CompilerPointer >& compilers)
{
    QVector< CompilerPointer > editableCompilers;
    for (const auto& compiler : compilers) {
        if (!compiler->editable()) {
            continue;
        }
        editableCompilers.append(compiler);
    }

    KConfigGroup config = KSharedConfig::openConfig()->group(ConfigConstants::compilersGroup);
    config.deleteGroup();
    config.writeEntry("number", editableCompilers.count());
    int i = 0;
    for (const auto& compiler : editableCompilers) {
        KConfigGroup grp = config.group(QString::number(i));
        ++i;

        grp.writeEntry(ConfigConstants::compilerNameKey, compiler->name());
        grp.writeEntry(ConfigConstants::compilerPathKey, compiler->path());
        grp.writeEntry(ConfigConstants::compilerTypeKey, compiler->factoryName());
        grp.writeEntry(ConfigConstants::compilerStandardKey, compiler->languageStandard());
    }
    config.sync();
}

QVector< CompilerPointer > SettingsManager::userDefinedCompilers() const
{
    QVector< CompilerPointer > compilers;

    KConfigGroup config = KSharedConfig::openConfig()->group(ConfigConstants::compilersGroup);
    int count = config.readEntry("number", 0);
    for (int i = 0; i < count; i++) {
        KConfigGroup grp = config.group(QString::number(i));

        auto name = grp.readEntry(ConfigConstants::compilerNameKey, QString());
        auto path = grp.readEntry(ConfigConstants::compilerPathKey, QString());
        auto type = grp.readEntry(ConfigConstants::compilerTypeKey, QString());

        auto cf = m_provider.compilerFactories();
        for (auto f : cf) {
            if (f->name() == type) {
                auto compiler = f->createCompiler(name, path);
                compiler->setLanguageStandard(grp.readEntry(ConfigConstants::compilerStandardKey, QString()));
                compilers.append(compiler);
            }
        }
    }
    return compilers;
}
