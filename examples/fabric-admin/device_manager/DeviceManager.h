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

#pragma once

#include <platform/CHIPDeviceLayer.h>

class DeviceManager
{
public:
    DeviceManager() = default;

    void Init();

    chip::NodeId GetNextAvailableNodeId()
    {
        mLastUsedNodeId++;
        VerifyOrDieWithMsg(mLastUsedNodeId < std::numeric_limits<chip::NodeId>::max(), NotSpecified, "No more available NodeIds.");

        return mLastUsedNodeId;
    }    

    chip::NodeId GetBridgeNodeId() const
    {
        return mBridgeNodeId;
    }

    void SetBridgeNodeId(chip::NodeId bridgeNodeId)
    {
        mBridgeNodeId = bridgeNodeId;
    }

    chip::NodeId GetRemoteBridgeNodeId() const
    {
        return mRemoteBridgeNodeId;
    }

    void SetRemoteBridgeNodeId(chip::NodeId remoteBridgeNodeId)
    {
        mRemoteBridgeNodeId = remoteBridgeNodeId;
    }    

private:
    friend DeviceManager & DeviceMgr();

    static DeviceManager sInstance;

    chip::NodeId mLastUsedNodeId = 0;
    chip::NodeId mBridgeNodeId = chip::kUndefinedNodeId;
    chip::NodeId mRemoteBridgeNodeId = chip::kUndefinedNodeId;    
};

/**
 * Returns the public interface of the DeviceManager singleton object.
 *
 * Applications should use this to access features of the DeviceManager
 * object.
 */
inline DeviceManager & DeviceMgr()
{
    return DeviceManager::sInstance;
}
