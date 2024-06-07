/*
 *   Copyright (c) 2024 Project CHIP Authors
 *   All rights reserved.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 */

#include "FabricSyncCommand.h"
#include <commands/common/RemoteDataModelLogger.h>
#include <commands/interactive/InteractiveCommands.h>
#include <thread>
#include <unistd.h>

#if defined(PW_RPC_ENABLED)
#include <rpc/RpcClient.h>
#endif

using namespace ::chip;

namespace {

constexpr uint16_t kRetryIntervalS = 3;

void PairSyncedDevice(System::Layer * systemLayer, void * appState)
{
    PushCommand("pairing onnetwork 3 20202021");
}

} // namespace

CHIP_ERROR FabricSyncAddDeviceCommand::RunCommand(NodeId remoteId)
{
#if defined(PW_RPC_ENABLED)
    AddSynchronizedDevice(remoteId);
    return CHIP_NO_ERROR;
#else
    return CHIP_ERROR_NOT_IMPLEMENTED;
#endif
}

CHIP_ERROR FabricSyncDeviceCommand::RunCommand(NodeId remoteId)
{
    PushCommand("pairing open-commissioning-window 1 3 1 300 1000 3840");

    DeviceLayer::SystemLayer().StartTimer(System::Clock::Seconds16(kRetryIntervalS), PairSyncedDevice, nullptr);

    return CHIP_NO_ERROR;
}
