import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2

import GlobalQmlSettings 1.0
import Authentication.LoginForm 1.0
import Authentication.RegisterForm 1.0
import MessengerForm 1.0
import AdminPanel 1.0

ApplicationWindow {
    id: root
    property int bw: 5
    background: Rectangle {
        anchors.fill: parent
        color: Material.backgroundColor
    }

    width: GlobalSettings.defaultFormWidth
    height: GlobalSettings.defaultFormHeight
    visible: true
    title: qsTr("CorpChat")

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: /*adminPanel*/ loginFormStackComponent
        property real offset: 10
        Component {
            id: adminPanel
            AdminPanel {
                anchors.fill: parent
                onBack: {
                    stackView.pop()
                }
            }
        }

        Component {
            id: mainFormStackComponent
            MessengerForm {
                id: mainForm
                anchors.fill: parent
                onMenuClick: {
                    stackView.push(adminPanel)
                }
            }
        }
        Component {
            id: registerFormStackComponent
            RegisterForm {
                id: registerForm
                onBack: {
                    stackView.pop()
                }
            }
        }
        Component {
            id: loginFormStackComponent
            Login {
                id: loginForm
                onGotoRegister: {
                    stackView.push(registerFormStackComponent)
                }

                Connections {
                    target: client
                    function onUserBaned()
                    {

                    }

                    function onAuthSuccsess() {
                        if (stackView.depth > 1)
                            stackView.pop()
                        stackView.push(mainFormStackComponent)
                    }
                }
            }
        }
    }
}
