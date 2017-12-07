#pragma once

#define EXTERNAL

namespace Core {

struct IUnknown {
};

template <typename CONTEXT>
class ProxyType
{
};

namespace JSON {

struct EXTERNAL IElement {
};

} // namespace JSON
} // namespace Core

namespace WPEFramework {
namespace PluginHost {

struct EXTERNAL IShell {
};

class EXTERNAL Channel {
};

} // namespace PluginHost

namespace Web {

class EXTERNAL Request {
};
class EXTERNAL Response {
};

} // namespace Web
} // namespace WPEFramework

typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
class string {
};