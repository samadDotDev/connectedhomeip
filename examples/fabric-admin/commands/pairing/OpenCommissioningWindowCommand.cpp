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

#include "OpenCommissioningWindowCommand.h"

#include <system/SystemClock.h>

using namespace ::chip;

CHIP_ERROR OpenCommissioningWindowCommand::RunCommand()
{
    mWindowOpener = Platform::MakeUnique<Controller::CommissioningWindowOpener>(&CurrentCommissioner());
    if (mCommissioningWindowOption == Controller::CommissioningWindowOpener::CommissioningWindowOption::kOriginalSetupCode)
    {
        return mWindowOpener->OpenBasicCommissioningWindow(mNodeId, System::Clock::Seconds16(mCommissioningWindowTimeout),
                                                           &mOnOpenBasicCommissioningWindowCallback);
    }

    if (mCommissioningWindowOption == Controller::CommissioningWindowOpener::CommissioningWindowOption::kTokenWithRandomPIN)
    {
        CHIP_ERROR err = CHIP_NO_ERROR;
        SetupPayload ignored;
        const uint8_t payload[] = { 0x15, 0x36, 0x01, 0x15, 0x35, 0x01, 0x26, 0x00, 0x72, 0x4D, 0xDB, 0xCB,
                                    0x37, 0x01, 0x24, 0x02, 0x00, 0x24, 0x03, 0x1F, 0x24, 0x04, 0x00 };
        chip::ByteSpan payloadSpan(payload);
        chip::Optional<unsigned int> setupPIN(20202021);
        chip::Optional<chip::ByteSpan> salt(payloadSpan);

        if (mEndpointId == kRootEndpointId)
        {
            err =
                mWindowOpener->OpenCommissioningWindow(mNodeId, System::Clock::Seconds16(mCommissioningWindowTimeout), mIteration,
                                                       mDiscriminator, setupPIN, salt, &mOnOpenCommissioningWindowCallback, ignored,
                                                       /* readVIDPIDAttributes */ true);
        }
        else
        {
            Controller::CommissioningWindowOpener::CommissioningWindowParams params = {
                .deviceId      = mNodeId,
                .endpointId    = mEndpointId,
                .timeout       = System::Clock::Seconds16(mCommissioningWindowTimeout),
                .iteration     = mIteration,
                .discriminator = mDiscriminator,
                .setupPIN      = setupPIN,
                .salt          = salt,
                .callback      = &mOnOpenCommissioningWindowCallback,
            };

            err = mWindowOpener->OpenCommissioningWindow(params, ignored);
        }

        return err;
    }

    ChipLogError(NotSpecified, "Unknown commissioning window option: %d", to_underlying(mCommissioningWindowOption));
    return CHIP_ERROR_INVALID_ARGUMENT;
}

void OpenCommissioningWindowCommand::OnOpenCommissioningWindowResponse(void * context, NodeId remoteId, CHIP_ERROR err,
                                                                       chip::SetupPayload payload)
{
    OpenCommissioningWindowCommand * self = static_cast<OpenCommissioningWindowCommand *>(context);
    if (self->mDelegate)
    {
        self->mDelegate->OnCommissioningWindowOpened(remoteId, err, payload);
        self->UnregisterDelegate();
    }

    LogErrorOnFailure(err);
    OnOpenBasicCommissioningWindowResponse(context, remoteId, err);
}

void OpenCommissioningWindowCommand::OnOpenBasicCommissioningWindowResponse(void * context, NodeId remoteId, CHIP_ERROR err)
{
    LogErrorOnFailure(err);

    OpenCommissioningWindowCommand * command = reinterpret_cast<OpenCommissioningWindowCommand *>(context);
    VerifyOrReturn(command != nullptr, ChipLogError(NotSpecified, "OnOpenCommissioningWindowCommand: context is null"));
    command->SetCommandExitStatus(err);
}
