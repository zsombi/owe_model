QT *= network quick

SOURCES += \
    openweather/weatherapi.cpp \
    openweather/citymodel.cpp \
    openweather/openweatherforecast.cpp \
    openweather/openweatherconditions.cpp \
    openweather/openweathercache.cpp

HEADERS += \
    openweather/weatherapi.h \
    openweather/citymodel_p.h \
    openweather/openweatherforecast_p.h \
    openweather/openweatherconditions_p.h \
    openweather/openweathercache_p_p.h
