/*
 *
 *    Copyright (c) 2024 Project CHIP Authors
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "RpcClient.h"
#include "RpcClientProcessor.h"

#include <string>
#include <thread>
#include <unistd.h>

#include "fabric_bridge_service/fabric_bridge_service.rpc.pb.h"
#include "pw_assert/check.h"
#include "pw_hdlc/decoder.h"
#include "pw_hdlc/default_addresses.h"
#include "pw_hdlc/rpc_channel.h"
#include "pw_rpc/client.h"
#include "pw_stream/socket_stream.h"

using namespace chip;

namespace {

// Constants
constexpr uint32_t kRpcTimeoutMs     = 1000;
constexpr uint32_t kDefaultChannelId = 1;

// Fabric Bridge Client
rpc::pw_rpc::nanopb::FabricBridge::Client fabricBridgeClient(rpc::client::GetDefaultRpcClient(), kDefaultChannelId);
pw::rpc::NanopbUnaryReceiver<::pw_protobuf_Empty> addSynchronizedDeviceCall;

template <typename CallType>
CHIP_ERROR WaitForResponse(CallType & call)
{
    if (!call.active())
    {
        return CHIP_ERROR_INTERNAL;
    }

    // Wait for the response or timeout
    uint32_t elapsedTimeMs     = 0;
    const uint32_t sleepTimeMs = 100;

    while (call.active() && elapsedTimeMs < kRpcTimeoutMs)
    {
        usleep(sleepTimeMs * 1000);
        elapsedTimeMs += sleepTimeMs;
    }

    if (elapsedTimeMs >= kRpcTimeoutMs)
    {
        fprintf(stderr, "RPC Response timed out!");
        return CHIP_ERROR_TIMEOUT;
    }

    return CHIP_NO_ERROR;
}

// Callback function to be called when the RPC response is received
void OnAddDeviceResponseCompleted(const pw_protobuf_Empty & response, pw::Status status)
{
    if (status.ok())
    {
        ChipLogProgress(NotSpecified, "AddSynchronizedDevice RPC call succeeded!");
    }
    else
    {
        ChipLogProgress(NotSpecified, "AddSynchronizedDevice RPC call failed with status: %d\n", status.code());
    }
}

} // namespace

CHIP_ERROR InitRpcClient(uint16_t rpcServerPort)
{
    rpc::client::SetRpcServerPort(rpcServerPort);
    return rpc::client::StartPacketProcessing();
}

CHIP_ERROR AddSynchronizedDevice(chip::NodeId nodeId)
{
    ChipLogProgress(NotSpecified, "AddSynchronizedDevice");

    if (addSynchronizedDeviceCall.active())
    {
        ChipLogError(NotSpecified, "OpenCommissioningWindow is in progress\n");
        return CHIP_ERROR_BUSY;
    }

    chip_rpc_SynchronizedDevice device;
    device.node_id = nodeId;

    // The RPC will remain active as long as `addSynchronizedDeviceCall` is alive.
    addSynchronizedDeviceCall = fabricBridgeClient.AddSynchronizedDevice(device, OnAddDeviceResponseCompleted);

    if (!addSynchronizedDeviceCall.active())
    {
        return CHIP_ERROR_INTERNAL;
    }

    return WaitForResponse(addSynchronizedDeviceCall);
}
