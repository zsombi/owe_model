QT *= network quick

message($$OWE_SOURCE_DIR)
SOURCES += \
    $$OWE_SOURCE_DIR/openweather/weatherapi.cpp \
    $$OWE_SOURCE_DIR/openweather/citymodel.cpp \
    $$OWE_SOURCE_DIR/openweather/openweatherforecast.cpp \
    $$OWE_SOURCE_DIR/openweather/openweatherconditions.cpp \
    $$OWE_SOURCE_DIR/openweather/openweathercache.cpp

HEADERS += \
    $$OWE_SOURCE_DIR/openweather/weatherapi.h \
    $$OWE_SOURCE_DIR/openweather/citymodel_p.h \
    $$OWE_SOURCE_DIR/openweather/openweatherforecast_p.h \
    $$OWE_SOURCE_DIR/openweather/openweatherconditions_p.h \
    $$OWE_SOURCE_DIR/openweather/openweathercache_p_p.h
