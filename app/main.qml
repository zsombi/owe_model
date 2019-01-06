import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import edu.openweather 1.0;

ApplicationWindow {
    id: appWindow
    visible: true
    width: 400
    height: 600
    title: "Weather App"
    property bool appReady: false

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            ToolButton {
                text: "\u2630" // Burger button
                onClicked: drawer.open()
            }
            Label {
                text: swipeView.currentItem ? swipeView.currentItem.item.title : "No place set"
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }
            ToolButton {
                text: "?"
            }
        }
    }

    PlaceDrawerForm {
        id: drawer
        y: header.height
        width: appWindow.width * 0.8
        height: appWindow.height - header.height - footer.height

        Binding {
            target: swipeView
            property: "currentIndex"
            value: drawer.cityIndex
        }
    }

    Binding {
        target: OpenWeather
        property: "lastCityIndex"
        value: swipeView.currentIndex
        when: appWindow.appReady
    }

    Connections {
        target: OpenWeather
        onLastCityIndexChanged: print("cityIndex:", OpenWeather.lastCityIndex)
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent

        Repeater {
            model: OpenWeather.cityModel

            Loader {
                active: SwipeView.isCurrentItem || SwipeView.isNextItem || SwipeView.isPreviousItem
                sourceComponent: WeatherForm {
                    title: modelData
                }
            }
        }
    }

    footer: Pane {
        width: parent.width
        PageIndicator {
            anchors.horizontalCenter: parent.horizontalCenter
            currentIndex: OpenWeather.lastCityIndex
            count: swipeView.count
        }
    }

    Component.onCompleted: {
        swipeView.currentIndex = OpenWeather.lastCityIndex
        appWindow.appReady = true;
    }
}
