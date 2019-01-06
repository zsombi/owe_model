import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import edu.openweather 1.0;

Drawer {
    id: cityManager
    property alias cityIndex: listView.currentIndex

    CityPickerDialog {
        id: dlgCityLookup
    }

    ColumnLayout {
        anchors.fill: parent
        ToolBar {
            id: tools
            Layout.fillWidth: true

            RowLayout {
                anchors.horizontalCenter: parent.horizontalCenter
                ToolButton {
                    id: addCityButton
                    text: "+"
                    onClicked: dlgCityLookup.open()
                }
                ToolButton {
                    id: deleteCity
                    text: "-"
                    onClicked: OpenWeather.removeCity(listView.currentIndex)
                }
            }
        }
        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            ScrollIndicator.vertical: ScrollIndicator {}

            model: OpenWeather.cityModel
            delegate: ItemDelegate {
                width: parent.width
                height: 40
                text: modelData
                highlighted: ListView.isCurrentItem
                onClicked: listView.currentIndex = index;
            }
        }
    }
}
