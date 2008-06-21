/* KDevelop xUnit plugin
 *
 * Copyright 2006 systest.ch <qxrunner@systest.ch>
 * Copyright 2008 Manuel Breugelmans <mbr.nxi@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include "cppunitmodel.h"
#include "testbase.h"
#include "cppunitregister.h"
#include <KProcess>

using QxCppUnit::TestBase;
using QxCppUnit::CppUnitModel;
using QxCppUnit::CppUnitRegister;

CppUnitModel::CppUnitModel(QObject* parent)
        : RunnerModel(parent)
{
    // Define the set of expected results.
    setExpectedResults(QxRunner::RunWarning | QxRunner::RunError);
}

CppUnitModel::~CppUnitModel()
{
}

void CppUnitModel::readTests(const QFileInfo& exe)
{
    KProcess proc;
    QStringList argv;
    argv << "-proto";
    proc.setProgram(exe.filePath(), argv);
    kDebug() << "executing " << proc.program();
    proc.setOutputChannelMode(KProcess::SeparateChannels);
    proc.start();
    proc.waitForFinished(-1);

    CppUnitRegister reg;
    reg.setExecutable(exe);
    reg.addFromXml(&proc);
    setRootItem(reg.rootItem());

/*    QTestOutputParser parser(proc);
    parser.go(this);*/
}

QString CppUnitModel::name() const
{
    return tr("QxCppUnit");
}

QString CppUnitModel::about() const
{
    return "";
}
