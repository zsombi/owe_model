import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import edu.openweather 1.0;

Page {
    id: page
    width: 600
    height: 800

    Item {
        id: currentWeather
        height: 160
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left

        OpenWeatherConditions {
            id: weatherNow
            place: page.title
        }

        Image {
            id: icon
            anchors.left: parent.left
            anchors.leftMargin: 15
            anchors.top: parent.top
            anchors.topMargin: 15
            source: weatherNow.icon
            sourceSize.width: 120
            sourceSize.height: 120
            width: 120
            height: 120
            smooth: true
        }

        Column {
            anchors.left: icon.right
            anchors.leftMargin: 30
            anchors.top: parent.top
            anchors.topMargin: 30
            anchors.bottom: icon.bottom
            anchors.right: parent.right
            spacing: 6

            Label {
                text: weatherNow.description + ", " + weatherNow.temperature + OpenWeather.temperatureSymbol
                font.pixelSize: 20
                font.bold: true
            }
            Label {
                text: qsTr("Wind: ") + weatherNow.windSpeed + " km/h" +
                      (weatherNow.windGust != 0 ? qsTr(", with gust of ") + weatherNow.windGust + " km/h" : "")
            }
            Image {
                source: "arrow.png"
                rotation: weatherNow.windDirection
                sourceSize.width: 25
                sourceSize.height: 25
                smooth: true
            }
            Label {
                text: qsTr("Pressure: ") + weatherNow.pressure + " hpa"
            }
            Label {
                text: qsTr("humidity: ") + weatherNow.humidity + "%"
            }
        }
    }

    ListView {
        id: listView
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.top: currentWeather.bottom
        anchors.margins: 15
        clip: true

        ScrollIndicator.vertical: ScrollIndicator{}

        model: OpenWeatherForecast {
            place: page.title
        }

        delegate: ItemDelegate {
            width: parent.width
            height: 130
            RowLayout {
                anchors.fill: parent
                ColumnLayout {
                    Label {
                        text: day
                    }
                    Label {
                        text: time
                    }
                }

                Image {
                    source: weatherIcon
                    sourceSize.width: 55
                    sourceSize.height: 55
                    smooth: true
                }

                ColumnLayout
                {
                    Label {
                        text: description + ", " + temperature + OpenWeather.temperatureSymbol
                        font.pixelSize: 14
                        font.bold: true
                    }
                    Label {
                        text: qsTr("Wind: ") + windSpeed + " km/h" +
                              (windGust != 0 ? qsTr(", with gust of ") + windGust + " km/h" : "")
                    }
                    Image {
                        source: "arrow.png"
                        rotation: windDirection
                        sourceSize.width: 15
                        sourceSize.height: 15
                        smooth: true
                    }
                    Label {
                        text: qsTr("Pressure: ") + pressure + " hpa"
                    }
                    Label {
                        text: qsTr("humidity: ") + humidity + "%"
                    }
                }
            }
        }
    }
}
