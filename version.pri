win32:VERSION = 0.5.1.0 # major.minor.patch.build
else:VERSION = 0.5.1    # major.minor.patch
DEFINES += APP_VERSION="\\\"$$VERSION\\\""
DEFINES += PROTOCOL_VERSION="\\\"0.2.0\\\""
