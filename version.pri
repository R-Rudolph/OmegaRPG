win32:VERSION = 0.6.0.0 # major.minor.patch.build
else:VERSION = 0.6.0    # major.minor.patch
DEFINES += APP_VERSION="\\\"$$VERSION\\\""
DEFINES += PROTOCOL_VERSION="\\\"0.3.0\\\""
