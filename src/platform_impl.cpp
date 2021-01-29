#include "../include/platform_impl.h"
#include "linuxSystemFontHelper.h"
#include "gl/hardware.h"
#include "log.h"
#include <algorithm>
#include <stdio.h>
#include <stdarg.h>
#include <libgen.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/syscall.h>

#include <GLFW/glfw3.h>


namespace Tangram {

void logMsg(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

PlatformImpl::PlatformImpl()
    : PlatformImpl(UrlClient::Options{}) {}

PlatformImpl::PlatformImpl(UrlClient::Options urlClientOptions) :
    m_urlClient(std::make_unique<UrlClient>(urlClientOptions)) {
    m_fcConfig = FcInitLoadConfigAndFonts();
}

PlatformImpl::~PlatformImpl() {
    FcConfigDestroy(m_fcConfig);
}

void PlatformImpl::shutdown() {
    // Stop all UrlWorker threads
    m_urlClient.reset();

    Platform::shutdown();
}

void PlatformImpl::requestRender() const {
    if (m_shutdown) { return; }
    glfwPostEmptyEvent();
}

std::vector<FontSourceHandle> PlatformImpl::systemFontFallbacksHandle() const {

    // Read system fontconfig to get list of fallback font for each
    // supported language
    auto fallbackFonts = systemFallbackFonts(m_fcConfig);

    // Create FontSourceHandle from the found list of fallback fonts
    std::vector<FontSourceHandle> handles;
    handles.reserve(fallbackFonts.size());

    std::transform(std::begin(fallbackFonts), std::end(fallbackFonts),
                   std::back_inserter(handles),
                   [](auto& path) { return FontSourceHandle(Url(path)); });

    return handles;
}

FontSourceHandle PlatformImpl::systemFont(const std::string& _name,
                                           const std::string& _weight,
                                           const std::string& _face) const {

    auto fontFile = systemFontPath(m_fcConfig, _name, _weight, _face);

    if (fontFile.empty()) { return {}; }

    return FontSourceHandle(Url(fontFile));
}

bool PlatformImpl::startUrlRequestImpl(const Url& _url, const UrlRequestHandle _request, UrlRequestId& _id) {

    _id = m_urlClient->addRequest(_url.string(),
                                  [this, _request](UrlResponse&& response) {
                                      onUrlResponse(_request, std::move(response));
                                  });
    return true;
}

void PlatformImpl::cancelUrlRequestImpl(const UrlRequestId _id) {
    if (m_urlClient) {
        m_urlClient->cancelRequest(_id);
    }
}

void setCurrentThreadPriority(int priority) {
    setpriority(PRIO_PROCESS, 0, priority);
}

void initGLExtensions() {
    Tangram::Hardware::supportsMapBuffer = true;
}

} // namespace Tangram
