/*
 * Copyright (C) 2009 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "CredentialStorage.h"

#if USE(CFURLCONNECTION)

#include "AuthenticationCF.h"
#include "Credential.h"
#include "ProtectionSpace.h"
#include <pal/spi/cf/CFNetworkSPI.h>
#include <wtf/RetainPtr.h>

#if PLATFORM(COCOA)
#include "WebCoreSystemInterface.h"
#elif PLATFORM(WIN)
#include <WebKitSystemInterface/WebKitSystemInterface.h>
#endif

#if PLATFORM(IOS)
#include <CFNetwork/CFURLCredentialStorage.h>
#endif

namespace WebCore {
    
static inline CFURLCredentialRef copyCredentialFromProtectionSpace(CFURLProtectionSpaceRef protectionSpace)
{
    auto storage = adoptCF(CFURLCredentialStorageCreate(kCFAllocatorDefault));
    return CFURLCredentialStorageCopyDefaultCredentialForProtectionSpace(storage.get(), protectionSpace);
}

Credential CredentialStorage::getFromPersistentStorage(const ProtectionSpace& protectionSpace)
{
#if PLATFORM(COCOA)
    auto credentialCF = adoptCF(copyCredentialFromProtectionSpace(protectionSpace.cfSpace()));
    return Credential(credentialCF.get());
#else
    auto protectionSpaceCF = adoptCF(createCF(protectionSpace));
    auto credentialCF = adoptCF(copyCredentialFromProtectionSpace(protectionSpaceCF.get()));
    return core(credentialCF.get());
#endif
}

} // namespace WebCore

#endif // USE(CFURLCONNECTION)
